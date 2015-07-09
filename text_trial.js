var camera = new dawn.OrthographicCamera(4 * 16 / 9, 4, -1, 1000);

var s = '<span color="orange" font="24">YEAR</span>1983\n147<span color="orange" font="24">MIN</span>\nMP3\n36<span color="orange" font="24">TRACKS</span>'
var style = new dawn.TextStyle("Ubuntu bold 72", new dawn.vec4(1, 1, 1, 1), 1, 400, -1);
var text_image = new dawn.TextImage(s, style);

var mesh = new dawn.Mesh3D(new dawn.PlaneGeometry(text_image.width, text_image.height), new dawn.ShaderMaterial("shaders/map"));
mesh.material.uniform("map", text_image);

var scene = new dawn.Scene3D(camera, mesh, 1280, 720);

emitResizeEvent = function (width, height) {
    scene.camera.projection(width, height, -1.0, 1000.0);
    scene.size(width, height);
}

var first = true;
emitKeyEvent = function (device, key, down) {
    if (!down && device == "keyboard") {
        if (first) {
            s = "";
            first = false;
            layout.font = "Ubuntu regular 32";
        }

        if (key == "Space") {
            s += " "
        } else if (key == "Return") {
            s += "\n"
        } else if (key.length == 1) {
            s += key;
        }

        text_image.text = s;
        mesh.geometry.size(text_image.width, text_image.height);
    }
};

update = function () {
    return scene.instance;
}
