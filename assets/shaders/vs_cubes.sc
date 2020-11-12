$input i_position, i_normal, i_offset
$output v_normal, v_position

#include <bgfx_shader.sh>

void main()
{
  vec3 out_position = i_position + i_offset;
  
  v_normal = i_normal;
  v_position = out_position;
  gl_Position = mul(u_modelViewProj, vec4(i_position, 1.0));
}