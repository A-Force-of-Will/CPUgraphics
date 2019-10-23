#version 430
	
layout (location = 0) in vec3 vposition;
layout (location = 1) in vec3 vcolor;
layout (location = 2) in vec2 vtexcoord;

out vec3 fcolor;
out vec2 ftexcoord;

uniform mat4 mvp_matrix;

void main()
{
	fcolor = vcolor;
	ftexcoord = vtexcoord; //* 2.0;
	gl_Position = mvp_matrix * vec4(vposition, 1.0);
}
