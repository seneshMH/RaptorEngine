#type vertex
#version 330 core

layout(location=0) in vec3 aPosition;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 vTexCoords;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(aPosition,1.0);
}

#type fragment
#version 330 core

layout(location=0) out vec4 color;

uniform vec4 uColor;

void main()
{
	color = uColor;
}		
