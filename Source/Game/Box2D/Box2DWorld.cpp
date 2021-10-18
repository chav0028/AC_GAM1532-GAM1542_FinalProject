#include "CommonHeader.h"
#include "Box2DWorld.h"


Box2DWorld::Box2DWorld(b2Vec2 aGravity):
m_pWorld(nullptr),
m_pDebugDrawer(nullptr),
m_pContactListener(nullptr),
m_pCenterWorldBody(nullptr),
m_worldStepTimer(0)
{
    //Create the world
    m_pWorld = new b2World(aGravity);
    
    //Create the debug draw
    m_pDebugDrawer = new Box2DDebugDraw();
    m_pWorld->SetDebugDraw(m_pDebugDrawer);

    //Create the contact listener
    m_pContactListener = new Box2DContactListener();
	m_pWorld->SetContactListener(m_pContactListener);

	//Create the center world body
	b2BodyDef bodyDefinition;
	bodyDefinition.type = b2_staticBody;
	bodyDefinition.position = b2Vec2(0, 0);
	bodyDefinition.userData = nullptr;
	m_pCenterWorldBody = m_pWorld->CreateBody(&bodyDefinition);
}


Box2DWorld::~Box2DWorld()
{
    SAFE_DELETE(m_pWorld);
    SAFE_DELETE(m_pDebugDrawer);
    SAFE_DELETE(m_pContactListener);
}

void Box2DWorld::Update(double aTimePassed)
{
	m_worldStepTimer += aTimePassed; //Increase tiemr

    //Fixed time step
    while (m_worldStepTimer > 1.0f / BOX2D_FRAMES_PER_SECOND)//Check if it time for an update
	{
		if (m_pWorld != nullptr)
		{
            m_pWorld->Step(1.0f / BOX2D_FRAMES_PER_SECOND, BOX2D_WORLD_VELOCITY_ITERATIONS, BOX2D_WORLD_POSITION_ITERATIONS);//Update the physics world, this is done before updating all the visual representation of the objects
		}

        //Reset timer
        m_worldStepTimer -= 1.0f / BOX2D_FRAMES_PER_SECOND;
	}
}

void Box2DWorld::SetWorldGravity(b2Vec2 aGravity)
{
	if (m_pWorld != nullptr)
	{
		return m_pWorld->SetGravity(aGravity);
	}
}

b2Vec2 Box2DWorld::GetWorldGravity()
{
	if (m_pWorld != nullptr)
	{
		return m_pWorld->GetGravity();
	}

	return b2Vec2(0, 0);

}

uint32 Box2DWorld::GetDebugDrawFlags()
{
	if (m_pDebugDrawer != nullptr)
	{
		return m_pDebugDrawer->GetFlags();
	}

	return 0;;
}

void Box2DWorld::SetDebugDrawFlags(uint32 aFlags)
{
	if (m_pDebugDrawer != nullptr)
	{
		m_pDebugDrawer->SetFlags(aFlags);
	}

}

void Box2DWorld::DrawDebugDrawer()
{
    if (m_pDebugDrawer != nullptr)
    {
        m_pWorld->DrawDebugData();
    }
}

void Box2DWorld::SetDebugDrawViewMatrix(mat4* aViewMatrix)
{
    if (m_pDebugDrawer != nullptr)
    {
        m_pDebugDrawer->SetViewMatrix(aViewMatrix);//Nullptr check done inside debug draw function
    }
}

void Box2DWorld::SetDebugDrawProjectionMatrix(mat4* aProjectionMatrix)
{
    if (m_pDebugDrawer != nullptr)
    {
        m_pDebugDrawer->SetProjectionMatrix(aProjectionMatrix);//Nullptr check done inside debug draw function
    }
}

void Box2DWorld::SetDebugDrawShader(ShaderProgram* aShader)
{
    if (m_pDebugDrawer != nullptr)
    {
        m_pDebugDrawer->SetShader(aShader);//Nullptr check done inside debug draw function
    }
}

b2Joint* Box2DWorld::CreateJoint(b2JointDef* aJointDefinition)
{
	if(m_pWorld != nullptr)
	{
		return m_pWorld->CreateJoint(aJointDefinition);
	}

	return nullptr;
}

void Box2DWorld::RayCast(b2RayCastCallback* aCallback, const b2Vec2& aPoint1, const b2Vec2& aPoint2)
{
    if (m_pWorld != nullptr)
    {
        m_pWorld->RayCast(aCallback, aPoint1, aPoint2);
    }
}