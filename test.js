print("wiii");

dawn = { };

print("test");

var PTRNAME = "instance";

var obj2ptr = function (o) {
    return o[PTRNAME];
};

var prepareVec = function (args, length) {
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
        return array;
    } else if (array.length == length) {
        return array;
    } else {
        print("OH NOES!", array, length);
        throw "Bad amount of arguments";
    }
};

dawn.vec2 = function () {
    var args = prepareVec([].slice.call(arguments), 2);

    if (this === window || this === dawn) {
        return new dawn.vec2(args);
    }

    this[PTRNAME] = args;
};

dawn.vec3 = function () {
    var args = prepareVec([].slice.call(arguments), 3);

    if (this === window || this === dawn) {
        return new dawn.vec3(args);
    }

    this[PTRNAME] = args;
};

dawn.vec4 = function () {
    var args = prepareVec([].slice.call(arguments), 4);

    if (this === window || this === dawn) {
        return new dawn.vec4(args);
    }

    this[PTRNAME] = args;
};

dawn.mat3 = function () {
    var args = prepareVec([].slice.call(arguments), 9);

    if (this === window || this === dawn) {
        return new dawn.mat3(args);
    }

    this[PTRNAME] = args;
};

dawn.mat4 = function () {
    var args = prepareVec([].slice.call(arguments), 16);

    if (this === window || this === dawn) {
        return new dawn.mat4(args);
    }

    this[PTRNAME] = args;
};

dawn.Element = function () {
    if (this === window || this === dawn) {
        new dawn.Element();
    }
};

dawn.createElement = function () {
    return new dawn.Element()
};

//render(new dawn.vec2(1, 2))

update = function () {
    return new dawn.vec2(1, 2);
}