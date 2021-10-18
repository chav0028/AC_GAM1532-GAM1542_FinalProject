#include "CommonHeader.h"
#include "PrismaticEnemy.h"


PrismaticEnemy::PrismaticEnemy(int aCollisionDamage, int aHealth, double aShieldTimer, double aUnshieldTimer, int aShieldHealth, Scene* apScene, std::string aName, Vector3 aPos, Vector3 aRot, Vector3 aScale, Mesh* apMesh,
    ShaderProgram* apShader, GLuint aTexture, Mesh* apShieldMesh, ShaderProgram* apShieldShader, GLuint aShieldTexture,
    vec2 aUVScale, vec2 aUVOffset, const char* aCameraName, unsigned int aDrawRenderOrder) :
    LivingObject(aCollisionDamage, aHealth, apScene, aName, aPos, aRot, aScale, apMesh, apShader, aTexture, aUVScale, aUVOffset, false, aCameraName, aDrawRenderOrder),
    m_pShield(nullptr),
    m_pJoint(nullptr),
    m_defaultUnshieldTimer(aUnshieldTimer),
    m_unshieldTimer(aUnshieldTimer),
    m_shieldSpeed(PRISMATIC_ENEMY_DEFAULT_SHIELD_SPEED),
    m_shieldTimer(aShieldTimer),
    m_defaultShieldTimer(aShieldTimer),
    m_enemyVelocity(PRISMATIC_ENEMY_DEFAULT_VELOCITY),
    m_shielded(true)
{
    //Set it to collide with enviorment,player and its projectiles
    signed short maskBit = CollisionFilteringCategory_Enviroment |
        CollisionFilteringCategory_Player |
        CollisionFilteringCategory_PlayerProjectile;


    //Create physics body
    CreatePhysicsBox(vec2(GetXLength(), GetYLength()), b2_kinematicBody, vec2(0, 0), false, CollisionFilteringGroup_PowerUpsAndEnemies, CollisionFilteringCategory_Enemy, maskBit);
    m_pPhysicsBody->SetGravityScale(0);
    m_pPhysicsBody->SetFixedRotation(true);

    SetDefaultConditions();
    CreateShield(aShieldHealth, apShieldMesh, apShieldShader, aShieldTexture);

    Reset();
}

PrismaticEnemy::~PrismaticEnemy()
{
    SAFE_DELETE(m_pShield);

}

void PrismaticEnemy::SetDefaultConditions(Vector2 enemyVelocity, float shieldSpeed, float shieldMaxLimit, float shieldMinLimit , float shieldMaxSpeed)
{
    m_defaultEnemyVelocity = enemyVelocity;
    m_defaultShieldSpeed = shieldSpeed;
    m_defaultShieldMaxLimit = shieldMaxLimit;
    m_defaultShieldMinLimit = shieldMinLimit;
    m_defaultShieldMaxSpeed = shieldMaxSpeed;
}

void PrismaticEnemy::Update(double aDelta)
{

    LivingObject::Update(aDelta);

    //Update the shield
    if (m_pShield != nullptr)
    {
        if (m_pShield->GetEnabled() == true)
        {
            m_pShield->Update(aDelta);
        }
    }


    if (m_pJoint != nullptr)
    {
        float jointTranslation = m_pJoint->GetJointTranslation();
        float upperLimit = m_pJoint->GetUpperLimit();
        float lowerLimit = m_pJoint->GetLowerLimit();

        //If the joint is at either limit
        if (jointTranslation >= upperLimit || jointTranslation <= lowerLimit)
        {
            m_pJoint->EnableMotor(false);//Disable motor
        }

        //Error check the status in case the shield went out of limits
        if (jointTranslation >= upperLimit)
        {
            m_shielded = false;
        }
        else if (jointTranslation <= lowerLimit)
        {
            m_shielded = true;
        }

        //If the enemy is shielded
        if (m_shielded == true)
        {
            //Wait for joint to be at bottom to start timer
            if (jointTranslation <= lowerLimit)
            {
                m_unshieldTimer -= aDelta;//Decrease timer
            }

            if (m_unshieldTimer <= 0)
            {
                m_pJoint->SetMotorSpeed(m_shieldSpeed);//Set motor speed
                m_pJoint->EnableMotor(true);//Enable motor
                m_unshieldTimer = m_defaultUnshieldTimer;//Reset timer
                m_shielded = false;//State the enemy is not shielded
            }
        }
        else//if the enemy is not shielded
        {
            //Wait for joint to be at top to start timer
            if (jointTranslation >= upperLimit)
            {
                m_shieldTimer -= aDelta;//Decrease timer
            }

            if (m_shieldTimer <= 0)
            {
                m_pJoint->SetMotorSpeed(-m_shieldSpeed);//Set motor speed
                m_pJoint->EnableMotor(true);//Enable motor
                m_shieldTimer = m_defaultShieldTimer;//Reset timer
                m_shielded = true;//State the enemy is shieded
            }
        }
    }
}

