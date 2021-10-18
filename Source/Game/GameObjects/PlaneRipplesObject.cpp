#include "CommonHeader.h"

PlaneRippleObject::PlaneRippleObject(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 UVScale, vec2 UVOffset) :
GameObject(pScene, name, pos, rot, scale, pMesh, pShader, texture, UVScale, UVOffset),
m_impactSound(nullptr),
m_numberActiveCollisionPoints(0)
{
    //CreatePhysicsBox(vec2(PLANE_SIZE.x, PLANE_SIZE.y), b2_staticBody);//Create the physics body for the plane
    m_Color = PLANE_DEFAULT_COLOR;
	m_impactSound = new Sound("Data/Audio/WaterSplash.wav", true);//Sound when something collides against it
}



PlaneRippleObject::~PlaneRippleObject()
{
	SAFE_DELETE(m_impactSound);
}

void PlaneRippleObject::Update(double aTimePassed)
{
    //Go through the active number of ripples (collisionpoints) active, and decrease their timer
    std::vector<int> indexToErase;
    if (m_collisionPointsTimer.empty() == false && m_collisionPointsPositionAndRadius.empty() == false)
    {      

        for (unsigned int i = 0; i < (unsigned int)m_numberActiveCollisionPoints; i++)
        {
            if (i >= m_collisionPointsTimer.size() && i >= m_collisionPointsPositionAndRadius.size())
            {
                break;
            }

                m_collisionPointsTimer.at(i) -=(float) aTimePassed;//Decrease timer

                float percentageLeft = (m_collisionPointsTimer.at(i) / PLANE_RIPPLE_DEFAULT_DURATION);  //Get the percentage of completion of the ripple and Convert from percentage completed to percentage left
                percentageLeft = 1.0f - percentageLeft;
                m_timersPercentageLeft.at(i) = percentageLeft;
                

                if (m_collisionPointsTimer.at(i) <= 0)//If the time is over
                {
                    indexToErase.push_back(i);               
                }
        }
    }
    
    for (unsigned int i = 0; i < indexToErase.size(); i++)
    {
        //Remove the elements in both vectors
        m_collisionPointsPositionAndRadius.erase(m_collisionPointsPositionAndRadius.begin() + indexToErase.at(i));
        m_collisionPointsTimer.erase(m_collisionPointsTimer.begin() + indexToErase.at(i));
        m_timersPercentageLeft.erase(m_timersPercentageLeft.begin() + indexToErase.at(i));

        //Decrease the counter of objects used
        m_numberActiveCollisionPoints--;

        //Since we are adding them with push back and is a timer, technically the first element could be deleted first  since its timer would be over
        //first. But for flexibility better to do this.
    }

    GameObject::Update(aTimePassed);

}

void PlaneRippleObject::Draw(int aRenderorder)
{
  
    Mesh::SetupCollisionPointsUniform(m_pShaderProgram, m_collisionPointsPositionAndRadius, m_timersPercentageLeft, m_numberActiveCollisionPoints);//Change the uniforms used for collision

	Mesh::SetupTexture2Uniform(m_pShaderProgram, g_pGame->GetTexture("Blocks"));

    GameObject::Draw(aRenderorder);//Draw the game object normally
}

void PlaneRippleObject::BeginCollision(b2Fixture * aFixtureCollided, b2Fixture * aFixtureCollidedAgainst, GameObject * aObjectCollidedagainst, b2Vec2 aCollisionNormal, b2Vec2 aContactPoint, bool aTouching)
{

    if (aTouching ==true)
	{
        //Calculate the momentum which will be used as effect radius
        b2Vec2 velocity= aObjectCollidedagainst->GetPhysicsBody()->GetLinearVelocity();
         float density=aFixtureCollided->GetDensity();
         float momentumRadius = abs(density*(velocity.x, velocity.y));//Momentum = mass v velocity, absolute value so that it is always positive
         
         momentumRadius /= DEFAULT_COLLISION_RADIUS_MODIFIER;//modifiy the momentum so that it "fits" in a game

        //Add collision points to vectorS and its timer
        m_collisionPointsPositionAndRadius.push_back(Vector4(aContactPoint.x, aContactPoint.y, aObjectCollidedagainst->GetPosition().z,momentumRadius));

        m_collisionPointsTimer.push_back(PLANE_RIPPLE_DEFAULT_DURATION);
        m_timersPercentageLeft.push_back(1.0f);//100 percent
        m_numberActiveCollisionPoints++;


		b2Body* collidedPhysicBody = aObjectCollidedagainst->GetPhysicsBody();
		if (collidedPhysicBody != nullptr)
		{
			if (collidedPhysicBody->GetLinearVelocity().y < 0.0f) //Only play sound if object is actually falling
			{
				if (m_impactSound != nullptr)
				{
					m_impactSound->PlayAudio();
				}
			}

		}
	}

}

void PlaneRippleObject::Reset()
{
    //Clear the vectors
    m_collisionPointsPositionAndRadius.erase(m_collisionPointsPositionAndRadius.begin(), m_collisionPointsPositionAndRadius.end());
    m_collisionPointsTimer.erase(m_collisionPointsTimer.begin(), m_collisionPointsTimer.end());
    m_timersPercentageLeft.erase(m_timersPercentageLeft.begin(), m_timersPercentageLeft.end());

    //Set the number of active ripples to 0
    m_numberActiveCollisionPoints = 0;

    GameObject::Reset();
}