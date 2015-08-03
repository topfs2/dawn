#include "GeometryUtils.h"
#include "LinearAlgebra.h"

#include "SegmentedPath.h"

#include <iostream>

using namespace dawn;
using namespace std;

bool GeometryUtils::point_in_triangle(vec2f P, vec2f A, vec2f B, vec2f C) {
    bool b0 = vec2f(P[0] - A[0], P[1] - A[1]).dot(vec2f(A[1] - B[1], B[0] - A[0])) > 0;
    bool b1 = vec2f(P[0] - B[0], P[1] - B[1]).dot(vec2f(B[1] - C[1], C[0] - B[0])) > 0;
    bool b2 = vec2f(P[0] - C[0], P[1] - C[1]).dot(vec2f(C[1] - A[1], A[0] - C[0])) > 0;

    return (b0 == b1 && b1 == b2);
}

bool GeometryUtils::is_ear(int ai, int bi, int ci, const vec2farray &vertices) {
    vec2f a = vertices[ai];
    vec2f b = vertices[bi];
    vec2f c = vertices[ci];

    for (unsigned int i = 0; i < vertices.size(); i++) {
        if (i != ai && i != bi && i != ci) {
            vec2f p = vertices[i];

            if (point_in_triangle(p, a, b, c)) {
                return false;
            }
        }
    }

    return true;
}

void GeometryUtils::triangulate_ec(const vec2farray &vertices, std::vector<uint8_t> &indices) {
    size_t triangles_length = 0;
    size_t vertices_length = vertices.size();
    if (vertices_length == 3) {
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
    } else if (vertices_length > 3) {
        int l[vertices_length], r[vertices_length];
        int i = 0;

	    for (i = 0; i < vertices_length; i++) {
		    l[i] = ((i - 1) + vertices_length) % vertices_length;
		    r[i] = ((i + 1) + vertices_length) % vertices_length;
	    }

        int loops = 0;

        i = 0;
        while (triangles_length < (vertices_length - 2)) {
            if (is_ear(l[i], i, r[i], vertices)) {
                // Add triangle
                indices.push_back(l[i]);
                indices.push_back(i);
                indices.push_back(r[i]);
                triangles_length += 1;


			    l[ r[i] ] = l[i];
			    r[ l[i] ] = r[i];
            }

            i = r[i];

            loops++;
            if (loops > 1000) {
                cerr << "Aborting triangulation!" << endl;
                return;
            }
        }
    }
}

void GeometryUtils::create_uvs(const vec2farray &vertices, vec2farray &uvs, vec4f uv) {
    float x0, x1, y0, y1;

    x0 = x1 = vertices[0][0];
    y0 = y1 = vertices[0][1];

    float u0 = uv[0];
    float u1 = uv[1];
    float v0 = uv[2];
    float v1 = uv[3];

    for (vec2farray::const_iterator itr = vertices.begin(); itr != vertices.end(); itr++) {
        x0 = std::min((*itr)[0], x0);
        x1 = std::max((*itr)[0], x1);

        y0 = std::max((*itr)[1], y0);
        y1 = std::min((*itr)[1], y1);
    }

    for (vec2farray::const_iterator itr = vertices.begin(); itr != vertices.end(); itr++) {
        float u = lerp(u0, u1, ((*itr)[0] - x0) / (x1 - x0));
        float v = lerp(v0, v1, ((*itr)[1] - y0) / (y1 - y0));

        uvs.push_back(vec2f(u, v));
    }
}

void GeometryUtils::arc(vec2farray &positions, float cx, float cy, float r, float start, float sweep)
{
    float segments = 12; // TODO Calculate this based on r and sweep angle etc.
    float degrees = (sweep) / (float)segments;

    // TODO Should it start at 0 or should it skip first if positions.size() > 0 (as in cairo)
    for (int i = 0; i < segments; i++) {
        float x = cx + cos(degrees * (float)i + start) * r;
        float y = cy + sin(degrees * (float)i + start) * r;

        positions.push_back(vec2f(x, y));
    }
}

