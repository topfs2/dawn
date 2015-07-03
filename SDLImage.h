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
            if (surface) {
                const uint8_t *originalRaw = static_cast<uint8_t *>(surface->pixels);
                uint8_t *flippedRaw = new uint8_t[surface->pitch * surface->h];

                for (unsigned i = 0; i < surface->h; ++i) {
                    const uint8_t *srcBeg = originalRaw + (surface->pitch *(surface->h - i - 1));
                    const uint8_t *srcEnd = srcBeg + surface->pitch;

                    std::copy(srcBeg, srcEnd, flippedRaw + surface->pitch * i);
                }

                int bpp = surface->format->BytesPerPixel;
                if (bpp == 4) {
                    m_format = (surface->format->Rmask == 0x000000ff) ? CONSTANTS::RGBAFormat : CONSTANTS::BGRAFormat;
                } else if (bpp == 3) {
                    m_format = (surface->format->Rmask == 0x000000ff) ? CONSTANTS::RGBFormat : CONSTANTS::BGRFormat;
                }

                if (bpp == 4) {
                    for(unsigned int pos = 0; pos < (surface->h * surface->pitch); pos += 4) {
                        unsigned char c0 = flippedRaw[pos+0];
                        unsigned char c1 = flippedRaw[pos+1];
                        unsigned char c2 = flippedRaw[pos+2];
                        float a = flippedRaw[pos+3] / 256.0;

                        flippedRaw[pos+0] = round(c0 * a);
                        flippedRaw[pos+1] = round(c1 * a);
                        flippedRaw[pos+2] = round(c2 * a);
                    }
                }

                m_width = surface->w;
                m_height = surface->h;
                m_buffer = BufferPtr(new Buffer(flippedRaw, surface->pitch * surface->h));

                SDL_FreeSurface(surface);
                delete [] flippedRaw;
            }
        }

        BufferPtr m_buffer;
        unsigned int m_width;
        unsigned int m_height;
        CONSTANTS::PixelFormat m_format;
    };
}
