#ifndef __Scene_H__
#define __Scene_H__

const unsigned int MAX_RENDER_ORDER = 5;

const float DEFAULT_PROJECTION_SCALE = 5;
const float DEFAULT_HUD_PROJECTION_SCALE = 5;

const unsigned int MAX_NUMBER_OF_LIGHTS = 5;

class LightObject;
class ParticleSystem;

class Scene
{
protected:
    std::map<std::string, GameObject*> m_pGameObjects;
    std::vector<GameObject*> m_pInputHandlerObjects;
    std::vector<Box2DJointInfo> m_pSceneJoints;
	std::vector<LightObject*> m_pSceneLights;
	std::vector<ParticleSystem*>m_pParticleSystems;
	std::vector<ShaderProgram*> m_pLinkedPostProcessingShaders;

    bool m_updateable;
    bool m_drawable;
    bool m_sceneIsOpaque;
	bool m_usePostProcessEffects;

    Box2DWorld* m_pSceneWorld;
    mat4 m_projectionMatrix;
    mat4 m_HUDProjectionMatrix;
    GameObject* m_pSkyBox;//1 Skybox per scene
	float m_ambientLightPercentage;

public:
    Scene();
    virtual ~Scene();

    virtual void OnSurfaceChanged(unsigned int width, unsigned int height);
    virtual void LoadContent(b2Vec2 worldGravity = b2Vec2(0.0f, -9.8f));

    virtual bool HandleInput(InputEvent& inputevent,double delta);
    virtual void Update(double TimePassed);
    virtual void Draw();
	virtual void Reset();

    GameObject* GetGameObject(const char* name) { return m_pGameObjects[name]; }
    GameObject* AddGameObject(std::string name, GameObject* object);
    void RemoveGameObject(std::string name);

    void SetSkyBox(GameObject* aSkyBoxObject);
    GameObject* GetSkyBox(){ return m_pSkyBox; }

    Box2DWorld* GetBox2DWorld(){ return m_pSceneWorld; }

    virtual void CheckControlsDebugDrawing(InputEvent& aInputevent);

    virtual void SetUpdateable(bool aStat){ m_updateable = aStat; }
    bool GetUpdateable(){ return m_updateable; }
    virtual void SetDrawable(bool aStat){ m_drawable = aStat; }
    bool GetDrawable(){ return m_drawable; }
	bool GetUsePostProcessEffects() { return m_usePostProcessEffects; }
    bool GetSceneIsOpaque(){ return m_sceneIsOpaque; }

    mat4* GetProjectionMatrix(){ return &m_projectionMatrix; }
    mat4* GetHUDProjectionMatrix(){ return &m_HUDProjectionMatrix; }

    void AddInputHandler(GameObject* aInputHandler);

    void AddLight(LightObject* aLight);
	std::vector<LightObject*> GetSceneLights() { return m_pSceneLights; }
	float GetAmbientLightPercentage() { return m_ambientLightPercentage; }

	void AddParticleSystem(ParticleSystem* partifcleSystem);
	ParticleSystem* GetParticleSystem(unsigned int index = 0);
  
	void AddPostProcessingShader(ShaderProgram* postProcessingShader);
	void RemovePostProcessingShader(ShaderProgram* postProcessingShader);
	std::vector<ShaderProgram*> GetPostProcessShaders() { return m_pLinkedPostProcessingShaders; }

    virtual void LoadGameObjectsScene(cJSON* jRoot);
	virtual GameObject* LoadGameObject(cJSON* jGameObject, GameObject* pGameObject = nullptr,bool addToScene=true);
	virtual void LoadGameObjectsJointsScene(cJSON* jRoot);
	virtual Box2DJointInfo LoadGameObjectJoint(cJSON* jGameObject);

    ShaderProgram* LoadOrCreateShader(char* shaderName);
    Mesh* LoadOrCreateOBJ(char* objName);
    GLuint LoadOrCreateTexture(char* textureName);
    std::string GetFileNameFromPath(std::string str, bool withFileExtension = false);

private:
	//Loading functions for organizaiton
	void LoadGameObjectName(cJSON* jGameObject,  GameObject* pGameObject);
	void LoadGameObjectPosition(cJSON* jGameObject, GameObject* pGameObject);
	void LoadGameObjectRotation(cJSON* jGameObject, GameObject* pGameObject);
	void LoadGameObjectScale(cJSON* jGameObject, GameObject* pGameObject);
	void LoadGameObjectComponents(cJSON* jGameObject, GameObject* pGameObject);
	void LoadGameObjectSpriteComponent(cJSON* jSpriteComponent, GameObject* pGameObject);
	void LoadGameObjectsBodiesComponent(std::vector<cJSON*>jBodyComponents,GameObject* pGameObject);
	void LoadGameObjectBoxBody(GameObject* pGameObject);
	void LoadGameObjectCircleBody(GameObject* pGameObject);
	void LoadGameObjectEdgeBody(GameObject* pGameObject);
	bool LoadGameObjectChainBody(cJSON * ajBodyComponent,GameObject* pGameObject );
	void LoadGameObjectPhysicsBody(cJSON* bodyComponent, GameObject*  apGameObject);
	void LoadGameObjectOBJMesh(cJSON* OBJComponent, GameObject* apGameObject);

};

#endif //__Scene_H__
