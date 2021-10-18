#include "CommonHeader.h"

ResetButton::ResetButton(const char * aCameraName, unsigned int aDrawRenderOrder) :Button(aCameraName, aDrawRenderOrder)
{
}

ResetButton::ResetButton(Scene * apScene, std::string aName, Vector3 aPos, Mesh * apMesh, ShaderProgram * apShader,
	GLuint aUnselectedTexture, GLuint aSelectedTexture, vec2 aUVScale, vec2 aUVOffset, const char * aCameraName, unsigned int aDrawRenderOrder):
	Button(apScene,aName,aPos,apMesh,apShader,aUnselectedTexture,aSelectedTexture,aUVScale,aUVOffset,aCameraName,aDrawRenderOrder)
{
}

void ResetButton::OnButtonPressed()
{
	SceneManager* sceneManager=g_pGame->GetSceneManager();

	if (sceneManager != nullptr)
	{
        sceneManager->PopAndDeleteScene();
	}
}
