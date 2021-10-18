#include "CommonHeader.h"

SceneManager::SceneManager()
	:m_pPostProcessMesh(nullptr),
	m_pMainFBO(nullptr),
	m_pSecondaryFBO(nullptr)
{
    m_scenesToBePopped = 0;

    m_makeTopSceneUpdatable = false;
    m_makeTopSceneDrawable = false;
    m_resetTopScene = false;

	//Create mesh for post processs
	g_pGame->AddMesh("PostProcessScreen", Mesh::CreatePostProcessScreen());
	m_pPostProcessMesh = g_pGame->GetMesh("PostProcessScreen");
    m_postProCessColorTexture = true;
    m_postProcessDepthTexture = false;
}

SceneManager::~SceneManager()
{
    for (unsigned int i = 0; i < m_pScenes.size(); i++)
    {
        SAFE_DELETE(m_pScenes.at(i));
    }

    m_pScenes.clear();

	SAFE_DELETE(m_pMainFBO);
	SAFE_DELETE(m_pSecondaryFBO);
}

void SceneManager::HandleInput(InputEvent& aInputevent, double aDelta)
{
	//Check that there are actually scenes
	if (m_pScenes.empty() == true)
	{
		return;
	}

    bool inputUsed = false;

    //Get the index of the last element in the vector. This is to handle the most recently added scene first
    int sceneIndex = m_pScenes.size() - 1;

    //Go through all the scenes
    while (sceneIndex >= 0)
    {
        if (m_pScenes.at(sceneIndex) != nullptr)
        {
            inputUsed=m_pScenes.at(sceneIndex)->HandleInput(aInputevent, aDelta);//handle input in each scene

            //If the input is used stop the loop
            if (inputUsed == true)
            {
                break;
            }
            
        }

        //Manually decrement for the loop.Decrement since the order of the loop is from last added to first.
        sceneIndex--;
    }
}

void SceneManager::Update(double aTimePassed)
{
    //Get the index of the last element in the vector. This is to handle the most recently added scene first
    int sceneIndex = m_pScenes.size() - 1;

    //Go through all the scenes
    while (sceneIndex >= 0)
    {
        if (m_pScenes.at(sceneIndex) != nullptr)
        {
            if (m_pScenes.at(sceneIndex)->GetUpdateable() == true)
            {
                m_pScenes.at(sceneIndex)->Update(aTimePassed);//Update in each scene
            }
        }

        //Manually decrement for the loop.Decrement since the order of the loop is from last added to first.
        sceneIndex--;
    }

    //Go through all the scenes that are to be removed
    for (unsigned int i = 0; i < m_scenesToBePopped; i++)
    {
        SAFE_DELETE(m_pScenes.back());//Delete them
        m_pScenes.pop_back();      
    }
    m_scenesToBePopped = 0;

    for (unsigned int i = 0; i < m_scenesToBeAdded.size(); i++)
    {
        m_pScenes.push_back(m_scenesToBeAdded.at(i));
        m_scenesToBeAdded.pop_back();
            //Call on surface change
            OnSurfaceChanged(g_pGame->GetWindowWidth(), g_pGame->GetWindowHeight());//Call on surfaced changed so that the scenes create the projection matrices
    }

    //After all the scenes have been removed/added. If it has been set, set the top scened
    if (m_pScenes.empty() == false)
    {
        unsigned int topSceneIndex = m_pScenes.size() - 1;

        if (m_pScenes.at(topSceneIndex) != nullptr)
        {
            if (m_makeTopSceneUpdatable == true)
            {
                m_pScenes.at(topSceneIndex)->SetUpdateable(true);
                m_makeTopSceneUpdatable = false;
            }

            if (m_makeTopSceneDrawable == true)
            {
                m_pScenes.at(topSceneIndex)->SetDrawable(true);
                m_makeTopSceneDrawable = false;
            }

            if (m_resetTopScene == true)
            {
                m_pScenes.at(topSceneIndex)->Reset();
                m_resetTopScene = false;
            }
        }
    }
}

