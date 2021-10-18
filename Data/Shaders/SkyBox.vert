
attribute vec3 a_Position;
attribute vec2 a_UV;
attribute vec4 a_Color;

//uniform vec3 u_Position;
uniform vec3 u_CameraPosition;
uniform mat4 u_WorldMatrix;//Matrix used to transform from object space to model space
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

varying vec3 v_ObjectSpacePosition;

void main()
{
	v_ObjectSpacePosition=a_Position;

	vec4 objectPosition = vec4(a_Position,0);//Set to 0 so that it ignores translation
	
	//Transform from object space to world space
	//vec4 worldPosition=u_WorldMatrix*objectPosition;//Multiply the matrix by a vector. This would result in a vector as a result

	//Transform from world space to viewspace
	vec4 viewPosition=u_ViewMatrix*objectPosition;

	viewPosition.w=1;//Reset to 1 so that it

	//Transform from view space to clip space
	vec4 clipPosition =u_ProjectionMatrix* viewPosition;

    gl_Position =  clipPosition;

}
