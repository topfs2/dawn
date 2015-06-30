var scene = new dawn.Object3D();

var map = new dawn.Mesh3D(new dawn.PlaneGeometry(1, 1), new dawn.ShaderMaterial("shaders/map"));
map.material.uniform("map", new dawn.Image("kodi-thumbnail.png"));
map.material.uniform("color", new dawn.vec4(1));

scene.appendChild(map);

var frame = 0;
update = function () {
    frame += 0.01;

    map.transform = dawn.mat4.rotationY(frame);

    return scene.instance;
}