vec2f getBezierPoint(vec2farray points, float t) {
    size_t numPoints = points.size();

    vec2f* tmp = new vec2f[numPoints];
    for (unsigned int i = 0; i < numPoints; i++) {
        tmp[i] = points[i];
    }

    int i = numPoints - 1;
    while (i > 0) {
        for (int k = 0; k < i; k++)
            tmp[k] = tmp[k] + t * ( tmp[k+1] - tmp[k] );
        i--;
    }
    vec2f answer = tmp[0];
    delete[] tmp;
    return answer;
}

void GeometryUtils::fill(const Path *path, vec2farray &positions, std::vector<uint8_t> &indices)
{
    // TODO Clean up Bezier part
    if (path->type() == CONSTANTS::SegmentedPath) {
        const SegmentedPath *spath = static_cast<const SegmentedPath *>(path);
        SegmentList segments = spath->segments();

        positions.push_back(spath->start());
        for (SegmentList::iterator itr = segments.begin(); itr != segments.end(); itr++) {
            std::cout << "Adding BezierCurveToSegment" << std::endl;

            vec2farray points = (*itr)->points();
            points.insert(points.begin(), positions.back());

            if (points.size() > 2) {
                int interpolations = (points.size() - 1) * 20;
                for (unsigned int i = 0; i < interpolations; i++) {
                    float t = (float)i / (float)interpolations;
                    positions.push_back(getBezierPoint(points, t));
                }
            }

            positions.push_back(points.back());
        }
    }

    cout << "triangulate" << endl;
    triangulate_ec(positions, indices);
}

void GeometryUtils::stroke(const Path *path, float strokewidth, vec2farray &positions, std::vector<uint8_t> &indices)
{
    float w2 = strokewidth * 0.5f;

    if (path->type() == CONSTANTS::SegmentedPath) {
        const SegmentedPath *spath = static_cast<const SegmentedPath *>(path);

        vec2f start = spath->start();
        SegmentList segments = spath->segments();
        bool finish = spath->finish();

        BezierCurveToSegment *last = NULL;
        vec2f p0 = start;

        for (SegmentList::iterator itr = segments.begin(); itr != segments.end(); itr++) {
            std::cout << "Adding BezierCurveToSegment" << std::endl;

            vec2farray points = (*itr)->points();
            vec2f p1 = points.back();

            vec2f delta = p1 - p0;
            delta.normalize();

            vec2f l = vec2f(-delta[1] * w2,  delta[0] * w2);
            vec2f r = vec2f( delta[1] * w2, -delta[0] * w2);

            cout << "delta=" << delta[0] << ", " << delta[1] <<endl;
            cout << "l=" << l[0] << ", " << l[1] <<endl;
            cout << "r=" << r[0] << ", " << r[1] <<endl;

            vec2f a = p0 + l;
            vec2f b = p1 + l;
            vec2f c = p1 + r;
            vec2f d = p0 + r;

            cout << "p0=" << p0[0] << ", " << p0[1] <<endl;
            cout << "p1=" << p1[0] << ", " << p1[1] <<endl;

            cout << "a=" << a[0] << ", " << a[1] <<endl;
            cout << "b=" << b[0] << ", " << b[1] <<endl;
            cout << "c=" << c[0] << ", " << c[1] <<endl;
            cout << "d=" << d[0] << ", " << d[1] <<endl;

            if (!finish && !last) {
                cout << "Could add start cap" << p0[0] << ", " << p0[1] <<endl;

                positions.push_back(a);
                positions.push_back(p0 - delta * w2);
                positions.push_back(d);

                indices.push_back(0);
                indices.push_back(1);
                indices.push_back(2);
            }

            uint8_t index = positions.size();

            positions.push_back(a);
            positions.push_back(b);
            positions.push_back(c);
            positions.push_back(d);

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);

            indices.push_back(index);
            indices.push_back(index + 2);
            indices.push_back(index + 3);


            if (!finish && (itr + 1) == segments.end()) {
                cout << "Could add end cap" << p1[0] << ", " << p1[1] <<endl;

                positions.push_back(b);
                positions.push_back(p1 + delta * w2);
                positions.push_back(c);

                indices.push_back(index + 4);
                indices.push_back(index + 5);
                indices.push_back(index + 6);
            }

            p0 = p1;
        }
    }

//    fill(path, positions, indices);
/*
    positions.push_back(vec2f(-1,  0));
    positions.push_back(vec2f( 0, -1));
    positions.push_back(vec2f( 1,  0));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
*/

}