void SceneManager::Draw()
{
	//Check that there are actually scenes
	if (m_pScenes.empty() == true)
	{
		return;
	}

    //Get the index of the last element in the vector. This is to handle the most recently added scene first
    int sceneIndex = m_pScenes.size() - 1;

    while (sceneIndex >= 0)
    {
        if (m_pScenes.at(sceneIndex) != nullptr)
        {
            if (m_pScenes.at(sceneIndex)->GetDrawable() == true)//Conserved drawable variable for complete control of the scene
            {
                //Find the first scene that is opaque
                if (m_pScenes.at(sceneIndex)->GetSceneIsOpaque() == true)
                {
                    unsigned int scenesToDraw = sceneIndex;

                    //Draw all the scens that are on top of the opaque one
                    while (scenesToDraw<m_pScenes.size())
                    {
                        //Post process effects
                        if (m_pScenes.at(scenesToDraw)->GetUsePostProcessEffects() == true)
                        {
                                PostProcessScene(m_pScenes.at(scenesToDraw));
                        }
                        else
                        {
                            m_pScenes.at(scenesToDraw)->Draw();//Draw scene normally
                        }

                        scenesToDraw++;
                    }

                    break;//Break out of the loop
   
                }
                else
                {
                    sceneIndex--;//Go to lower scene
                }
            }
        }
    }
}

void SceneManager::PushScene(Scene* aScene)
{
    if (aScene != nullptr)
    {
        m_scenesToBeAdded.push_back(aScene);//Push the new scene into the vector
    }
}

void SceneManager::PopAndDeleteScene(bool resetTopScene,bool makeTopSceneUpdatable, bool makeTopSceneDrawable)
{
    m_scenesToBePopped++;
    m_resetTopScene = resetTopScene;
    m_makeTopSceneDrawable = makeTopSceneDrawable;
    m_makeTopSceneUpdatable = makeTopSceneUpdatable;
}

void SceneManager::PopAllScenes()
{
    m_scenesToBePopped = m_pScenes.size();
}

void SceneManager::PushAndLoadScene(Scene* aScene)
{
    if (aScene != nullptr)
    {
        m_scenesToBeAdded.push_back(aScene);//Push the new scene into the vector
        m_scenesToBeAdded.back()->LoadContent();//Load the scene
    }
}

//THIS FUNCTION DOESN'T DELETE THE SCENE
Scene* SceneManager::PopAndGetScene(bool resetTopScene,bool makeTopSceneUpdatable, bool makeTopSceneDrawable)
{
    //Save the scene that is about to be popped
    Scene* poppedScene = m_pScenes.back();

    //Remove scene from vector
    m_pScenes.pop_back();

    m_resetTopScene = resetTopScene;
    m_makeTopSceneDrawable = makeTopSceneDrawable;
    m_makeTopSceneUpdatable = makeTopSceneUpdatable;

    return poppedScene;//Return the scene that has been popped
}

void SceneManager::OnSurfaceChanged(unsigned int aWidth, unsigned int aHeight)
{
    //Notify all the scenes of the change in surface
    for (unsigned int i = 0; i < m_pScenes.size(); i++)
    {
        if (m_pScenes.at(i) != nullptr)
        {
            m_pScenes.at(i)->OnSurfaceChanged(aWidth, aHeight);
        }
    }
}

void SceneManager::SetPostProcessMesh(Mesh* aMesh)
{
	if (aMesh != nullptr)
	{
		m_pPostProcessMesh = aMesh;
	}
}

void SceneManager::SetMainFBO(unsigned int aTextureWidth, unsigned int aTextureHeight)
{
	if (m_pMainFBO == nullptr)
	{
		m_pMainFBO = new FBODefinition();
	}

	m_pMainFBO->Setup(aTextureWidth, aTextureHeight);
}

