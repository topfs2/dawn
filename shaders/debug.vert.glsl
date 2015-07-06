//
attribute vec4 aPosition;
attribute vec2 aUV;

varying vec2 vUV;
uniform mat4 uMVP;

void main(void)
{
  vUV = aUV;
  gl_Position = uMVP * aPosition;
}
