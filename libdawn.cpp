#include "libdawn.h"
#include "dawn.h"
#include "SDLImage.h"
#include "PangoCairoTextImage.h"

#include <iostream>

using namespace std;
using namespace dawn;

// TODO Error when not length == 4
vec4f duk_require_vec4f(duk_context *ctx, duk_idx_t index) {
    if (duk_is_array(ctx, index)) {
        int length = duk_get_length(ctx, index);
        float array[length];
        for (unsigned int i = 0; i < length; i++) {
            duk_get_prop_index(ctx, index, i);
            array[i] = duk_require_number(ctx, -1);
            duk_pop(ctx);
        }

        return vec4f(array[0], array[1], array[2], array[3]);
    }

    return vec4f(0, 0, 0, 0);
}

// TODO Error when not length == 2
vec2f duk_require_vec2f(duk_context *ctx, duk_idx_t index) {
    if (duk_is_array(ctx, index)) {
        int length = duk_get_length(ctx, index);
        float array[length];
        for (unsigned int i = 0; i < length; i++) {
            duk_get_prop_index(ctx, index, i);
            array[i] = duk_require_number(ctx, -1);
            duk_pop(ctx);
        }

        return vec2f(array[0], array[1]);
    }

    return vec2f(0, 0);
}

// TODO Switch to templates
vec2farray duk_require_vec2farray(duk_context *ctx, duk_idx_t index) {
    vec2farray array;
    if (duk_is_array(ctx, index)) {
        int length = duk_get_length(ctx, index);
        for (unsigned int i = 0; i < length; i++) {
            duk_get_prop_index(ctx, index, i);
            array.push_back(duk_require_vec2f(ctx, -1));
            duk_pop(ctx);
        }
    }

    return array;
}

FilterList duk_require_filterlist(duk_context *ctx, duk_idx_t index) {
    FilterList array;
    if (duk_is_array(ctx, index)) {
        int length = duk_get_length(ctx, index);
        for (unsigned int i = 0; i < length; i++) {
            duk_get_prop_index(ctx, index, i);
            array.push_back(static_cast<Filter *>(duk_require_pointer(ctx, -1)));
            duk_pop(ctx);
        }
    }

    return array;
}

uniform_t duk_require_uniform(duk_context *ctx, duk_idx_t index) {
    if (duk_is_number(ctx, index)) {
        return duk_get_number(ctx, index);
    } else if (duk_is_array(ctx, index)) {
        int length = duk_get_length(ctx, index);
        float array[length];
        for (unsigned int i = 0; i < length; i++) {
            duk_get_prop_index(ctx, index, i);
            array[i] = duk_require_number(ctx, -1);
            duk_pop(ctx);
        }

        if (length == 2) {
            return vec2f(array[0], array[1]);
        } else if (length == 3) {
            return vec3f(array[0], array[1], array[2]);
        } else if (length == 4) {
            return vec4f(array[0], array[1], array[2], array[3]);
        } else if (length == 16) {
            return mat4f(mat4f(array).transpose()); // TODO Use Map http://eigen.tuxfamily.org/dox/group__TutorialMapClass.html
        } else {
            cout << "Bad array length" << endl;
        }
    } else if (duk_is_pointer(ctx, index)) {
        Object *p = static_cast<Object *>(duk_get_pointer(ctx, index));

        if (dynamic_cast<Object3D *>(p)) {
            return uniform_t(dynamic_cast<Object3D *>(p));
        } else if (dynamic_cast<Pixmap *>(p)) {
            return uniform_t(dynamic_cast<Pixmap *>(p));
        } else {
            cout << "Bad pointer" << endl;
        }
    }

    return uniform_t();
}

extern duk_ret_t object_destroy(duk_context *ctx) {
    Object *p = static_cast<Object *>(duk_require_pointer(ctx, 0));
    cout << "Object.Destroy" << p << endl;
    delete p;

    return 0;
}

extern duk_ret_t object_id(duk_context *ctx) {
    Object *p = static_cast<Object *>(duk_require_pointer(ctx, 0));

    duk_push_string(ctx, p->id().c_str());
    return 1;
}

