#include "CommonHeader.h"
#include "Core\Utility\Tween.h"
#include "Core\Utility\TweenFuncs.h"

using namespace std;

GameObject::GameObject(const char* aCameraName, unsigned int aDrawRenderOrder) :
	m_pScene(nullptr),
	m_pPhysicsBody(nullptr),
	m_pMesh(nullptr),
	m_pShaderProgram(nullptr),
	m_enabled(true),
    m_elementOfHUD(false),
	m_speed(0),
	m_UVScale(vec2(1, 1)),
	m_UVOffset(vec2(0, 0)),
	m_Color(Vector4(1, 0, 0, 1)),
	m_cameraDrawerName(aCameraName),
	m_drawRenderOrder(aDrawRenderOrder),
	m_Name("UNNAMED"),
    m_reflectivity(0.2f),
    m_isCheckingCollisionDepth(true),
    m_pParentObject(nullptr)
{
	m_Position.Set(0, 0, 0);
	m_Rotation.Set(0, 0, 0);
	m_Scale.Set(0, 0, 0);

	m_TextureHandle = 0;
}

GameObject::GameObject(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 aUVScale, vec2 aUVOffset,
	const char* aCameraName, unsigned int aDrawRenderOrder) :
	m_pScene(nullptr),
	m_pPhysicsBody(nullptr),
	m_pMesh(nullptr),
	m_pShaderProgram(nullptr),
    m_elementOfHUD(false),
	m_enabled(true),
	m_speed(0),
	m_Color(Vector4(1, 0, 0, 1)),
	m_cameraDrawerName(aCameraName),
    m_drawRenderOrder(aDrawRenderOrder),
    m_isCheckingCollisionDepth(true),
    m_reflectivity(0.2f),
    m_pParentObject(nullptr)
{
	Init(pScene, name, pos, rot, scale, pMesh, pShader, texture, aUVScale, aUVOffset);
}

GameObject::~GameObject()
{
	//The b2World destroys the bodies.

	//Destroy all the tweens
	while (m_pfloatTweens.empty() == false)
	{
		SAFE_DELETE(m_pfloatTweens.back());
		m_pfloatTweens.pop_back();
	}		   

	while (m_pVec3Tweens.empty() == false)
	{
		SAFE_DELETE(m_pVec3Tweens.back());
		m_pVec3Tweens.pop_back();
	}

	//Inform the parent this object has been deleted
	if (m_pParentObject != nullptr)
	{
		m_pParentObject->RemoveChild(this);
	}
}

void GameObject::Init(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 aUVScale, vec2 aUVOffset)
{
	m_pScene = pScene;

	m_Name = name;

	m_Position = pos;
	m_Rotation = rot;
	m_Scale = scale;

	m_pMesh = pMesh;

	m_pShaderProgram = pShader;
	m_TextureHandle = texture;

	m_UVScale = aUVScale;
	m_UVOffset = aUVOffset;

	SetDefaultTransform(m_Position, m_Rotation, m_Scale);
}

void GameObject::Update(double TimePassed)
{

	if (m_pPhysicsBody != nullptr)
	{
		m_pPhysicsBody->SetActive(m_enabled);//If there is a physics body set it

		if (m_pPhysicsBody->IsActive() == true)
		{
			//Check for NaN values
			assert(!(m_Position.x != m_Position.x));
			assert(!(m_Position.y != m_Position.y));

			m_Position = Vector3(m_pPhysicsBody->GetPosition().x, m_pPhysicsBody->GetPosition().y, m_Position.z);//Make the object match its physic body position

            if (m_Rotation.x > 90.0f && m_Rotation.x<270.0f)
            {
                m_Rotation = Vector3(m_Rotation.x, m_Rotation.y, (m_pPhysicsBody->GetAngle() * 180 / PI));//Box2D is only 2D, only change Z angle.Negative because in Box2D angles go counter clockwise but in this program is the other way around.
            }
            else
            {
                    m_Rotation = Vector3(m_Rotation.x, m_Rotation.y, -(m_pPhysicsBody->GetAngle() * 180 / PI));//Box2D is only 2D, only change Z angle.Negative because in Box2D angles go counter clockwise but in this program is the other way around.
            }
            
            //Engine angle is clockwise in degrees
			//Box2D angle is counterclockwise in radians

		}
	}

    //Make sure all the childs of the objects match the enabled status of the parent
    for (unsigned int i = 0; i < m_pChildObjects.size(); i++)
    {
        if (m_pChildObjects.at(i) != nullptr)
        {
            m_pChildObjects.at(i)->SetEnabled(GetEnabled());
        }
    }

	//Update the tweens
    //Float tweens
	for (unsigned int i = 0; i < m_pfloatTweens.size(); i++)
	{
		if (m_pfloatTweens.at(i) != nullptr)
		{
			m_pfloatTweens.at(i)->Update(TimePassed);
		}
	}

    //Vector 3 Tweens
	for (unsigned int i = 0; i < m_pVec3Tweens.size(); i++)
	{
		if (m_pVec3Tweens.at(i) != nullptr)
		{
			m_pVec3Tweens.at(i)->Update(TimePassed);
		}
	}
}

void GameObject::Draw(int aRenderorder)
{
	if (m_pMesh == 0)
		return;

	if (m_drawRenderOrder == aRenderorder)
	{
		ShaderProgram* pShaderProgram = m_pShaderProgram;

		m_pMesh->SetupAttributes(pShaderProgram);

		//store the values in the matrix
		mat4 worldMatrix;
        worldMatrix = GetWorldMatrix();

		CameraObject* pCamera = dynamic_cast<CameraObject*>(m_pScene->GetGameObject(m_cameraDrawerName));//get the camera that will draw this gameobject

		if (pCamera != nullptr)
		{
			Vector3 cameraPosition = pCamera->m_Position;

			mat4* pViewMatrix = pCamera->GetViewMatrix();//Get camera view matrix

			mat4* pProjectionMatrix;

			//If the camera drawing it is the HUD
			//if (m_cameraDrawerName == HUD_DEFAULT_CAMERA_NAME)//OLD CODE USED IN THRUSTERS GAME
            if (m_elementOfHUD==true)
			{
				pProjectionMatrix = m_pScene->GetHUDProjectionMatrix();//Get the projection matrix for the HUD
				// turn off the depth test for the hud
				glDisable(GL_DEPTH_TEST);
			}
			else
			{
				pProjectionMatrix = m_pScene->GetProjectionMatrix();//Get the projection matrix for normal game
			}

			if (pProjectionMatrix != nullptr && pViewMatrix!=nullptr)
			{
				if (m_pScene != nullptr)//Set the lights
				{
					m_pMesh->SetLightUniform(pShaderProgram, m_pScene->GetSceneLights(),m_pScene->GetAmbientLightPercentage());
				}

                m_pMesh->SetMaterialLightingUniform(pShaderProgram, m_reflectivity);//Set the object uniforms according to the light
                 m_pMesh->SetupUniforms(pShaderProgram, &worldMatrix, pViewMatrix, pProjectionMatrix, cameraPosition, m_TextureHandle, m_Color, m_UVScale, m_UVOffset);//Set uniforms
                m_pMesh->SetupTexture2Uniform(pShaderProgram, m_secondaryTexture);//Set a second texture, in case it is used
                m_pMesh->Draw(pShaderProgram);//Draw the mesh

				//Ensure the depth test is turned on after the draw
				glEnable(GL_DEPTH_TEST);
			}
		}
	}
}

//Empty so that it can be overwritten by child classes
bool GameObject::HandleInput(InputEvent& inputevent, double aDelta)
{
    return false;
}

