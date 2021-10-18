#include "CommonHeader.h"

Button::Button(const char * aCameraName, unsigned int aDrawRenderOrder):GameObject(aCameraName,aDrawRenderOrder),
m_selected(false)
{
	SetElementOfHUD(true);
}

Button::Button(Scene * apScene, std::string aName, Vector3 aPos, Mesh * apMesh,
	ShaderProgram * apShader, GLuint aUnselectedTexture, GLuint aSelectedTexture, vec2 aUVScale, vec2 aUVOffset, const char * aCameraName, unsigned int aDrawRenderOrder):
	GameObject(apScene,aName,aPos,vec3(0,0,0),vec3(1,1,1),apMesh,apShader,aUnselectedTexture,aUVScale,aUVOffset,aCameraName,aDrawRenderOrder),
	m_selected(false),
	m_unselectedTexture(aUnselectedTexture),
	m_selectedTexture(aSelectedTexture)
{
	SetElementOfHUD(true);
}

Button::~Button()
{
}
//Function only checks mouse input, override function for individual keys.
bool Button::HandleInput(InputEvent& aInputEvent, double aDelta)
{
	if (aInputEvent.type == InputEventType_Mouse)
	{

        Vector2 mousePosition = vec2(aInputEvent.x, aInputEvent.y);
       // if (RayCastPositionIn2d(mousePosition))
        vec2 finalMouseWorldPositon = ConvertScreenCoordsToWorld(mousePosition);
      
		//Check if the mouse is hovering over the button
        if (CheckPointIsInSquare2D(finalMouseWorldPositon) == true)  
        {
			//Change the texture being used
			OnButtonSelected();


			//Check if button is pressed
			if (aInputEvent.state == InputEventState_Up)
			{
				if (aInputEvent.mousebutton == 0)
				{
					OnButtonPressed();
					return true;
				}
			
			}
		}
		else
		{
			//If mouse is no longer hovering over button
			OnButtonUnselected();
		}
	}

	return false;
}

void Button::OnButtonSelected()
{
	UseSelectedTexture();
}

void Button::OnButtonUnselected()
{
	UseUnselectedTexture();
}

Vector2 Button::ConvertScreenCoordsToWorld(Vector2 aScreenCoord)
{
    Vector2 screenSize = Vector2((float)g_pGame->GetWindowWidth(), (float)g_pGame->GetWindowHeight());
    float HUDSize = (DEFAULT_HUD_PROJECTION_SCALE * 2);

    Vector2 orthoWorldCoords;

    //Convert screen coords to the Orthographic HUD size
    orthoWorldCoords.x=aScreenCoord.x / screenSize.x;
    orthoWorldCoords.y = aScreenCoord.y / screenSize.y;
    orthoWorldCoords *= HUDSize;

    //Made the coordinates match the -5 to 5 of the HUD.
    orthoWorldCoords.x -= HUDSize / 2;
    orthoWorldCoords.y -= (HUDSize/2);
    orthoWorldCoords.y *= -1;
        
    
    return orthoWorldCoords;

}