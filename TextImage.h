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
        virtual void layout(TextLayout *layout) { markDirty(m_layout != layout); m_layout = layout; }

        virtual bool isDirty(bool recursive = false) const {
            if (Pixmap::isDirty(recursive)) {
                return true;
            } else if (recursive) {
                return m_layout->isDirty(recursive);
            }

            return false;
        }

        virtual void clean() {
            Pixmap::clean();

            m_layout->clean();
        }

    protected:
        TextLayout *m_layout;
    };
}
