#type vertex
#version 440

layout(location=0) in vec3 a_Position;
layout(location=1) in vec4 a_Color;
layout(location=2) in vec2 a_TexCoords;
layout(location=3) in float a_TexIndex;
layout(location=4) in float a_TilingFactor;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoords;
out vec4 v_Color;
out flat float v_TexIndex;
out float v_TilingFactor;

void main()
{
	v_TexCoords = a_TexCoords;
	v_Color = a_Color;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	gl_Position = u_ViewProjection * vec4(a_Position,1.0);
}

#type fragment
#version 440

layout(location=0) out vec4 color;

in vec2 v_TexCoords;
in vec4 v_Color;
in flat float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Texture[32];


void main()
{
	vec4 texColor = v_Color;
	switch(int(v_TexIndex))
	{
		case 0:  texColor *= texture(u_Texture[0],  v_TexCoords * v_TilingFactor); break;
		case 1:  texColor *= texture(u_Texture[1],  v_TexCoords * v_TilingFactor); break;
		case 2:  texColor *= texture(u_Texture[2],  v_TexCoords * v_TilingFactor); break;
		case 3:  texColor *= texture(u_Texture[3],  v_TexCoords * v_TilingFactor); break;
		case 4:  texColor *= texture(u_Texture[4],  v_TexCoords * v_TilingFactor); break;
		case 5:  texColor *= texture(u_Texture[5],  v_TexCoords * v_TilingFactor); break;
		case 6:  texColor *= texture(u_Texture[6],  v_TexCoords * v_TilingFactor); break;
		case 7:  texColor *= texture(u_Texture[7],  v_TexCoords * v_TilingFactor); break;
		case 8:  texColor *= texture(u_Texture[8],  v_TexCoords * v_TilingFactor); break;
		case 9:  texColor *= texture(u_Texture[9],  v_TexCoords * v_TilingFactor); break;
		case 10: texColor *= texture(u_Texture[10], v_TexCoords * v_TilingFactor); break;
		case 11: texColor *= texture(u_Texture[11], v_TexCoords * v_TilingFactor); break;
		case 12: texColor *= texture(u_Texture[12], v_TexCoords * v_TilingFactor); break;
		case 13: texColor *= texture(u_Texture[13], v_TexCoords * v_TilingFactor); break;
		case 14: texColor *= texture(u_Texture[14], v_TexCoords * v_TilingFactor); break;
		case 15: texColor *= texture(u_Texture[15], v_TexCoords * v_TilingFactor); break;
		case 16: texColor *= texture(u_Texture[16], v_TexCoords * v_TilingFactor); break;
		case 17: texColor *= texture(u_Texture[17], v_TexCoords * v_TilingFactor); break;
		case 18: texColor *= texture(u_Texture[18], v_TexCoords * v_TilingFactor); break;
		case 19: texColor *= texture(u_Texture[19], v_TexCoords * v_TilingFactor); break;
		case 20: texColor *= texture(u_Texture[20], v_TexCoords * v_TilingFactor); break;
		case 21: texColor *= texture(u_Texture[21], v_TexCoords * v_TilingFactor); break;
		case 22: texColor *= texture(u_Texture[22], v_TexCoords * v_TilingFactor); break;
		case 23: texColor *= texture(u_Texture[23], v_TexCoords * v_TilingFactor); break;
		case 24: texColor *= texture(u_Texture[24], v_TexCoords * v_TilingFactor); break;
		case 25: texColor *= texture(u_Texture[25], v_TexCoords * v_TilingFactor); break;
		case 26: texColor *= texture(u_Texture[26], v_TexCoords * v_TilingFactor); break;
		case 27: texColor *= texture(u_Texture[27], v_TexCoords * v_TilingFactor); break;
		case 28: texColor *= texture(u_Texture[28], v_TexCoords * v_TilingFactor); break;
		case 29: texColor *= texture(u_Texture[29], v_TexCoords * v_TilingFactor); break;
		case 30: texColor *= texture(u_Texture[30], v_TexCoords * v_TilingFactor); break;
		case 31: texColor *= texture(u_Texture[31], v_TexCoords * v_TilingFactor); break;
	}
	color = texColor;
}		
