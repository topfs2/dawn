//
uniform sampler2D map;
uniform float strength;
uniform vec2 texelSize;

varying vec2 vUV;

void main(void)
{
  vec4 sum = vec4(0.0);
  float blurSize = texelSize.x;

  // blur in y (vertical)
  // take nine samples, with the distance blurSize between them
  sum += texture2D(map, vec2(vUV.x - 4.0*blurSize, vUV.y)) * 0.05;
  sum += texture2D(map, vec2(vUV.x - 3.0*blurSize, vUV.y)) * 0.09;
  sum += texture2D(map, vec2(vUV.x - 2.0*blurSize, vUV.y)) * 0.12;
  sum += texture2D(map, vec2(vUV.x - blurSize,     vUV.y)) * 0.15;
  sum += texture2D(map, vec2(vUV.x,                vUV.y)) * 0.16;
  sum += texture2D(map, vec2(vUV.x + blurSize,     vUV.y)) * 0.15;
  sum += texture2D(map, vec2(vUV.x + 2.0*blurSize, vUV.y)) * 0.12;
  sum += texture2D(map, vec2(vUV.x + 3.0*blurSize, vUV.y)) * 0.09;
  sum += texture2D(map, vec2(vUV.x + 4.0*blurSize, vUV.y)) * 0.05;

  gl_FragColor = sum;
}
