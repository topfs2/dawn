//
uniform vec4 color;
uniform sampler2D map;

varying vec2 vUV;

void main()
{
  vec4 c = texture2D(map, vUV);
  gl_FragColor = vec4(c.rgb * color.a, c.a * color.a);
}