void  GameObject::CreatePhysicsBody(b2BodyType aBodyType)
{
	if (m_pScene != nullptr)
	{
		b2World* pWorld = m_pScene->GetBox2DWorld()->GetWorld();

		if (pWorld != nullptr)//if the world is valid
		{
			//Create the Body definition
			b2BodyDef bodyDefinition;

			//Set the body inital state to match the game object
			bodyDefinition.angle = m_Rotation.z*PI / 180;
			bodyDefinition.position = b2Vec2(m_Position.x, m_Position.y);

			bodyDefinition.type = aBodyType;//Set body type
			bodyDefinition.fixedRotation = false;//Set it so that its angle change on collision 
			bodyDefinition.userData = this;//Set the user data for the collision listener

			//Create the body
			m_pPhysicsBody = pWorld->CreateBody(&bodyDefinition);//Tell the world to create a body
		}
	}
}

void GameObject::CreateFixture(b2BodyType aBodyType, b2Shape* aShape, bool aSensor, signed short aFixtureGroup, unsigned short aFixtureCategory, unsigned short aFixtureMasks)
{

	if (m_pPhysicsBody == nullptr)//If there isn'ta body, create it
	{
		CreatePhysicsBody(aBodyType);
	}

	if (m_pPhysicsBody != nullptr)//Check that the world was succesfully create (world may be null)
	{
		//Create the fixture definition
		b2FixtureDef fixtureDefiniton;
		fixtureDefiniton.shape = aShape;//Set the shape of the fixture defintion
		fixtureDefiniton.isSensor = aSensor;
		fixtureDefiniton.density = 1;

		//Collision filtering
		fixtureDefiniton.filter.groupIndex = aFixtureGroup;
		fixtureDefiniton.filter.categoryBits = aFixtureCategory;
		fixtureDefiniton.filter.maskBits = aFixtureMasks;

		//Create the fixture     
		m_pFixtures.push_back(m_pPhysicsBody->CreateFixture(&fixtureDefiniton));//Use the body to create a fixture, using fixture definition variable
	}
}

//Calculates the distance between this object position and another position
float GameObject::Calculate3DDistanceBetween(Vector3 aPosition)
{
	//calculate distance using Pythagorean Theorem 
	//distance=Sqrt(x^2+y^2+z^2)
	//distance=sqrt((x-x2)^2+(y-y2)^2+(z-z2)^2)
	float xPowerOfTwo = (m_Position.x - aPosition.x)*(m_Position.x - aPosition.x);
	float yPowerOfTwo = (m_Position.y - aPosition.y)*(m_Position.y - aPosition.y);
	float zPowerOfTwo = (m_Position.z - aPosition.z)*(m_Position.z - aPosition.z);

	float distance = sqrtf(xPowerOfTwo + yPowerOfTwo + zPowerOfTwo);

	return distance;
}

//Calculates the distance between this object position and another position
float GameObject::Calculate2DDistanceBetween(Vector2 aPosition)
{
	//calculate distance using Pythagorean Theorem 
	//distance=Sqrt(x^2+y^2)
	//distance=sqrt((x-x2)^2+(y-y2)^2)
	float xPowerOfTwo = (m_Position.x - aPosition.x)*(m_Position.x - aPosition.x);
	float yPowerOfTwo = (m_Position.y - aPosition.y)*(m_Position.y - aPosition.y);

	float distance = sqrtf(xPowerOfTwo + yPowerOfTwo);

	return distance;
}

bool GameObject::DepthCollisionCheck(float aTop, float aBottom)
{
	//Check if the rectangles are colliding
	//If any of these conditions is true the rectangles are not colliding
	if (aTop < GetMinXYZ().z)//top < bottom
	{
		return false;
	}
	else if (aBottom > GetMaxXYZ().z)//bottom > top
	{
		return false;
	}

	return true;
}

float GameObject::Get2DAngleBetween(Vector2 aPosition)
{
	//Use the arctangent between the position vectors to get the angle
	float adjacent = m_Position.x - aPosition.x;//Get the adjacent
	float opposite = m_Position.y - aPosition.y; //Get the opposite

	float angle = atan2f(opposite, adjacent);//Calculate the angle between them
	angle = angle*180.0f / PI;//Convert the angle from radians to degrees

	return angle;//return the angle back in degrees
}

//Function only works in 2D,
//Assumes pivot point is in center of gameobject
//Assumes the game object is a square using the whole lengths of the game object to plot it
//Takes the position as a single point (no radius or size)
bool GameObject::CheckPointIsInSquare2D(Vector2 aPosition)
{
    float halfWidth = GetXLength() / 2.0f;
    float halfHeight = GetYLength() / 2.0f;

    //Horizontal chceck
    if (aPosition.x > m_Position.x - halfWidth&& aPosition.x < m_Position.x + halfWidth)
    {
        //vertical check
        if (aPosition.y > m_Position.y - halfHeight&& aPosition.y < m_Position.y + halfHeight)
        {
            return true;//Position is inside shape
        }
    }

    return false;//Position is not inside square
}

void GameObject::CreatePhysicsBox(vec2 aSize, b2BodyType aBodyType, vec2 aOffset, bool aSensor, signed short aFixtureGroup, unsigned short aFixtureCategory, unsigned short aFixtureMasks)
{
	//Create the shape
	b2PolygonShape boxShape;
	boxShape.m_centroid = b2Vec2(aOffset.x, aOffset.y);
	boxShape.SetAsBox(aSize.x / 2, aSize.y / 2, b2Vec2(aOffset.x, aOffset.y), 0);//Divide the size by 2, since Box2D wants the width and length from the center, not the edges.
	//The angle is set to 0, because this is then set in the update mehthod.

	CreateFixture(aBodyType, &boxShape, aSensor, aFixtureGroup, aFixtureCategory, aFixtureMasks);//Create and set the body
}

void GameObject::CreatePhysicsCircle(float aRadius, b2BodyType aBodyType, vec2 aOffset, bool aSensor, signed short aFixtureGroup, unsigned short aFixtureCategory, unsigned short aFixtureMasks)
{
	//Create the shape
	b2CircleShape circleShape;
	circleShape.m_radius = aRadius;//Set radius of circle

	circleShape.m_p.Set(aOffset.x, aOffset.y);//Offset the shape

	CreateFixture(aBodyType, &circleShape, aSensor, aFixtureGroup, aFixtureCategory, aFixtureMasks);//Create and set the body
}

void GameObject::CreatePhysicsLine(vec2 aLineStart, vec2 aLineEnd, b2BodyType aBodyType, vec2 aOffset, bool aSensor, signed short aFixtureGroup, unsigned short aFixtureCategory, unsigned short aFixtureMasks)
{
	b2EdgeShape lineShape;

	b2Vec2 startingPoint = b2Vec2(aLineStart.x + aOffset.x, aLineStart.y + aOffset.y);//Get the starting point of line, considering offset
	b2Vec2 endingPoint = b2Vec2(aLineEnd.x + aOffset.x, aLineEnd.y + aOffset.y);//Get the ending point of line, considering offset

	lineShape.Set(startingPoint, endingPoint);//Set positions of the line,  considering offset

	CreateFixture(aBodyType, &lineShape, aSensor, aFixtureGroup, aFixtureCategory, aFixtureMasks);//Create and set the body
}

void GameObject::CreatePhysicsLineStrip(b2Vec2* aVertices, unsigned int aNumVertices, bool aLoop, b2BodyType aBodyType, vec2 aOffset, bool aSensor, signed short aFixtureGroup, unsigned short aFixtureCategory, unsigned short aFixtureMasks)
{
	b2ChainShape chainShape;

	//Check if we have an offset
	if (aOffset != 0)
	{
		for (unsigned int i = 0; i < aNumVertices; i++)//Go through all the vertices
		{
			aVertices[i] = aVertices[i] + b2Vec2(aOffset.x, aOffset.y);//Offset each individual vertice
		}
	}

	if (aLoop == false)
	{
		chainShape.CreateChain(aVertices, aNumVertices);
	}
	else
	{
		chainShape.CreateLoop(aVertices, aNumVertices);
	}

	CreateFixture(aBodyType, &chainShape, aSensor, aFixtureGroup, aFixtureCategory, aFixtureMasks);
}

