uniform sampler2D u_Texture;//OBJECT texture
uniform samplerCube u_CubeMapTexture2;//CUBE MAP texture

uniform vec3 u_CameraPosition;
uniform float u_MaterialReflectivity;
varying vec3 v_cubeMapUV;
varying vec2 v_UV;
varying vec3 v_WorldSpaceNormal;
varying vec3 v_WorldSpacePixelPosition;
uniform int u_NumberOfLights;
uniform float u_ambientPercentage;

#define light_point 0
#define light_directional 1
#define light_spot 2

struct Lights
{
    int m_lightType;
    vec3 m_worldPosition;
    vec4 m_lightColor;
    vec4 m_specularColor;
    vec3 m_attenuation;//constant Linear Quadratic
    vec3 m_facingDirection;
	float m_angleRange;
};

uniform Lights u_Lights[5];

vec4 CalculatePointLight(vec3 aWorldSpaceNormal, vec3 aWorldSpacePixelPosition,vec4 aMaterialColor,vec4 aLightColor,vec4 aSpecularColor, float aMaterialReflectivity,vec3 aWorldSpaceLightPosition,vec3 aConstantLinearQuadraticAttenuation) 
{
    float totalAttenuation;
    float distanceFromLight;
    vec4 diffusedColor;
    vec3 normal;
    vec3 directionTowardLight;
    float diffusePercentage;
	float specularPercentage;
    vec4 finalDiffusedColor;
    vec3 directionTowardView;
    vec3 halfVector;
    vec4 finalSpecularColor;

    //DIFFUSED LIGHTING		
	diffusedColor=aMaterialColor*aLightColor;//Calculate the color the material will have when lit
    normal=aWorldSpaceNormal;
    directionTowardLight=aWorldSpaceLightPosition-aWorldSpacePixelPosition; //Get the direction toward the light
   
	//Normalize the vectors
	normal=normalize(normal);
    distanceFromLight=length(directionTowardLight);

    //Save the distance from the light, before being normalized
	directionTowardLight=normalize(directionTowardLight);
    diffusePercentage=dot(normal,directionTowardLight); //Get the percentage lit
   			
	//Ensure the diffuse percentage is not less than 0.
	diffusePercentage=max(diffusePercentage,0.0);

    //Attenuation
	//constantLinearQuadraticAttenuation.x//Constant attenuation, no value change required
	aConstantLinearQuadraticAttenuation.y=aConstantLinearQuadraticAttenuation.y*distanceFromLight; //Linear attenuation, constant * distance
	aConstantLinearQuadraticAttenuation.z=aConstantLinearQuadraticAttenuation.z*(distanceFromLight*distanceFromLight);//Quadratic attenuation, constant * (distance* distance)   
	totalAttenuation=(aConstantLinearQuadraticAttenuation.x+aConstantLinearQuadraticAttenuation.y+aConstantLinearQuadraticAttenuation.z);//Total attenuation, constant attenuation + linear attenuation + quadratic attenuation
   	
	diffusePercentage/=totalAttenuation;//Apply attenuation to percent lit, percent lit/total attenuation   

	//Calculate the final diffused color the object will have
	finalDiffusedColor=diffusedColor*diffusePercentage;

    //SPECULAR LIGHTING	
	directionTowardView=u_CameraPosition-aWorldSpacePixelPosition;//Get the direction toward the light
    directionTowardView=normalize(directionTowardView); //Normalize the directin

   //Calculate the half vector, or the direction toward the fake specular light
	halfVector=directionTowardView+directionTowardLight;//Add the light and view vector to get  the direction of the half vector 
	halfVector=normalize(halfVector);//Normalize the vector
    
	specularPercentage=dot(normal,halfVector);//Get the percentage of the specular light
    
	//Ensure the specular percentage is not less than 0.
	specularPercentage=max(specularPercentage,0.0);

    //Attenuation
	specularPercentage/=totalAttenuation;
   
    //Apply attenuation to percent lit, percent lit/total attenuation	
	specularPercentage=pow(specularPercentage,aMaterialReflectivity);///Narrow the percentage affected by the specular light

    //Calculate the final specular color accordign to the percentage
	finalSpecularColor=aSpecularColor*specularPercentage;

	//Ambient light
	vec4 finalAmbientColor=diffusedColor*u_ambientPercentage;

	return (finalDiffusedColor+finalSpecularColor+finalAmbientColor);//Return the diffused and specular color that this light will have
}

