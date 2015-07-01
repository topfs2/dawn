#pragma once
#include "Object3D.h"
#include "Camera.h"

namespace dawn
{
    class Scene3D : public Object
    {
    public:
        Scene3D(Camera *camera, Object3D *stage) : m_camera(camera), m_stage(stage) { }
        virtual CONSTANTS::Type type() const { return CONSTANTS::Scene; }

        Camera *camera() const { return m_camera; }
        void camera(Camera *camera) { markDirty(m_camera != camera); m_camera = camera; }

        Object3D *stage() const { return m_stage; }
        void stage(Object3D *stage) { markDirty(m_stage != stage); m_stage = stage; }

        virtual bool isDirty(bool recursive = false) const {
            if (Object::isDirty(recursive)) {
                return true;
            } else if (recursive) {
                return m_camera->isDirty(recursive) || m_stage->isDirty(recursive);
            }

            return false;
        }

        virtual void clean() {
            Object::clean();

            m_camera->clean();
            m_stage->clean();
        }

    protected:
        Camera *m_camera;
        Object3D *m_stage;
    };
}