//Contact listener functions empty so that they can be overwritten in child class
void  GameObject::BeginCollision(b2Fixture* aFixtureCollided, b2Fixture* aFixtureCollidedAgainst, GameObject* aObjectCollidedagainst, b2Vec2 aCollisionNormal, b2Vec2 aContactPoint, bool aTouching)
{
}

//Contact listener functions empty so that they can be overwritten in child class
void  GameObject::EndCollision(b2Fixture* aFixtureCollided, b2Fixture* aFixtureCollidedAgainst, GameObject* aObjectCollidedagainst, b2Vec2 aCollisionNormal, b2Vec2 aContactPoint, bool aTouching)
{
}

//Contact listener functions empty so that they can be overwritten in child class
void GameObject::CollsionPostSolve(b2Contact* aContact, b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, const  b2ContactImpulse* impulse)
{
}

//Contact listener functions empty so that they can be overwritten in child class
void GameObject::CollsionPreSolve(b2Contact* aContact, b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal)
{
}

void GameObject::SetFixtureCollisionsFiltering(int aFixtureIndex, signed short aFixtureGroup, unsigned short aFixtureCategory, unsigned short aFixtureMasks)
{
	SetFixtureCollisionsFilteringGroup(aFixtureIndex, aFixtureGroup);
	SetFixtureCollisionsFilteringCategory(aFixtureIndex, aFixtureCategory);
	SetFixtureCollisionsFilteringMask(aFixtureIndex, aFixtureMasks);
}

void GameObject::SetFixtureCollisionsFilteringGroup(int aFixtureIndex, signed short aFixtureGroup)
{
	if (aFixtureIndex >= -1)//Check for positive values
	{
		//Modify all fixtures
		if (aFixtureIndex == -1 && m_pFixtures.empty() == false)
		{
			for (unsigned int i = 0; i < m_pFixtures.size(); i++)
			{
				if (m_pFixtures.at(i) != nullptr)
				{
					b2Filter collisionFilterData;//temporary variable to store the filter data
					collisionFilterData = m_pFixtures.at(i)->GetFilterData();//Get the current collision filtering
					collisionFilterData.groupIndex = aFixtureGroup;//Set the group filter
					m_pFixtures.at(i)->SetFilterData(collisionFilterData);//Set the updated filter data in the fixture
				}
			}
		}
		//Modify only 1 fixture
		else if (m_pFixtures.empty() == false && (unsigned int)aFixtureIndex < m_pFixtures.size())
		{

			if (m_pFixtures.at(aFixtureIndex) != nullptr)
			{
				b2Filter collisionFilterData;//temporary variable to store the filter data
				collisionFilterData = m_pFixtures.at(aFixtureIndex)->GetFilterData();//Get the current collision filtering
				collisionFilterData.groupIndex = aFixtureGroup;//Set the group filter
				m_pFixtures.at(aFixtureIndex)->SetFilterData(collisionFilterData);//Set the updated filter data in the fixture
			}
		}
	}
}

void GameObject::SetFixtureCollisionsFilteringCategory(int aFixtureIndex, unsigned short aFixtureCategory)
{
	if (aFixtureIndex >= -1)
	{
		//Modify all fixtures
		if (aFixtureIndex == -1 && m_pFixtures.empty() == false)
		{
			for (unsigned int i = 0; i < m_pFixtures.size(); i++)
			{
				if (m_pFixtures.at(i) != nullptr)
				{
					b2Filter collisionFilterData;//temporary variable to store the filter data
					collisionFilterData = m_pFixtures.at(i)->GetFilterData();//Get the current collision filtering
					collisionFilterData.categoryBits = aFixtureCategory;//Set the category filter
					m_pFixtures.at(i)->SetFilterData(collisionFilterData);//Set the updated filter data in the fixture
				}
			}
		}
		//Modify only 1 fixture
		else if (m_pFixtures.empty() == false && (unsigned int)aFixtureIndex < m_pFixtures.size())
		{
			if (m_pFixtures.at(aFixtureIndex) != nullptr)
			{
				b2Filter collisionFilterData;//temporary variable to store the filter data
				collisionFilterData = m_pFixtures.at(aFixtureIndex)->GetFilterData();//Get the current collision filtering
				collisionFilterData.categoryBits = aFixtureCategory;//Set the category filter
				m_pFixtures.at(aFixtureIndex)->SetFilterData(collisionFilterData);//Set the updated filter data in the fixture
			}
		}
	}
}

void GameObject::SetFixtureCollisionsFilteringMask(int aFixtureIndex, unsigned short aFixtureMasks)
{
	if (aFixtureIndex >= -1)
	{
		//Modify all fixtures
		if (aFixtureIndex == -1 && m_pFixtures.empty() == false)
		{
			for (unsigned int i = 0; i < m_pFixtures.size(); i++)
			{
				if (m_pFixtures.at(i) != nullptr)
				{
					b2Filter collisionFilterData;//temporary variable to store the filter data
					collisionFilterData = m_pFixtures.at(i)->GetFilterData();//Get the current collision filtering
					collisionFilterData.maskBits = aFixtureMasks;//Set the mask filter
					m_pFixtures.at(i)->SetFilterData(collisionFilterData);//Set the updated filter data in the fixture
				}
			}
		}
		//Modify only 1 fixture
		else if (m_pFixtures.empty() == false && (unsigned int)aFixtureIndex < m_pFixtures.size())
		{
			if (m_pFixtures.at(aFixtureIndex) != nullptr)
			{
				b2Filter collisionFilterData;//temporary variable to store the filter data
				collisionFilterData = m_pFixtures.at(aFixtureIndex)->GetFilterData();//Get the current collision filtering
				collisionFilterData.maskBits = aFixtureMasks;//Set the mask filter
				m_pFixtures.at(aFixtureIndex)->SetFilterData(collisionFilterData);//Set the updated filter data in the fixture
			}
		}
	}
}

void GameObject::Reset()
{
	SetEnabled(true);//Enable object
	SetTransform(m_defaultPosition, m_defaultRotation, m_defaultScale);//Move the object to its default location
	ResetPhysicsBody();//Reset physics body
}

void GameObject::ResetPhysicsBody()
{
	if (m_pPhysicsBody != nullptr)
	{
		//Ensure the body is activated
		m_pPhysicsBody->SetActive(true);
		m_pPhysicsBody->SetAwake(true);

		//Remove any previous force/velocity the body had
		ResetPhysicsBodyVelocities();
	}
}

void GameObject::ResetPhysicsBodyVelocities()
{
	if (m_pPhysicsBody != nullptr)
	{
		m_pPhysicsBody->SetLinearVelocity(b2Vec2(0, 0));//reset the linear velocity
		m_pPhysicsBody->SetAngularVelocity(0);//reset the angular velocity
	}
}

void GameObject::ResetPhysicsBodyXVelocity()
{
	if (m_pPhysicsBody != nullptr)
	{
		m_pPhysicsBody->SetLinearVelocity(b2Vec2(0, m_pPhysicsBody->GetLinearVelocity().y));//reset the linear velocity	on X axis
	}
}

void GameObject::ResetPhysicsBodyYVelocity()
{
	if (m_pPhysicsBody != nullptr)
	{
		m_pPhysicsBody->SetLinearVelocity(b2Vec2(m_pPhysicsBody->GetLinearVelocity().x, 0));//reset the linear velocity	on Y axis
	}
}

void GameObject::SetActivePhysicsBody(bool aStatus)
{
	if (m_pPhysicsBody != nullptr)
	{
		m_pPhysicsBody->SetActive(aStatus);
	}
}

//Function to set all the transform values of the object at once
void GameObject::SetTransform(Vector3 aPosition, Vector3 aRotation, Vector3 aScale)
{
	m_Position = aPosition;
	m_Rotation = aRotation;
	m_Scale = aScale;

	//Move the physics body
	if (m_pPhysicsBody != nullptr)
	{
		m_pPhysicsBody->SetTransform(b2Vec2(m_Position.x, m_Position.y), m_Rotation.z);
	}
}

