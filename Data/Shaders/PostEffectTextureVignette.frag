
uniform sampler2D u_Texture;
uniform vec2 u_ScreenSize;

varying vec2 v_UV;

void main()
{
	vec4 originalColor=texture2D( u_Texture, v_UV );//Get the original color from the texture
	
	//Get circle in middle of screen
	//Get screen center position
    vec2 centerPosition = (gl_FragCoord.xy / u_ScreenSize.xy)-vec2(0.5);//-0.5 to get the center of screen

	//Account for aspect ratio
	centerPosition.x*=u_ScreenSize.x/u_ScreenSize.y;

    //determine the vector length of the center position
    float lengthCircle = length(centerPosition);

	float circleRadius=0.7;
	float softness=0.09;
	float opacity=0.5;

	//Smoothstep is used to interpolate values
	float vignette=smoothstep(circleRadius,circleRadius-softness,lengthCircle);

	//Match vignetter color with texture color
    vec4 vignetterColor=originalColor;
	vignetterColor.x=originalColor.x*vignette;
	vignetterColor.y=originalColor.y*vignette;
	vignetterColor.z=originalColor.z*vignette;

	vignetterColor.w=originalColor.w;
	vignetterColor.xyz=mix(originalColor.xyz,originalColor.xyz*vignette,0.5);//Mix used to reduce  hardness of edges


	gl_FragColor = vignetterColor;

	//gl_FragColor = vignetterColor;//Don't change the alpha value

}
