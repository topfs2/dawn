var camera = new dawn.OrthographicCamera(4 * 16 / 9, 4, -1, 1000);

var bg = new dawn.Mesh3D(new dawn.PlaneGeometry(4 * 16 / 9, 4), new dawn.ShaderMaterial("shaders/map"));
bg.material.uniform("map", new dawn.Image("bg.jpg"));

var image = new dawn.Image("kodi-thumbnail.png");

var filters = [
//  new dawn.BlurFilter(10),
    new dawn.GrayscaleFilter(1.0),
//    new dawn.GrayscaleFilter(0.0)
//    new dawn.ShaderFilter("shaders/filter.bv"),
//    new dawn.ShaderFilter("shaders/filter.bh")
//  new dawn.PixelateFilter(32)
];

var mesh = new dawn.Mesh3D(new dawn.PlaneGeometry(1, 1), new dawn.FilterMaterial(image, filters));


var stage = new dawn.Object3D();
stage.appendChild(bg);
stage.appendChild(mesh);

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

//    filters[0].saturation = s;

    return scene.instance;
}