//Function to merely set all default values
void GameObject::SetDefaultTransform(Vector3 aPosition, Vector3 aRotation, Vector3 aScale)
{
	m_defaultPosition = aPosition;
	m_defaultRotation = aRotation;
	m_defaultScale = aScale;

	//Move the physics body
	if (m_pPhysicsBody != nullptr)
	{
		m_pPhysicsBody->SetTransform(b2Vec2(m_defaultPosition.x, m_defaultPosition.y), m_defaultRotation.z);
	}
}

//Utility function to set both the position of the box2d body and of the 3D Gameobject
void GameObject::SetPhysicsAndGameObjectPosition(vec3 aPosition)
{
	m_Position = aPosition;
	if (m_pPhysicsBody != nullptr)
	{
		float currentAngle = m_pPhysicsBody->GetTransform().q.GetAngle();//Get the current angle of the body in radians
		currentAngle = currentAngle * 180 / PI;

		SetPhysicsBodyTransform(vec2(aPosition.x, aPosition.y), currentAngle);//This function checks  that the b2Body is not null
	}
}

Vector3 GameObject::GetDirection3D()
{						 
	//Direction from rotation
	//x=cos(yRot)*cos(zRot)
	//y=sin(yRot)*cos(zRot)
	//z=sin(zRot)

	Vector3 rotation = GetRotationRadians();
	Vector3 facingDirection;
	facingDirection.x = cos(rotation.y)*cos(rotation.z);
	facingDirection.y = sin(rotation.y)*cos(rotation.z);
	facingDirection.z = sin(rotation.z);

	//Normalize the direction
	facingDirection.Normalize();

	return facingDirection;
}

Vector2 GameObject::GetDirectionXY()
{
	Vector3 rotation = GetRotationRadians();
	Vector2 facingDirection;
	facingDirection.x = cos(rotation.z);
	facingDirection.y = sin(rotation.z);

	//Normalize the direction
	facingDirection.Normalize();

	return facingDirection;
}

Vector2 GameObject::GetDirectionXZ()
{
	Vector3 rotation = GetRotationRadians();
	Vector2 facingDirection;
	facingDirection.x = cos(rotation.y);
	facingDirection.y = sin(rotation.y);

	//Normalize the direction
	facingDirection.Normalize();

	return facingDirection;
}

void GameObject::SetEnabled(bool aEnabled)
{
	//Not done in here since we can't change a body mid a Box2D World step
	//if (m_pPhysicsBody != nullptr)
	//{
	//    m_pPhysicsBody->SetActive(aEnabled);//If there is a physics body set it
	//}

	m_enabled = aEnabled;
}

bool GameObject::GetEnabled()
{
	return m_enabled;
}

void GameObject::SetScene(Scene* aScene)
{
	if (aScene != nullptr)
	{
		m_pScene = aScene;
	}
}

void GameObject::SetRenderOrder(unsigned int aRenderOrder)
{
	if (aRenderOrder < MAX_RENDER_ORDER)
	{
		m_drawRenderOrder = aRenderOrder;
	}
}

MyMatrix GameObject::GetWorldMatrix()
{
    mat4 worldMatrix;

    //Basic object matrix
    worldMatrix.CreateSRT(m_Scale, m_Rotation, m_Position);//Set in the matrix its scale, rotation and position values.

    //If the object has a parent
    if (m_pParentObject != nullptr)
    {
        //Get the parent world matrix
        MyMatrix parentMatrix = m_pParentObject->GetWorldMatrix();
        
        //Multiply the objects world matrix by its parent world matrix
        worldMatrix = parentMatrix*worldMatrix;
    }

    return worldMatrix;
}

Vector3 GameObject::GetWorldPosition()
{
    if (m_pParentObject != nullptr)
    {
        //Get the parent world matrix
        MyMatrix parentMatrix = m_pParentObject->GetWorldMatrix();

        return parentMatrix*m_Position;

    }


    return m_Position;
}

void GameObject::SetParentObject(GameObject* apParent)
{
    if (apParent != nullptr)
    {
        //Remove this object from its previous parents child list
        if (m_pParentObject != nullptr)
        {
            m_pParentObject->RemoveChild(this);
        }

        //This will remove whatever previous parent the object had. All objects can only have 1 parent (directly)
        m_pParentObject = apParent;

        //Notify the other object that this object is now its child
        apParent->AddChild(this);
    }
}

void GameObject::AddChild(GameObject* aChild)
{
    if (aChild != nullptr)
    {
        m_pChildObjects.push_back(aChild);
    }
}

void GameObject::RemoveChild(GameObject* aChild)
{
    if (aChild != nullptr)
    {
        //Search for the child
        //Go through all the childs
        for (unsigned int i = 0; i < m_pChildObjects.size(); i++)
        { 
            //Compare each child with the gameobject that wants to be removed
            if (m_pChildObjects.at(i) == aChild)
            {
                //If the child was found remove it from vector
                m_pChildObjects.erase(m_pChildObjects.begin() + i);

                //Break out of loop
                break;
            }
        }
    }
}

Vector3 GameObject::GetSizes()
{
	Vector3 size;
	size.x = GetXLength();
	size.y = GetYLength();
	size.z = GetZLength();

	return size;
}

float GameObject::GetXLength()
{
	//Get minimum and max value
	float minX = GetMinXYZ().x;
	float maxX = GetMaxXYZ().x;

	return abs(maxX - minX);//Length will be the difference between the 2 values,because it is a distance it will be absolute value
}

float GameObject::GetYLength()
{
	//Get minimum and max value
	float minY = GetMinXYZ().y;
	float maxY = GetMaxXYZ().y;

	return abs(maxY - minY);//Length will be the difference between the 2 values,because it is a distance it will be absolute value
}

float GameObject::GetZLength()
{
	//Get minimum and max value
	float minZ = GetMinXYZ().z;
	float maxZ = GetMaxXYZ().z;

	return abs(maxZ - minZ);//Length will be the difference between the 2 values,because it is a distance it will be absolute value
}

float GameObject::GetXYRadius()
{
	float radius = (GetXLength() + GetYLength()) / 2;//Add the Y and X length divided by 2 to get the average diameter
	radius /= 2;//Divide the addition of the lengths by 2 to get the average radius

	return radius;
}

Vector3 GameObject::GetMinXYZ()
{
	if (m_pMesh != nullptr)
	{
		Vector3 scaledMeshMinXYZ = m_pMesh->GetMeshMinXYZ();//Get the original minimum values from the mesh
		scaledMeshMinXYZ = Vector3(scaledMeshMinXYZ.x*m_Scale.x, scaledMeshMinXYZ.y*m_Scale.y, scaledMeshMinXYZ.z*m_Scale.z);//Scale the positions according to the current object scale
		scaledMeshMinXYZ += m_Position;//Consider the current object position
		return scaledMeshMinXYZ;
	}

	return m_Position;
}

Vector3 GameObject::GetMaxXYZ()
{
	if (m_pMesh != nullptr)
	{
		Vector3 scaledMeshMaxXYZ = m_pMesh->GetMeshMaxXYZ();//Get the original maximum values from the mesh
		scaledMeshMaxXYZ = Vector3(scaledMeshMaxXYZ.x*m_Scale.x, scaledMeshMaxXYZ.y*m_Scale.y, scaledMeshMaxXYZ.z*m_Scale.z);//Scale the positions according to the current object scale
		scaledMeshMaxXYZ += m_Position;//Consider the current object position
		return scaledMeshMaxXYZ;
	}

	return m_Position;
}

void GameObject::SetReflectivity(float aReflectivity)
{
    //Cap the value so that it is a percentage (0.0f to 1.0f)
    if (aReflectivity > 1.0f) 
        m_reflectivity = 1.0f;
    else if (aReflectivity < 0.0f)
        m_reflectivity = 0.0f;
    else 
    m_reflectivity = aReflectivity; 
}

