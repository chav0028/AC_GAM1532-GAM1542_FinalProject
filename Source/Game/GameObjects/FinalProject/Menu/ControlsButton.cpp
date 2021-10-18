#include "CommonHeader.h"

ControlsButton::ControlsButton(const char * aCameraName, unsigned int aDrawRenderOrder) :Button(aCameraName, aDrawRenderOrder)
{
}

ControlsButton::ControlsButton(Scene * apScene, std::string aName, Vector3 aPos, Mesh * apMesh, ShaderProgram * apShader,
	GLuint aUnselectedTexture, GLuint aSelectedTexture, vec2 aUVScale, vec2 aUVOffset, const char * aCameraName, unsigned int aDrawRenderOrder):
	Button(apScene,aName,aPos,apMesh,apShader,aUnselectedTexture,aSelectedTexture,aUVScale,aUVOffset,aCameraName,aDrawRenderOrder)
{
}

void ControlsButton::OnButtonPressed()
{
	SceneManager* sceneManager=g_pGame->GetSceneManager();

	if (sceneManager != nullptr)
	{
        AudioCue* sound = g_pGame->GetAudioCue("InputSound");
        if (sound != nullptr)
        {
            sound->PlayAudio();
        }

		sceneManager->PushAndLoadScene(new ControlsScene());
	}
}
