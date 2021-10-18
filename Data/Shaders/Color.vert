
attribute vec3 a_Position;
attribute vec2 a_UV;
attribute vec4 a_Color;

//uniform vec3 u_Position;
uniform vec3 u_CameraPosition;
uniform mat4 u_WorldMatrix;//Matrix used to transform from object space to model space
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

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

}
