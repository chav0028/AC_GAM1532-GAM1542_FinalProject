
attribute vec3 a_Position;
attribute vec2 a_UV;
attribute vec4 a_Color;

//uniform vec3 u_Position;
uniform vec3 u_CameraPosition;
uniform mat4 u_WorldMatrix;//Matrix used to transform from object space to model space
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

uniform vec2 u_UVOffset;
uniform vec2 u_UVScale;
varying vec2 v_UV;

uniform float u_Time;

void main()
{
	vec4 objectPosition = vec4(a_Position,1);

	//Transform from object space to world space
	vec4 worldPosition=u_WorldMatrix*objectPosition;//Multiply the matrix by a vector. This would result in a vector as a result

	//Vertical plane
	worldPosition.y+=sin(worldPosition.x+u_Time*0.5)*0.6;
	worldPosition.y+=sin(worldPosition.z+u_Time*2)*0.2;

	//Transform from world space to viewspace
	vec4 viewPosition=u_ViewMatrix*worldPosition;

	//Transform from view space to clip space
	vec4 clipPosition =u_ProjectionMatrix* viewPosition;

    gl_Position =  clipPosition;

	//Set the UV coords
	vec2 uvCoords=a_UV;
	vec2 uvOffset=u_UVOffset;
	//uvOffset.y+=sin((a_Position.x+u_Time)+0.5)/40;//Animate the texture, multiply inside parentheses to control speed and outside to control amplitude
	uvOffset.x+=u_Time*0.05;//Animate the texture, multiply inside parentheses to control speed and outside to control amplitude

	v_UV = (uvCoords/u_UVScale)+uvOffset;
}
