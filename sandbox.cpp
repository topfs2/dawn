#include <iostream>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>

#include "config.h"

#include "UUID.h"

#include "OpenGLRenderer.h"
#include "OrthographicCamera.h"

#include "Mesh.h"
#include "ShaderMaterial.h"
#include "PlaneGeometry.h"
#include "CircleGeometry.h"

#include "Image.h"

#include "LinearAlgebra.h"

using namespace std;
using namespace dawn;

class SDLImage : public Image
{
public:
  SDLImage(const string &path) : Image(0, 0, CONSTANTS::UnknownFormat), m_path(path)
  {
    SDL_Surface *surface = IMG_Load(path.c_str());
    if (surface)
    {
      const uint8_t *originalRaw = static_cast<uint8_t *>(surface->pixels);
      uint8_t *flippedRaw = new uint8_t[surface->pitch * surface->h];

      for (unsigned i = 0; i < surface->h; ++i)
      {
          const uint8_t *srcBeg = originalRaw + (surface->pitch *(surface->h - i - 1));
          const uint8_t *srcEnd = srcBeg + surface->pitch;

          std::copy(srcBeg, srcEnd, flippedRaw + surface->pitch * i);
      }

      m_buffer = BufferPtr(new Buffer(flippedRaw, surface->pitch * surface->h));

      GLint bpp = surface->format->BytesPerPixel;
      if (bpp == 4)
        format = (surface->format->Rmask == 0x000000ff) ? CONSTANTS::RGBAFormat : CONSTANTS::BGRAFormat;
      else if (bpp == 3)
        format = (surface->format->Rmask == 0x000000ff) ? CONSTANTS::RGBFormat : CONSTANTS::BGRFormat;

      width = surface->w;
      height = surface->h;

      SDL_FreeSurface(surface);
    }
  }

  virtual std::string id() { return m_path; }
  virtual BufferPtr buffer() { return m_buffer; }

private:
  std::string m_path;
  BufferPtr m_buffer;
};

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
    Object3D *scene = new Object3D();
    ICamera *camera = new OrthographicCamera(4.0 * WINDOW_WIDTH / (float)WINDOW_HEIGHT, 4.0, -1.0, 1000.0);

    Object3D *group = new Object3D();

    Image *logo = new SDLImage("kodi-thumbnail.png");
    Image *bg = new SDLImage("kodi-background.jpg");

    ShaderMaterial * listShader = new ShaderMaterial("shaders/map");
    listShader->uniforms["map"] = bg;

    ShaderMaterial * popupShader = new ShaderMaterial("shaders/map");
    popupShader->uniforms["map"] = logo;

    float bgAR = bg->width / (float)bg->height;
    float logoAR = logo->width / (float)logo->height;

    Mesh *q1 = new Mesh(new PlaneGeometry(1.0 * bgAR, 1.0), listShader);
    Mesh *q2 = new Mesh(new PlaneGeometry(1.0 * bgAR, 1.0), listShader);
    Mesh *q3 = new Mesh(new PlaneGeometry(1.0 * bgAR, 1.0), listShader);

    Mesh *q4 = new Mesh(new PlaneGeometry(1.0 * logoAR, 1.0), popupShader);
    q4->visible(false);
//    Mesh *q3 = new Mesh(new PlaneGeometry(2, 2, 1, 1), new VideoMaterial("default"));

    q1->transform(translation(-2.0f, 0.0f, 0.0f));
    q2->transform(translation( 0.0f, 0.0f, 0.0f) * scaling(1.1f, 1.1f, 1.0f));
    q3->transform(translation( 2.0f, 0.0f, 0.0f));

    group->push(q1);
    group->push(q2);
    group->push(q3);

    scene->push(group);
    scene->push(q4);

    q1->filters(Filter(FILTER_GRAYSCALE, 1.0f));
    q3->filters(Filter(FILTER_GRAYSCALE, 1.0f));

    cout << "q1 " << q1->uuid.uuid() << endl;
    cout << "q2 " << q2->uuid.uuid() << endl;
    cout << "q3 " << q3->uuid.uuid() << endl;
    cout << "group " << group->uuid.uuid() << endl;
    cout << "scene " << scene->uuid.uuid() << endl;

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
            group->filters(Filter(FILTER_GAUSIAN_BLUR, 1.0f));
          } else {
            group->filters(FilterList()); // TODO Better way to disable it?
          }

          if (selected == 0) {
            q1->filters(FilterList());
          } else {
            q1->filters(Filter(FILTER_GRAYSCALE, 1.0f));
          }

          if (selected == 1) {
            q2->filters(FilterList());
          } else {
            q2->filters(Filter(FILTER_GRAYSCALE, 1.0f));
          }

          if (selected == 2) {
            q3->filters(FilterList());
          } else {
            q3->filters(Filter(FILTER_GRAYSCALE, 1.0f));
          }

          break;
        }
      }

      renderer->render(camera, scene);
      SDL_GL_SwapWindow(window);

#ifdef DEBUG_WRITE_FBO
      running = false;
#endif
    }

    SDL_Quit();

    std::cout << "Kill world" << std::endl;
    return 0;
}

