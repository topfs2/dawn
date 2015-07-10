var camera = new dawn.OrthographicCamera(4 * 16 / 9, 4, -1, 1000);

var geometries = [];
geometries.push(new dawn.PlaneGeometry(1, 1));
geometries.push(new dawn.RoundedRectangleGeometry(1, 1, new dawn.vec4(0, 0.1, 0.2, 0.3)));
geometries.push(new dawn.EllipsisGeometry(1, 1, 32));
geometries.push(new dawn.ArcGeometry(1, 1, 2, 32));
geometries.push(new dawn.PolygonGeometry([
    [ -1,    -1.5 ],
    [  1.5,  -1.5 ],
    [  1,     1.5 ],
    [ -1.5,   1.5 ]
]));

var mesh = new dawn.Mesh3D(geometries[0], new dawn.ShaderMaterial("shaders/color"));
mesh.material.uniform("color", new dawn.vec4(1));

var scene = new dawn.Scene3D(camera, mesh, 1280, 720);

var time = function () {
    return (new Date()).getTime();
};

emitResizeEvent = function (width, height) {
    scene.camera.projection(4.0 * width / height, 4.0, -1.0, 1000.0);
    scene.size(width, height);
}

var frame = 0;
var start = time();
update = function () {
    var now = time();
    var t = (now - start) / 1000;

    var index = Math.floor(t) % geometries.length;
    mesh.geometry = geometries[index];

    return scene.instance;
}

