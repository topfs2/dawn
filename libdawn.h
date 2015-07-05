#pragma once

#include "duktape.h"

extern duk_ret_t object_destroy(duk_context *ctx);
extern duk_ret_t object_id(duk_context *ctx);
extern duk_ret_t object_ischanged(duk_context *ctx);

extern duk_ret_t textlayout_create(duk_context *ctx);
extern duk_ret_t textlayout_text(duk_context *ctx);
extern duk_ret_t textlayout_font(duk_context *ctx);
extern duk_ret_t textlayout_foreground(duk_context *ctx);
extern duk_ret_t textlayout_align(duk_context *ctx);

extern duk_ret_t pixmap_width(duk_context *ctx);
extern duk_ret_t pixmap_height(duk_context *ctx);

extern duk_ret_t textimage_layout(duk_context *ctx);

extern duk_ret_t image_path(duk_context *ctx);

extern duk_ret_t planegeometry_create(duk_context *ctx);
extern duk_ret_t planegeometry_width(duk_context *ctx);
extern duk_ret_t planegeometry_height(duk_context *ctx);
extern duk_ret_t planegeometry_size(duk_context *ctx);

extern duk_ret_t ellipsisgeometry_create(duk_context *ctx);
extern duk_ret_t ellipsisgeometry_width(duk_context *ctx);
extern duk_ret_t ellipsisgeometry_height(duk_context *ctx);
extern duk_ret_t ellipsisgeometry_segments(duk_context *ctx);

extern duk_ret_t arcgeometry_create(duk_context *ctx);
extern duk_ret_t arcgeometry_radius(duk_context *ctx);
extern duk_ret_t arcgeometry_angle1(duk_context *ctx);
extern duk_ret_t arcgeometry_angle2(duk_context *ctx);
extern duk_ret_t arcgeometry_segments(duk_context *ctx);

extern duk_ret_t shadermaterial_create(duk_context *ctx);
extern duk_ret_t shadermaterial_uniform(duk_context *ctx);

extern duk_ret_t scene3d_create(duk_context *ctx);
extern duk_ret_t scene3d_camera(duk_context *ctx);
extern duk_ret_t scene3d_stage(duk_context *ctx);
extern duk_ret_t scene3d_width(duk_context *ctx);
extern duk_ret_t scene3d_height(duk_context *ctx);
extern duk_ret_t scene3d_size(duk_context *ctx);

extern duk_ret_t object3d_create(duk_context *ctx);
extern duk_ret_t object3d_transform(duk_context *ctx);
extern duk_ret_t object3d_visible(duk_context *ctx);
extern duk_ret_t object3d_appendchild(duk_context *ctx);
extern duk_ret_t object3d_removechild(duk_context *ctx);
extern duk_ret_t object3d_replacechild(duk_context *ctx);

extern duk_ret_t mesh3d_create(duk_context *ctx);
extern duk_ret_t mesh3d_geometry(duk_context *ctx);
extern duk_ret_t mesh3d_material(duk_context *ctx);

extern duk_ret_t orthographiccamera_create(duk_context *ctx);
extern duk_ret_t orthographiccamera_projection(duk_context *ctx);

extern duk_ret_t perspectivecamera_create(duk_context *ctx);
extern duk_ret_t perspectivecamera_projection(duk_context *ctx);

/* Module initialization */
static const duk_function_list_entry libdawn_funcs[] = {
    { "object_destroy", object_destroy, 1 },
    { "object_id", object_id, 1 },

    { "textlayout_create", textlayout_create, 4 },
    { "textlayout_text", textlayout_text, 2 },
    { "textlayout_font", textlayout_font, 2 },
    { "textlayout_foreground", textlayout_foreground, 2 },
    { "textlayout_align", textlayout_align, 2 },

    { "pixmap_width", pixmap_width, 1 },
    { "pixmap_height", pixmap_height, 1 },

    { "textimage_layout", textimage_layout, 2 }, 

    { "image_path", image_path, 2 }, 

    { "planegeometry_create", planegeometry_create, 2 },
    { "planegeometry_width", planegeometry_width, 2 },
    { "planegeometry_height", planegeometry_height, 2 },
    { "planegeometry_size", planegeometry_size, 3 },

    { "ellipsisgeometry_create", ellipsisgeometry_create, 3 },
    { "ellipsisgeometry_width", ellipsisgeometry_width, 2 },
    { "ellipsisgeometry_height", ellipsisgeometry_height, 2 },
    { "ellipsisgeometry_segments", ellipsisgeometry_segments, 3 },

    { "arcgeometry_create", arcgeometry_create, 4 },
    { "arcgeometry_radius", arcgeometry_radius, 2 },
    { "arcgeometry_angle1", arcgeometry_angle1, 2 },
    { "arcgeometry_angle2", arcgeometry_angle2, 2 },
    { "arcgeometry_segments", arcgeometry_segments, 3 },

    { "shadermaterial_create", shadermaterial_create, 1 },
    { "shadermaterial_uniform", shadermaterial_uniform, 3 },

    { "scene3d_create", scene3d_create, 4 },
    { "scene3d_camera", scene3d_camera, 2 },
    { "scene3d_stage", scene3d_stage, 2 },
    { "scene3d_width", scene3d_width, 2 },
    { "scene3d_height", scene3d_height, 2 },
    { "scene3d_size", scene3d_size, 3 },

    { "object3d_create", object3d_create, 0 },
    { "object3d_transform", object3d_transform, 2 },
    { "object3d_visible", object3d_visible, 2 },
    { "object3d_appendchild", object3d_appendchild, 2 },
    { "object3d_removechild", object3d_removechild, 2 },
    { "object3d_replacechild", object3d_replacechild, 3 },

    { "mesh3d_create", mesh3d_create, 2 },
    { "mesh3d_geometry", mesh3d_geometry, 2 },
    { "mesh3d_material", mesh3d_material, 2 },

    { "orthographiccamera_create", orthographiccamera_create, 6 },
    { "orthographiccamera_projection", orthographiccamera_projection, 7 },

    { "perspectivecamera_create", perspectivecamera_create, 4 },
    { "perspectivecamera_projection", perspectivecamera_projection, 5 },

    { NULL, NULL, 0 }
};

static const duk_function_list_entry libdawn_debug_funcs[] = {
    // TODO Remove the below two from api, UI Shouldn't need to worry
    { "object_ischanged", object_ischanged, DUK_VARARGS },

    { NULL, NULL, 0 }
};

extern duk_ret_t sdl_image_create(duk_context *ctx);

static const duk_function_list_entry libdawn_sdl_funcs[] = {
    // TODO Remove the below two from api, UI Shouldn't need to worry
    { "image_create", sdl_image_create, 1 },

    { NULL, NULL, 0 }
};

extern duk_ret_t pango_textimage_create(duk_context *ctx);

static const duk_function_list_entry libdawn_cairopango_funcs[] = {
    // TODO Remove the below two from api, UI Shouldn't need to worry
    { "textimage_create", pango_textimage_create, 1 },

    { NULL, NULL, 0 }
};

static const duk_number_list_entry libdawn_consts[] = {
    { "FLAG_FOO", (double) (1 << 0) },
    { NULL, 0.0 }
};

/* Init function name is dukopen_<modname>. */
extern duk_ret_t dukopen_libdawn(duk_context *ctx);
