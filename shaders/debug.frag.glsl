//
uniform sampler2D map;

varying vec2 vUV;

void main()
{
  vec4 tex = texture2D(map, vUV);
  gl_FragColor = tex + vec4(0.1, 0.1, 0.1, 0.0);
  gl_FragColor.a = tex.a;
}
