#pragma once
#include "Object3D.h"
#include "Camera.h"

namespace dawn
{
    class Scene3D : public Object
    {
    public:
        Scene3D(Camera *camera, Object3D *stage, unsigned int width, unsigned int height) : m_camera(camera), m_stage(stage), m_width(width), m_height(height) { }
        virtual CONSTANTS::Type type() const { return CONSTANTS::Scene3D; }

        Camera *camera() const { return m_camera; }
        void camera(Camera *camera) { setChanged(m_camera != camera); m_camera = camera; }

        Object3D *stage() const { return m_stage; }
        void stage(Object3D *stage) { setChanged(m_stage != stage); m_stage = stage; }

        unsigned int width() const { return m_width; }
        void width(unsigned int width) { setChanged(m_width != width); m_width = width; }

        unsigned int height() const { return m_height; }
        void height(unsigned int height) { setChanged(m_height != height); m_height = height; }

        void size(float width, float height) { setChanged(m_height != height || m_width != width); m_width = width; m_height = height; }

        virtual bool isChanged(etag_t *etag, bool recursive) {
            bool changed = Object::isChanged(etag, recursive);

            if (recursive) {
                changed |= m_camera->isChanged(etag, recursive);
                changed |= m_stage->isChanged(etag, recursive);
            }

            return changed;
        }

    protected:
        Camera *m_camera;
        Object3D *m_stage;
        unsigned int m_width;
        unsigned int m_height;
    };
}