extern duk_ret_t object_ischanged(duk_context *ctx) {
    Object *p = static_cast<Object *>(duk_require_pointer(ctx, 0));
    duk_idx_t args = duk_get_top(ctx);

    etag_t etag = 0;
    bool recursive = false;
    if (args == 2) {
        recursive = duk_require_boolean(ctx, 1) != 0;
    } else {
        etag = duk_require_number(ctx, 1);
        recursive = duk_require_boolean(ctx, 2) != 0;
    }

    duk_push_boolean(ctx, p->isChanged(&etag, recursive) ? 1 : 0);
    return 1;
}

extern duk_ret_t textstyle_create(duk_context *ctx) {
    TextStyle *p = new TextStyle(
        duk_require_string(ctx, 0),
        duk_require_vec4f(ctx, 1),
        (CONSTANTS::TextAlign)duk_require_int(ctx, 2),
        duk_require_int(ctx, 3),
        duk_require_int(ctx, 4)
    );
    cout << "TextStyle.Create " << p << endl;

    duk_push_pointer(ctx, p);
    return 1;
}

extern duk_ret_t textstyle_font(duk_context *ctx) {
    TextStyle *p = static_cast<TextStyle *>(duk_require_pointer(ctx, 0));

    p->font(duk_require_string(ctx, 1));
    return 0;
}

extern duk_ret_t textstyle_foreground(duk_context *ctx) {
    TextStyle *p = static_cast<TextStyle *>(duk_require_pointer(ctx, 0));
    p->foreground(duk_require_vec4f(ctx, 1));

    return 0;
}

extern duk_ret_t textstyle_align(duk_context *ctx) {
    TextStyle *p = static_cast<TextStyle *>(duk_require_pointer(ctx, 0));
    p->align((CONSTANTS::TextAlign)duk_require_int(ctx, 1));

    return 0;
}

extern duk_ret_t pango_textimage_create(duk_context *ctx) {
    std::string text = duk_require_string(ctx, 0);
    TextStyle *style = static_cast<TextStyle *>(duk_require_pointer(ctx, 1));

    TextImage *p = new PangoCairoTextImage(text, style);
    cout << "PangoCairoTextImage.Create " << p << " " << p->text() << " " << p->style() << endl;

    duk_push_pointer(ctx, p);
    return 1;
}

extern duk_ret_t textimage_text(duk_context *ctx) {
    TextImage *p = static_cast<TextImage *>(duk_require_pointer(ctx, 0));
    p->text(duk_require_string(ctx, 1));

    return 0;
}

extern duk_ret_t textimage_style(duk_context *ctx) {
    TextImage *p = static_cast<TextImage *>(duk_require_pointer(ctx, 0));
    p->style(static_cast<TextStyle *>(duk_require_pointer(ctx, 1)));

    return 0;
}

extern duk_ret_t sdl_image_create(duk_context *ctx) {
    const char *path = duk_get_string(ctx, 0);

    Image *p = new SDLImage(path);
    cout << "SDLImage.Create " << p << " " << p->id() << endl;

    duk_push_pointer(ctx, p);
    return 1;
}

extern duk_ret_t pixmap_width(duk_context *ctx) {
    Image *p = static_cast<Image *>(duk_require_pointer(ctx, 0));

    duk_push_number(ctx, p->width());
    return 1;
}

extern duk_ret_t pixmap_height(duk_context *ctx) {
    Image *p = static_cast<Image *>(duk_require_pointer(ctx, 0));

    duk_push_number(ctx, p->height());
    return 1;
}

extern duk_ret_t image_path(duk_context *ctx) {
    Image *p = static_cast<Image *>(duk_require_pointer(ctx, 0));
    p->path(duk_require_string(ctx, 1));

    return 0;
}

extern duk_ret_t shaderfilter_create(duk_context *ctx) {
    const char *path = duk_get_string(ctx, 0);

    ShaderFilter *p = new ShaderFilter(path);
    cout << "ShaderFilter.Create " << p << " " << p->path() << endl;

    duk_push_pointer(ctx, p);
    return 1;
}

extern duk_ret_t shaderfilter_uniform(duk_context *ctx) {
    ShaderFilter *m = static_cast<ShaderFilter *>(duk_require_pointer(ctx, 0));
    m->uniform(duk_require_string(ctx, 1), duk_require_uniform(ctx, 2));

    return 0;
}

