#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 Projection;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 TIModelMatrix;

uniform int isCube;

out vec3 TexCoord;
// Normal of vertex in world space
out vec3 Normal;
// Position of vertex in world space
out vec3 FragPos;

void main() {
  gl_Position = Projection * ViewMatrix * ModelMatrix * vec4(position, 1.0);
  if (isCube == 0)
  {
	TexCoord = vec3(texCoord.x, texCoord.y, 0.0f);
  }
  else
  {
	TexCoord = vec3(position.x, position.y, position.z);
  }
  FragPos = vec3(ModelMatrix * vec4(position, 1.0f));
  Normal = mat3(TIModelMatrix) * normal;  
}