void SceneManager::SetSecondaryFBO(unsigned int aTextureWidth, unsigned int aTextureHeight)
{
	if (m_pSecondaryFBO == nullptr)
	{
		m_pSecondaryFBO = new FBODefinition();
	}

	m_pSecondaryFBO->Setup(aTextureWidth, aTextureHeight);
}

void SceneManager::PostProcessScene(Scene* aSceneToPostProcess)
{
    if (aSceneToPostProcess != nullptr)
    {
        std::vector<ShaderProgram*> postProcesses = aSceneToPostProcess->GetPostProcessShaders();

        //If there are no effects return
        if (postProcesses.empty() == true)
        {
            return;
        }

        //If there is no FBO create one
        if (m_pMainFBO == nullptr)
        {
            SetMainFBO(g_pGame->GetWindowWidth(), g_pGame->GetWindowHeight());
        }

        //If there are more than 2 Post Processes, ensure there is a 2nd FBO
        if (postProcesses.size() > 1 && m_pSecondaryFBO == nullptr)
        {
            SetSecondaryFBO(g_pGame->GetWindowWidth(), g_pGame->GetWindowHeight());
        }

        if (m_pPostProcessMesh == nullptr)
        {
            return;
        }

        GLuint colorTextureHandle = 0;
        GLuint depthTextureHandle = 0;
        Vector2 textureSize;
        Vector2 uvScale;

        bool usingMainFBO = true;
        //Go through all the post process
        int postProcessIndex = 0;
        for (unsigned int i = 0; i < postProcesses.size() + 1; i++)
        {
            if (postProcesses.at(postProcessIndex) != nullptr)
            {
                //Use primary FBO	
                if ((usingMainFBO == true && postProcessIndex != postProcesses.size() - 1) || i == 0)
                {
                    //Bind the FBO
                    m_pMainFBO->Bind();

                    //glClearColor(1.0f, 0.0f, 0.3f, 0.0f);
                    glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    //Change viewport according to size of texture
                    textureSize = Vector2((int)m_pMainFBO->m_TextureWidth, (int)m_pMainFBO->m_TextureHeight);
                    glViewport(0, 0, (GLsizei)textureSize.x, (GLsizei)textureSize.y);

                    if (m_pSecondaryFBO != nullptr)
                    {
                        uvScale = Vector2((int)m_pSecondaryFBO->m_TextureWidth / textureSize.x, (int)m_pSecondaryFBO->m_TextureWidth / textureSize.y);
                        uvScale = Vector2(1.0f / uvScale.x, 1.0f / uvScale.y);
                    }


                    //Get the texture of the FBO
                    colorTextureHandle = m_pMainFBO->m_ColorTextureHandle;
                    depthTextureHandle = m_pMainFBO->m_DepthTextureHandle;
                }
                //Use secondary FBO
                else if (usingMainFBO == false && postProcessIndex != postProcesses.size() - 1)//If it is not the last post process
                {       //Bind the FBO
                    m_pSecondaryFBO->Bind();

                    //glClearColor(1.0f, 0.0f, 0.3f, 0.0f);
                    glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    //Change viewport according to size of texture
                    textureSize = Vector2((int)m_pSecondaryFBO->m_TextureWidth, (int)m_pSecondaryFBO->m_TextureHeight);
                    glViewport(0, 0, m_pSecondaryFBO->m_TextureWidth, m_pSecondaryFBO->m_TextureHeight);

                    if (m_pMainFBO != nullptr)
                    {
                        uvScale = Vector2((int)m_pMainFBO->m_TextureWidth / textureSize.x, (int)m_pMainFBO->m_TextureWidth / textureSize.y);
                        uvScale = Vector2(1.0f / uvScale.x, 1.0f / uvScale.y);
                    }


                    //Get the texture of the FBO
                    colorTextureHandle = m_pSecondaryFBO->m_ColorTextureHandle;
                    depthTextureHandle = m_pSecondaryFBO->m_DepthTextureHandle;
                }

                //If it is the first scene
                if (i == 0)
                {
                    //Draw the scene
                    aSceneToPostProcess->Draw();
                }
                else if (postProcessIndex != postProcesses.size() - 1)//If it is not the first, nor the last scene draw the mesh so that it is post processed
                {
                    if (usingMainFBO == false)
                    {
                        colorTextureHandle = m_pMainFBO->m_ColorTextureHandle;
                        depthTextureHandle = m_pMainFBO->m_DepthTextureHandle;

                    }
                    else
                    {
                        colorTextureHandle = m_pSecondaryFBO->m_ColorTextureHandle;
                        depthTextureHandle = m_pSecondaryFBO->m_DepthTextureHandle;
                    }

                    //Draw the post effect as a texture
                    m_pPostProcessMesh->SetupAttributes(postProcesses.at(postProcessIndex));
                    SetPostProcessingUniforms(postProcesses.at(postProcessIndex), colorTextureHandle, depthTextureHandle, uvScale);
                    m_pPostProcessMesh->Draw(postProcesses.at(postProcessIndex));
                    postProcessIndex++;
                }

                //Unbind the FBO
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                CheckForGLErrors();

                //If it is the last scene
                if (i == postProcesses.size())
                {
                    //glClearColor(1.0f, 0.0f, 0.3f, 0.0f);
                    glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    //Reset the viewport
                    Vector2 windowSize = Vector2((int)g_pGame->GetWindowWidth(), (int)g_pGame->GetWindowHeight());
                    glViewport(0, 0, (GLsizei)windowSize.x, (GLsizei)windowSize.y);

                    ////Get the uv scale to make the texture pixel perfect
                    //uvScale = Vector2(windowSize.x / textureSize.x, windowSize.y / textureSize.y);
                    //uvScale = Vector2(1.0f / uvScale.x, 1.0f / uvScale.y);

                    //Set uniforms and attributes
                    m_pPostProcessMesh->SetupAttributes(postProcesses.at(postProcessIndex));
                    SetPostProcessingUniforms(postProcesses.at(postProcessIndex), colorTextureHandle, depthTextureHandle, uvScale);

                    //Draw the scene/mesh where the post process will be used
                    m_pPostProcessMesh->Draw(postProcesses.at(postProcessIndex));
                    CheckForGLErrors();
                }

                usingMainFBO = !usingMainFBO;//Reverse which FBO will be used
            }
        }
    }
}