extern duk_ret_t grayscalefilter_create(duk_context *ctx) {
    float saturation = duk_require_number(ctx, 0);

    GrayscaleFilter *p = new GrayscaleFilter(saturation);
    cout << "GrayscaleFilter.Create " << p << " " << p->saturation() << endl;

    duk_push_pointer(ctx, p);
    return 1;
}

extern duk_ret_t grayscalefilter_saturation(duk_context *ctx) {
    GrayscaleFilter *p = static_cast<GrayscaleFilter *>(duk_require_pointer(ctx, 0));

    p->saturation(duk_get_number(ctx, 1));
    return 0;
}

extern duk_ret_t planegeometry_create(duk_context *ctx) {
    float w = duk_get_number(ctx, 0);
    float h = duk_get_number(ctx, 1);
    vec4f uv = duk_require_vec4f(ctx, 2);

    PlaneGeometry *p = new PlaneGeometry(w, h, uv);
    cout << "PlaneGeometry.Create " << p << " " << p->width() << " " << p->height() << endl;

    duk_push_pointer(ctx, p);

    return 1;
}

extern duk_ret_t planegeometry_width(duk_context *ctx) {
    PlaneGeometry *p = static_cast<PlaneGeometry *>(duk_require_pointer(ctx, 0));

    p->width(duk_get_number(ctx, 1));
    return 0;
}


extern duk_ret_t planegeometry_height(duk_context *ctx) {
    PlaneGeometry *p = static_cast<PlaneGeometry *>(duk_require_pointer(ctx, 0));

    p->height(duk_get_number(ctx, 1));
    return 0;
}

extern duk_ret_t planegeometry_size(duk_context *ctx) {
    PlaneGeometry *p = static_cast<PlaneGeometry *>(duk_require_pointer(ctx, 0));
    p->size(duk_get_number(ctx, 1), duk_get_number(ctx, 2));

    return 0;
}

extern duk_ret_t planegeometry_uv(duk_context *ctx) {
    PlaneGeometry *p = static_cast<PlaneGeometry *>(duk_require_pointer(ctx, 0));
    p->uv(duk_require_vec4f(ctx, 1));

    return 0;
}

extern duk_ret_t roundedrectanglegeometry_create(duk_context *ctx) {
    float w = duk_get_number(ctx, 0);
    float h = duk_get_number(ctx, 1);
    vec4f radius = duk_require_vec4f(ctx, 2);
    vec4f uv = duk_require_vec4f(ctx, 3);

    RoundedRectangleGeometry *p = new RoundedRectangleGeometry(w, h, radius, uv);
    cout << "RoundedRectangleGeometry.Create " << p << " " << p->width() << " " << p->height() << " " << endl;

    duk_push_pointer(ctx, p);

    return 1;
}

extern duk_ret_t roundedrectanglegeometry_radius(duk_context *ctx) {
    RoundedRectangleGeometry *p = static_cast<RoundedRectangleGeometry *>(duk_require_pointer(ctx, 0));

    p->radius(duk_require_vec4f(ctx, 1));
    return 0;
}

extern duk_ret_t ellipsisgeometry_create(duk_context *ctx) {
    int width = duk_get_number(ctx, 0);
    int height = duk_get_number(ctx, 1);
    int segments = duk_get_number(ctx, 2);
    vec4f uv = duk_require_vec4f(ctx, 3);

    EllipsisGeometry *p = new EllipsisGeometry(width, height, segments, uv);
    cout << "EllipsisGeometry.Create " << p << " " << p->width() << " " << p->height() <<  " " << p->segments() << endl;

    duk_push_pointer(ctx, p);

    return 1;
}

extern duk_ret_t ellipsisgeometry_width(duk_context *ctx) {
    EllipsisGeometry *p = static_cast<EllipsisGeometry *>(duk_require_pointer(ctx, 0));

    p->width(duk_get_number(ctx, 1));
    return 0;
}

extern duk_ret_t ellipsisgeometry_height(duk_context *ctx) {
    EllipsisGeometry *p = static_cast<EllipsisGeometry *>(duk_require_pointer(ctx, 0));

    p->height(duk_get_number(ctx, 1));
    return 0;
}

