var scene = new dawn.Object3D();

var plane = new dawn.PlaneGeometry(1, 1);
var ellipsis = new dawn.EllipsisGeometry(1, 1, 32);

var mesh = new dawn.Mesh3D(plane, new dawn.ShaderMaterial("shaders/color"));
mesh.material.uniform("color", new dawn.vec4(1));

scene.appendChild(mesh);

var time = function () {
    return (new Date()).getTime();
};

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