void PrismaticEnemy::Draw(int aRenderorder)
{
    LivingObject::Draw(GetRenderOrder());//Draw the main object

    //Draw the shield
    if (m_pShield != nullptr)
    {
        if (m_pShield->GetEnabled() == true)
        {
            m_pShield->Draw(m_pShield->GetRenderOrder());
        }
    }
}

void PrismaticEnemy::CreateShield(int aShieldHealth, Mesh* apShieldMesh, ShaderProgram* apshieldShader, GLuint aShieldTexture)
{
    if (apShieldMesh != nullptr && apshieldShader != nullptr&& m_pScene != nullptr)
    {
        //Calculate the position the shield will have. It will be set in the middle left position of the object.
        Vector3 middleLeftPosition = m_Position;
        middleLeftPosition.x = -abs(GetXLength() / 2 + (apShieldMesh->GetMeshXLength()*m_Scale.x));//Get the half width of the game object, and the shield mesh

        //Create the shield object
        m_pShield = new LivingObject(aShieldHealth, m_pScene, m_Name + "Shield", middleLeftPosition, m_Rotation, m_Scale, apShieldMesh, apshieldShader, aShieldTexture,
            m_UVScale, m_UVOffset, GetCameraDrawerName(), GetRenderOrder());
        m_pShield->SetCollisionDamage(m_collisionDamage);

        if (m_pPhysicsBody != nullptr)
        {
            //Create the shield body
            b2Fixture* objectFixtureList = m_pPhysicsBody->GetFixtureList();
            b2Filter objectFilderData = objectFixtureList->GetFilterData();
            m_pShield->CreatePhysicsBox(vec2(GetXLength(), GetYLength()), b2_dynamicBody, vec2(0, 0), objectFixtureList->IsSensor(), objectFilderData.groupIndex, objectFilderData.categoryBits, objectFilderData.maskBits);

            Box2DWorld* pSceneWorld = m_pScene->GetBox2DWorld();

            //Create the joint
            if (pSceneWorld != nullptr)
            {
                //Create the joint definition
                b2PrismaticJointDef jointDef;
                jointDef.bodyA = m_pPhysicsBody;
                jointDef.bodyB = m_pShield->GetPhysicsBody();
                jointDef.localAnchorA = b2Vec2(-GetXLength() / 2, 0);
                jointDef.localAnchorB = b2Vec2(m_pShield->GetXLength() / 2, 0);

                //Limit
                jointDef.localAxisA = b2Vec2(0, 1);
                jointDef.enableLimit = true;
                jointDef.upperTranslation = m_defaultShieldMaxLimit;
                jointDef.lowerTranslation = m_defaultShieldMinLimit;

                //Motor
                jointDef.enableMotor = false;
                jointDef.motorSpeed = m_shieldSpeed;
                jointDef.maxMotorForce = m_defaultShieldMaxSpeed;

                //Create the actual joint in the world
                m_pJoint = (b2PrismaticJoint*)pSceneWorld->CreateJoint(&jointDef);
            }

        }
    }
}

void PrismaticEnemy::OnDeath()
{
    LivingObject::OnDeath();//Kill this object

    //Kill the shield
    if (m_pShield != nullptr)
    {
        m_pShield->OnDeath();
    }
}

void PrismaticEnemy::Reset()
{
    //Reset the center object
    LivingObject::Reset();

    //Reset the shield
    if (m_pShield != nullptr)
    {
        m_pShield->Reset();
    }

    //Reset the shielded/unshielded variables
    m_shielded = true;
    m_shieldTimer = m_defaultShieldTimer;
    m_unshieldTimer = m_defaultUnshieldTimer;
    m_shieldSpeed = m_shieldSpeed;

    if (m_pJoint != nullptr)
    {
        m_pJoint->SetMotorSpeed(m_shieldSpeed);
        m_pJoint->SetMaxMotorForce(m_defaultShieldMaxSpeed);
        m_pJoint->SetLimits(m_defaultShieldMinLimit, m_defaultShieldMaxLimit);
    }

    if (m_pPhysicsBody != nullptr)
    {
        m_enemyVelocity = m_defaultEnemyVelocity;
        m_pPhysicsBody->SetLinearVelocity(b2Vec2(m_enemyVelocity.x, m_enemyVelocity.y));
    }

}