vec4 CalculateDirectionalLight(vec3 aWorldSpaceNormal, vec3 aWorldSpacePixelPosition,vec4 aMaterialColor,vec4 aLightColor,vec4 aSpecularColor, float aMaterialReflectivity,vec3 aLightFacingDirection) 
{
    vec4 diffusedColor;
    vec3 normal;
    vec3 directionTowardLight;
    float diffusePercentage;
	float specularPercentage;
    vec4 finalDiffusedColor;
    vec3 directionTowardView;
    vec3 halfVector;
    vec4 finalSpecularColor;

    //DIFFUSED LIGHTING		
	diffusedColor=aMaterialColor*aLightColor;//Calculate the color the material will have when lit
    normal=aWorldSpaceNormal;
    directionTowardLight=aLightFacingDirection;//The direction the light is facing
	directionTowardLight*=-1;//Inverse the direction, so that it is the direction toward the light
   
	//Normalize the vectors
	normal=normalize(normal);

    //Save the distance from the light, before being normalized
	directionTowardLight=normalize(directionTowardLight);
    diffusePercentage=dot(normal,directionTowardLight); //Get the percentage lit
   			
	//Ensure the diffuse percentage is not less than 0.
	diffusePercentage=max(diffusePercentage,0.0);

	//Calculate the final diffused color the object will have
	finalDiffusedColor=diffusedColor*diffusePercentage;

    //SPECULAR LIGHTING	
	directionTowardView=u_CameraPosition-aWorldSpacePixelPosition;//Get the direction toward the light
    directionTowardView=normalize(directionTowardView); //Normalize the directin

   //Calculate the half vector, or the direction toward the fake specular light
	halfVector=directionTowardView+directionTowardLight;//Add the light and view vector to get  the direction of the half vector 
	halfVector=normalize(halfVector);//Normalize the vector
    
	specularPercentage=dot(normal,halfVector);//Get the percentage of the specular light
    
	//Ensure the specular percentage is not less than 0.
	specularPercentage=max(specularPercentage,0.0);
   
    //Apply attenuation to percent lit, percent lit/total attenuation	
	specularPercentage=pow(specularPercentage,aMaterialReflectivity);///Narrow the percentage affected by the specular light

    //Calculate the final specular color accordign to the percentage
	finalSpecularColor=aSpecularColor*specularPercentage;

	//Ambient light
	vec4 finalAmbientColor=diffusedColor*u_ambientPercentage;

	return (finalDiffusedColor+finalSpecularColor+finalAmbientColor);//Return the diffused and specular color that this light will have
}

vec4 CalculateSpotLight(vec3 aWorldSpaceNormal, vec3 aWorldSpacePixelPosition,vec4 aMaterialColor,vec4 aLightColor,vec4 aSpecularColor, float aMaterialReflectivity,vec3 aWorldSpaceLightPosition,vec3 aConstantLinearQuadraticAttenuation,
	vec3 aLightFacingDirection, float aAngleRange) 
{
    float totalAttenuation;
    float distanceFromLight;
    vec4 diffusedColor;
    vec3 normal;
    vec3 directionTowardLight;
    float diffusePercentage;
	float specularPercentage;
    vec4 finalDiffusedColor;
    vec3 directionTowardView;
    vec3 halfVector;
    vec4 finalSpecularColor;

	vec3 directionFromLight;
	float angleBetweenLightPixel;

    //DIFFUSED LIGHTING		
	diffusedColor=aMaterialColor*aLightColor;//Calculate the color the material will have when lit
    normal=aWorldSpaceNormal;
    directionTowardLight=aWorldSpaceLightPosition-aWorldSpacePixelPosition; //Get the direction toward the light
   
	//Normalize the vectors
	normal=normalize(normal);
    distanceFromLight=length(directionTowardLight);

    //Save the distance from the light, before being normalized
	directionTowardLight=normalize(directionTowardLight);
    diffusePercentage=dot(normal,directionTowardLight); //Get the percentage lit
   			
	//Ensure the diffuse percentage is not less than 0.
	diffusePercentage=max(diffusePercentage,0.0);

    //Attenuation
	//constantLinearQuadraticAttenuation.x//Constant attenuation, no value change required
	aConstantLinearQuadraticAttenuation.y=aConstantLinearQuadraticAttenuation.y*distanceFromLight; //Linear attenuation, constant * distance
	aConstantLinearQuadraticAttenuation.z=aConstantLinearQuadraticAttenuation.z*(distanceFromLight*distanceFromLight);//Quadratic attenuation, constant * (distance* distance)   
	totalAttenuation=(aConstantLinearQuadraticAttenuation.x+aConstantLinearQuadraticAttenuation.y+aConstantLinearQuadraticAttenuation.z);//Total attenuation, constant attenuation + linear attenuation + quadratic attenuation
   	
	diffusePercentage/=totalAttenuation;//Apply attenuation to percent lit, percent lit/total attenuation   

    //SPECULAR LIGHTING	
	directionTowardView=u_CameraPosition-aWorldSpacePixelPosition;//Get the direction toward the light
    directionTowardView=normalize(directionTowardView); //Normalize the directin

   //Calculate the half vector, or the direction toward the fake specular light
	halfVector=directionTowardView+directionTowardLight;//Add the light and view vector to get  the direction of the half vector 
	halfVector=normalize(halfVector);//Normalize the vector
    
	specularPercentage=dot(normal,halfVector);//Get the percentage of the specular light
    
	//Ensure the specular percentage is not less than 0.
	specularPercentage=max(specularPercentage,0.0);

    //Attenuation
	specularPercentage/=totalAttenuation;
   
    //Apply attenuation to percent lit, percent lit/total attenuation	
	specularPercentage=pow(specularPercentage,aMaterialReflectivity);///Narrow the percentage affected by the specular light


	//CHECK IF PIXEL IS AFFECTED BY SPOT LIGHT
	//Get the direction FROM the light
	directionFromLight=directionTowardLight*-1;
	
	//Ensure the facing direction is normalized
	aLightFacingDirection=normalize(aLightFacingDirection);

	//Both directions are normalized, use dot product to find cosine
	angleBetweenLightPixel=dot(directionTowardLight,aLightFacingDirection);

	//From the cosine, get the angle using acos
	angleBetweenLightPixel=acos(angleBetweenLightPixel);

	//convert angle to degrees, sicne the angle range is passed as degrees
	angleBetweenLightPixel=angleBetweenLightPixel*180/3.14159;
	
	//Check if the pixel is within angle range of the spot light
	if(angleBetweenLightPixel>aAngleRange)
	{	
		//If the angle is not within range set the lit percentages to 0
		diffusePercentage=0;
		specularPercentage=0;
	}

	//Calculate the final diffused color the object will have
	finalDiffusedColor=diffusedColor*diffusePercentage;

    //Calculate the final specular color accordign to the percentage
	finalSpecularColor=aSpecularColor*specularPercentage;

	//Ambient light
	vec4 finalAmbientColor=diffusedColor*u_ambientPercentage;

	return (finalDiffusedColor+finalSpecularColor+finalAmbientColor);//Return the diffused and specular color that this light will have
}

