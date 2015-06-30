#pragma once

#include "duktape.h"

// TODO should we really duk prefix?
// TODO Switch PTRNAME to something more hidden?

#define PTRNAME ("instance")

template<class T>
void duk_put_prop_pointer(duk_context *ctx, duk_idx_t obj_index, const char *key, T *value) {
    duk_push_string(ctx, key);
    duk_push_pointer(ctx, value);
    duk_put_prop(ctx, obj_index - 2);
}

template<class T>
void duk_put_prop_object(duk_context *ctx, duk_idx_t obj_index, const char *key, T *value) {
    duk_push_string(ctx, key);
    duk_push_pointer(ctx, value);
    duk_put_prop(ctx, obj_index - 2);
}

template<class T>
T *duk_get_object(duk_context *ctx, duk_idx_t index) {
    duk_get_prop_string(ctx, index, PTRNAME);
    T *p = static_cast<T *>(duk_require_pointer(ctx, -1));
    duk_pop(ctx);

    return p;
}

template<class T>
static duk_ret_t object_destroy(duk_context *ctx) {
    T *p = duk_get_object<T>(ctx, 0);
    printf("Object.Destroy %p\n", (void *)p);
    delete p;

    //delete duk_get_object<T>(ctx, 0);
    return 0;
}

template<class T>
void duk_prepare_object(duk_context *ctx, T *p) {
    if (duk_is_constructor_call(ctx)) {
        duk_push_this(ctx);
    } else {
        duk_push_object(ctx);
    }

    p->userdata = duk_get_heapptr(ctx, -1);


//    duk_push_string(ctx, PTRNAME);
    duk_push_pointer(ctx, p);
    duk_put_prop_string(ctx, -2, PTRNAME);


    duk_push_c_function(ctx, object_destroy<T>, 1);
    duk_set_finalizer(ctx, -2);
}
