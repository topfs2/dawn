var camera = new dawn.OrthographicCamera(4 * 16 / 9, 4, -1, 1000);

var s = 'This is a very very very very very very very very long text';
var style = new dawn.TextStyle("Ubuntu bold 52", new dawn.vec4(1, 1, 1, 1), 1, -1, -1);
var text_image = new dawn.TextImage(s, style);

var ud = 200 / text_image.width;
var mesh = new dawn.Mesh3D(new dawn.PlaneGeometry(200, text_image.height, new dawn.vec4(0, ud, 0, 1)), new dawn.ShaderMaterial("shaders/map"));
mesh.material.uniform("map", text_image);

var scene = new dawn.Scene3D(camera, mesh, 1280, 720);

emitResizeEvent = function (width, height) {
    scene.camera.projection(width, height, -1.0, 1000.0);
    scene.size(width, height);
}

print(ud);

var time = function () {
    return (new Date()).getTime();
};

var lerp = function (v0, v1, t) {
    return v0 + t * (v1 - v0);
}

var frame = 0;
var start = time();
update = function () {
    var now = time();
    var t = (now - start) / 1000;
    var s = (1.0 + Math.sin(t / 4.0)) / 2.0;

    var u0 = lerp(0, 1.0 - ud, s);
    var u1 = u0 + ud;

    mesh.geometry.uv = new dawn.vec4(u0, u1, 0, 1);

    return scene.instance;
}
