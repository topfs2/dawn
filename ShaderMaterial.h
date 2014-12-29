#pragma once
#include <map>
#include "IMaterial.h"
#include "Image.h"
#include "types.h"

namespace dawn
{
  namespace uniforms
  {
    typedef std::map<std::string, uniform> UniformMap;
  }

  class ShaderMaterial : public IMaterial
  {
  public:
    ShaderMaterial(const std::string &path) : path(path) { }
    ShaderMaterial(const std::string &path, uniforms::UniformMap uniforms) : path(path), uniforms(uniforms) { }

    CONSTANTS::MaterialType type() const { return CONSTANTS::ShaderMaterial; }

    virtual std::string id() { return path; }

    std::string path;
    uniforms::UniformMap uniforms;
  };
}
