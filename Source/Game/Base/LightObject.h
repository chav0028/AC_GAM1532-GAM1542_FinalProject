#ifndef __LightObject_H__
#define __LightObject_H__

enum LightType
{
    LightType_PointLight=0,
    LightType_DirectionalLight,
    LightType_SpotLight
};


class LightObject : public GameObject
{
protected:
    Vector4 m_specularColor;
    Vector4 m_lightColor; 
    Vector3 m_constantLinearQuadraticAttenuation;
    LightType m_lightType;
	float m_angleRange;

    Vector3 m_parentPostionOffset;//The offset this object has when its a child of another object

public:
    LightObject(Scene* pScene, std::string name, Vector3 pos, Vector4 lightColor, Vector3 constantLinearQuadraticAttenuation=Vector3(0,0,0.03f),LightType lightType = LightType_PointLight, Vector3 rotation= Vector3(0,0,0));
    ~LightObject();
    
    virtual void Update(double delta);

	void SetLightType(LightType aType) { m_lightType = aType; }
    LightType GetLightType(){ return m_lightType; }

	//Light Color
	void SetLightColor(Vector4 aLightColor){ m_lightColor = aLightColor; }
    Vector4 GetLightColor(){ return m_lightColor; }

	//Specular Color
	void SetSpecularColor(Vector4 aSpecularColor) { m_specularColor = aSpecularColor; }
	Vector4 GetSpecularColor() { return m_specularColor; }

	//Attenuation
    void SetAttenuation(Vector3 aConstantLinearQuadraticAttenuation){ m_constantLinearQuadraticAttenuation = aConstantLinearQuadraticAttenuation; }
    Vector3 GetAttenuation(){ return m_constantLinearQuadraticAttenuation; }
    void SetConstantAttenuation(float aAttenuation){ m_constantLinearQuadraticAttenuation.x = aAttenuation; }
    float GetConstantAttenuation(float aAttenuation){ return m_constantLinearQuadraticAttenuation.x; }
    void SetLinearAttenuation(float aAttenuation){ m_constantLinearQuadraticAttenuation.y = aAttenuation; }
    float GetLinearAttenuation(float aAttenuation){ return m_constantLinearQuadraticAttenuation.y; }
    void SetQuadraticAttenuation(float aAttenuation){ m_constantLinearQuadraticAttenuation.z = aAttenuation; }
    float GetQuadraticAttenuation(float aAttenuation){ return m_constantLinearQuadraticAttenuation.z; }

	//Angle range-Spot lights
	float GetAngleRange() { return m_angleRange; }
	void SetAngleRange(float aAngle) { m_angleRange = aAngle; }
	
	//Quickwatch functions (Not in Gameobject since they are only used for testing and to save space.
	void SetDefaultQuickWatch(QuickWatchManager* aQuickWatch, float minPos = -50, float maxPos = 50, bool addRotation = false, bool AddAttenuation = true, bool addAngleRange = false, bool addSpecularColor = true, bool addLightColor=true, bool addPosition = true, bool addLightType=false);
	void AddPositionToQuickWatch(QuickWatchManager* aQuickWatch, float min, float max, bool addXPosition = true, bool addYPosition = true, bool addZPosition = true);
	void AddRotationToQuickWatch(QuickWatchManager* aQuickWatch, float min = 0.f, float max = 360.0f, bool addXRotation = false, bool addYRotation = true, bool addZRotation = true);
	void AddLightColorToQuickWatch(QuickWatchManager* aQuickWatch, float min = 0.0f, float max = 1.0f, bool addRed = true, bool addGreen = true, bool addBlue = true, bool addAlpha = false);
	void AddSpecularColorToQuickWatch(QuickWatchManager* aQuickWatch, float min = 0.0f, float max = 1.0f, bool addRed = true, bool addGreen = true, bool addBlue = true, bool addAlpha = false);
	void AddAttenuationToQuickWatch(QuickWatchManager* aQuickWatch, float min, float max, bool addConstant = true, bool addLinear = true, bool addQuadratic = true);
	void AddAngleRangeToQuickWatch(QuickWatchManager* aQuickWatch);
	void AddLightTypeToQuickWatch(QuickWatchManager* aQuickWatch);

};

#endif
