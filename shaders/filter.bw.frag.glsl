//
uniform sampler2D map;
uniform float saturation;

varying vec2 vUV;

void main()
{
  vec4 c = texture2D(map, vUV);
  vec3 bw = vec3(0.2126 * c.r + 0.7152 * c.g + 0.0722 * c.b);
  vec3 rgb = mix(bw, c.rgb, saturation);
  gl_FragColor = vec4(rgb, c.a);
}