extern duk_ret_t ellipsisgeometry_segments(duk_context *ctx) {
    EllipsisGeometry *p = static_cast<EllipsisGeometry *>(duk_require_pointer(ctx, 0));

    p->segments(duk_get_number(ctx, 1));
    return 0;
}

extern duk_ret_t ellipsisgeometry_uv(duk_context *ctx) {
    EllipsisGeometry *p = static_cast<EllipsisGeometry *>(duk_require_pointer(ctx, 0));
    p->uv(duk_require_vec4f(ctx, 1));

    return 0;
}


extern duk_ret_t arcgeometry_create(duk_context *ctx) {
    float radius = duk_get_number(ctx, 0);
    float angle1 = duk_get_number(ctx, 1);
    float angle2 = duk_get_number(ctx, 2);
    int segments = duk_get_number(ctx, 3);
    vec4f uv = duk_require_vec4f(ctx, 4);

    ArcGeometry *p = new ArcGeometry(radius, angle1, angle2, segments, uv);
    cout << "ArcGeometry.Create " << p << " " << p->radius() << " " << p->angle1() << " " << p->angle2() <<  " " << p->segments() << endl;

    duk_push_pointer(ctx, p);

    return 1;
}

extern duk_ret_t arcgeometry_radius(duk_context *ctx) {
    ArcGeometry *p = static_cast<ArcGeometry *>(duk_require_pointer(ctx, 0));

    p->radius(duk_get_number(ctx, 1));
    return 0;
}

extern duk_ret_t arcgeometry_angle1(duk_context *ctx) {
    ArcGeometry *p = static_cast<ArcGeometry *>(duk_require_pointer(ctx, 0));

    p->angle1(duk_get_number(ctx, 1));
    return 0;
}

extern duk_ret_t arcgeometry_angle2(duk_context *ctx) {
    ArcGeometry *p = static_cast<ArcGeometry *>(duk_require_pointer(ctx, 0));

    p->angle2(duk_get_number(ctx, 1));
    return 0;
}


extern duk_ret_t arcgeometry_segments(duk_context *ctx) {
    ArcGeometry *p = static_cast<ArcGeometry *>(duk_require_pointer(ctx, 0));

    p->segments(duk_get_number(ctx, 1));
    return 0;
}

extern duk_ret_t arcgeometry_uv(duk_context *ctx) {
    ArcGeometry *p = static_cast<ArcGeometry *>(duk_require_pointer(ctx, 0));
    p->uv(duk_require_vec4f(ctx, 1));

    return 0;
}

extern duk_ret_t polygongeometry_create(duk_context *ctx) {
    vec2farray vertices = duk_require_vec2farray(ctx, 0);
    vec4f uv = duk_require_vec4f(ctx, 1);

    PolygonGeometry *p = new PolygonGeometry(vertices, uv);
    cout << "PolygonGeometry.Create " << p << " ";

    for (vec2farray::iterator itr = vertices.begin(); itr != vertices.end(); itr++) {
        cout << *itr << ", ";
    }
    cout << endl;

    duk_push_pointer(ctx, p);
    return 1;
}

extern duk_ret_t polygongeometry_vertices(duk_context *ctx) {
    PolygonGeometry *p = static_cast<PolygonGeometry *>(duk_require_pointer(ctx, 0));
    p->vertices(duk_require_vec2farray(ctx, 1));

    return 0;
}

extern duk_ret_t polygongeometry_uv(duk_context *ctx) {
    PolygonGeometry *p = static_cast<PolygonGeometry *>(duk_require_pointer(ctx, 0));
    p->uv(duk_require_vec4f(ctx, 1));

    return 0;
}

extern duk_ret_t shadermaterial_create(duk_context *ctx) {
    const char *path = duk_get_string(ctx, 0);

    ShaderMaterial *p = new ShaderMaterial(path);
    cout << "ShaderMaterial.Create " << p << " " << p->path() << endl;

    duk_push_pointer(ctx, p);
    return 1;
}

extern duk_ret_t shadermaterial_uniform(duk_context *ctx) {
    ShaderMaterial *m = static_cast<ShaderMaterial *>(duk_require_pointer(ctx, 0));
    m->uniform(duk_require_string(ctx, 1), duk_require_uniform(ctx, 2));

    return 0;
}

