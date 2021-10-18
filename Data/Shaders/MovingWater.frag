uniform sampler2D u_Texture;
uniform vec4 u_Color;

varying vec2 v_UV;

void main()
{
	gl_FragColor = texture2D( u_Texture, v_UV )+u_Color;
}
