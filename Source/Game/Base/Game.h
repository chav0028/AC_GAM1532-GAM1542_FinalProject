#ifndef __Game_H__
#define __Game_H__

class Game;

extern Game* g_pGame;

const char* const DEFAULT_FILE_PATH_OBJS = "OBJs/";
const char* const DEFAULT_FILE_PATH_TEXTURES = "Data/Textures/";
const char* const DEFAULT_FILE_PATH_SHADERS = "Data/Shaders/";
const char* const DEFAULT_FILE_PATH_SCENES = "Data/Scenes/";

const int SDL_MIXER_AFFECT_ALL_CHANNELS = -1;

class ShaderProgram;
class AudioCue;
class FBODefinition;

class Game
{
protected:
     SceneManager* m_pSceneManager;

    std::map<std::string, Mesh*> m_pMeshes;
    std::map<std::string, ShaderProgram*> m_pShaders;
    std::map<std::string, GLuint> m_pTextures;
    std::map<std::string, AudioCue*> m_pAudios;

	SoundSystem* m_pSoundSystem;

    unsigned int m_WindowWidth;
    unsigned int m_WindowHeight;

public:
    Game();
    virtual ~Game();

    virtual void OnSurfaceChanged(unsigned int width, unsigned int height);
    virtual void LoadContent();

    virtual void HandleInput(InputEvent& inputevent,double delta);
    virtual void Update(double TimePassed);
    virtual void Draw();

    SceneManager* GetSceneManager(){ return m_pSceneManager; }

    unsigned int GetWindowWidth(){ return m_WindowWidth; }
    unsigned int GetWindowHeight(){ return m_WindowHeight; }

    void AddMesh(const char* name, Mesh* pMesh);
    void AddShader(const char* name, ShaderProgram* pShader);
    void AddTexture(const char* name, GLuint texturehandle);
    void AddAudio(const char* name, AudioCue* pAudio);
    GLuint LoadTextureCubemap(const char** filenames);

    Mesh* GetMesh(const char* name) { return m_pMeshes[name]; }
    ShaderProgram* GetShader(const char* name) { return m_pShaders[name]; }
    GLuint GetTexture(const char* name) { return m_pTextures[name]; }
    AudioCue* GetAudioCue(const char* name){ return m_pAudios[name]; }

	SoundSystem* GetSoundSystem() { return m_pSoundSystem; }
	FMOD::System* GetFMODSoundSystem();

    void StopAllSound(){ Mix_HaltChannel(SDL_MIXER_AFFECT_ALL_CHANNELS); }//Stop the sound;
    void ResumeAllSound(){ Mix_Resume(SDL_MIXER_AFFECT_ALL_CHANNELS); }
    void PauseAllSound(){ Mix_Pause(SDL_MIXER_AFFECT_ALL_CHANNELS); }

    void LoadSDLAudio();			
    
   
};

#endif //__Game_H__