Tween<float>* GameObject::GetFloatTweener(unsigned int aIndex)
{
	if (aIndex < m_pfloatTweens.size())
	{
		return m_pfloatTweens.at(aIndex);
	}

	return nullptr;
}

Tween<float>* GameObject::GetFloatTweenerByName(std::string aTweenName)
{
    //Check if the tween is in the vector
    for (unsigned int i = 0; i < m_pfloatTweens.size(); i++)
    {
        if (m_pfloatTweens.at(i) != nullptr)
        {
            if (m_pfloatTweens.at(i)->GetTweenName() == aTweenName)
            {
                return m_pfloatTweens.at(i);//If the tween was found
            }
        }
    }

    //If the tween was not found
    return nullptr;
}

void GameObject::AddFloatTweener(Tween<float>* aTweener)
{
	if (aTweener != nullptr)
	{
		m_pfloatTweens.push_back(aTweener);
	}
}

bool GameObject::GetFloatTweenerOver(std::string aTweenName)
{
    //Search for the tween
    Tween<float>* tweener = GetFloatTweenerByName(aTweenName);

    //If the tween is over
    if (tweener != nullptr)
    {
        return tweener->GetTweenOver();//Return whether the tween is over or not
    }

    //If the tween was not found, return true
    return true;
}

Tween<vec3>* GameObject::GetVec3Tweener(unsigned int aIndex)
{
	if (aIndex < m_pVec3Tweens.size())
	{
		return m_pVec3Tweens.at(aIndex);
	}

	return nullptr;
}

Tween<vec3>* GameObject::GetVec3TweenerByName(std::string aTweenName)
{
    //Check if the tween is in the vector
    for (unsigned int i = 0; i < m_pVec3Tweens.size(); i++)
    {
        if (m_pVec3Tweens.at(i) != nullptr)
        {
            if (m_pVec3Tweens.at(i)->GetTweenName() == aTweenName)
            {
                return m_pVec3Tweens.at(i);//If the tween was found
            }
        }
    }

    //If the tween was not found
    return nullptr;
}

void GameObject::AddVec3Tweener(Tween<vec3>* aTweener)
{
	if (aTweener != nullptr)
	{
		m_pVec3Tweens.push_back(aTweener);
	}
}

bool GameObject::GetVec3TweenerOver(std::string aTweenName)
{
    //Search for the tween
    Tween<vec3>* tweener = GetVec3TweenerByName(aTweenName);

    //If the tween is over
    if (tweener != nullptr)
    {
        return tweener->GetTweenOver();//Return whether the tween is over or not
    }

    //If the tween was not found, return true
    return true;
}

void GameObject::SetTweenPosition(Vector3 aEndValue, double aEndTime, TweeningType aType, double aDelay)
{
	bool tweenCreated = false;

	//Check if the tween has already been created
    Tween<vec3>* tweener = GetVec3TweenerByName(GAMEOBJECT_POSITION_TWEEN_NAME);

    //If it was found
    if (tweener != nullptr)
    {
        tweenCreated = true;
    }

	//If there is no tween for it, created it
	if (tweenCreated == false)
	{
		tweener = new Tween<vec3>;//Create tween
		AddVec3Tweener(tweener);//Add it to vector
	}

	//Set the tween
    tweener->SetupTween(&m_Position, aEndValue, aEndTime, aType,  aDelay,GAMEOBJECT_POSITION_TWEEN_NAME);
}

void GameObject::SetTweenXPosition(float aEndValue, double aEndTime, TweeningType aType, double aDelay)
{
	bool tweenCreated = false;

    //Check if the tween has already been created
    Tween<float>* tweener = GetFloatTweenerByName(GAMEOBJECT_X_POSITION_TWEEN_NAME);

    //If it was found
    if (tweener != nullptr)
    {
        tweenCreated = true;
    }

	//If there is no tween for it, created it
	if (tweenCreated == false)
	{
		tweener = new Tween<float>;//Create tween
		AddFloatTweener(tweener);//Add it to vector
	}

	//Set the tween
    tweener->SetupTween(&m_Position.x, aEndValue, aEndTime, aType,aDelay,GAMEOBJECT_X_POSITION_TWEEN_NAME);
}

void GameObject::SetTweenYPosition(float aEndValue, double aEndTime, TweeningType aType, double aDelay)
{
	bool tweenCreated = false;

    //Check if the tween has already been created
    Tween<float>* tweener = GetFloatTweenerByName(GAMEOBJECT_Y_POSITION_TWEEN_NAME);

    //If it was found
    if (tweener != nullptr)
    {
        tweenCreated = true;
    }

	//If there is no tween for it, created it
	if (tweenCreated == false)
	{
		tweener = new Tween<float>;//Create tween
		AddFloatTweener(tweener);//Add it to vector
	}

	//Set the tween
    tweener->SetupTween(&m_Position.y, aEndValue, aEndTime, aType, aDelay,GAMEOBJECT_Y_POSITION_TWEEN_NAME);
}

void GameObject::SetTweenZPosition(float aEndValue, double aEndTime, TweeningType aType, double aDelay)
{
	bool tweenCreated = false;

    //Check if the tween has already been created
    Tween<float>* tweener = GetFloatTweenerByName(GAMEOBJECT_Z_POSITION_TWEEN_NAME);

    //If it was found
    if (tweener != nullptr)
    {
        tweenCreated = true;
    }

	//If there is no tween for it, created it
	if (tweenCreated == false)
	{
		tweener = new Tween<float>;//Create tween
		AddFloatTweener(tweener);//Add it to vector
	}

	//Set the tween
    tweener->SetupTween(&m_Position.z, aEndValue, aEndTime, aType, aDelay,GAMEOBJECT_Z_POSITION_TWEEN_NAME);
}

void GameObject::SetTweenRotation(Vector3 aEndValue, double aEndTime, TweeningType aType, double aDelay)
{
	bool tweenCreated = false;

    //Check if the tween has already been created
    Tween<vec3>* tweener = GetVec3TweenerByName(GAMEOBJECT_ROTATION_TWEEN_NAME);

    //If it was found
    if (tweener != nullptr)
    {
        tweenCreated = true;
    }

	//If there is no tween for it, created it
	if (tweenCreated == false)
	{
		tweener = new Tween<vec3>;//Create tween
		AddVec3Tweener(tweener);//Add it to vector
	}

	//Set the tween
    tweener->SetupTween(&m_Rotation, aEndValue, aEndTime, aType, aDelay,GAMEOBJECT_ROTATION_TWEEN_NAME);
}

void GameObject::SetTweenXRotation(float aEndValue, double aEndTime, TweeningType aType, double aDelay)
{
	bool tweenCreated = false;

    //Check if the tween has already been created
    Tween<float>* tweener = GetFloatTweenerByName(GAMEOBJECT_ROTATION_X_TWEEN_NAME);

    //If it was found
    if (tweener != nullptr)
    {
        tweenCreated = true;
    }

	//If there is no tween for it, created it
	if (tweenCreated == false)
	{
		tweener = new Tween<float>;//Create tween
		AddFloatTweener(tweener);//Add it to vector
	}

	//Set the tween
    tweener->SetupTween(&m_Rotation.x, aEndValue, aEndTime, aType,aDelay,GAMEOBJECT_ROTATION_X_TWEEN_NAME);
}

void GameObject::SetTweenYRotation(float aEndValue, double aEndTime, TweeningType aType,double aDelay)
{
	bool tweenCreated = false;

    //Check if the tween has already been created
    Tween<float>* tweener = GetFloatTweenerByName(GAMEOBJECT_ROTATION_Y_TWEEN_NAME);

    //If it was found
    if (tweener != nullptr)
    {
        tweenCreated = true;
    }

	//If there is no tween for it, created it
	if (tweenCreated == false)
	{
		tweener = new Tween<float>;//Create tween
		AddFloatTweener(tweener);//Add it to vector
	}

	//Set the tween
    tweener->SetupTween(&m_Rotation.y, aEndValue, aEndTime, aType, aDelay,GAMEOBJECT_ROTATION_Y_TWEEN_NAME);
}

