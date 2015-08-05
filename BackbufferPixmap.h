#pragma once
#include "Pixmap.h"

namespace dawn
{
    class BackbufferPixmap : public Pixmap
    {
    public:
        BackbufferPixmap(vec2i position, vec2i size) : m_position(position), m_size(size) { }
        virtual ~BackbufferPixmap() { }

        virtual CONSTANTS::PixmapType type() const { return CONSTANTS::BackbufferPixmap; }

        vec2i position() const { return m_position; }
        void position(vec2i position) { setChanged(m_position != position); m_position = position; }

        vec2i size() const { return m_size; }
        void size(vec2i size) { setChanged(m_size != size); m_size = size; }

        virtual BufferPtr buffer() { return BufferPtr(); } // TODO For now
        virtual unsigned int width() { return m_size[0]; }
        virtual unsigned int height() { return m_size[1]; }
        virtual CONSTANTS::PixelFormat format() { return CONSTANTS::RGBFormat; }

    protected:
        vec2i m_position;
        vec2i m_size;
    };
}
