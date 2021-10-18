#include "CommonHeader.h"

using namespace std;

RevolutingEnemy::RevolutingEnemy(int aCollisionDamage, int aHealth, int aArmHealth, Scene* apScene, std::string aName, Vector3 aPos, Vector3 aRot, Vector3 aScale, Mesh* apMesh, ShaderProgram* apShader, GLuint aTexture,
    Mesh* aArmMesh, ShaderProgram* apArmShader, GLuint aArmTexture, vec2 aUVScale, vec2 aUVOffset, const char* aCameraName, unsigned int aDrawRenderOrder) :
    LivingObject(aCollisionDamage, aHealth, apScene, aName, aPos, aRot, aScale, apMesh, apShader, aTexture, aUVScale, aUVOffset, false, aCameraName, aDrawRenderOrder)
{
    CreateBody();
    SetDefaultConditions();

    CreateArms(REVOLUTING_ENEMY_DEFAULT_NUMBER_ARMS, aArmHealth, aArmMesh, apArmShader, aArmTexture);

    Reset();
}

RevolutingEnemy::RevolutingEnemy(unsigned int aNumArms,int aCollisionDamage, int aHealth, int aArmHealth, Scene* apScene, std::string aName, Vector3 aPos, Vector3 aRot, Vector3 aScale, Mesh* apMesh, ShaderProgram* apShader, GLuint aTexture,
    Mesh* aArmMesh, ShaderProgram* apArmShader, GLuint aArmTexture, vec2 aUVScale, vec2 aUVOffset, const char* aCameraName, unsigned int aDrawRenderOrder) :
    LivingObject(aCollisionDamage, aHealth, apScene, aName, aPos, aRot, aScale, apMesh, apShader, aTexture, aUVScale, aUVOffset, false, aCameraName, aDrawRenderOrder)
{
    CreateBody();
    SetDefaultConditions();

    CreateArms(aNumArms, aArmHealth, aArmMesh, apArmShader, aArmTexture);

    Reset();
}

void RevolutingEnemy::CreateBody()
{
    //Set it to collide with enviorment,player and its projectiles
    signed short maskBit = CollisionFilteringCategory_Enviroment |
        CollisionFilteringCategory_Player |
        CollisionFilteringCategory_PlayerProjectile;

    //Create physics body
    CreatePhysicsCircle(GetXYRadius(), b2_kinematicBody, vec2(0, 0), false, CollisionFilteringGroup_PowerUpsAndEnemies, CollisionFilteringCategory_Enemy, maskBit);
    SetPhysicsBodyDensity(2);
    m_pPhysicsBody->SetGravityScale(0);
}

void RevolutingEnemy::SetDefaultConditions(float aArmsSpeedIncrease,float aDefaultArmMaxSpeed, float aArmDefaultSpeed)
{
    m_defaultArmsSpeed = aArmDefaultSpeed;
    m_defaultArmsMaxSpeed = aDefaultArmMaxSpeed;
    m_armsSpeedIncrease = aArmsSpeedIncrease;
}

RevolutingEnemy::~RevolutingEnemy()
{
    //Box2D destroy bodies and joints


    while (m_arms.empty() == false)
    {
        SAFE_DELETE(m_arms.back());
        m_arms.pop_back();        
    }
}

