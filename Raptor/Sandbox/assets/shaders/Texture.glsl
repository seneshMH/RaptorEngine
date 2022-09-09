#type vertex
#version 330 core

layout(location=0) in vec3 a_Position;
layout(location=1) in vec4 a_Color;
layout(location=2) in vec2 a_TexCoords;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoords;
out vec4 v_Color;

void main()
{
	v_TexCoords = a_TexCoords;
	v_Color = a_Color;
	gl_Position = u_ViewProjection * vec4(a_Position,1.0);
}

#type fragment
#version 330 core

layout(location=0) out vec4 color;

in vec2 vTexCoords;
in vec4 v_Color;

uniform sampler2D u_Texture;
uniform vec4 u_Color;
uniform float u_TilingFactor;

void main()
{
	//color = texture(u_Texture,vTexCoords * u_TilingFactor) * u_Color;
	color = v_Color;
}		
