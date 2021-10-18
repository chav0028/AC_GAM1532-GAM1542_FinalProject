
uniform sampler2D u_Texture;

varying vec2 v_UV;


void main()
{
	vec4 originalColor=texture2D( u_Texture, v_UV );//Get the original color from the texture
	vec4 sumOfRGBA=vec4(0,0,0,0);

	//Amount in pixels to blur
	float blur=0.0005;
	
	//Direction of blur, make player appear to go "Fast" , so blur back
	float XStep=-1.0;
	float yStep=0.0;//No y only horizontal

	//Blur with predefined gaussian values
    sumOfRGBA += texture2D(u_Texture, vec2(v_UV.x - 4.0*blur*XStep, v_UV.y - 4.0*blur*yStep)) * 0.0162162162;
    sumOfRGBA += texture2D(u_Texture, vec2(v_UV.x - 3.0*blur*XStep, v_UV.y - 3.0*blur*yStep)) * 0.0540540541;
    sumOfRGBA += texture2D(u_Texture, vec2(v_UV.x - 2.0*blur*XStep, v_UV.y - 2.0*blur*yStep)) * 0.1216216216;
    sumOfRGBA += texture2D(u_Texture, vec2(v_UV.x - 1.0*blur*XStep, v_UV.y - 1.0*blur*yStep)) * 0.1945945946;

    sumOfRGBA += texture2D(u_Texture, vec2(v_UV.x, v_UV.y)) * 0.2270270270;

    sumOfRGBA += texture2D(u_Texture, vec2(v_UV.x + 1.0*blur*XStep, v_UV.y + 1.0*blur*yStep)) * 0.1945945946;
    sumOfRGBA += texture2D(u_Texture, vec2(v_UV.x + 2.0*blur*XStep, v_UV.y + 2.0*blur*yStep)) * 0.1216216216;
    sumOfRGBA += texture2D(u_Texture, vec2(v_UV.x + 3.0*blur*XStep, v_UV.y + 3.0*blur*yStep)) * 0.0540540541;
    sumOfRGBA += texture2D(u_Texture, vec2(v_UV.x + 4.0*blur*XStep, v_UV.y + 4.0*blur*yStep)) * 0.0162162162;


    gl_FragColor = vec4(sumOfRGBA.xyz, 1.0);
	//gl_FragColor = originalColor*vec4(sumOfRGBA.xyz, 1.0);
}
