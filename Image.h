#pragma once
#include <string>
#include <cstring>
#include <boost/shared_ptr.hpp>

#include "hashlib/md5.h"
#include "constants.h"
#include "Object.h"

namespace dawn
{
  class Buffer : public Object
  {
  public:
    Buffer(const void *data, size_t length)
    {
      m_data = new uint8_t[length];
      std::memcpy(m_data, data, length);
      m_length = length;

      MD5 md5;
      m_hash = md5(data, length);
    }

    virtual ~Buffer()
    {
      delete [] m_data;
    }

    const uint8_t *raw() const
    {
      return m_data;
    }

    std::string hash() const
    {
      return m_hash;
    }

  private:
    uint8_t *m_data;
    size_t m_length;
    std::string m_hash;
  };

  typedef boost::shared_ptr<Buffer> BufferPtr;

  class Image : public Object
  {
  public:
    Image(unsigned int width, unsigned int height, CONSTANTS::PixelFormat format) : width(width), height(height), format(format) { }
    virtual ~Image() { }

    virtual BufferPtr buffer() = 0;

    unsigned int width;
    unsigned int height;
    CONSTANTS::PixelFormat format;
  };

  typedef boost::shared_ptr<Image> ImagePtr;

  class BufferImage : public Image
  {
  public:
    BufferImage(BufferPtr buffer, unsigned int width, unsigned int height, CONSTANTS::PixelFormat format) : m_buffer(buffer), Image(width, height, format) { }

    virtual std::string id() { return m_buffer->hash(); }
    virtual BufferPtr buffer() { return m_buffer; }

  private:
    BufferPtr m_buffer;
  };
}
