#include "dawn_duktape_helpers.h"
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
/*
extern duk_ret_t vec_create(duk_context *ctx) {
    int length = duk_get_top(ctx);

    gsl_vector *p = gsl_vector_alloc(length);
    for (unsigned int i = 0; i < length; i++) {
        double v = duk_get_number(ctx, i);
        gsl_vector_set(p, i, v);
        cout << "array[" << i << "] = " << v << endl;
    }

    duk_push_pointer(ctx, p);

    return 1;
}

extern duk_ret_t vec_set(duk_context *ctx) {
    gsl_vector *p = static_cast<gsl_vector *>(duk_require_pointer(ctx, 0));
    int i = duk_require_int(ctx, 1);
    double v = duk_require_number(ctx, 3);

    gsl_vector_set(p, i, v);
    return 0;
}

extern duk_ret_t vec_get(duk_context *ctx) {
    gsl_vector *p = static_cast<gsl_vector *>(duk_require_pointer(ctx, 0));
    int i = duk_require_int(ctx, 1);
    double v = gsl_vector_get(p, i);

    duk_put_number(ctx, v);
    return 1;
}

extern duk_ret_t vec_destroy(duk_context *ctx) {    
    gsl_vector *p = static_cast<gsl_vector *>(duk_require_pointer(ctx, 0));
    gsl_vector_free(static_cast<gsl_vector *>(p));

    return 0;
}

extern duk_ret_t mat_create(duk_context *ctx) {
    int length = duk_get_top(ctx);

    if (length == 16) {
        gsl_matrix *p = gsl_matrix_alloc(4, 4);


        duk_push_pointer(ctx, p);

        return 1;
    }

    return 0;
}

extern duk_ret_t mat_set(duk_context *ctx) {
    gsl_matrix *p = static_cast<gsl_matrix *>(duk_require_pointer(ctx, 0));
    int i = duk_require_int(ctx, 1);
    int j = duk_require_int(ctx, 2);
    double v = duk_require_number(ctx, 3);

    gsl_vector_set(p, i, j, v);
    return 0;
}

extern duk_ret_t mat_get(duk_context *ctx) {
    gsl_vector *p = static_cast<gsl_vector *>(duk_require_pointer(ctx, 0));
    int i = duk_require_int(ctx, 1);
    int j = duk_require_int(ctx, 2);
    double v = gsl_vector_get(p, i, j);

    duk_put_number(ctx, v);
    return 1;
}

extern duk_ret_t mat_destroy(duk_context *ctx) {    
    void *p = duk_require_pointer(ctx, 0);
    gsl_matrix_free(static_cast<gsl_matrix *>(p));

    return 0;
}
*/

/*
extern duk_ret_t vec_create(duk_context *ctx) {
    int length = duk_get_top(ctx);

    cout << "Current length " << length << endl;
    if (length < 2 || length > 4) {
        cout << "Bad length " << length << endl;
    }

    float array[length];
    for (unsigned int i = 0; i < length; i++) {
        array[i] = duk_require_number(ctx, i);
        cout << "array[" << i << "] = " << array[i] << endl;
    }

    if (length == 2) {
        cout << "Creating vec2" << endl;
        duk_push_pointer(ctx, new vec2f(array[0], array[1]));
    } else if (length == 3) {
        cout << "Creating vec3" << endl;
        duk_push_pointer(ctx, new vec3f(array[0], array[1], array[2]));
    } else if (length == 4) {
        cout << "Creating vec4" << endl;
        duk_push_pointer(ctx, new vec4f(array[0], array[1], array[2], array[3]));
    }

    return 1;
}

extern duk_ret_t vec_destroy(duk_context *ctx) {    
    void *p = duk_require_pointer(ctx, 0);
    int length = duk_require_number(ctx, 1);

    if (length == 4) {
        cout << "Destroying vec4" << endl;
        delete static_cast<vec4f *>(p);
    } else if (length == 3) {
        cout << "Destroying vec3" << endl;
        delete static_cast<vec3f *>(p);
    } else if (length == 2) {
        cout << "Destroying vec2" << endl;
        delete static_cast<vec2f *>(p);
    }

    return 0;
}

extern duk_ret_t mat4_create(duk_context *ctx) {
    mat4f *p = new mat4f();
    (*p) << duk_require_number(ctx, 0),  duk_require_number(ctx, 1),  duk_require_number(ctx, 2),  duk_require_number(ctx, 3),
            duk_require_number(ctx, 4),  duk_require_number(ctx, 5),  duk_require_number(ctx, 6),  duk_require_number(ctx, 7),
            duk_require_number(ctx, 8),  duk_require_number(ctx, 9),  duk_require_number(ctx, 10), duk_require_number(ctx, 11),
            duk_require_number(ctx, 12), duk_require_number(ctx, 13), duk_require_number(ctx, 14), duk_require_number(ctx, 15);
    duk_push_pointer(ctx, p);

    return 1;
}

extern duk_ret_t mat_destroy(duk_context *ctx) {    
    void *p = duk_require_pointer(ctx, 0);

    if (static_cast<mat4f *>(p)) {
        delete static_cast<mat4f *>(p);
    }

    return 0;
}
*/

extern duk_ret_t sdl_image_create(duk_context *ctx) {
    const char *path = duk_get_string(ctx, 0);

    Image *p = new SDLImage(path);
    cout << "SDLImage.Create " << p << " " << p->id() << endl;

    duk_push_pointer(ctx, p);
    return 1;
}

extern duk_ret_t image_width(duk_context *ctx) {
    Image *p = static_cast<Image *>(duk_require_pointer(ctx, 0));

    duk_push_number(ctx, p->width());
    return 1;
}

extern duk_ret_t image_height(duk_context *ctx) {
    Image *p = static_cast<Image *>(duk_require_pointer(ctx, 0));

    duk_push_number(ctx, p->height());
    return 1;
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
/*
        } else if (static_cast<vec2f *>(p)) {
            cout << "Casted to vec2f" << endl;
            m->uniform(key, static_cast<vec2f *>(p));
        } else if (static_cast<vec3f *>(p)) {
            cout << "Casted to vec3f" << endl;
            m->uniform(key, static_cast<vec3f *>(p));
        } else if (static_cast<vec4f *>(p)) {
            cout << "Casted to vec4f" << endl;
            m->uniform(key, static_cast<vec4f *>(p));
        } else if (static_cast<mat4f *>(p)) {
            cout << "Casted to mat4f" << endl;
            m->uniform(key, static_cast<mat4f *>(p));
*/
        } else {
            cout << "Bad pointer" << endl;
        }
    }

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
    
    cout << "OrthographicCamera.Create " << p << endl;
    duk_push_pointer(ctx, p);

    return 1;
}

extern duk_ret_t perspectivecamera_create(duk_context *ctx) {
    PerspectiveCamera *p = new PerspectiveCamera(
                                                    duk_require_number(ctx, 0),
                                                    duk_require_number(ctx, 1),
                                                    duk_require_number(ctx, 2),
                                                    duk_require_number(ctx, 3)
                                                );
    
    cout << "PerspectiveCamera.Create " << p << endl;
    duk_push_pointer(ctx, p);

    return 1;
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
