uniform samplerCube u_CubeMapTexture;//CUBE MAP texture

uniform float u_MaterialReflectivity;
varying vec3 v_cubeMapUV;

void main()
{
	vec4 skyBoxColor=textureCube( u_CubeMapTexture, v_cubeMapUV );//Get color from cube Map texture
	float materialReflectivity=u_MaterialReflectivity;

	//Get the percentage of the skyboxcolor that will be reflected
	skyBoxColor=materialReflectivity*skyBoxColor;

	gl_FragColor=skyBoxColor;   

}