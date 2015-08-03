#pragma once

#include "Path.h"

namespace dawn
{
    class BezierCurveToSegment : public Object
    {
    public:
        BezierCurveToSegment(vec2farray points) : m_points(points) { }

        vec2farray points() const { return m_points; }
        void points(vec2farray points) { setChanged(); m_points = points; }

    private:
        vec2farray m_points;
    };

    typedef std::vector<BezierCurveToSegment *> SegmentList;

    class SegmentedPath : public Path
    {
    public:
        SegmentedPath(vec2f start, SegmentList segments, bool finish) : m_start(start), m_segments(segments), m_finish(finish) { }
        virtual ~SegmentedPath() { }

        virtual CONSTANTS::PathType type() const { return CONSTANTS::PathType::SegmentedPath; }

        vec2f start() const { return m_start; }
        void start(vec2f start) { setChanged(m_start != start); m_start = start; }

        SegmentList segments() const { return m_segments; }
        void segments(SegmentList segments) { setChanged(); m_segments = segments; }

        bool finish() const { return m_finish; }
        void finish(bool finish) { setChanged(m_finish != finish); m_finish = finish; }

        virtual bool isChanged(etag_t *etag, bool recursive) {
            bool changed = Path::isChanged(etag, recursive);

            if (recursive) {
                for (SegmentList::const_iterator itr = m_segments.begin(); itr != m_segments.end(); itr++) {
                    changed |= (*itr)->isChanged(etag, recursive);
                }
            }

            return changed;
        }

    private:
        vec2f m_start;
        SegmentList m_segments;
        bool m_finish;
    };
}
