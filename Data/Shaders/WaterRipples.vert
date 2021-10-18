
attribute vec3 a_Position;
attribute vec2 a_UV;
attribute vec4 a_Color;
//uniform vec3 u_Position;
uniform vec3 u_CameraPosition;
uniform mat4 u_WorldMatrix;
//Matrix used to transform from object space to model space
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform vec2 u_UVOffset;
uniform vec2 u_UVScale;
varying vec2 v_UV;
varying vec2 v_UV2;
uniform vec4 u_CollisionPoints[50];
//Made into vec4s for the potential case to make effects on a horizontal or vertical plane. The 4th, W value will be used as
//the radius or area of effect of the collision.
uniform float u_CollisionPointsTimerPercentageLeft[50];
//The percentage in time before the ripple is over
uniform int u_NumberOfCollisionPoints;
//Number to keep track of how many collision points are set in the array.

varying vec4 v_ModifierColor;

uniform float u_Time;
//Function used to check if the vertex is withing the radius of the collision point
//This is done in 2D using X and Z coordinates, so is set for a horizontal plane
float CalculateDistance(float aRadius,vec3 aVertexPosition, vec3 aCollisionPointPosition)
{
    //r^2=(x-X2)^2+(Y-Y2)^2+(z-Z2)^2
	float xPowerOfTwo = (aVertexPosition.x - aCollisionPointPosition.x)*(aVertexPosition.x - aCollisionPointPosition.x);
    float yPowerOfTwo = (aVertexPosition.y - aCollisionPointPosition.y)*(aVertexPosition.x - aCollisionPointPosition.x);
    float zPowerOfTwo = (aVertexPosition.z - aCollisionPointPosition.z)*(aVertexPosition.z - aCollisionPointPosition.z);

    float distanceBetweenPoints = sqrt(xPowerOfTwo + yPowerOfTwo+zPowerOfTwo);
    //Calculate distance between the collision point and vertex

	return distanceBetweenPoints;
}


void main()
{
    vec4 objectPosition = vec4(a_Position,1);
    //Transform from object space to world space
	vec4 worldPosition=u_WorldMatrix*objectPosition;
    //Multiply the matrix by a vector. This would result in a vector as a result

	//Ripples Effect
	/***************************************************************************************************************************/

	bool isWaving=false;
    //Variable in case we want to do something only once if the objecti s in a wave.
	vec4 startingWorldPosition=worldPosition;
    //Save the world position before any changes so that we can use it as a variable in the operations of sin wave.
	//Generally this wouldn't cause an issue, but since the value of world position will change is better to get the starting position before any transformation was applied to be safe.

	//Transform everything in World position, since the collision points are in World position
	for(int i=0;i<u_NumberOfCollisionPoints;i++)
	{
        //Calculate the distance between the vertex and the collision hit points
		float distanceBetween=CalculateDistance(u_CollisionPoints[i].w,vec3(startingWorldPosition.x,startingWorldPosition.y,startingWorldPosition.z),vec3(u_CollisionPoints[i].x,u_CollisionPoints[i].y,u_CollisionPoints[i].z));
        
		
		//Check if this vertex is being affected by the collision point ripple
		if(distanceBetween<=u_CollisionPoints[i].w)//Check if the distance is smaller than the radius
		{
            float circularPosition=(startingWorldPosition.x*startingWorldPosition.x)+(startingWorldPosition.z*startingWorldPosition.z);
            //Used so that the ripple is in a circle around the position
			//Use x or z position so that its more wavy rather than as an impact
			
			float percentDistance=1-distanceBetween/u_CollisionPoints[i].w;  //Get the percentage of the distance
			
			//Apply ripple
			worldPosition.y	+=sin((circularPosition+u_Time));//Move the y Position
            
			worldPosition.y*=u_CollisionPointsTimerPercentageLeft[i];//Change amplitude of wave according to how much time it has left so that it will start strong and end smoothly (supposedly)
            worldPosition.y*=0.5;//Made the ripple slightly small so that it looks better and blend well with the water

			worldPosition.y*=percentDistance;//Use the distance percent to control the effects of the ripple

			//worldPosition.y+=sin(distanceBetween-u_Time)*u_CollisionPointsTimerPercentageLeft[i];//Alternative way using the hat formula, doesn't look as good.

			isWaving=true;
			
		}
	}

	/***************************************************************************************************************************/

	//Transform from world space to viewspace
	vec4 viewPosition=u_ViewMatrix*worldPosition;
    //Transform from view space to clip space
	vec4 clipPosition =u_ProjectionMatrix* viewPosition;
    gl_Position =  clipPosition;

	if(isWaving=true)
	{
	//Basically blur the vertices being affected by the ripple
		v_UV.x=(worldPosition.x/0.5)+15;//Change its uv coordinates slightly to highlight the ripples
		v_UV.y=(worldPosition.y/0.5)+15;
		v_ModifierColor=vec4(1,1,1,1);//Slightly change the color, since they are varying the change will be minimal
	}

    //Set the UV coords
	v_UV.x = (a_UV.x/1.5)+u_Time/3;
    v_UV.y=(a_UV.y/1.5)+u_Time/4;
    v_UV2=(a_UV/1)+u_Time/25;
	v_ModifierColor=vec4(0,0.3,0.3,1);
}

