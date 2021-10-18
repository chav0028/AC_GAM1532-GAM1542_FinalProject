uniform sampler2D u_Texture;
uniform sampler2D u_Texture2;
uniform vec4 u_Color;

varying vec4 v_ModifierColor;

uniform float u_Time;

varying vec2 v_UV;
varying vec2 v_UV2;

void main()
{

	vec4 texturesColor=texture2D( u_Texture2, v_UV2 )+(v_ModifierColor);//U color could be used instead, but to change when there is a ripple a varying is used.
	gl_FragColor = texture2D( u_Texture, v_UV);

}
