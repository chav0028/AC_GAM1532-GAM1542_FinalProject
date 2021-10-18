
attribute vec3 a_Position;
attribute vec2 a_UV;
attribute vec4 a_Color;

uniform vec2 u_UVOffset;
uniform vec2 u_UVScale;
varying vec2 v_UV;

void main()
{
	vec4 objectPosition = vec4(a_Position,1);
	
     gl_Position =  objectPosition;//Supposing coordinates are already in a -1 to 1 range

	//Set the UV coords
	v_UV = (a_UV/u_UVScale)+u_UVOffset;
}
