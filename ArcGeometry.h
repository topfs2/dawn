#pragma once
#include "Geometry.h"

namespace dawn
{
    // TODO Switcht to androids parameters public void drawArc (RectF oval, float startAngle, float sweepAngle, boolean useCenter, Paint paint)
    class ArcGeometry : public Geometry
    {
    public:
        ArcGeometry(float radius, float angle1, float angle2, unsigned int segments) : m_radius(radius), m_angle1(angle1), m_angle2(angle2), m_segments(segments) { }

        CONSTANTS::GeometryType type() const { return CONSTANTS::ArcGeometry; }

        float radius() const { return m_radius; }
        void radius(float radius) { setChanged(m_radius != radius); m_radius = radius; }

        float angle1() const { return m_angle1; }
        void angle1(float angle1) { setChanged(m_angle1 != angle1); m_angle1 = angle1; }

        float angle2() const { return m_angle2; }
        void angle2(float angle2) { setChanged(m_angle2 != angle2); m_angle2 = angle2; }

        unsigned int segments() const { return m_segments; }
        void segments(unsigned int segments) { setChanged(m_segments != segments); m_segments = segments; }

    protected:
        float m_radius;
        float m_angle1;
        float m_angle2;
        unsigned int m_segments;
    };
}
