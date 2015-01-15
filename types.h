#pragma once
#include <cstdint>
#include <boost/any.hpp>
#include <Eigen/Dense>
#include <map>

namespace dawn
{
  struct uuid_t {
    uuid_t() : id_(0) {}
    explicit uuid_t(uint64_t id) : id_(id) {}
    uuid_t(uint32_t index, uint32_t version) : id_(uint64_t(index) | uint64_t(version) << 32UL) {}

    uint64_t uuid() const { return id_; }

    bool operator == (const uuid_t &other) const { return id_ == other.id_; }
    bool operator != (const uuid_t &other) const { return id_ != other.id_; }
    bool operator <  (const uuid_t &other) const { return id_ < other.id_; }

    uint32_t index() const { return id_ & 0xffffffffUL; }
    uint32_t version() const { return id_ >> 32; }

  private:
    uint64_t id_;
  };

  typedef Eigen::Vector2f vec2f;
  typedef Eigen::Vector3f vec3f;
  typedef Eigen::Vector4f vec4f;

  typedef Eigen::Matrix4f mat4f;

  typedef boost::any uniform;
  typedef std::map<std::string, uniform> UniformMap;
}
