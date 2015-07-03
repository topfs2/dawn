#pragma once
#include "constants.h"
#include "Object.h"

namespace dawn
{
    class TextLayout : public Object
    {
    public:
        TextLayout(std::string text, std::string font, vec4f foreground, CONSTANTS::TextAlign align) : m_text(text), m_font(font), m_foreground(foreground), m_align(align) { }
        virtual ~TextLayout() { }

        std::string text() const { return m_text; }
        void text(std::string text) { markDirty(m_text != text); m_text = text; }

        std::string font() const { return m_font; }
        void font(std::string font) { markDirty(m_font != font); m_font = font; }

        vec4f foreground() const { return m_foreground; }
        void foreground(vec4f foreground) { markDirty(m_foreground != foreground); m_foreground = foreground; }

        CONSTANTS::TextAlign align() const { return m_align; }
        void align(CONSTANTS::TextAlign align) { markDirty(m_align != align); m_align = align; }

    protected:
        std::string m_text;
        std::string m_font;
        vec4f m_foreground;
        CONSTANTS::TextAlign m_align;
    };
}