void RevolutingEnemy::Update(double aDelta)
{

    SetEnabled(m_alive);
    LivingObject::Update(aDelta);

    unsigned int numberDisableddArms = 0;
    vector<int> enabledArmIndex;

    //Update all the arms
    for (unsigned int i = 0; i < m_arms.size(); i++)
    {
        if (m_arms.at(i) != nullptr)
        {
            if (m_arms.at(i)->GetEnabled() == true)
            {
                m_arms.at(i)->SetEnabled(m_arms.at(i)->GetAlive());
                m_arms.at(i)->Update(aDelta);//Update each individual arm

                //Check if after the update the object is still enabled
                if (m_arms.at(i)->GetEnabled() == true)
                {
                    enabledArmIndex.push_back(i);//Save the index of the enabled arm
                }
                else
                {
                    numberDisableddArms++;//Increase the count of disabled arms.
                }
            }
            else
            {
                numberDisableddArms++;//Increase the count of disabled arms.
            }
        }
    }

    //Increase the speed of the joints according to the number of arms that are disabled
    for (unsigned int i = 0; i < enabledArmIndex.size(); i++)
    {
        unsigned int enableArmCurrentIndex = enabledArmIndex.at(i);//Get the index of a enabled arm

        if (enableArmCurrentIndex < m_armsJoints.size())
        {
            b2RevoluteJoint* currentArmJoint = m_armsJoints.at(enableArmCurrentIndex);

            if (currentArmJoint != nullptr)
            {
                currentArmJoint->SetMaxMotorTorque(m_defaultArmsMaxSpeed + (m_armsSpeedIncrease*numberDisableddArms));//increase the max speed

                float newSpeed = currentArmJoint->GetMotorSpeed() + (m_armsSpeedIncrease*numberDisableddArms);//Calculate the speed of the motor

                if (newSpeed <= currentArmJoint->GetMaxMotorTorque())//If the speed doesn't increase the max
                {
                    currentArmJoint->SetMotorSpeed(newSpeed);//increase the motor speed
                }
            }
        }
    }

}

void RevolutingEnemy::Draw(int aRenderorder)
{
    LivingObject::Draw(GetRenderOrder());//Draw the main object

    //Draw all the arms
    for (unsigned int i = 0; i < m_arms.size(); i++)
    {
        if (m_arms.at(i) != nullptr)
        {
            if (m_arms.at(i)->GetEnabled() == true)
            {
                m_arms.at(i)->Draw(m_arms.at(i)->GetRenderOrder());//Draw each individual arm
            }
        }
    }

}

void RevolutingEnemy::CreateArms(int aNumberArms, int aArmHealth, Mesh* aArmMesh, ShaderProgram* apArmShader, GLuint aArmTexture)
{
    if (m_pPhysicsBody != nullptr && m_pScene != nullptr && aArmMesh != nullptr && apArmShader != nullptr)
    {
        SetPhysicsBodyTransform(vec2(m_Position.x, m_Position.y), m_Rotation.z);

        //Create variabels to use in circle equation
        float tempX = 0;
        float tempY = 0;

        float PiDouble = PI * 2;//Circle = 2 PI radians
        float angleSlice = PiDouble / (float)aNumberArms;//Calculate the angle of each arm
        float currentAngle = 0;
        LivingObject* tempArm = nullptr;
        b2RevoluteJointDef tempJointDef;
        b2Fixture* centerObjectFixtureList = nullptr;//Used to obtain certain properties of the object, when setting the new b2body
        Box2DWorld* psceneWorld = m_pScene->GetBox2DWorld();
		b2Filter objectFilderData = m_pPhysicsBody->GetFixtureList()->GetFilterData();

        //Get the radius of the circle using the the scaled size of the arm
        //Note that in this method the arm will be created INSIDE the center object. So that we can simply make them spin
        float radius = (aArmMesh->GetMeshXLength()*m_Scale.y / 2); //Calculate the scale half width of the arm object
        //Use the x length since the angle 0, starting angle, is straight to the right

        for (int i = 0; i < aNumberArms; i++)
        {
            //Calculate the arm position
            currentAngle = (float)i*angleSlice;
            tempX = m_Position.x + (radius * cosf(-currentAngle));//Calculates the x position by using the radius and cos, + center of the circle
            tempY = m_Position.y + (radius * sinf(-currentAngle)); // Calculates the y position by using the radius and cos, + enter of the circle

            //Create the game object, using the center object properties, and its own different properties
            tempArm = new LivingObject(m_collisionDamage, aArmHealth, m_pScene, m_Name + "Arm" + to_string(i), Vector3(tempX, tempY, m_Position.y), Vector3(m_Rotation.x, m_Rotation.y, currentAngle), m_Scale,
                aArmMesh, apArmShader, aArmTexture, m_UVScale, m_UVOffset, false, GetCameraDrawerName(), GetRenderOrder());

            //Create the physics body
            centerObjectFixtureList = m_pPhysicsBody->GetFixtureList();
            tempArm->CreatePhysicsBox(vec2(tempArm->GetXLength(), tempArm->GetYLength()), b2_dynamicBody, vec2(0, 0), centerObjectFixtureList->IsSensor(), objectFilderData.groupIndex, objectFilderData.categoryBits, objectFilderData.maskBits);
            tempArm->SetPhysicsBodyTransform(vec2(tempArm->GetPosition().x, tempArm->GetPosition().y), tempArm->GetRotation().z);
            tempArm->GetPhysicsBody()->SetGravityScale(0);

            // Create the actual joint in the world
            if (psceneWorld != nullptr)
            {
                // Initialize the joint definition
                b2Transform centerObjectBodyTransform = m_pPhysicsBody->GetTransform();
                //tempJointDef.Initialize(m_pPhysicsBody, tempArm->GetPhysicsBody(), centerObjectBodyTransform.p);///Makes the center object orbit around arm

                //Join all the arms around the center object
                tempJointDef.bodyB = m_pPhysicsBody;
                tempJointDef.bodyA = tempArm->GetPhysicsBody();
                tempJointDef.localAnchorB = b2Vec2(tempArm->GetMinFixtureXY().x, tempArm->GetMinFixtureXY().y);
                tempJointDef.localAnchorA = b2Vec2(tempArm->GetMaxFixtureXY().x, tempArm->GetMaxFixtureXY().y);

                tempJointDef.motorSpeed = m_defaultArmsSpeed;
                tempJointDef.maxMotorTorque = m_defaultArmsMaxSpeed;
                tempJointDef.enableMotor = true;

                m_armsJoints.push_back((b2RevoluteJoint*)psceneWorld->CreateJoint(&tempJointDef));//Add the joint to the list of joints
            }

            //Add the arm to the list
            m_arms.push_back(tempArm);
        }
    }
}

