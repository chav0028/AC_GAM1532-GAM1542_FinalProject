#ifndef __ControlsButton_H__
#define __ControlsButton_H__

class ControlsButton :public Button
{
public:
    ControlsButton(const char* cameraName = HUD_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);
    ControlsButton(Scene* pScene, std::string name, Vector3 pos, Mesh* pMesh, ShaderProgram* pShader, GLuint unselectedTexture, GLuint selectedTexture, vec2 UVScale = vec2(1, 1), vec2 UVOffset = vec2(0, 0),
		const char* cameraName = HUD_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);

	virtual void OnButtonPressed();
};

#endif