void GameObject::SetTweenZRotation(float aEndValue, double aEndTime, TweeningType aType, double aDelay)
{
	bool tweenCreated = false;

    //Check if the tween has already been created
    Tween<float>* tweener = GetFloatTweenerByName(GAMEOBJECT_ROTATION_Z_TWEEN_NAME);

    //If it was found
    if (tweener != nullptr)
    {
        tweenCreated = true;
    }

	//If there is no tween for it, created it
	if (tweenCreated == false)
	{
		tweener = new Tween<float>;//Create tween
		AddFloatTweener(tweener);//Add it to vector
	}

	//Set the tween
    tweener->SetupTween(&m_Rotation.z, aEndValue, aEndTime, aType,aDelay, GAMEOBJECT_ROTATION_Z_TWEEN_NAME);
}

void GameObject::SetTweenScale(vec3 aEndValue, double aEndTime, TweeningType aType, double aDelay)
{
	bool tweenCreated = false;
	
    //Check if the tween has already been created
    Tween<vec3>* tweener = GetVec3TweenerByName(GAMEOBJECT_SCALE_TWEEN_NAME);

    //If it was found
    if (tweener != nullptr)
    {
        tweenCreated = true;
    }

	//If there is no tween for it, created it
	if (tweenCreated == false)
	{
		tweener = new Tween<vec3>;//Create tween
		AddVec3Tweener(tweener);//Add it to vector
	}

	//Set the tween
    tweener->SetupTween(&m_Scale, aEndValue, aEndTime, aType, aDelay,GAMEOBJECT_SCALE_TWEEN_NAME);
}

void GameObject::SetTweenXScale(float aEndValue, double aEndTime, TweeningType aType, double aDelay)
{
	bool tweenCreated = false;
	
    //Check if the tween has already been created
    Tween<float>* tweener = GetFloatTweenerByName(GAMEOBJECT_SCALE_X_TWEEN_NAME);

    //If it was found
    if (tweener != nullptr)
    {
        tweenCreated = true;
    }

	//If there is no tween for it, created it
	if (tweenCreated == false)
	{
		tweener = new Tween<float>;//Create tween
		AddFloatTweener(tweener);//Add it to vector
	}

	//Set the tween
    tweener->SetupTween(&m_Scale.x, aEndValue, aEndTime, aType, aDelay,GAMEOBJECT_SCALE_X_TWEEN_NAME);
}

void GameObject::SetTweenYScale(float aEndValue, double aEndTime, TweeningType aType, double aDelay)
{
	bool tweenCreated = false;

    //Check if the tween has already been created
    Tween<float>* tweener = GetFloatTweenerByName(GAMEOBJECT_SCALE_Y_TWEEN_NAME);

    //If it was found
    if (tweener != nullptr)
    {
        tweenCreated = true;
    }

	//If there is no tween for it, created it
	if (tweenCreated == false)
	{
		tweener = new Tween<float>;//Create tween
		AddFloatTweener(tweener);//Add it to vector
	}

	//Set the tween
    tweener->SetupTween(&m_Scale.y, aEndValue, aEndTime, aType, aDelay,GAMEOBJECT_SCALE_Y_TWEEN_NAME);
}

void GameObject::SetTweenZScale(float aEndValue, double aEndTime, TweeningType aType, double aDelay)
{
	bool tweenCreated = false;

    //Check if the tween has already been created
    Tween<float>* tweener = GetFloatTweenerByName(GAMEOBJECT_SCALE_Z_TWEEN_NAME);

    //If it was found
    if (tweener != nullptr)
    {
        tweenCreated = true;
    }

	//If there is no tween for it, created it
	if (tweenCreated == false)
	{
		tweener = new Tween<float>;//Create tween
		AddFloatTweener(tweener);//Add it to vector
	}

	//Set the tween
    tweener->SetupTween(&m_Scale.z, aEndValue, aEndTime, aType,aDelay, GAMEOBJECT_SCALE_Z_TWEEN_NAME);
}

vector<b2Vec2> GameObject::GetShapeVertices(b2Shape* aShape)
{
	vector<b2Vec2> shapeVertices;

	if (aShape != nullptr)
	{
		unsigned int numVertices = 0;
		b2Shape::Type shapeType = aShape->GetType();//Get the type of the shape

		//Cast the shape as its corresponding type and get all its vertices
		switch (shapeType)
		{

		case b2Shape::e_circle:
		{
			b2CircleShape* circleShape = (b2CircleShape*)aShape;//Cast to circle shape
			numVertices = circleShape->GetVertexCount();//Get the number of vertices

			//Go through all the vertices
			for (unsigned int i = 0; i < numVertices; i++)
			{
				shapeVertices.push_back(circleShape->GetVertex(i));
			}
			break;
		}

		case b2Shape::e_edge:
		{
			b2EdgeShape* edgeShape = (b2EdgeShape*)aShape;//Cast to edge shape

			//If there is a vertex 0
			if (edgeShape->m_hasVertex0 == true)
			{
				shapeVertices.push_back(edgeShape->m_vertex0);//Add vertex
				numVertices++;
			}

			shapeVertices.push_back(edgeShape->m_vertex1);//Add vertex
			numVertices++;

			shapeVertices.push_back(edgeShape->m_vertex2);//Add vertex
			numVertices++;

			//If there is a vertex 3
			if (edgeShape->m_hasVertex3 == true)
			{
				shapeVertices.push_back(edgeShape->m_vertex3);//Add vertex
				numVertices++;
			}

			break;
		}

		case b2Shape::e_polygon:
		{
			b2PolygonShape* polygonShape = (b2PolygonShape*)aShape;//Cast to polygon shape
			numVertices = polygonShape->GetVertexCount();//Get number of vertices

			//Go through all the vertices
			for (unsigned int i = 0; i < numVertices; i++)
			{
				shapeVertices.push_back(polygonShape->GetVertex(i));
			}

			break;
		}

		case b2Shape::e_chain:
		{
			b2ChainShape* chainShape = (b2ChainShape*)aShape;//Cast to polygon shape
			numVertices = chainShape->m_count;//Get the number of vertices

			//Go through all the vertices
			for (unsigned int i = 0; i < numVertices; i++)
			{
				shapeVertices.push_back(chainShape->m_vertices[i]);
			}

			break;
		}

		case b2Shape::e_typeCount:
			break;
		default:
			break;
		}

	}

	return shapeVertices;
}

vec2 GameObject::GetMinFixtureXY(int aFixtureIndex)
{
	b2Vec2 minValues = b2Vec2(0, 0);

	if (m_pPhysicsBody != nullptr)
	{
		vector<b2Vec2> shapeVertices;

		//Set all fixtures
		if (aFixtureIndex = -1)
		{
			vector<b2Vec2>currentShapeVertices;

			for (unsigned int i = 0; i < m_pFixtures.size(); i++)
			{
				if (m_pFixtures.at(i) != nullptr)
				{
					//currentShapeVertices = GetShapeVertices(m_pFixtures.at(i)->GetShape());//Get the vertices of each fixture
					shapeVertices.insert(std::end(shapeVertices), std::begin(currentShapeVertices), std::end(currentShapeVertices));//Insert all the vertices of the fixture into a single vector
				}

			}
		}//Set a single fixture
		else if (m_pFixtures.empty() == false && (unsigned int)aFixtureIndex < m_pFixtures.size())
		{
			if (m_pFixtures.at(aFixtureIndex) != nullptr)
			{
				shapeVertices = GetShapeVertices(m_pFixtures.at(aFixtureIndex)->GetShape());//Get the vertices of the fixture
			}
		}

		//Calculate the min values in the vector of vertices by comparing each vertex to the minimum value
		for (unsigned int i = 0; i < shapeVertices.size(); i++)
		{
			//Check x value
			if (shapeVertices.at(i).x < minValues.x)
			{
				minValues.x = shapeVertices.at(i).x;
			}

			//Check y value
			if (shapeVertices.at(i).y < minValues.y)
			{
				minValues.y = shapeVertices.at(i).y;
			}
		}
	}

	return vec2(minValues.x, minValues.y);
}

