#pragma once

#include "constants.h"
#include "Filter.h"

namespace dawn
{
  class GrayscaleFilter : public Filter
  {
  public:
    GrayscaleFilter(float saturation) : m_saturation(saturation) { }
    virtual ~GrayscaleFilter() { }

    float saturation() const { return m_saturation; }
    void saturation(float saturation) { setChanged(m_saturation != saturation); m_saturation = saturation; }

    virtual CONSTANTS::FilterType type() const { return CONSTANTS::GrayscaleFilter; }

  private:
    float m_saturation;
  };
}
