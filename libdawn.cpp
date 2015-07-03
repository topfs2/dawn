#include "libdawn.h"
#include "dawn.h"
#include "SDLImage.h"

#include <iostream>

using namespace std;
using namespace dawn;

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

extern duk_ret_t object_isdirty(duk_context *ctx) {
    Object *p = static_cast<Object *>(duk_require_pointer(ctx, 0));
    bool recursive = duk_require_boolean(ctx, 1) != 0;

    duk_push_boolean(ctx, p->isDirty(recursive) ? 1 : 0);
    return 1;
}

extern duk_ret_t object_clean(duk_context *ctx) {
    Object *p = static_cast<Object *>(duk_require_pointer(ctx, 0));
    p->clean();

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

extern duk_ret_t planegeometry_create(duk_context *ctx) {
    int w = duk_get_number(ctx, 0);
    int h = duk_get_number(ctx, 1);

    PlaneGeometry *p = new PlaneGeometry(w, h);
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

extern duk_ret_t ellipsisgeometry_create(duk_context *ctx) {
    int width = duk_get_number(ctx, 0);
    int height = duk_get_number(ctx, 1);
    int segments = duk_get_number(ctx, 2);

    EllipsisGeometry *p = new EllipsisGeometry(width, height, segments);
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


extern duk_ret_t arcgeometry_create(duk_context *ctx) {
    float radius = duk_get_number(ctx, 0);
    float angle1 = duk_get_number(ctx, 1);
    float angle2 = duk_get_number(ctx, 2);
    int segments = duk_get_number(ctx, 3);

    ArcGeometry *p = new ArcGeometry(radius, angle1, angle2, segments);
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

extern duk_ret_t shadermaterial_create(duk_context *ctx) {
    const char *path = duk_get_string(ctx, 0);

    ShaderMaterial *p = new ShaderMaterial(path);
    cout << "ShaderMaterial.Create " << p << " " << p->path() << endl;

    duk_push_pointer(ctx, p);
    return 1;
}

extern duk_ret_t shadermaterial_uniform(duk_context *ctx) {
    ShaderMaterial *m = static_cast<ShaderMaterial *>(duk_require_pointer(ctx, 0));

    std::string key = duk_require_string(ctx, 1);

    if (duk_is_number(ctx, 2)) {
        m->uniform(key, duk_get_number(ctx, 2));
        cout << key << "=> Number" << endl;
    } else if (duk_is_array(ctx, 2)) {
        int length = duk_get_length(ctx, 2);
        float array[length];
        for (unsigned int i = 0; i < length; i++) {
            duk_get_prop_index(ctx, 2, i);
            array[i] = duk_require_number(ctx, -1);
            duk_pop(ctx);
        }

        if (length == 2) {
            m->uniform(key, vec2f(array[0], array[1]));
        } else if (length == 3) {
            m->uniform(key, vec3f(array[0], array[1], array[2]));
        } else if (length == 4) {
            m->uniform(key, vec4f(array[0], array[1], array[2], array[3]));
        } else if (length == 16) {
            m->uniform(key, mat4f(mat4f(array).transpose()));
        } else {
            cout << "Bad array length" << endl;
        }
    } else if (duk_is_pointer(ctx, 2)) {
        Object *p = static_cast<Object *>(duk_get_pointer(ctx, 2));
        cout << "Setting uniform " << key << " Pointer " << p << endl;

        if (dynamic_cast<Object3D *>(p)) {
            m->uniform(key, dynamic_cast<Object3D *>(p));
        } else if (dynamic_cast<Image *>(p)) {
            m->uniform(key, dynamic_cast<Image *>(p));
        } else {
            cout << "Bad pointer" << endl;
        }
    }

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
    duk_put_number_list(ctx, -1, libdawn_consts);

    /* Return value is the module object.  It's up to the caller to decide
     * how to use the value, e.g. write to global object.
     */
    return 1;
}
