#version 330

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec2 texCoord;

out vec4 color;
out vec2 tex;

// Shading
out vec3 normal;
out vec4 eye;

uniform mat4 uModelViewMatrix;
uniform mat3 normalMatrix;

void main (void) 
{
  gl_Position = uModelViewMatrix * vPosition;
  color = vColor;
  tex = texCoord;

  normal = (normalMatrix * normalize(vNormal)); 
  eye = - (gl_Position); 
}
