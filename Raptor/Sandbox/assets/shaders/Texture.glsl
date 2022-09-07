#type vertex
#version 330 core

layout(location=0) in vec3 aPosition;
layout(location=1) in vec2 aTexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 vTexCoords;

void main()
{
	vTexCoords = aTexCoords;
	gl_Position = u_ViewProjection * u_Transform * vec4(aPosition,1.0);
}

#type fragment
#version 330 core

layout(location=0) out vec4 color;

in vec2 vTexCoords;
uniform sampler2D uTexture;

void main()
{
	color = texture(uTexture,vTexCoords);
}		