void RevolutingEnemy::SetArmColors(Vector4 aColor)
{
    //Set the color variable in all the arms
    for (unsigned int i = 0; i < m_arms.size(); i++)
    {
        if (m_arms.at(i) != nullptr)
        {
            m_arms.at(i)->SetColor(aColor);
        }
    }

}

void RevolutingEnemy::OnDeath()
{
    LivingObject::OnDeath();//Kill this object

    //Kill all the joints
    for (unsigned int i = 0; i < m_arms.size(); i++)
    {
        if (m_arms.at(i) != nullptr)
        {
            m_arms.at(i)->OnDeath();
        }
    }
}

void RevolutingEnemy::Reset()
{
    //Reset the center object
    LivingObject::Reset();

    //Reset each arm
    for (unsigned int i = 0; i < m_arms.size(); i++)
    {
        if (m_arms.at(i) != nullptr)
        {
            m_arms.at(i)->Reset();
        }
    }

    //Reset the speed of each joint
    for (unsigned int i = 0; i < m_armsJoints.size(); i++)
    {
        if (m_armsJoints.at(i) != nullptr)
        {
            m_armsJoints.at(i)->SetMotorSpeed(m_defaultArmsSpeed);
            m_armsJoints.at(i)->SetMaxMotorTorque(m_defaultArmsMaxSpeed);
        }
    }

}

vec2 RevolutingEnemy::GetMinFixtureXY(int aFixtureIndex)
{

	vec2 objectMinXY = GameObject::GetMinFixtureXY(aFixtureIndex);//Get the object min
	vec2 tempArmMinXY;

	for (unsigned int i = 0; i < m_arms.size();i++)
	{
		if (m_arms.at(i) != nullptr)
		{
			tempArmMinXY = m_arms.at(i)->GetMinFixtureXY(aFixtureIndex);//Get the arm min

			//Get the min values between both of them
			//X axis
			if (tempArmMinXY.x < objectMinXY.x)
			{
				objectMinXY.x = tempArmMinXY.x;
			}

			//Y axis
			if (tempArmMinXY.y < objectMinXY.y)
			{
				objectMinXY.y = tempArmMinXY.y;
			}
		}

	}

	return objectMinXY;
}

