#pragma once
#include "Pixmap.h"
#include "TextStyle.h"

namespace dawn
{
    class TextImage : public Pixmap
    {
    public:
        TextImage(std::string text, TextStyle *style) : m_text(text), m_style(style) { }
        virtual ~TextImage() { }

        std::string text() const { return m_text; }
        void text(std::string text) { setChanged(m_text != text); m_text = text; }

        TextStyle *style() const { return m_style; }
        virtual void style(TextStyle *style) { setChanged(m_style != style); m_style = style; }

        virtual bool isChanged(etag_t *etag, bool recursive) {
            bool changed = Pixmap::isChanged(etag, recursive);

            if (recursive) {
                changed |= m_style->isChanged(etag, recursive);
            }

            return changed;
        }

    protected:
        std::string m_text;
        TextStyle *m_style;
    };
}
