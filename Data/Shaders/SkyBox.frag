uniform samplerCube u_CubeMapTexture;

varying vec2 v_UV;
varying vec3 v_ObjectSpacePosition;

void main()
{
	gl_FragColor = textureCube( u_CubeMapTexture, v_ObjectSpacePosition );
}
