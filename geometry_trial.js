var camera = new dawn.OrthographicCamera(4 * 16 / 9, 4, -1, 1000);

var plane = new dawn.PlaneGeometry(1, 1);
var ellipsis = new dawn.EllipsisGeometry(1, 1, 32);

var mesh = new dawn.Mesh3D(plane, new dawn.ShaderMaterial("shaders/color"));
mesh.material.uniform("color", new dawn.vec4(1));

var scene = new dawn.Scene3D(camera, mesh);

var time = function () {
    return (new Date()).getTime();
};

resize = function (width, height) {
    scene.camera = new dawn.OrthographicCamera(4.0 * width / height, 4.0, -1.0, 1000.0);
}

var frame = 0;
var start = time();
update = function () {
    var now = time();
    var t = (now - start) / 1000;

    if (Math.floor(t) % 2 == 1) {
        mesh.geometry = ellipsis;
    } else {
        mesh.geometry = plane;
    }

    return scene.instance;
}