vec2 RevolutingEnemy::GetMaxFixtureXY(int aFixtureIndex)
{
	vec2 objectMaxXY = GameObject::GetMaxFixtureXY(aFixtureIndex);//Get the object max
	vec2 tempArmMaxXY;

	for (unsigned int i = 0; i < m_arms.size(); i++)
	{
		if (m_arms.at(i) != nullptr)
		{
			tempArmMaxXY = m_arms.at(i)->GetMaxFixtureXY(aFixtureIndex);//Get the arm max

			//Get the max values between both of them
			//X axis
			if (tempArmMaxXY.x > objectMaxXY.x)
			{
				objectMaxXY.x = tempArmMaxXY.x;
			}

			//Y axis
			if (tempArmMaxXY.y > objectMaxXY.y)
			{
				objectMaxXY.y = objectMaxXY.y;
			}
		}
	}

	return objectMaxXY;
}

void RevolutingEnemy::BounceXObjectBody(float restitutionCollidingObject, float frictionCollidingObject)
{
	GameObject::BounceXObjectBody(restitutionCollidingObject, frictionCollidingObject);

	for (unsigned int i = 0; i < m_arms.size(); i++)
	{
		if (m_arms.at(i) != nullptr)
		{
			m_arms.at(i)->BounceXObjectBody(restitutionCollidingObject, frictionCollidingObject);
		}
	}

}

void RevolutingEnemy::BounceYObjectBody(float restitutionCollidingObject, float frictionCollidingObject)
{
	GameObject::BounceYObjectBody(restitutionCollidingObject, frictionCollidingObject);

	for (unsigned int i = 0; i < m_arms.size(); i++)
	{
		if (m_arms.at(i) != nullptr)
		{
			m_arms.at(i)->BounceYObjectBody(restitutionCollidingObject, frictionCollidingObject);
		}
	}
}

//Suppose for the positions in the X and Y axis, because the arms will be rotating. Don't affect the Z axis.
void RevolutingEnemy::SetPhysicsAndGameObjectPosition(vec3 aPosition)
{
	vec3 objectOriginalPosition = m_Position;
	vec3 differenceInPosition;
	GameObject::SetPhysicsAndGameObjectPosition(aPosition);

	for (unsigned int i = 0; i < m_arms.size(); i++)
	{
		if (m_arms.at(i) != nullptr)
		{
			//Calculate the distance between the objects psition and the arm positon
			differenceInPosition = m_arms.at(i)->GetPosition();
			differenceInPosition -= objectOriginalPosition;

			//Set the new position according to the new object position, and the distance the objects had before. EXCEPT FOR Z AXIS
			m_arms.at(i)->SetPhysicsAndGameObjectPosition( vec3(aPosition.x + differenceInPosition.x, aPosition.y+ differenceInPosition.y,aPosition.z));
		}
	}
}

//Suppose for the max value in X  and Y axis, because the arms will be rotating. Don't affect the Z axis
Vector3 RevolutingEnemy::GetSizes()
{
	vec3 objectSize = GameObject::GetSizes(); //get object size

	if (m_arms.empty() == false)
	{
		if (m_arms.at(0) != nullptr)
		{
			vec3 armSizes = m_arms.at(0)->GetSizes();

			//Because rotation in Z angle (affecting X and Y axis), add to both of those axis
			objectSize.x += armSizes.x;
			objectSize.y += armSizes.y;

			//Use the biggest Z value
			objectSize.y = max(armSizes.z, objectSize.z);
		}
	}

	return objectSize;
}

void RevolutingEnemy::SetArmsCollisionFiltering(int fixtureIndex, signed short fixtureGroup, unsigned short fixtureCategory, unsigned short fixtureMasks)
{
    for (unsigned int i = 0; i < m_arms.size(); i++)
    {
        if (m_arms.at(i) != nullptr)
        {
            m_arms.at(i)->SetFixtureCollisionsFiltering(fixtureIndex, fixtureGroup, fixtureCategory, fixtureMasks);
        }
    }

}