vec2 GameObject::GetMaxFixtureXY(int aFixtureIndex)
{
	b2Vec2 maxValues = b2Vec2(0, 0);

	if (m_pPhysicsBody != nullptr)
	{
		vector<b2Vec2> shapeVertices;

		//Set all fixtures
		if (aFixtureIndex = -1)
		{
			vector<b2Vec2>currentShapeVertices;

			for (unsigned int i = 0; i < m_pFixtures.size(); i++)
			{
				if (m_pFixtures.at(i) != nullptr)
				{
					currentShapeVertices = GetShapeVertices(m_pFixtures.at(i)->GetShape());//Get the vertices of each fixture
					shapeVertices.insert(std::end(shapeVertices), std::begin(currentShapeVertices), std::end(currentShapeVertices));//Insert all the vertices of the fixture into a single vector.
					//In this case the inser function inserts the currentShapeVertices (from beginning to end) at the end of the shape vertices.
				}

			}
		}//Set a single fixture
		else if (m_pFixtures.empty() == false && (unsigned int)aFixtureIndex < m_pFixtures.size())
		{
			if (m_pFixtures.at(aFixtureIndex) != nullptr)
			{
				shapeVertices = GetShapeVertices(m_pFixtures.at(aFixtureIndex)->GetShape());//Get the vertices of the fixture
			}
		}

		//Calculate the max values in the vector of vertices by comparing each vertex to the maximum value
		for (unsigned int i = 0; i < shapeVertices.size(); i++)
		{
			//Check x value
			if (shapeVertices.at(i).x > maxValues.x)
			{
				maxValues.x = shapeVertices.at(i).x;
			}

			//Check y value
			if (shapeVertices.at(i).y > maxValues.y)
			{
				maxValues.y = shapeVertices.at(i).y;
			}
		}
	}

	return vec2(maxValues.x, maxValues.y);
}

void GameObject::SetPhysicsBodyType(b2BodyType aType)
{
	if (m_pPhysicsBody != nullptr)
	{
		m_pPhysicsBody->SetType(aType);
	}
}

b2BodyType GameObject::GetPhysicsBodyType()
{
	if (m_pPhysicsBody != nullptr)
	{
		return m_pPhysicsBody->GetType();
	}

	//Default value if body not found
	return b2_dynamicBody;
}

void GameObject::SetPhysicsBodyIsSensor(bool aState, int aFixtureIndex)
{
	if (m_pPhysicsBody != nullptr)
	{
		//Set all fixtures
		if (aFixtureIndex = -1)
		{
			for (unsigned int i = 0; i < m_pFixtures.size(); i++)
			{
				if (m_pFixtures.at(i) != nullptr)
				{
					m_pFixtures.at(i)->SetSensor(aState);
				}
			}
		}//Set a single fixture
		else if (m_pFixtures.empty() == false && (unsigned int)aFixtureIndex < m_pFixtures.size())
		{
			if (m_pFixtures.at(aFixtureIndex) != nullptr)
			{
				m_pFixtures.at(aFixtureIndex)->SetSensor(aState);
			}
		}
	}
}

bool GameObject::GetPhysicsBodyIsSensor(int aFixtureIndex)
{
	if (m_pPhysicsBody != nullptr)
	{
		if (m_pFixtures.empty() == false && (unsigned int)aFixtureIndex < m_pFixtures.size())
		{
			if (m_pFixtures.at(aFixtureIndex) != nullptr)
			{
				return m_pFixtures.at(aFixtureIndex)->IsSensor();
			}
		}
	}

	return false;
}

vec2 GameObject::GetPhysicsBodyVelocity()
{

	if (m_pPhysicsBody != nullptr)
	{
		b2Vec2 physicsVelocity;
		physicsVelocity = m_pPhysicsBody->GetLinearVelocity();

		return vec2(physicsVelocity.x, physicsVelocity.y);
	}

	return vec2(0, 0);
}

void GameObject::SetPhysicsBodyTransform(vec2 aPosition, float aAngle)
{
	if (m_pPhysicsBody != nullptr)
	{
		//The setTransform takes the angle in radians.
		float radianAngle = aAngle*PI / 180;//Convert the angle to radians.

		m_pPhysicsBody->SetTransform(b2Vec2(aPosition.x, aPosition.y), radianAngle);
	}
}

void GameObject::SetPhysicsBodyRestitution(float aRestitution, int aFixtureIndex)
{
	if (m_pPhysicsBody != nullptr)
	{
		//Set all fixtures
		if (aFixtureIndex = -1)
		{
			for (unsigned int i = 0; i < m_pFixtures.size(); i++)
			{
				if (m_pFixtures.at(i) != nullptr)
				{
					m_pFixtures.at(i)->SetRestitution(aRestitution);
				}
			}
		}//Set a single fixture
		else if (m_pFixtures.empty() == false && (unsigned int)aFixtureIndex < m_pFixtures.size())
		{
			if (m_pFixtures.at(aFixtureIndex) != nullptr)
			{
				m_pFixtures.at(aFixtureIndex)->SetRestitution(aRestitution);
			}
		}
	}
}

void GameObject::SetPhysicsBodyDensity(float aDensity, int aFixtureIndex)
{
	if (m_pPhysicsBody != nullptr)
	{
		//Set all fixtures
		if (aFixtureIndex = -1)
		{
			for (unsigned int i = 0; i < m_pFixtures.size(); i++)
			{
				if (m_pFixtures.at(i) != nullptr)
				{
					m_pFixtures.at(i)->SetDensity(aDensity);
					m_pPhysicsBody->ResetMassData();//Since the density was changed,update the new mass of the body
				}
			}
		}//Set a single fixture
		else if (m_pFixtures.empty() == false && (unsigned int)aFixtureIndex < m_pFixtures.size())
		{
			if (m_pFixtures.at(aFixtureIndex) != nullptr)
			{
				m_pFixtures.at(aFixtureIndex)->SetDensity(aDensity);
				m_pPhysicsBody->ResetMassData();//Since the density was changed,update the new mass of the body
			}
		}
	}
}

void GameObject::SetPhysicsBodyFriction(float aFriction, int aFixtureIndex)
{
	if (m_pPhysicsBody != nullptr)
	{

		//Set all fixtures
		if (aFixtureIndex = -1)
		{
			for (unsigned int i = 0; i < m_pFixtures.size(); i++)
			{
				if (m_pFixtures.at(i) != nullptr)
				{
					m_pFixtures.at(i)->SetFriction(aFriction);
				}
			}
		}//Set a single fixture
		else if (m_pFixtures.empty() == false && (unsigned int)aFixtureIndex < m_pFixtures.size())
		{
			if (m_pFixtures.at(aFixtureIndex) != nullptr)
			{
				m_pFixtures.at(aFixtureIndex)->SetFriction(aFriction);
			}
		}
	}
}

//Get the restitution between 2 objects
float GameObject::GetSimulatedRestitution(float aRestitutionCollidingObject, int aFixtureIndex)
{
	float restitution = aRestitutionCollidingObject;// max(m_pFixtures.at(0)->GetRestitution(), b.restitution) Box2D formula for restitution

	if (m_pPhysicsBody != nullptr)
	{
		if (m_pFixtures.empty() == false)
		{
			//Calculate the highest restitution among all objects
			if (aFixtureIndex = -1)
			{
				float fixtureRestitution;

				for (unsigned int i = 0; i < m_pFixtures.size(); i++)
				{
					if (m_pFixtures.at(i) != nullptr)
					{
						fixtureRestitution = m_pFixtures.at(i)->GetRestitution();

						if (restitution < fixtureRestitution)//See which is the biggest restitution
						{
							restitution = fixtureRestitution;//Set the new highest restitution
						}
					}
				}

				restitution = b2MixRestitution(restitution, aRestitutionCollidingObject);//Get the restitution

			}//Set a single fixture
			else if ((unsigned int)aFixtureIndex < m_pFixtures.size())
			{
				if (m_pFixtures.at(aFixtureIndex) != nullptr)
				{
					restitution = b2MixRestitution(m_pFixtures.at(aFixtureIndex)->GetRestitution(), aRestitutionCollidingObject);//Get the restitution in the  fixture
				}
			}
		}
	}

	return restitution;
}

