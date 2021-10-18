#include "CommonHeader.h"

MainMenuButton::MainMenuButton(const char * aCameraName, unsigned int aDrawRenderOrder) :Button(aCameraName, aDrawRenderOrder)
{
}

MainMenuButton::MainMenuButton(Scene * apScene, std::string aName, Vector3 aPos, Mesh * apMesh, ShaderProgram * apShader,
	GLuint aUnselectedTexture, GLuint aSelectedTexture, vec2 aUVScale, vec2 aUVOffset, const char * aCameraName, unsigned int aDrawRenderOrder):
	Button(apScene,aName,aPos,apMesh,apShader,aUnselectedTexture,aSelectedTexture,aUVScale,aUVOffset,aCameraName,aDrawRenderOrder)
{
}

void MainMenuButton::OnButtonPressed()
{
	SceneManager* sceneManager=g_pGame->GetSceneManager();

	if (sceneManager != nullptr)
	{
        AudioCue* sound = g_pGame->GetAudioCue("InputSound");
        if (sound != nullptr)
        {
            sound->PlayAudio();
        }

		sceneManager->PopAllScenes();
		sceneManager->PushAndLoadScene(new MainMenu());
	}
}