void main()
{
    vec4 finalColor;
	vec4 tempColor;

    vec4 materialColor=texture2D( u_Texture, v_UV );
	vec4 skyBoxColor=textureCube( u_CubeMapTexture2, v_cubeMapUV );
	float materialReflectivity=u_MaterialReflectivity;
	
	vec3 worldSpaceNormal=v_WorldSpaceNormal;
	vec3 worldSpacePixelPosition=v_WorldSpacePixelPosition;
	float ambientPercent=u_ambientPercentage;

	int lightType;
	vec4 lightColor;
	vec3 worldSpaceLightPosition;
    vec4 specularColor;
	vec3 constantLinearQuadraticAttenuation;
	vec3 direction;
	float angleRange;

	//Get the percentage of the skyboxcolor that will be reflected
	skyBoxColor=materialReflectivity*skyBoxColor;

	vec4 objectColor=skyBoxColor*materialColor;

    //Go through all the lights
	for(int i=0;i<u_NumberOfLights;i++)
	{
        //Get the light properties	
		lightType=u_Lights[i].m_lightType;
		worldSpaceLightPosition=u_Lights[i].m_worldPosition;
        lightColor=u_Lights[i].m_lightColor;
        specularColor=u_Lights[i].m_specularColor;
        constantLinearQuadraticAttenuation=u_Lights[i].m_attenuation;
		direction=u_Lights[i].m_facingDirection;
		direction=normalize(direction);//Ensure the direction is normallized
		angleRange=u_Lights[i].m_angleRange;

		//Point Light
		if(lightType==light_point)
		{
			tempColor=CalculatePointLight(worldSpaceNormal,worldSpacePixelPosition,objectColor,lightColor,specularColor,materialReflectivity,worldSpaceLightPosition,constantLinearQuadraticAttenuation);
		}
		else if (lightType==light_directional)//Directional light
		{
			tempColor=CalculateDirectionalLight(worldSpaceNormal,worldSpacePixelPosition,objectColor,lightColor,specularColor,materialReflectivity,direction);
		}
		else//Spot light
		{
			tempColor=CalculateSpotLight(worldSpaceNormal,worldSpacePixelPosition,objectColor,lightColor,specularColor,materialReflectivity,worldSpaceLightPosition,constantLinearQuadraticAttenuation,direction,angleRange);
		}


        //Modify the material according to its effect on light
		finalColor=finalColor+tempColor;
    }

	//finalColor+=skyBoxColor;

	gl_FragColor = finalColor;//Get the final color the object will have.
	
	gl_FragColor.a=materialColor.a;//Reset the alpha so that it never changes
    

}