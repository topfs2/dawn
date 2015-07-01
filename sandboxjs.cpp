
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

bool resize(duk_context *ctx, int width, int height) {
    duk_push_global_object(ctx);
    duk_get_prop_string(ctx, -1, "resize");
    duk_push_int(ctx, width);
    duk_push_int(ctx, height);

    if (duk_pcall(ctx, 2) != 0) {
        printf("resize.error: %s\n", duk_safe_to_string(ctx, -1));
        return false;
    }

    return true;
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow( 
        "dawn",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    SDL_GLContext context = SDL_GL_CreateContext(window);

    glewInit();

    IRenderer *renderer = new OpenGLRenderer();

    duk_context *ctx = duk_create_heap(NULL, NULL, NULL, NULL, fatal);

    duk_push_c_function(ctx, dukopen_libdawn, 0);
    duk_call(ctx, 0);
    duk_put_global_string(ctx, "libdawn");

    // polyfill
    duk_eval_string(ctx, "document = window = this;");

    if (duk_peval_file(ctx, "dawn.js") != 0) {
        printf("core.error: %s\n", duk_safe_to_string(ctx, -1));
    }

    for (int i = 1; i < argc; i++) {
        printf("Running %s\n", argv[i]);

        if (duk_peval_file(ctx, argv[i]) != 0) {
            printf("eval.error: %s\n", duk_safe_to_string(ctx, -1));
        }
    }

    duk_gc(ctx, 0);
    resize(ctx, WINDOW_WIDTH, WINDOW_HEIGHT);

    bool lastDirty = false;
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    resize(ctx, event.window.data1, event.window.data2);
                }

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
                Object *p = static_cast<Object *>(duk_get_pointer(ctx, -1));
                Scene3D *scene = dynamic_cast<Scene3D *>(p);
                if (scene) {
                    if (scene->isDirty(true)) {
                        if (!lastDirty) {
                            cout << "Rendering as scene is dirty" << endl;
                            lastDirty = true;
                        }

                        renderer->render(scene);
                        SDL_GL_SwapWindow(window);
                        scene->clean();
                    } else {
                        if (lastDirty) {
                            cout << "Skipping rendering as scene is not dirty" << endl;
                            lastDirty = false;
                        }

                        duk_gc(ctx, 0); // TODO need to call gc at other points aswell?
                        usleep(16666);
                    }
                }
            }
        }

        duk_pop(ctx);
    }

    SDL_Quit();

    duk_destroy_heap(ctx);
    return 0;
}

