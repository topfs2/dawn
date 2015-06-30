
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>

#include "config.h"

#include "OpenGLRenderer.h"
#include "dawn.h"

#include "duktape.h"
#include "libdawn.h"

using namespace std;
using namespace dawn;

void fatal(duk_context *ctx, int code, const char *msg) {
    printf("FATAL: %i %s\n", code, msg);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow( 
        "dawn",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
    );
    SDL_GLContext context = SDL_GL_CreateContext(window);

    glewInit();

    IRenderer *renderer = new OpenGLRenderer();
    Camera *camera = new OrthographicCamera(4.0 * WINDOW_WIDTH / (float)WINDOW_HEIGHT, 4.0, -1.0, 1000.0);

    duk_context *ctx = duk_create_heap(NULL, NULL, NULL, NULL, fatal);

    duk_push_c_function(ctx, dukopen_libdawn, 0);
    duk_call(ctx, 0);
    duk_put_global_string(ctx, "libdawn");

    // polyfill
    duk_eval_string(ctx, "document = window = this;");

    if (duk_peval_file(ctx, "dawn.js") != 0) {
        printf("Error: %s\n", duk_safe_to_string(ctx, -1));
    }

    for (int i = 1; i < argc; i++) {
        printf("Running %s\n", argv[i]);

        if (duk_peval_file(ctx, argv[i]) != 0) {
            printf("Error: %s\n", duk_safe_to_string(ctx, -1));
        }
    }

    duk_gc(ctx, 0);

    bool lastDirty = false;
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN:
                break;
            }
        }

        duk_push_global_object(ctx);
        duk_get_prop_string(ctx, -1, "update");

        if (duk_pcall(ctx, 0) != 0) {
            printf("Error: %s\n", duk_safe_to_string(ctx, -1));
            running = false;
        } else {
            if (duk_is_pointer(ctx, -1)) {
                void *p = duk_get_pointer(ctx, -1);
                Object3D *scene = static_cast<Object3D *>(p);
                if (scene) {
                    if (scene->isDirty(true)) {
                        if (!lastDirty) {
                            cout << "Rendering as scene is dirty" << endl;
                            lastDirty = true;
                        }

                        renderer->render(camera, scene);
                        SDL_GL_SwapWindow(window);
                        scene->clean();
                    } else {
                        if (lastDirty) {
                            cout << "Skipping rendering as scene is not dirty" << endl;
                            lastDirty = false;
                        }

                        usleep(16666);
                    }
                }
            }
        }

        duk_pop(ctx);  /* pop result/error */
    }

    SDL_Quit();

    duk_destroy_heap(ctx);
    return 0;
}

