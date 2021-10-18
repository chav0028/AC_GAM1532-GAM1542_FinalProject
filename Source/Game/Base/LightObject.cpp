#include "CommonHeader.h"

using namespace std;

LightObject::LightObject(Scene* apScene, std::string aName, Vector3 aPos, Vector4 aLightColor, Vector3 aConstantLinearQuadraticAttenuation, LightType aLightType, Vector3 aRotation) 
	:GameObject(apScene, aName, aPos,
		aRotation, vec3(1,1,1),nullptr,nullptr,0),//By default the light object won't be drawn or renderable
    m_lightColor(aLightColor),
    m_lightType(aLightType),
    m_specularColor(aLightColor),
    m_constantLinearQuadraticAttenuation(aConstantLinearQuadraticAttenuation),
	m_angleRange(0)
{
}

LightObject::~LightObject()
{
}

void LightObject::Update(double aDelta)
{
    if (m_pParentObject != nullptr)
    {
        m_Position = GetWorldPosition();//Make the light match the parent transform.
    }

    GameObject::Update(aDelta);
}

void LightObject::SetDefaultQuickWatch(QuickWatchManager * aQuickWatch, float minPos, float maxPos, bool addRotation, bool AddAttenuation, bool addAngleRange, bool addSpecularColor, bool addLightColor, bool addPosition, bool addLightType)
{
	if (aQuickWatch != nullptr)
	{
		if(addPosition==true)
		AddPositionToQuickWatch(g_pQuickWatchManager, minPos, maxPos);
		
		if(addRotation==true)
		AddRotationToQuickWatch(g_pQuickWatchManager);
		
		if(addLightColor==true)
		AddLightColorToQuickWatch(g_pQuickWatchManager);
		
		if(addSpecularColor==true)
		AddSpecularColorToQuickWatch(g_pQuickWatchManager);
		
		if(AddAttenuation==true)
		AddAttenuationToQuickWatch(g_pQuickWatchManager, -5, 5);
		
		if(addAngleRange==true)
		AddAngleRangeToQuickWatch(g_pQuickWatchManager);
		
		if(addLightType==true)
		AddLightTypeToQuickWatch(g_pQuickWatchManager);
	}
}

void LightObject::AddPositionToQuickWatch(QuickWatchManager * aQuickWatch, float min, float max,bool addXPosition, bool addYPosition, bool addZPosition)
{
	if (aQuickWatch != nullptr)
	{
		string QuickWatchName;

		//Add X position
		if (addXPosition == true)
		{
			QuickWatchName = m_Name + "XPos";
			aQuickWatch->AddFloat(QuickWatchName.c_str(),&m_Position.x,min,max);
		}

		//Add Y position
		if (addYPosition == true)
		{
			QuickWatchName = m_Name + "YPos";
			aQuickWatch->AddFloat(QuickWatchName.c_str(), &m_Position.y, min, max);
		}

		//Add Z position
		if (addZPosition == true)
		{
			QuickWatchName = m_Name + "ZPos";
			aQuickWatch->AddFloat(QuickWatchName.c_str(), &m_Position.z, min, max);
		}
	}

}

void LightObject::AddRotationToQuickWatch(QuickWatchManager * aQuickWatch, float min, float max, bool addXRotation, bool addYRotation, bool addZRotation)
{
	if (aQuickWatch != nullptr)
	{
		string QuickWatchName;

		//Add X rotation
		if (addXRotation == true)
		{
			QuickWatchName = m_Name + "XRot";
			aQuickWatch->AddFloat(QuickWatchName.c_str(), &m_Rotation.x, min, max);
		}

		//Add Y rotation
		if (addYRotation == true)
		{
			QuickWatchName = m_Name + "YRot";
			aQuickWatch->AddFloat(QuickWatchName.c_str(), &m_Rotation.y, min, max);
		}

		//Add X rotation
		if (addZRotation == true)
		{
			QuickWatchName = m_Name + "ZRot";
			aQuickWatch->AddFloat(QuickWatchName.c_str(), &m_Rotation.z, min, max);
		}
	}
}

