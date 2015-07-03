#include <iostream>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>

#include "config.h"

#include "UUID.h"

#include "OpenGLRenderer.h"
#include "OrthographicCamera.h"

#include "Mesh3D.h"
#include "ShaderMaterial.h"
#include "PlaneGeometry.h"
#include "EllipsisGeometry.h"

#include "SDLImage.h"

#include "LinearAlgebra.h"

using namespace std;
using namespace dawn;



int main (int argc, char *argv[]) {
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
    Object3D *stage = new Object3D();
    Camera *camera = new OrthographicCamera(4.0 * WINDOW_WIDTH / (float)WINDOW_HEIGHT, 4.0, -1.0, 1000.0);

    Scene3D *scene = new Scene3D(camera, stage, WINDOW_WIDTH, WINDOW_HEIGHT);

    Object3D *group = new Object3D();

    Image *logo = new SDLImage("kodi-thumbnail.png");
    Image *bg = new SDLImage("kodi-background.jpg");

    ShaderMaterial * listShader = new ShaderMaterial("shaders/map");
    listShader->uniform("map", bg);

    ShaderMaterial * popupShader = new ShaderMaterial("shaders/map");
    popupShader->uniform("map", logo);

    float bgAR = bg->width() / (float)bg->height();
    float logoAR = logo->width() / (float)logo->height();

    Mesh3D *q1 = new Mesh3D(new PlaneGeometry(1.0 * bgAR, 1.0), listShader);
    Mesh3D *q2 = new Mesh3D(new PlaneGeometry(1.0 * bgAR, 1.0), listShader);
    Mesh3D *q3 = new Mesh3D(new PlaneGeometry(1.0 * bgAR, 1.0), listShader);

    Mesh3D *q4 = new Mesh3D(new PlaneGeometry(1.0 * logoAR, 1.0), popupShader);
    q4->visible(false);
//    Mesh3D *q3 = new Mesh3D(new PlaneGeometry(2, 2, 1, 1), new VideoMaterial("default"));

    q1->transform(translation(-2.0f, 0.0f, 0.0f));
    q2->transform(translation( 0.0f, 0.0f, 0.0f) * scaling(1.1f, 1.1f, 1.0f));
    q3->transform(translation( 2.0f, 0.0f, 0.0f));

    group->appendChild(q1);
    group->appendChild(q2);
    group->appendChild(q3);

    stage->appendChild(group);
    stage->appendChild(q4);

    UniformMap uniforms;
    uniforms["strength"] = 1.0f;

    q1->filters(Filter(FILTER_GRAYSCALE, uniforms));
    q3->filters(Filter(FILTER_GRAYSCALE, uniforms));

    float angle = 0.0f;
    float rootGray = 0.0f;

    int selected = 1;
    bool popup = false;

    bool running = true;
    while (running)
    {
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        switch (event.type)
        {
        case SDL_QUIT:
          running = false;
          break;

			  case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_LEFT)
            selected--;

          if (event.key.keysym.sym == SDLK_RIGHT)
            selected++;

          if (event.key.keysym.sym == SDLK_SPACE)
            popup = !popup;

          selected = max(0, min(2, selected));

          q4->visible(popup);
          if (popup) {
            group->filters(Filter(FILTER_GAUSIAN_BLUR, uniforms));
          } else {
            group->filters(FilterList()); // TODO Better way to disable it?
          }

          if (selected == 0) {
            q1->filters(FilterList());
          } else {
            q1->filters(Filter(FILTER_GRAYSCALE, uniforms));
          }

          if (selected == 1) {
            q2->filters(FilterList());
          } else {
            q2->filters(Filter(FILTER_GRAYSCALE, uniforms));
          }

          if (selected == 2) {
            q3->filters(FilterList());
          } else {
            q3->filters(Filter(FILTER_GRAYSCALE, uniforms));
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

    std::cout << "Kill world" << std::endl;
    return 0;
}

