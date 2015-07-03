var camera = new dawn.OrthographicCamera(4 * 16 / 9, 4, -1, 1000);

var layout = new dawn.TextLayout("Hello world!", "Droid sans light 72", new dawn.vec4(1, 1, 1, 1), 1);
var text_image = new dawn.TextImage(layout);

var mesh = new dawn.Mesh3D(new dawn.PlaneGeometry(text_image.width, text_image.height), new dawn.ShaderMaterial("shaders/map"));
mesh.material.uniform("map", text_image);

var scene = new dawn.Scene3D(camera, mesh, 1280, 720);

var time = function () {
    return (new Date()).getTime();
};

resize = function (width, height) {
    scene.camera.projection(width, height, -1.0, 1000.0);
    scene.size(width, height);
}

var frame = 0;
var start = time();
update = function () {
    var now = time();
    var t = (now - start) / 1000;

    return scene.instance;
}
