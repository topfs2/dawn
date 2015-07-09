var camera = new dawn.OrthographicCamera(4 * 16 / 9, 4, -1, 1000);

var topGeometry = new dawn.PolygonGeometry([
    [ -1.0,  -1.0 ],
    [  1.0,  -1.0 ],
    [  1.0,   0.8 ],
    [ -1.0,   1.2 ]
]);

var midGeometry = new dawn.PolygonGeometry([
    [ -1.0,  -0.8 ],
    [  1.0,  -1.2 ],
    [  1.0,   0.8 ],
    [ -1.0,   1.2 ]
]);

var botGeometry = new dawn.PolygonGeometry([
    [ -1.0,  -0.8 ],
    [  1.0,  -1.2 ],
    [  1.0,   1.0 ],
    [ -1.0,   1.0 ]
]);

var padding = 0.1;

var style = new dawn.TextStyle("Ubuntu bold 72", new dawn.vec4(1, 1, 1, 1), 2, 200, 200);

var top = new dawn.Mesh3D(topGeometry, new dawn.ShaderMaterial("shaders/debug"));
top.material.uniform("map", new dawn.TextImage("1", style));

var mid = new dawn.Mesh3D(midGeometry, new dawn.ShaderMaterial("shaders/debug"));
mid.material.uniform("map", new dawn.TextImage("2", style));
mid.transform = dawn.mat4.translation(0, 2 + padding, 0);

var bot = new dawn.Mesh3D(botGeometry, new dawn.ShaderMaterial("shaders/debug"));
bot.material.uniform("map", new dawn.TextImage("3", style));
bot.transform = dawn.mat4.translation(0, 4 + 2 * padding, 0);

var stage = new dawn.Object3D();
stage.appendChild(top);
stage.appendChild(mid);
stage.appendChild(bot);

var scene = new dawn.Scene3D(camera, stage, 1280, 720);

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
    var s = (1.0 + Math.sin(t)) / 2.0;

    stage.transform = dawn.mat4.translation(0, s * -5.0, 0);

    return scene.instance;
}
