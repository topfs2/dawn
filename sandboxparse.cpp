#include <iostream>

#include "config.h"
#include "dawn.h"

#include "duktape.h"

using namespace std;
using namespace dawn;

void fatal(duk_context *ctx, int code, const char *msg) {
    printf("FATAL: %i %s\n", code, msg);
}

bool duk_dawn_instanceof(duk_context *ctx, const char *name, duk_idx_t index) {
    cout << "duk_instanceof " << name << " " << index << endl;
    duk_get_global_string(ctx, "dawn");
    duk_get_prop_string(ctx, -1, name);
    duk_remove(ctx, -2);

    duk_bool_t res = duk_instanceof(ctx, index - 1, -1);

    duk_pop(ctx);

    return res == 1;
}

bool duk_dawn_require_vecXf(duk_context *ctx, duk_idx_t index, float *array, size_t length) {
    duk_get_prop_string(ctx, index, "instance");

    if (duk_is_array(ctx, -1)) {
        int length = duk_get_length(ctx, -1);
        float array[length];
        for (unsigned int i = 0; i < length; i++) {
            duk_get_prop_index(ctx, -1, i);
            array[i] = duk_require_number(ctx, -1);
            duk_pop(ctx);
        }
    }

    duk_pop(ctx);

    return true;
}

vec2f duk_dawn_require_vec2f(duk_context *ctx, duk_idx_t index) {
    float array[2];
    duk_dawn_require_vecXf(ctx, index, array, 2);
    vec2f v = vec2f(array[0], array[1]);

    cout << "vec2" << v << endl;

    return v;
}

vec3f duk_dawn_require_vec3f(duk_context *ctx, duk_idx_t index) {
    float array[3];
    duk_dawn_require_vecXf(ctx, index, array, 3);
    return vec3f(array[0], array[1], array[2]);
}

vec4f duk_dawn_require_vec4f(duk_context *ctx, duk_idx_t index) {
    float array[4];
    duk_dawn_require_vecXf(ctx, index, array, 4);
    return vec4f(array[0], array[1], array[2], array[3]);
}

mat3f duk_dawn_require_mat3f(duk_context *ctx, duk_idx_t index) {
    float array[9];
    duk_dawn_require_vecXf(ctx, index, array, 9);
    return mat3f(mat3f(array).transpose());
}

mat4f duk_dawn_require_mat4f(duk_context *ctx, duk_idx_t index) {
    float array[16];
    duk_dawn_require_vecXf(ctx, index, array, 16);
    return mat4f(mat4f(array).transpose());
}

Element duk_dawn_require_element(duk_context *ctx, duk_idx_t index) {
/*
    Element scene("Scene3D", {
        { "width", 1280 },
        { "height", 720 }
    },
        Element("OrthographicCamera", {
            { "width", (4.0f * 16.0f / 9.0f) },
            { "height", 4.0f },
            { "near", -1.0f },
            { "far", 1000.0f }
        },
            Element("Mesh3D", {
                { "material", material },
                { "geometry", geometry }
            })
        )
    );
*/

    return Element("test");
}

boost::any duk_dawn_require_any(duk_context *ctx, duk_idx_t index) {
    if (duk_is_string(ctx, index)) {
        printf("value at idx_val is a string \n");
        return std::string(duk_get_string(ctx, index));
    } else if (duk_is_boolean(ctx, index)) {
        printf("value at idx_val is a boolean\n");
        return duk_get_boolean(ctx, index) == 1;
    } else if (duk_is_number(ctx, index)) {
        printf("value at idx_val is a number\n");
        return (float)duk_get_number(ctx, index); // TODO float or double?
    } else if (duk_dawn_instanceof(ctx, "vec2", index)) {
        printf("value at idx_val is an instanceof vec2\n");
        return duk_dawn_require_vec2f(ctx, index);
    } else if (duk_dawn_instanceof(ctx, "vec3", index)) {
        printf("value at idx_val is an instanceof vec3\n");
        return duk_dawn_require_vec3f(ctx, index);
    } else if (duk_dawn_instanceof(ctx, "vec4", index)) {
        printf("value at idx_val is an instanceof vec4\n");
        return duk_dawn_require_vec4f(ctx, index);
    } else if (duk_dawn_instanceof(ctx, "mat3", index)) {
        printf("value at idx_val is an instanceof mat3\n");
        return duk_dawn_require_mat3f(ctx, index);
    } else if (duk_dawn_instanceof(ctx, "mat4", index)) {
        printf("value at idx_val is an instanceof mat4\n");
        return duk_dawn_require_mat4f(ctx, index);
    } else if (duk_dawn_instanceof(ctx, "Element", index)) {
        printf("value at idx_val is an instanceof Element\n");
        return duk_dawn_require_element(ctx, index);
    } else if (duk_is_array(ctx, index)) {
        printf("value at idx_val is an array\n");
        std::vector<boost::any> array;
        size_t length = duk_get_length(ctx, index);
        for (unsigned int i = 0; i < length; i++) {
            duk_get_prop_index(ctx, index, i);
            array.push_back(duk_dawn_require_any(ctx, -1));
            duk_pop(ctx);
        }

        return array;
    } else {
        printf("value at idx_val is an Object\n");
        UniformMap obj;

        duk_enum(ctx, index, DUK_ENUM_OWN_PROPERTIES_ONLY);

        while (duk_next(ctx, -1, 1)) {
            /* [ ... enum key ] */
            std::string key(duk_to_string(ctx, -2));
            boost::any val = duk_dawn_require_any(ctx, -1);

            obj[key] = val;

            duk_pop_2(ctx);
        }

        duk_pop(ctx);

        return obj;
    }
}

duk_ret_t render(duk_context *ctx) {
  unsigned int n = duk_get_top(ctx);  /* #args */

  for (unsigned int i = 0; i < n; i++) {
    boost::any val = duk_dawn_require_any(ctx, i);
  }

  return 0;
}

int main(int argc, char *argv[]) {
    // Get the last position of '/'
    string aux(argv[0]);

    // get '/' or '\\' depending on unix/mac or windows.
#if defined(_WIN32) || defined(WIN32)
    int pos = aux.rfind('\\');
#else
    int pos = aux.rfind('/');
#endif

    // Get the path and the name
    string path = aux.substr(0,pos+1);
    string name = aux.substr(pos+1);
    // show results
    std::cout << "Path: " << path << std::endl;
    std::cout << "Name: " << name << std::endl;

    duk_context *ctx = duk_create_heap(NULL, NULL, NULL, NULL, fatal);

    duk_push_c_function(ctx, render, DUK_VARARGS);
    duk_put_global_string(ctx, "render");

    // polyfill
    duk_eval_string(ctx, "document = window = this;");

    if (duk_peval_file(ctx, "test.js") != 0) {
        printf("eval.error: %s\n", duk_safe_to_string(ctx, -1));
    }


    duk_push_global_object(ctx);
    duk_get_prop_string(ctx, -1, "update");

    if (duk_pcall(ctx, 0) != 0) {
        printf("Error: %s\n", duk_safe_to_string(ctx, -1));
    } else {
        boost::any val = duk_dawn_require_any(ctx, -1);
    }

    duk_destroy_heap(ctx);
    return 0;
}
