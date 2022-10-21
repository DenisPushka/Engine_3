#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextCoord;

out vec2 textCoord;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view; 
uniform mat4 projection; 

uniform mat4 mouse_transform;

void main()
{
	
	gl_Position = transform * mouse_transform * projection * view * model * vec4(aPos, 1.0);
	textCoord = aTextCoord;
}