vec2 PrismaticEnemy::GetMinFixtureXY(int aFixtureIndex)
{

    vec2 objectMinXY = GameObject::GetMinFixtureXY(aFixtureIndex);//Get the object min

    if (m_pShield != nullptr)
    {
        vec2 shieldMinXY = m_pShield->GetMinFixtureXY(aFixtureIndex);//Get the shield min

        //Get the min values between both of them
        //X axis
        if (shieldMinXY.x < objectMinXY.x)
        {
            objectMinXY.x = shieldMinXY.x;
        }

        //Y axis
        if (shieldMinXY.y < objectMinXY.y)
        {
            objectMinXY.y = shieldMinXY.y;
        }
    }

    return objectMinXY;
}

vec2 PrismaticEnemy::GetMaxFixtureXY(int aFixtureIndex)
{
    vec2 objectMaxXY = GameObject::GetMaxFixtureXY(aFixtureIndex);//Get the object max

    if (m_pShield != nullptr)
    {
        vec2 shieldMaxXY = m_pShield->GetMaxFixtureXY(aFixtureIndex);//Get the shield max

        //Get the max values between both of them
        //X axis
        if (shieldMaxXY.x > objectMaxXY.x)
        {
            objectMaxXY.x = shieldMaxXY.x;
        }

        //Y axis
        if (shieldMaxXY.y > objectMaxXY.y)
        {
            objectMaxXY.y = objectMaxXY.y;
        }
    }

    return objectMaxXY;
}

void PrismaticEnemy::BounceXObjectBody(float restitutionCollidingObject, float frictionCollidingObject)
{
    GameObject::BounceXObjectBody(restitutionCollidingObject, frictionCollidingObject);

    if (m_pShield != nullptr)
    {
        m_pShield->BounceXObjectBody(restitutionCollidingObject, frictionCollidingObject);

        //Reverse its direction
        m_pShield->SetRotationY(m_pShield->GetRotation().x + 180);

        //Calculate the position the shield will have. It will be set in the middle right position of the object.
        Vector3 middleRightPosition = m_pShield->GetPosition();
        middleRightPosition.x = m_Position.x + abs(GetXLength() + m_pShield->GetXLength()) * 5;//Get the half width of the game object, and the shield

        m_pShield->SetPhysicsAndGameObjectPosition(middleRightPosition);

        //Rotate shield and object in Z axis to move the joint
        if (m_pJoint != nullptr)
        {
            SetPhysicsBodyTransform(vec2(m_Position.x, m_Position.y), m_Rotation.z + 180);
        }

        if (m_pPhysicsBody != nullptr)
        {
            //Reverse velocity
            m_enemyVelocity = m_enemyVelocity *= -1;
            m_pPhysicsBody->SetLinearVelocity(b2Vec2(m_enemyVelocity.x, m_enemyVelocity.y));
        }
    }
}

void PrismaticEnemy::BounceYObjectBody(float restitutionCollidingObject, float frictionCollidingObject)
{
    GameObject::BounceYObjectBody(restitutionCollidingObject, frictionCollidingObject);

    if (m_pShield != nullptr)
    {
        m_pShield->BounceYObjectBody(restitutionCollidingObject, frictionCollidingObject);
    }
}

void PrismaticEnemy::SetPhysicsAndGameObjectPosition(vec3 aPosition)
{
    Vector3 differenceInPosition;
    Vector3 objectOriginalPosition = m_Position;

    GameObject::SetPhysicsAndGameObjectPosition(aPosition);

    if (m_pShield != nullptr)
    {
        //Calculate the distance between the objects psition and the arm positon
        differenceInPosition = m_pShield->GetPosition();
        differenceInPosition -= objectOriginalPosition;

        //Set the new position according to the new object position, and the distance the objects had before, EXCEPT FOR Z AXIS
        m_pShield->SetPhysicsAndGameObjectPosition(vec3(aPosition.x + differenceInPosition.x, aPosition.y + differenceInPosition.y, aPosition.z));
    }
}

//SUPPOSING THE SHIELD ONLY AFFECTS THE X DIRECTLY, ADDING IT, THE OTHER VALUESI T WILL CHOOSE THE MAX
//ONE
Vector3 PrismaticEnemy::GetSizes()
{
    vec3 objectSize = GameObject::GetSizes();

    if (m_pShield != nullptr)
    {
        vec3 shieldSize = m_pShield->GetSizes();

        objectSize.x += shieldSize.x;

        objectSize.y = max(shieldSize.y, objectSize.y);
        objectSize.y = max(shieldSize.z, objectSize.z);
    }

    return objectSize;
}

void PrismaticEnemy::SetShieldCollisionFiltering(int fixtureIndex, signed short fixtureGroup, unsigned short fixtureCategory, unsigned short fixtureMasks)
{
    if (m_pShield != nullptr)
    {
        m_pShield->SetFixtureCollisionsFiltering(fixtureIndex, fixtureGroup, fixtureCategory, fixtureMasks);
    }
}