var scene = new dawn.Object3D();

var map = new dawn.Mesh3D(new dawn.PlaneGeometry(1, 1), new dawn.ShaderMaterial("shaders/map"));
map.material.uniform("map", new dawn.Image("kodi-thumbnail.png"));

scene.appendChild(map);

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
        map.transform = dawn.mat4.rotationZ(frame);
    }

    return scene.instance;
}
