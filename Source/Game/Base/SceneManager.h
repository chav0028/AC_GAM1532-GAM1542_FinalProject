#ifndef __SceneManager_H__
#define __SceneManager_H__

class Scene;

class SceneManager
{
protected:
    unsigned int m_scenesToBePopped;
    std::vector<Scene*>m_pScenes;
    std::vector<Scene*>m_scenesToBeAdded;

	FBODefinition* m_pMainFBO;//Vector fo frame buffering objects
	FBODefinition* m_pSecondaryFBO;
	Mesh* m_pPostProcessMesh;
    bool m_postProCessColorTexture;
    bool m_postProcessDepthTexture;
    bool m_makeTopSceneUpdatable;//Variable used when we pop a scene, so that the scene below it is set as drawable and updatable.
    bool m_makeTopSceneDrawable; 
    bool m_resetTopScene;

public:
    SceneManager();
    virtual ~SceneManager();

    void OnSurfaceChanged(unsigned int width, unsigned int height);
    void HandleInput(InputEvent& inputevent, double delta);
    void Update(double TimePassed);
    void Draw();

    void PushScene(Scene* aScene);
    void PushAndLoadScene(Scene* aScene);
    Scene* PopAndGetScene(bool resetTopScene=true,bool makeTopSceneUpdatable=true,bool makeTopSceneDrawable=true);
    void PopAndDeleteScene(bool resetTopScene = true, bool makeTopSceneUpdatable = true, bool makeTopSceneDrawable = true);
    void PopAllScenes();
    
	void SetPostProcessMesh(Mesh* aMesh);
	void SetMainFBO(unsigned int aTextureWidth, unsigned int aTextureHeight);
	void SetSecondaryFBO(unsigned int aTextureWidth, unsigned int aTextureHeight);
	void PostProcessScene(Scene* sceneToPostProcess);
    void SetPostProcessingUniforms(ShaderProgram* aShader, GLint colorTexture, GLint depthTexture = 0,Vector2 uvScale=vec2(1,1));
};

#endif //__Scene_H__
