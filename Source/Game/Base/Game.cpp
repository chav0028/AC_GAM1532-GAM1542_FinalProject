#include "CommonHeader.h"

Game* g_pGame = nullptr;

extern bool g_KeyStates[256];
extern bool g_OldKeyStates[256];

Game::Game()
{
    g_pGame = this;

    m_pSceneManager = nullptr;

    m_pSoundSystem = nullptr;

}

Game::~Game()
{
    Mix_CloseAudio();

    SAFE_DELETE(m_pSceneManager);

    for each(auto iterator in m_pMeshes)
        delete iterator.second;

    for each(auto iterator in m_pShaders)
        delete iterator.second;

    for each(auto iterator in m_pTextures)
        glDeleteTextures(1, &iterator.second);

    for each(auto iterator in m_pAudios)
        SAFE_DELETE(iterator.second);

    SAFE_DELETE(m_pSoundSystem);

}

void Game::OnSurfaceChanged(unsigned int width, unsigned int height)
{
    glViewport(0, 0, width, height);

    m_WindowWidth = width;
    m_WindowHeight = height;

    //Call onSurfaceChanged on the scene
    if (m_pSceneManager != nullptr)
    {
        m_pSceneManager->OnSurfaceChanged(width, height);
    }

}

void Game::LoadContent()
{
    //Initalize FMOD sound
    m_pSoundSystem = new SoundSystem();

    // turn on depth buffer testing.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);//Enable culling so a side isn't draw
    glCullFace(GL_BACK);//Specify back faces won't be drawn
    glFrontFace(GL_CW);//Specify the front face is when the vertices are clock wise

    // turn on alpha blending.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    LoadSDLAudio();

    m_pSceneManager = new SceneManager();

    if (m_pSceneManager != nullptr)
    {
        //m_pSceneManager->PushAndLoadScene(new LightScene());
        m_pSceneManager->PushAndLoadScene(new SplashScreen());
    }

    CheckForGLErrors();
}

void Game::HandleInput(InputEvent& inputevent, double aDelta)
{
    if (m_pSceneManager != nullptr)
    {
        m_pSceneManager->HandleInput(inputevent, aDelta);
    }

}

void Game::Update(double TimePassed)
{
    CheckForGLErrors();

    if (m_pSceneManager != nullptr)
    {
        m_pSceneManager->Update(TimePassed);
    }

    //Update FMOD sound system
    if (m_pSoundSystem != nullptr)
    {
        FMOD::System* FMODSystem = m_pSoundSystem->GetSoundSystem();
        if (FMODSystem != nullptr)
        {
            FMODSystem->update();
        }
    }

    CheckForGLErrors();

}

void Game::Draw()
{
    CheckForGLErrors();



    //////Clearing the back buffer, so its alpha doesn't matter
    glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_pSceneManager->Draw();


    CheckForGLErrors();
}

void Game::AddMesh(const char* aName, Mesh* apMesh)
{
    //Check that the object hasn't already been added
    if (m_pMeshes[aName] == nullptr)
    {
        m_pMeshes[aName] = apMesh;//If it hasn't been been added already, add it.
    }
}

void Game::AddShader(const char* aName, ShaderProgram* apShader)
{
    //Check that the object hasn't already been added
    if (m_pShaders[aName] == nullptr)
    {
        m_pShaders[aName] = apShader;//If it hasn't been been added already, add it.
    }
}

void Game::AddTexture(const char* aName, GLuint aTexturehandle)
{
    //Check that the object hasn't already been added
    if (m_pTextures[aName] == 0)
    {
        m_pTextures[aName] = aTexturehandle;//If it hasn't been been added already, add it.
    }
}

void Game::AddAudio(const char* aName, AudioCue* apAudio)
{
    //Check that the object hasn't already been added
    if (m_pAudios[aName] == nullptr)
    {
        m_pAudios[aName] = apAudio;//If it hasn't been been added already, add it.
    }
}
FMOD::System * Game::GetFMODSoundSystem()
{
    if (m_pSoundSystem != nullptr)
    {
        return m_pSoundSystem->GetSoundSystem();//Return the direct FMOD system from the wrapper class
    }

    return nullptr;
}

void Game::LoadSDLAudio()
{
    //Default values
    int audio_rate = 22050;
    uint16 audio_format = MIX_DEFAULT_FORMAT;
    int audio_channels = 2;
    int audio_buffers = 512;

    //Initialize SDL
    SDL_Init(SDL_INIT_AUDIO);

    //Initialize SDL mixer
    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
    {
        OutputDebugString("Error intializing SDL Mixer");
    }

    //Get the number of audio channels available
    int numberChannels = Mix_GroupCount(-1);

    //Assign channels
    Mix_GroupChannel(0, AudioGroups_Music);
    Mix_GroupChannel(1, AudioGroups_Dialogues);
    Mix_GroupChannels(2, numberChannels - 1, AudioGroups_Effects);


}

GLuint Game::LoadTextureCubemap(const char** filenames)
{
    unsigned char* pngbuffer = 0;
    unsigned int width = 0, height = 0;

    GLuint texhandle = 0;
    glGenTextures(1, &texhandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texhandle);

    for (int i = 0; i<6; i++)
    {
        lodepng_decode32_file(&pngbuffer, &width, &height, filenames[i]);
        assert(pngbuffer != 0);
        if (pngbuffer == 0)
            return 0;

        //Flip32BitImageVertically( pngbuffer, width, height );

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pngbuffer);

        delete(pngbuffer);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return texhandle;
}

