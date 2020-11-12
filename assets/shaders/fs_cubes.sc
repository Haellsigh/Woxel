$input v_normal, v_position

#include <bgfx_shader.sh>

// Simple diffuse material
struct Material {
  //SAMPLER2D diffuse;
  vec3 diffuseColor;
};

// Directional lightning
struct Light {
  vec3 direction;
  
  vec3 ambient;
  vec3 diffuse;
};

uniform Material material;
uniform Light light;

void main()
{
  // ambient
  vec3 ambient = mul(light.ambient, material.diffuseColor);
  
  // diffuse
  vec3 norm = normalize(v_normal);
  vec3 lightDirection = normalize(-light.direction);
  float diff = max(dot(norm, lightDirection), 0.0);
  vec3 diffuse = mul(light.diffuse * diff, material.diffuseColor);
  
  gl_FragColor.xyz = ambient + diffuse;
  gl_FragColor.w   = 1.0;
}