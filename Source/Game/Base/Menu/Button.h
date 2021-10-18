#ifndef __Button_H__
#define __Button_H__

class Button :public GameObject
{

protected:
	bool m_selected;
	GLuint m_unselectedTexture;
	GLuint m_selectedTexture;

public:
	Button(const char* cameraName = HUD_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);
	Button(Scene* pScene, std::string name, Vector3 pos, Mesh* pMesh, ShaderProgram* pShader, GLuint unselectedTexture, GLuint selectedTexture, vec2 UVScale = vec2(1, 1), vec2 UVOffset = vec2(0, 0),
		const char* cameraName = HUD_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);
	virtual ~Button();

	virtual bool HandleInput(InputEvent& inputEvent, double aDelta);

	virtual void OnButtonSelected();
	virtual void OnButtonUnselected();
	virtual void OnButtonPressed() = 0;

	void UseUnselectedTexture() {m_TextureHandle = m_unselectedTexture;}
	void UseSelectedTexture() { m_TextureHandle = m_selectedTexture; }

    Vector2 ConvertScreenCoordsToWorld(Vector2 aScreenCoord);
};

#endif