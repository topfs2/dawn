
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <iostream>

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

void duk_polyfill(duk_context *ctx, std::string path, std::string file) {
    string polyfill = path + file;
    if (duk_peval_file(ctx, polyfill.c_str()) != 0) {
        printf("core.error: loading %s - %s\n", polyfill.c_str(), duk_safe_to_string(ctx, -1));
    }
}

bool emitResizeEvent(duk_context *ctx, int width, int height) {
    duk_push_global_object(ctx); // TODO Switch to push dawn
    duk_get_prop_string(ctx, -1, "emitResizeEvent");
    duk_push_int(ctx, width);
    duk_push_int(ctx, height);

    if (duk_pcall(ctx, 2) != 0) {
        printf("resize.error: %s\n", duk_safe_to_string(ctx, -1));
        return false;
    }

    return true;
}

bool emitMotionEvent(duk_context *ctx, std::string device, std::string key, unsigned int x, unsigned int y) {
    duk_push_global_object(ctx); // TODO Switch to push dawn
    duk_get_prop_string(ctx, -1, "emitMotionEvent");
    duk_push_string(ctx, device.c_str());
    duk_push_string(ctx, key.c_str());
    duk_push_int(ctx, x);
    duk_push_int(ctx, y);

    if (duk_pcall(ctx, 4) != 0) {
        printf("emitMotionEvent.error: %s\n", duk_safe_to_string(ctx, -1));
        return false;
    }

    return true;
}

bool emitKeyEvent(duk_context *ctx, bool down, std::string device, std::string key) {
    duk_push_global_object(ctx); // TODO Switch to push dawn
    duk_get_prop_string(ctx, -1, "emitKeyEvent");
    duk_push_string(ctx, device.c_str());
    duk_push_string(ctx, key.c_str());
    duk_push_boolean(ctx, down);

    if (duk_pcall(ctx, 3) != 0) {
        printf("emitKeyEvent.error: %s\n", duk_safe_to_string(ctx, -1));
        return false;
    }

    return true;
}


int main(int argc, char *argv[]) {
    // Get the last position of '/'
    string aux(argv[0]);

    // get '/' or '\\' depending on unix/mac or windows.
#if defined(_WIN32) || defined(WIN32)
    int pos = aux.rfind('\\');
#else
    int pos = aux.rfind('/');
#endif

    // Get the path and the name
    string path = aux.substr(0,pos+1);
    string name = aux.substr(pos+1);
    // show results
    std::cout << "Path: " << path << std::endl;
    std::cout << "Name: " << name << std::endl;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);
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

    duk_polyfill(ctx, path, "modsearch.js");
    duk_polyfill(ctx, path, "tween.js");
    duk_polyfill(ctx, path, "dawn.js");

    for (int i = 1; i < argc; i++) {
        printf("Running %s\n", argv[i]);

        if (duk_peval_file(ctx, argv[i]) != 0) {
            printf("eval.error: %s\n", duk_safe_to_string(ctx, -1));
        }
    }

    duk_gc(ctx, 0);
    emitResizeEvent(ctx, WINDOW_WIDTH, WINDOW_HEIGHT);

    etag_t dirty_etag = 0;
    bool lastDirty = false;
    bool running = true;
    while (running) {
        std::stringstream device;
        std::stringstream key;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    emitResizeEvent(ctx, event.window.data1, event.window.data2);
                }

                break;

            case SDL_MOUSEMOTION:
                device << "mouse-" << (int)event.motion.which;
                key << "mouse";

                emitMotionEvent(ctx, device.str(), key.str(), event.motion.x, event.motion.y);
                break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                device << "mouse-" << (int)event.button.which;
                key << (int)event.button.button;

                emitKeyEvent(ctx, event.type == SDL_MOUSEBUTTONDOWN, device.str(), key.str());
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                device << "keyboard";
                key << SDL_GetKeyName(event.key.keysym.sym);

                if (event.key.repeat == 0) {
                    emitKeyEvent(ctx, event.type == SDL_KEYDOWN, device.str(), key.str());
                }
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
                    if (scene->isChanged(&dirty_etag, true)) {
                        if (!lastDirty) {
                            cout << "Rendering as scene is dirty " << dirty_etag << endl;
                            lastDirty = true;
                        }

                        renderer->render(scene);
                        SDL_GL_SwapWindow(window);
                    } else {
                        if (lastDirty) {
                            cout << "Skipping rendering as scene is not dirty " << dirty_etag << endl;
                            lastDirty = false;
                        }

                        duk_gc(ctx, 0); // TODO need to call gc at other points aswell?
                        usleep(16666);
                    }
                }
            }
        }

        duk_pop(ctx);

#ifdef DEBUG_WRITE_FBO
        running = false;
#endif
    }

    SDL_Quit();

    duk_destroy_heap(ctx);
    return 0;
}

