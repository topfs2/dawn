#pragma once
#include "Pixmap.h"
#include "TextLayout.h"

namespace dawn
{
    class TextImage : public Pixmap
    {
    public:
        TextImage(TextLayout *layout) : m_layout(layout) { }
        virtual ~TextImage() { }

        TextLayout *layout() const { return m_layout; }
        virtual void layout(TextLayout *layout) { setChanged(m_layout != layout); m_layout = layout; }

        virtual bool isChanged(etag_t *etag, bool recursive) {
            bool changed = Pixmap::isChanged(etag, recursive);

            if (recursive) {
                changed |= m_layout->isChanged(etag, recursive);
            }

            return changed;
        }

    protected:
        TextLayout *m_layout;
    };
}
