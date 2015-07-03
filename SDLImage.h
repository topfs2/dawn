#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "constants.h"
#include "Image.h"

namespace dawn
    {
    class SDLImage : public Image
    {
    public:
        SDLImage(const std::string &path) : Image(path) {
            LoadImage();
        }

        virtual void path(const std::string &path) {
            if (m_path != path) {
                markDirty();
                m_path = path;
                LoadImage();
            }
        }

        virtual BufferPtr buffer() { return m_buffer; }
        virtual unsigned int width() { return m_width; }
        virtual unsigned int height() { return m_height; }
        virtual CONSTANTS::PixelFormat format() { return m_format; }

    private:
        void LoadImage() {
            std::cout << "Loading " << m_path << std::endl;
            SDL_Surface *surface = IMG_Load(m_path.c_str());
            if (surface)
            {
                const uint8_t *originalRaw = static_cast<uint8_t *>(surface->pixels);
                uint8_t *flippedRaw = new uint8_t[surface->pitch * surface->h];

                for (unsigned i = 0; i < surface->h; ++i) {
                    const uint8_t *srcBeg = originalRaw + (surface->pitch *(surface->h - i - 1));
                    const uint8_t *srcEnd = srcBeg + surface->pitch;

                    std::copy(srcBeg, srcEnd, flippedRaw + surface->pitch * i);
                }

                std::cout << "Stored buffer" << std::endl;
                m_buffer = BufferPtr(new Buffer(flippedRaw, surface->pitch * surface->h));

                int bpp = surface->format->BytesPerPixel;
                if (bpp == 4)
                    m_format = (surface->format->Rmask == 0x000000ff) ? CONSTANTS::RGBAFormat : CONSTANTS::BGRAFormat;
                else if (bpp == 3)
                    m_format = (surface->format->Rmask == 0x000000ff) ? CONSTANTS::RGBFormat : CONSTANTS::BGRFormat;

                m_width = surface->w;
                m_height = surface->h;

                SDL_FreeSurface(surface);
            }
        }

        BufferPtr m_buffer;
        unsigned int m_width;
        unsigned int m_height;
        CONSTANTS::PixelFormat m_format;
    };
}
