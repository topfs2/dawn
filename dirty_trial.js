var camera = new dawn.OrthographicCamera(4 * 16 / 9, 4, -1, 1000);

var mesh = new dawn.Mesh3D(new dawn.PlaneGeometry(1, 1), new dawn.ShaderMaterial("shaders/map"));
mesh.material.uniform("map", new dawn.Image("kodi-thumbnail.png"));

var scene = new dawn.Scene3D(camera, mesh, 1280, 720);

emitResizeEvent = function (width, height) {
    scene.camera.projection(4.0 * width / height, 4.0, -1.0, 1000.0);
    scene.size(width, height);
}

var time = function () {
    return (new Date()).getTime();
};

var frame = 0;
var start = time();
update = function () {
    var now = time();
    var t = (now - start) / 1000;

    if (Math.floor(t) % 2 == 1) {
        frame += 0.01;
        mesh.transform = dawn.mat4.rotationZ(frame);
    }

    return scene.instance;
}
