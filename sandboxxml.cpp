
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <expat.h>
#include <stdio.h>
#include <string.h>
#include <regex>

#include "config.h"

#include "OpenGLRenderer.h"
#include "SDLImage.h"

using namespace std;
using namespace dawn;

#ifdef TEST_COMPONENTS
Component *ExampleFactory(const std::string &type, const PropertyMap &props) {
    return NULL;
}

ComponentConstructor Example(ExampleFactory, "Example");

int main() {
    Element foo1("foo");
    Element foo2("foo");

    Element bar("bar");

    Element fooComponent(Example);
    Element fooComponent2(Example);
    Element fooComponent3(Example);

    std::cout << " a=" << fooComponent.sameAs(fooComponent2)
              << " b=" << fooComponent.sameAs(fooComponent3)
              << " c=" << fooComponent2.sameAs(bar)
              << " d=" << fooComponent2.sameAs(foo1)
              << " e=" << foo1.sameAs(bar)
              << " f=" << foo1.sameAs(foo2) << std::endl;
}
#endif

bool emitResizeEvent(int width, int height) {
    return true;
}

bool emitMotionEvent(std::string device, std::string key, unsigned int x, unsigned int y) {
    return true;
}

bool emitKeyEvent(bool down, std::string device, std::string key) {
    return true;
}

int
main(int argc, char **argv)
{
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

#ifdef NEW
    PropertyMap uniforms = {
        { "color", vec4f(1.0f, 1.0f, 1.0f, 1.0f) }
    };

    PropertyMap attributes;

    Element scene("Scene3D", {
        { "width", 1280 },
        { "height", 720 }
    },
        Element("OrthographicCamera", {
            { "width", (4.0f * 16.0f / 9.0f) },
            { "height", 4.0f },
            { "near", -1.0f },
            { "far", 1000.0f }
        },
            Element("Mesh3D", {
                { "program", "color" },
                { "uniforms", uniforms },
                { "attributes", attributes }
            })
        )
    );
#else
    PropertyMap uniforms = {
        { "color", vec4f(1.0f, 1.0f, 1.0f, 1.0f) }
    };

    Pixmap *map = new SDLImage("kodi-background.jpg");

    ShaderMaterial material("shaders/debug");
    material.uniform("color", vec4f(0.0f, 1.0f, 1.0f, 1.0f));
    material.uniform("map", map);

    PlaneGeometry geometry(1.0f, 1.0f, vec4f(0.0f, 1.0f, 0.0f, 1.0f));

    Element scene("Scene3D", {
        { "width", 1280 },
        { "height", 720 }
    },
        Element("OrthographicCamera", {
            { "width", (4.0f * 16.0f / 9.0f) },
            { "height", 4.0f },
            { "near", -1.0f },
            { "far", 1000.0f }
        },
            Element("Mesh3D", {
                { "material", material },
                { "geometry", geometry }
            })
        )
    );
#endif


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
                    emitResizeEvent(event.window.data1, event.window.data2);
                }

                break;

            case SDL_MOUSEMOTION:
                device << "mouse-" << (int)event.motion.which;
                key << "mouse";

                emitMotionEvent(device.str(), key.str(), event.motion.x, event.motion.y);
                break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                device << "mouse-" << (int)event.button.which;
                key << (int)event.button.button;

                emitKeyEvent(event.type == SDL_MOUSEBUTTONDOWN, device.str(), key.str());
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                device << "keyboard";
                key << SDL_GetKeyName(event.key.keysym.sym);

                if (event.key.repeat == 0) {
                    emitKeyEvent(event.type == SDL_KEYDOWN, device.str(), key.str());
                }
                break;
            }
        }

        renderer->render(scene);
        SDL_GL_SwapWindow(window);

#ifdef DEBUG_WRITE_FBO
        running = false;
#endif
    }

    SDL_Quit();


    return 0;
}
