#pragma once
#include "constants.h"
#include "Object.h"

namespace dawn
{
    class TextStyle : public Object
    {
    public:
        TextStyle(std::string font, vec4f foreground, CONSTANTS::TextAlign align, unsigned int maxWidth, unsigned int maxHeight) : m_font(font), m_foreground(foreground), m_align(align), m_maxWidth(maxWidth), m_maxHeight(maxHeight) { }
        virtual ~TextStyle() { }

        std::string font() const { return m_font; }
        void font(std::string font) { setChanged(m_font != font); m_font = font; }

        vec4f foreground() const { return m_foreground; }
        void foreground(vec4f foreground) { setChanged(m_foreground != foreground); m_foreground = foreground; }

        CONSTANTS::TextAlign align() const { return m_align; }
        void align(CONSTANTS::TextAlign align) { setChanged(m_align != align); m_align = align; }

        unsigned int maxWidth() const { return m_maxWidth; }
        void maxWidth(unsigned int maxWidth) { setChanged(m_maxWidth != maxWidth); m_maxWidth = maxWidth; }

        unsigned int maxHeight() const { return m_maxHeight; }
        void maxHeight(unsigned int maxHeight) { setChanged(m_maxHeight != maxHeight); m_maxHeight = maxHeight; }

    protected:
        std::string m_font;
        vec4f m_foreground;
        CONSTANTS::TextAlign m_align;
        unsigned int m_maxWidth;
        unsigned int m_maxHeight;
    };
}