extern duk_ret_t filtermaterial_create(duk_context *ctx) {
    Pixmap *pixmap = static_cast<Pixmap *>(duk_require_pointer(ctx, 0));
    FilterList filters = duk_require_filterlist(ctx, 1);

    FilterMaterial *p = new FilterMaterial(pixmap, filters);
    cout << "FilterMaterial.Create " << p << " " << p->pixmap() << endl;

    duk_push_pointer(ctx, p);
    return 1;
}

extern duk_ret_t filtermaterial_filters(duk_context *ctx) {
    FilterMaterial *m = static_cast<FilterMaterial *>(duk_require_pointer(ctx, 0));

    m->filters(duk_require_filterlist(ctx, 1));
    return 0;
}

extern duk_ret_t scene3d_create(duk_context *ctx) {
    Camera *camera = static_cast<Camera *>(duk_require_pointer(ctx, 0));
    Object3D *stage = static_cast<Object3D *>(duk_require_pointer(ctx, 1));
    unsigned int width = duk_require_int(ctx, 2);
    unsigned int height = duk_require_int(ctx, 3);

    Scene3D *p = new Scene3D(camera, stage, width, height);
    cout << "Scene3D.Create " << p << " " << p->camera() << " " << p->stage() << " " << p->width() << " " << p->height() << endl;

    duk_push_pointer(ctx, p);
    return 1;
}

extern duk_ret_t scene3d_camera(duk_context *ctx) {
    Scene3D *p = static_cast<Scene3D *>(duk_require_pointer(ctx, 0));
    p->camera(static_cast<Camera *>(duk_require_pointer(ctx, 1)));

    return 0;
}

extern duk_ret_t scene3d_stage(duk_context *ctx) {
    duk_push_this(ctx); 
    Scene3D *p = static_cast<Scene3D *>(duk_require_pointer(ctx, 0));
    p->stage(static_cast<Object3D *>(duk_require_pointer(ctx, 1)));

    return 0;
}

extern duk_ret_t scene3d_width(duk_context *ctx) {
    Scene3D *p = static_cast<Scene3D *>(duk_require_pointer(ctx, 0));

    p->width(duk_get_number(ctx, 1));
    return 0;
}

extern duk_ret_t scene3d_height(duk_context *ctx) {
    Scene3D *p = static_cast<Scene3D *>(duk_require_pointer(ctx, 0));

    p->height(duk_get_number(ctx, 1));
    return 0;
}

extern duk_ret_t scene3d_size(duk_context *ctx) {
    Scene3D *p = static_cast<Scene3D *>(duk_require_pointer(ctx, 0));

    p->size(duk_require_int(ctx, 1), duk_require_int(ctx, 2));
    return 0;
}

extern duk_ret_t object3d_create(duk_context *ctx) {
    Object3D *p = new Object3D();
    cout << "Object3D.Create " << p << endl;

    duk_push_pointer(ctx, p);

    return 1;
}

extern duk_ret_t object3d_transform(duk_context *ctx) {
    Object3D *p = static_cast<Object3D *>(duk_require_pointer(ctx, 0));
    if (duk_is_array(ctx, 1)) {
        int length = duk_get_length(ctx, 1);
        if (length == 16) {
            float array[length];
            for (unsigned int i = 0; i < length; i++) {
                duk_get_prop_index(ctx, 1, i);
                array[i] = duk_require_number(ctx, -1);
                duk_pop(ctx);
            }

            p->transform(mat4f(array).transpose());

        } else {
            cerr << "Bad transform value" << endl;
        }
    } else {
        cerr << "Bad argument, needs to be mat4" << endl;
    }

    return 0;
}

extern duk_ret_t object3d_visible(duk_context *ctx) {
    Object3D *p = static_cast<Object3D *>(duk_require_pointer(ctx, 0));
    bool visible = duk_require_boolean(ctx, 1);

    p->visible(visible);

    return 0;
}


extern duk_ret_t object3d_appendchild(duk_context *ctx) {
    Object3D *p = static_cast<Object3D *>(duk_require_pointer(ctx, 0));
    Object3D *c = static_cast<Object3D *>(duk_require_pointer(ctx, 1));

    p->appendChild(c);

    return 0;
}

extern duk_ret_t object3d_removechild(duk_context *ctx) {
    Object3D *p = static_cast<Object3D *>(duk_require_pointer(ctx, 0));
    Object3D *c = static_cast<Object3D *>(duk_require_pointer(ctx, 1));

    p->removeChild(c);

    return 0;
}

