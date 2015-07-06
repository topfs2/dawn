#include "GeometryUtils.h"

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
        }
    }
}

void GeometryUtils::create_uvs(const vec2farray &vertices, vec2farray &uvs) {
    float x0, x1, y0, y1;

    x0 = x1 = vertices[0][0];
    y0 = y1 = vertices[0][1];

    for (vec2farray::const_iterator itr = vertices.begin(); itr != vertices.end(); itr++) {
        x0 = std::min((*itr)[0], x0);
        x1 = std::max((*itr)[0], x1);

        y0 = std::max((*itr)[1], y0);
        y1 = std::min((*itr)[1], y1);
    }

    for (vec2farray::const_iterator itr = vertices.begin(); itr != vertices.end(); itr++) {
        float u = ((*itr)[0] - x0) / (x1 - x0);
        float v = ((*itr)[1] - y0) / (y1 - y0);

        uvs.push_back(vec2f(u, v));
    }
}
