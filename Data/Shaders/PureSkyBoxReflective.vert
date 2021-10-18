attribute vec3 a_Position;
attribute vec2 a_UV;
attribute vec4 a_Color;
attribute vec3 a_Normal;

uniform vec3 u_CameraPosition;
uniform mat4 u_WorldMatrix;//Matrix used to transform from object space to model space
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

varying vec3 v_cubeMapUV;

void main()
{
	vec4 objectPosition = vec4(a_Position,1);
	
	//Transform from object space to world space
	vec4 worldPosition=u_WorldMatrix*objectPosition;//Multiply the matrix by a vector. This would result in a vector as a result
	
	//Transform from world space to viewspace
	vec4 viewPosition=u_ViewMatrix*worldPosition;

	//Transform from view space to clip space
	vec4 clipPosition =u_ProjectionMatrix* viewPosition;

    gl_Position =  clipPosition;

	//Rotate the normals
	vec4 normal=u_WorldMatrix*vec4(a_Normal,0);//Rotate and scale the normal to world space, pass 0 so that it cancels the translation.
	vec3 worldNormals=vec3(normal.x,normal.y,normal.z);//Pass the normal of the vertex

	//Get direction toward pixel
	vec3 directionTowardPixel=vec3(worldPosition.x,worldPosition.y,worldPosition.z)-u_CameraPosition;//DirectionTowardPixel=worldSpacePosOfMesh-worldSpaceCameraPos
	directionTowardPixel=normalize(directionTowardPixel);//Normalize the direction

	//Reflect to get the corresponding angle
	vec3 cubeMapUV;
	cubeMapUV=reflect(directionTowardPixel,normalize(worldNormals));

	//Set the UV coords for the cubemap
	v_cubeMapUV = cubeMapUV;
}