void SceneManager::SetPostProcessingUniforms(ShaderProgram* aShader, GLint colorTexture, GLint depthTexture,Vector2 uvScale)
{
    //Values for uniforms
    mat4 worldMat;
    worldMat.SetIdentity();
    mat4 viewMat;
    viewMat.SetIdentity();
    mat4 projMat;
    projMat.SetIdentity();
    Vector3 camPos = 0;

    //Only use color texture
    if (m_postProCessColorTexture == true && m_postProcessDepthTexture == false)
    {
        Mesh::SetupUniforms(aShader, &worldMat, &viewMat, &projMat, camPos, colorTexture, Vector4(0,0,0,0), uvScale);
    }
    else if (m_postProCessColorTexture == false && m_postProcessDepthTexture == true)//Use only depth
    {
        Mesh::SetupUniforms(aShader, &worldMat, &viewMat, &projMat, camPos, depthTexture,Vector4(0, 0, 0, 0), uvScale);
    }
    else if (m_postProCessColorTexture == true && m_postProcessDepthTexture == true)//Use  depth and color
    {
        Mesh::SetupUniforms(aShader, &worldMat, &viewMat, &projMat, camPos, colorTexture, Vector4(0, 0, 0, 0), uvScale);
        Mesh::SetupTexture2Uniform(aShader, depthTexture);
    }
}