float GameObject::GetSimulatedFriction(float aFrictionCollidingObject, int aFixtureIndex)
{
	float friction = aFrictionCollidingObject;

	if (m_pPhysicsBody != nullptr)
	{
		if (m_pFixtures.empty() == false)
		{
			//Calculate the highest restitution among all objects
			if (aFixtureIndex = -1)
			{
				float fixtureFriction;

				for (unsigned int i = 0; i < m_pFixtures.size(); i++)
				{
					if (m_pFixtures.at(i) != nullptr)
					{
						fixtureFriction = m_pFixtures.at(i)->GetFriction();

						if (fixtureFriction > friction)//See which is the biggest friction
						{
							friction = fixtureFriction;//Set the new highest friction
						}
					}
				}

				friction = b2MixFriction(friction, aFrictionCollidingObject);//Get the friction

			}//Set a single fixture
			else if ((unsigned int)aFixtureIndex < m_pFixtures.size())
			{
				if (m_pFixtures.at(aFixtureIndex) != nullptr)
				{
					friction = b2MixFriction(m_pFixtures.at(aFixtureIndex)->GetRestitution(), aFrictionCollidingObject);//Get the friction in the  fixture
				}
			}
		}
	}

	return aFrictionCollidingObject;
}

//Function that simulates bouncing  a Box2D body, function is 2D.
void GameObject::BounceObjectBody(float aRestituionCollidingObject, float aFrictionCollidingObject)
{
	if (m_pPhysicsBody != nullptr)
	{
		//Get the restitution of the objects
		float restitution = GetSimulatedRestitution(aRestituionCollidingObject);

		//Calculate the new velocity the objects will have
		b2Vec2 newVelocity = m_pPhysicsBody->GetLinearVelocity();//Get the current object velocity
		newVelocity *= -1;//Inverse velocities
		newVelocity *= restitution;//Apply the restitution

		//Set the new bouncing velocities
		m_pPhysicsBody->SetLinearVelocity(newVelocity);
		BounceAngleZ(aFrictionCollidingObject);
	}
}

//Function that simulates bouncing a Box2D body in X axis
void GameObject::BounceXObjectBody(float aRestituionCollidingObject, float aFrictionCollidingObject)
{
	if (m_pPhysicsBody != nullptr)
	{
		//Get the restitution of the objects
		float restitution = GetSimulatedRestitution(aRestituionCollidingObject);

		//Calculate the new velocity the objects will have
		b2Vec2 newVelocity = m_pPhysicsBody->GetLinearVelocity();//Get the current object velocity
		newVelocity.x *= -1;//Only inverse the x Axis
		newVelocity.x *= restitution;//Apply the restitution only to X axis

		//Set the new bouncing velocities
		m_pPhysicsBody->SetLinearVelocity(newVelocity);
		BounceAngleZ(aFrictionCollidingObject);
	}
}

//Function that simulates bouncing  a Box2D body in Y axis
void GameObject::BounceYObjectBody(float aRestituionCollidingObject, float aFrictionCollidingObject)
{
	if (m_pPhysicsBody != nullptr)
	{
		//Get the restitution of the objects
		float restitution = GetSimulatedRestitution(aRestituionCollidingObject);

		//Calculate the new velocity the objects will have
		b2Vec2 newVelocity = m_pPhysicsBody->GetLinearVelocity();//Get the current object velocity
		newVelocity.y *= -1;//Only inverse the Y Axis
		newVelocity.y *= restitution;//Apply the restitution only to Y axis

		//Set the new bouncing velocities
		m_pPhysicsBody->SetLinearVelocity(newVelocity);
	}
}

//Function that simulates the angular velocity a box2d body would have on a collision. This only affects the angular velocity, it doesn't create a angular velocity.
void GameObject::BounceAngleZ(float aFrictionCollidingObject)
{
	if (m_pPhysicsBody != nullptr)
	{
		//Get the restitution of the objects
		float friction = GetSimulatedFriction(aFrictionCollidingObject);

		//Calculate the new velocity the objects will have
		float newAngularVelocity = m_pPhysicsBody->GetAngularVelocity();//Get the current object velocity
		newAngularVelocity *= -1;//Only inverse the direction
		newAngularVelocity *= friction;//Apply the friction

		//Set the new bouncing velocities
		m_pPhysicsBody->SetAngularVelocity(newAngularVelocity);
	}
}

//The screen position is top left up.
bool GameObject::RayCastPositionIn2d(Vector2 aScreenPosition)
{
    if (m_pScene == nullptr)
    {
        return false;
    }

    Vector2 screenSize = vec2((int)g_pGame->GetWindowWidth(), (int)g_pGame->GetWindowHeight());

    mat4 projectionMatrix = *m_pScene->GetHUDProjectionMatrix();
    CameraObject* camera = (CameraObject*)m_pScene->GetGameObject(GetCameraDrawerName());
    if (camera == nullptr)
    {
        return false;
    }
    
    mat4 viewMatrix = *camera->GetViewMatrix();

    //transform to 3d Normalised Device Coordinates
    float ndcX = (2.0f * aScreenPosition.x) / screenSize.x - 1.0f;//Convert to -1 to 1 range
    float ndcY = 1.0f - (2.0f * aScreenPosition.y) / screenSize.y;
    float ndcZ = 1.0f;//1 USED FOR 2D
    vec3 rayNDC = vec3(ndcX, ndcY, ndcZ);

    //transform to 4d Homogeneous Clip Coordinates
    Vector4 rayDirection = Vector4(rayNDC.x, rayNDC.y, 1.0, 1.0);//Make the ray point forward, by giving it a value of -1 2D

    //transform to  4d Eye (Camera) Coordinates
    projectionMatrix.Inverse();//Inverse the projection matrix
    Vector4 ray = projectionMatrix * rayDirection;//Unproject the coordinates
    ray = Vector4(ray.x, ray.y, -1.0, 0.0);//Set the point to be forward 2D

    //transform to 4D world coordinates
    Vector4 tempRayInWorld = (viewMatrix * ray);
    vec3 rayInWorld = Vector3(tempRayInWorld.x, tempRayInWorld.y, tempRayInWorld.z);
    rayInWorld.Normalize();//Normalize the ray

    ////Raycast vs plane
    vec3 planeNormal = (0.0f, 0.0f, -1.0f);//2D
    float planeOffset = 0;
    vec3 rayOrigin = camera->GetPosition();
    //Vector3 rayCamDirection=camera->GetDirection3D();
    rayDirection.Normalize();
    vec3 rayFacingDirection = vec3(rayDirection.x, rayDirection.y, rayDirection.z);

   //float distanceFromRayOrigin = ((rayOrigin - planeNormal + planeOrigin) / (planeOrigin*planeNormal));

    //if (distanceFromRayOrigin <= 0)
    //{
    //   return false;
    //}


    return true;
}

void GameObject::AddReflectivityToQuickWatch(QuickWatchManager * aQuickWatch)
{
	if (aQuickWatch != nullptr)
	{
		string QuickWatchName;
		QuickWatchName = m_Name + " Material Reflectiivty";
		aQuickWatch->AddFloat(QuickWatchName.c_str(), &m_reflectivity, 0.0f, 1.0f);//Range between 0 and 1
	}
}
