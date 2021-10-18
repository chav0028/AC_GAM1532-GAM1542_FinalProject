#ifndef __QuitButton_H__
#define __QuitButton_H__

class QuitButton :public Button
{
public:
    QuitButton(const char* cameraName = HUD_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);
    QuitButton(Scene* pScene, std::string name, Vector3 pos, Mesh* pMesh, ShaderProgram* pShader, GLuint unselectedTexture, GLuint selectedTexture, vec2 UVScale = vec2(1, 1), vec2 UVOffset = vec2(0, 0),
		const char* cameraName = HUD_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);

	virtual void OnButtonPressed();
};

#endif