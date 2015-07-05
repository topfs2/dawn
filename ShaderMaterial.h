#pragma once
#include <map>
#include "Material.h"
#include "Pixmap.h"
#include "Scene3D.h"
#include "types.h"

namespace dawn
{
    class ShaderMaterial : public Material
    {
    public:
        ShaderMaterial(std::string path) : m_path(path) { }

        CONSTANTS::MaterialType type() const { return CONSTANTS::ShaderMaterial; }

        std::string path() const { return m_path; }
        void path(std::string path) { setChanged(m_path != path); m_path = path; } // TODO Should it exist?

        UniformMap uniforms() const { return m_uniforms; }

        // TODO make read const
        uniform_t uniform(std::string key) { return m_uniforms[key]; }
        void uniform(std::string key, uniform_t value) { setChanged(); m_uniforms[key] = value; }

        virtual bool isChanged(etag_t *etag, bool recursive) {
            bool changed = Material::isChanged(etag, recursive);

            if (recursive) {
                for (UniformMap::const_iterator itr = m_uniforms.begin(); itr != m_uniforms.end(); itr++) {
                    uniform_t u = itr->second;

                    if (u.type() == typeid(Pixmap *)) {
                        changed |= boost::any_cast<Pixmap *>(u)->isChanged(etag, recursive);
                    } else if (u.type() == typeid(Scene3D *)) {
                        changed |= boost::any_cast<Pixmap *>(u)->isChanged(etag, recursive);
                    }
                }
            }

            return changed;
        }

    private:
        std::string m_path;
        UniformMap m_uniforms;
    };
}