void LightObject::AddLightColorToQuickWatch(QuickWatchManager * aQuickWatch, float min, float max, bool addRed, bool addGreen, bool addBlue, bool addAlpha)
{
	if (aQuickWatch != nullptr)
	{
		string QuickWatchName;

		//Add Red 
		if (addRed == true)
		{
			QuickWatchName = m_Name + "LightRed";
			aQuickWatch->AddFloat(QuickWatchName.c_str(), &m_lightColor.x, min, max);
		}

		//Add Green
		if (addGreen == true)
		{
			QuickWatchName = m_Name + "LightGreen";
			aQuickWatch->AddFloat(QuickWatchName.c_str(), &m_lightColor.y, min, max);
		}

		//Add Blue
		if (addBlue == true)
		{
			QuickWatchName = m_Name + "LightBlue";
			aQuickWatch->AddFloat(QuickWatchName.c_str(), &m_lightColor.z, min, max);
		}

		//Add Alpha
		if (addAlpha == true)
		{
			QuickWatchName = m_Name + "LightAlpha";
			aQuickWatch->AddFloat(QuickWatchName.c_str(), &m_lightColor.w, min, max);
		}
	}
}

void LightObject::AddSpecularColorToQuickWatch(QuickWatchManager * aQuickWatch, float min, float max, bool addRed, bool addGreen, bool addBlue, bool addAlpha)
{
	if (aQuickWatch != nullptr)
	{
		string QuickWatchName;

		//Add Red 
		if (addRed == true)
		{
			QuickWatchName = m_Name + "SpecularRed";
			aQuickWatch->AddFloat(QuickWatchName.c_str(), &m_specularColor.x, min, max);
		}

		//Add Green
		if (addGreen == true)
		{
			QuickWatchName = m_Name + "SpecularGreen";
			aQuickWatch->AddFloat(QuickWatchName.c_str(), &m_specularColor.y, min, max);
		}

		//Add Blue
		if (addBlue == true)
		{
			QuickWatchName = m_Name + "SpecularBlue";
			aQuickWatch->AddFloat(QuickWatchName.c_str(), &m_specularColor.z, min, max);
		}

		//Add Alpha
		if (addAlpha == true)
		{
			QuickWatchName = m_Name + "SpecularAlpha";
			aQuickWatch->AddFloat(QuickWatchName.c_str(), &m_specularColor.w, min, max);
		}
	}
}

void LightObject::AddAttenuationToQuickWatch(QuickWatchManager * aQuickWatch, float min, float max, bool addConstant, bool addLinear, bool addQuadratic)
{
	if (aQuickWatch != nullptr)
	{
		string QuickWatchName;

		//Add Constant Attenuation 
		if (addConstant == true)
		{
			QuickWatchName = m_Name + "ConstantAttenuation";
			aQuickWatch->AddFloat(QuickWatchName.c_str(), &m_constantLinearQuadraticAttenuation.x, min, max);
		}

		//Add Linear Attenuation 
		if (addLinear == true)
		{
			QuickWatchName = m_Name + "LinearAttenuation";
			aQuickWatch->AddFloat(QuickWatchName.c_str(), &m_constantLinearQuadraticAttenuation.y, min, max);
		}

		//Add Quadratic Attenuation
		if (addQuadratic == true)
		{
			QuickWatchName = m_Name + "QuaraticAttenuation";
			aQuickWatch->AddFloat(QuickWatchName.c_str(), &m_constantLinearQuadraticAttenuation.z, min, max);
		}
	}
}

void LightObject::AddAngleRangeToQuickWatch(QuickWatchManager * aQuickWatch)
{
	if (aQuickWatch != nullptr)
	{
		string QuickWatchName;
		QuickWatchName = m_Name + "Angle Range";
		aQuickWatch->AddFloat(QuickWatchName.c_str(), &m_angleRange, 0.0f, 360.0f);//Range between 0 and 360
	}
}

void LightObject::AddLightTypeToQuickWatch(QuickWatchManager * aQuickWatch)
{
	if (aQuickWatch != nullptr)
	{
		string QuickWatchName;
		QuickWatchName = m_Name + "Light Type";
		//CASTING AN ENUM TO A FLOAT. CAREFUL RESULTS MAY NOT BE ACCURATE
		aQuickWatch->AddFloat(QuickWatchName.c_str(), (float*)&m_lightType, 0.0f, 2.0f);//Range between 0 and 3
	}
}