extern duk_ret_t object3d_replacechild(duk_context *ctx) {
    Object3D *p = static_cast<Object3D *>(duk_require_pointer(ctx, 0));
    Object3D *c1 = static_cast<Object3D *>(duk_require_pointer(ctx, 1));
    Object3D *c2 = static_cast<Object3D *>(duk_require_pointer(ctx, 2));

    p->replaceChild(c1, c2);

    return 0;
}

extern duk_ret_t mesh3d_create(duk_context *ctx) {
    Geometry *g = static_cast<Geometry *>(duk_require_pointer(ctx, 0));
    Material *m = static_cast<Material *>(duk_require_pointer(ctx, 1));

    Mesh3D *p = new Mesh3D(g, m);
    cout << "Mesh3D.Create " << p << " " << p->geometry() << " " << p->material() << endl;

    duk_push_pointer(ctx, p);

    return 1;
}

extern duk_ret_t mesh3d_geometry(duk_context *ctx) {
    Mesh3D *p = static_cast<Mesh3D *>(duk_require_pointer(ctx, 0));
    p->geometry(static_cast<Geometry *>(duk_require_pointer(ctx, 1)));

    return 0;
}


extern duk_ret_t mesh3d_material(duk_context *ctx) {
    duk_push_this(ctx); 
    Mesh3D *p = static_cast<Mesh3D *>(duk_require_pointer(ctx, 0));
    p->material(static_cast<Material *>(duk_require_pointer(ctx, 1)));

    return 0;
}

extern duk_ret_t orthographiccamera_create(duk_context *ctx) {
    OrthographicCamera *p = new OrthographicCamera(
                                                    duk_require_number(ctx, 0),
                                                    duk_require_number(ctx, 1),
                                                    duk_require_number(ctx, 2),
                                                    duk_require_number(ctx, 3),
                                                    duk_require_number(ctx, 4),
                                                    duk_require_number(ctx, 5)
                                                );
    
    cout << "OrthographicCamera.Create " << p << " " << p->projection() << endl;
    duk_push_pointer(ctx, p);

    return 1;
}

extern duk_ret_t orthographiccamera_projection(duk_context *ctx) {
    OrthographicCamera *p = static_cast<OrthographicCamera *>(duk_require_pointer(ctx, 0));
    p->projection(
        duk_require_number(ctx, 1),
        duk_require_number(ctx, 2),
        duk_require_number(ctx, 3),
        duk_require_number(ctx, 4),
        duk_require_number(ctx, 5),
        duk_require_number(ctx, 6)
    );

    return 0;
}

extern duk_ret_t perspectivecamera_create(duk_context *ctx) {
    PerspectiveCamera *p = new PerspectiveCamera(
                                                    duk_require_number(ctx, 0),
                                                    duk_require_number(ctx, 1),
                                                    duk_require_number(ctx, 2),
                                                    duk_require_number(ctx, 3)
                                                );
    
    cout << "PerspectiveCamera.Create " << p << " " << p->projection() << endl;
    duk_push_pointer(ctx, p);

    return 1;
}

extern duk_ret_t perspectivecamera_projection(duk_context *ctx) {
    PerspectiveCamera *p = static_cast<PerspectiveCamera *>(duk_require_pointer(ctx, 0));
    p->projection(
        duk_require_number(ctx, 1),
        duk_require_number(ctx, 2),
        duk_require_number(ctx, 3),
        duk_require_number(ctx, 4)
    );

    return 0;
}

extern duk_ret_t dukopen_libdawn(duk_context *ctx) {
    duk_push_object(ctx);
    duk_put_function_list(ctx, -1, libdawn_funcs);
    duk_put_function_list(ctx, -1, libdawn_debug_funcs); // TODO only on debug
    duk_put_function_list(ctx, -1, libdawn_sdl_funcs); // TODO hide this from binding
    duk_put_function_list(ctx, -1, libdawn_cairopango_funcs); // TODO hide this from binding
    duk_put_number_list(ctx, -1, libdawn_consts);

    /* Return value is the module object.  It's up to the caller to decide
     * how to use the value, e.g. write to global object.
     */
    return 1;
}
