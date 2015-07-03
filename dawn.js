var dawn = { };

(function () {
    var PTRNAME = "instance";

    var obj2ptr = function (o) {
        return o[PTRNAME];
    };

    var identity = function (i) {
        return i;
    };

    var object_destroy = function (o) {
        libdawn.object_destroy(obj2ptr(o));
    };

    var prepareObject = function (o, p) {
        o[PTRNAME] = p;
        Duktape.fin(o, object_destroy);

        o.isDirty = function (recursive) {
          return libdawn.object_isdirty(p, recursive);
        };

        o.clean = function () {
          libdawn.object_clean(p);
        }
    };

    var prepareMatrix = function (o, p) {
        o[PTRNAME] = p;
        Duktape.fin(o, function (o) {
            libdawn.mat_destroy(p);
        });
    };

    var prepare_prop = function (o, prefix, prop, value, parse) {
        parse = parse || identity;
        var setter = libdawn[prefix + "_" + prop];

        if (setter) {
            Object.defineProperty(o, prop, {
                get: function() { return value; },
                set: function(new_value) {
                    value = new_value;
                    setter(obj2ptr(o), parse(new_value));
                },
                enumerable: true,
                configurable: false
            });
        } else {
            print("Failed to bind to setter libdawn." + prefix + "_" + prop);
        }
    };

    var prepareObject3D = function (o) {
        prepare_prop(o, "object3d", "transform", dawn.mat4.identity(), obj2ptr);
        prepare_prop(o, "object3d", "visible", true);

        o.children = []; // TODO Needs to be read only const

        o.appendChild = function (c) {
            libdawn.object3d_appendchild(obj2ptr(o), obj2ptr(c));

            o.children.push(c);
        };

        o.removeChild = function (c) {
            libdawn.object3d_removechild(obj2ptr(o), obj2ptr(c));

            var index = o.children.indexOf(c);
            if (index > -1) {
                o.children.splice(index, 1);
            }
        };

        o.replaceChild = function (a, b) {
            libdawn.object3d_replacechild(obj2ptr(o), obj2ptr(a), obj2ptr(b));

            var index = o.children.indexOf(a);
            if (index > -1) {
                o.children[index] = b;
            }
        };
    };

    var prepareVec = function (vec, args, length) {
        var array = [];

        for (var i = 0; i < args.length; i++) {
            var arg = args[i];
            array = array.concat(obj2ptr(arg) || arg);
        }

        if (array.length == 1) {
            var v = array[0];
            for (var i = 0; i < length; i++) {
                array[i] = v;
            }
            vec[PTRNAME] = array;
        } else if (array.length == length) {
            vec[PTRNAME] = array;
        } else {
            print("OH NOES!", array, length);
            throw "Bad amount of arguments";
        }
    };

    dawn.vec2 = function () {
        prepareVec(this, [].slice.call(arguments), 2);
    };

    dawn.vec3 = function () {
        prepareVec(this, [].slice.call(arguments), 3);
    };

    dawn.vec3.prototype = {
        swizzle: function (s) {
        }
    };

    dawn.vec4 = function () {
        prepareVec(this, [].slice.call(arguments), 4);
    };

    dawn.mat4 = function () {
        this[PTRNAME] = [].slice.call(arguments);
    };

    dawn.mat4.identity = function () {
        return new dawn.mat4(1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             0, 0, 0, 1);
    };

    dawn.mat4.rotationX = function (angle) {
        var cov = Math.cos(angle);
        var siv = Math.sin(angle);

        return new dawn.mat4(1, 0,    0,   0,
                             0, cov, -siv, 0,
                             0, siv,  cov, 0,
                             0, 0,    0,   1);
    };

    dawn.mat4.rotationY = function (angle) {
        var cov = Math.cos(angle);
        var siv = Math.sin(angle);

        return new dawn.mat4(cov, 0, siv, 0,
                             0,   1, 0,   0,
                            -siv, 0, cov, 0,
                             0,   0, 0,   1);
    };

    dawn.mat4.rotationZ = function (angle) {
        var cov = Math.cos(angle);
        var siv = Math.sin(angle);

        return new dawn.mat4(cov, -siv, 0, 0,
                             siv,  cov, 0, 0,
                             0,    0,   1, 0,
                             0,    0,   0, 1);
    };

    dawn.mat4.translation = function (x, y, z) {
        return new dawn.mat4(1, 0, 0, x,
                             0, 1, 0, y,
                             0, 0, 1, z,
                             0, 0, 0, 1);
    };

    dawn.mat4.scaling = function (x, y, z) {
        return new dawn.mat4(x, 0, 0, 0,
                             0, y, 0, 0,
                             0, 0, z, 0,
                             0, 0, 0, 1);
    };

    dawn.Image = function (src) {
        prepareObject(this, libdawn.image_create(src));

        prepare_prop(this, 'image', 'path', camera);

        Object.defineProperty(this, "width", {
            get: function() { return libdawn.pixmap_width(obj2ptr(this)); },
            enumerable: true,
            configurable: false
        });

        Object.defineProperty(this, "height", {
            get: function() { return libdawn.pixmap_height(obj2ptr(this)); },
            enumerable: true,
            configurable: false
        });
    };

    dawn.Scene3D = function (camera, stage, width, height) {
        prepareObject(this, libdawn.scene3d_create(obj2ptr(camera), obj2ptr(stage), width, height));

        prepare_prop(this, 'scene3d', 'camera', camera, obj2ptr);
        prepare_prop(this, 'scene3d', 'stage', stage, obj2ptr);
        prepare_prop(this, 'scene3d', 'width', width);
        prepare_prop(this, 'scene3d', 'height', height);

        this.size = function (width, height) {
            libdawn.scene3d_size(obj2ptr(this), width, height);
        };
    };

    dawn.Object3D = function () {
        prepareObject(this, libdawn.object3d_create());

        prepareObject3D(this);
    };

    dawn.PlaneGeometry = function (width, height) {
        prepareObject(this, libdawn.planegeometry_create(width, height));

        prepare_prop(this, 'planegeometry', 'width', width);
        prepare_prop(this, 'planegeometry', 'height', height);

        this.size = function (width, height) {
            libdawn.planegeometry_size(obj2ptr(this), width, height);
        };
    };

    dawn.EllipsisGeometry = function (width, height, segments) {
        prepareObject(this, libdawn.ellipsisgeometry_create(width, height, segments));

        prepare_prop(this, 'ellipsisgeometry', 'width', width);
        prepare_prop(this, 'ellipsisgeometry', 'height', height);
        prepare_prop(this, 'ellipsisgeometry', 'segments', segments);
    };

    dawn.ShaderMaterial = function (path) {
        prepareObject(this, libdawn.shadermaterial_create(path));

        var uniforms = { };
        this.uniform = function (key, value) {
            if (value) {
                uniforms[key] = value;
                libdawn.shadermaterial_uniform(obj2ptr(this), key, obj2ptr(value) || value);
            } else {
                return uniforms[key];
            }
        };
    };

    dawn.Mesh3D = function (geometry, material) {
        prepareObject(this, libdawn.mesh3d_create(obj2ptr(geometry), obj2ptr(material)));
        prepareObject3D(this);

        prepare_prop(this, 'mesh3d', 'geometry', geometry, obj2ptr);
        prepare_prop(this, 'mesh3d', 'material', material, obj2ptr);
    };

    dawn.OrthographicCamera = function () {
        if (arguments.length == 4) {
            prepareObject(this, libdawn.orthographiccamera_create(arguments[0] / -2, arguments[0] / 2, arguments[1] / 2, arguments[1] / -2, arguments[2], arguments[3]));
        } else if (arguments.length == 6) {
            prepareObject(this, libdawn.orthographiccamera_create(arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5]));
        } else {
            throw "Bad amount of arguments";
        }

        this.projection = function () {
            if (arguments.length == 4) {
                libdawn.orthographiccamera_projection(obj2ptr(this), arguments[0] / -2, arguments[0] / 2, arguments[1] / 2, arguments[1] / -2, arguments[2], arguments[3]);
            } else if (arguments.length == 6) {
                libdawn.orthographiccamera_projection(obj2ptr(this), arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5]);
            } else {
                throw "Bad amount of arguments";
            }
        };
    };

    dawn.PerspectiveCamera = function (fovy, aspect, near, far) {
        prepareObject(this, libdawn.perspectivecamera_create(fovy, aspect, near, far));

        this.projection = function (fovy, aspect, near, far) {
            libdawn.perspectivecamera_projection(obj2ptr(this), fovy, aspect, near, far);
        };
    };
})();
