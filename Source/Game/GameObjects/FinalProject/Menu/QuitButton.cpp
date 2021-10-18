#include "CommonHeader.h"

QuitButton::QuitButton(const char * aCameraName, unsigned int aDrawRenderOrder) :Button(aCameraName, aDrawRenderOrder)
{
}

QuitButton::QuitButton(Scene * apScene, std::string aName, Vector3 aPos, Mesh * apMesh, ShaderProgram * apShader,
	GLuint aUnselectedTexture, GLuint aSelectedTexture, vec2 aUVScale, vec2 aUVOffset, const char * aCameraName, unsigned int aDrawRenderOrder):
	Button(apScene,aName,aPos,apMesh,apShader,aUnselectedTexture,aSelectedTexture,aUVScale,aUVOffset,aCameraName,aDrawRenderOrder)
{
}

void QuitButton::OnButtonPressed()
{
    //Take advantage of the globals placed in the source code 
    g_KeyStates[VK_ESCAPE] = true;
}
