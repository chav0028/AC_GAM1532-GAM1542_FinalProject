#include "CommonHeader.h"

using namespace std;

Cannon::Cannon(b2Body* aBodyBase, float aLaunchSpeed, Scene* apScene, std::string aName, Vector3 aPos, Vector3 aRot, Vector3 aScale, Mesh* apMesh, ShaderProgram* apShader, GLuint aTexture) :
	GameObject(apScene, aName, aPos, aRot, aScale, apMesh, apShader, aTexture),
	m_poolProjectiles(0),								
	m_launchSpeed(aLaunchSpeed)
{
	m_Color = Vector4(0.3f, 0.3f, 0.3f, 1.0f);
	m_speed = CANNON_ROTATION_SPEED;

	//Create physics body
	CreatePhysicsBox(vec2(GetXLength(), GetYLength()),b2_dynamicBody,vec2(0,0),false, CannonSceneFilteringGroups_CannonAndProjectile);
    m_pPhysicsBody->SetAngularDamping(1);

	if (aBodyBase != nullptr)
	{
		aBodyBase->SetTransform(b2Vec2(m_Position.x, m_Position.y), aRot.z);
		//Create the joints
		b2RevoluteJointDef tempJointDef;
		tempJointDef.Initialize(m_pPhysicsBody, aBodyBase, aBodyBase->GetTransform().p);

		if (m_pScene != nullptr)
		{
			m_pScene->GetBox2DWorld()->CreateJoint(&tempJointDef);
		}
	}

	//Initialize the player projectiles
	CannonProjectile* tempProjectile = nullptr;

	for (unsigned int i = 0; i < CANNON_DEFAULT_NUMBER_PROJECTILES; i++)
	{
		tempProjectile = new CannonProjectile(&m_poolProjectiles);//Add enemy to scene
		tempProjectile->Init(apScene, "CannonProjectile" + to_string(i), m_Position, m_Rotation, m_Scale/1.5f, g_pGame->GetMesh("SphereOBJ"), g_pGame->GetShader("TextureColor"), g_pGame->GetTexture("White"));
		tempProjectile->SetEnabled(false);
		tempProjectile->SetActivePhysicsBody(false);
		tempProjectile->SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		m_projectileSize = vec2(tempProjectile->GetXLength(), tempProjectile->GetYLength());
		m_poolProjectiles.AddExistingObject(tempProjectile);//Add the enemy to the pool
	}

	//Create mesh to draw the trajectory
	m_pMeshTrajectory = new Mesh();
	m_pMeshTrajectory->SetPrimitiveType(GL_LINE_STRIP);

	//Set the raycast to use for trajectory
	m_trajectoryRayCast.AddBodyTypeToIgnore(b2_dynamicBody);
	m_trajectoryRayCast.SetRayCastType(Box2DRayCastType_Closest);

}


Cannon::~Cannon()
{
	//Remove all the currently active objects
	vector<CannonProjectile*> currentlyActiveProjectiles = m_poolProjectiles.GetCurrentlyActiveObjects();//Get the projectiles that are currently active

	if (currentlyActiveProjectiles.empty() == false)
	{
		for (unsigned int i = 0; i < currentlyActiveProjectiles.size(); i++)
		{
			if (currentlyActiveProjectiles.at(i) != nullptr)
			{
				if (m_pScene != nullptr)
				{
					m_pScene->RemoveGameObject(currentlyActiveProjectiles.at(i)->GetName());//Remove the object from the scene

					//Pool deletes objects from active and inactive pool, no need to deactivate
				}
			}
		}
	}

	SAFE_DELETE(m_pMeshTrajectory);
}

void Cannon::Update(double TimePassed)
{
    if (m_pScene != nullptr)
    {
            CalculateTrajectory();
			RayCastTrajectory();
    }

	GameObject::Update(TimePassed);
}

void Cannon::Draw(int aRenderorder)
{

	DrawTrajectory(aRenderorder);

	GameObject::Draw(aRenderorder);

}

void Cannon::CalculateTrajectory()
{
	if (m_pPhysicsBody != nullptr)
	{
		//Erase the previous points
		m_trajectoryRayCast.ResetRayCast();
		m_numberActiveTrajectoryPoints = 0;
		m_trajectoryPoints.clear();
		

		//Get the direction vector
		vec2 directionVector;
		float currentAngle;
		vec3 startingPosition = CalculateLaunchPositionAndDirectionVector(&directionVector, &currentAngle);

		m_trajectoryPoints.push_back(VertexFormat(startingPosition, vec2(0, 0), vec3(0, 0, 0)));
		m_trajectoryPoints.back().color = MyColor(1, 0, 0, 1);

		//Scale the direction acccording to speed
		vec2 initialVelocity;
		initialVelocity = directionVector*m_launchSpeed;

		vec2 velocityPerFrame = initialVelocity / BOX2D_FRAMES_PER_SECOND;
		vec2 velocityChangePerFrame;

		if (m_pScene != nullptr)
		{
			velocityChangePerFrame.x = m_pScene->GetBox2DWorld()->GetWorldGravity().x / (BOX2D_FRAMES_PER_SECOND*BOX2D_FRAMES_PER_SECOND);//Calculate the chanve in velocity according to gravity
			velocityChangePerFrame.y = m_pScene->GetBox2DWorld()->GetWorldGravity().y / (BOX2D_FRAMES_PER_SECOND*BOX2D_FRAMES_PER_SECOND);//Calculate the chanve in velocity according to gravity
		}

		//Start index at 1 since first position in vector has already been set
		for (unsigned int i = 1; i < PROJECTILE_TRAJECTORY_SECONDS_TO_PLOT*BOX2D_FRAMES_PER_SECOND; i++)
		{
			m_trajectoryPoints.push_back(VertexFormat(vec3(0, 0, 0), vec2(0, 0), vec3(0, 0, 0)));//Push empty 
            m_trajectoryPoints.back().color = MyColor(1, 0, 0, 1);
			velocityPerFrame += velocityChangePerFrame;//Change the velocity
			m_trajectoryPoints.back().pos.x = m_trajectoryPoints.at(i - 1).pos.x + velocityPerFrame.x;
            m_trajectoryPoints.back().pos.y = m_trajectoryPoints.at(i - 1).pos.y + velocityPerFrame.y;
		}
	}
}

void Cannon::DrawTrajectory(int aRenderOrder)
{
	if (GetRenderOrder() == aRenderOrder)
	{
		if (m_pMesh != nullptr&& m_pScene != nullptr)
		{

			ShaderProgram* pShaderProgram = g_pGame->GetShader("TextureColor");

			m_pMeshTrajectory->SetupAttributes(pShaderProgram);

			//store the values in the matrix
			mat4 objectMatrix;
			objectMatrix.CreateSRT(m_Scale, vec3(0, 0, 0), vec3(0, 0, 0));//Set an world space offset of 0. Since the points are already in world space, the object will start drawing at 0,0,0 of worldspace.

			CameraObject* pCamera = dynamic_cast<CameraObject*>(m_pScene->GetGameObject(GetCameraDrawerName()));//Get the camera that will draw this gameobject

			if (pCamera != nullptr)
			{
				Vector3 cameraPosition = pCamera->GetPosition();
				mat4* pViewMatrix = pCamera->GetViewMatrix();//Get camera view matrix
				mat4* pProjectionMatrix;

				pProjectionMatrix = m_pScene->GetProjectionMatrix();//Get the projection matrix for normal game

				if (pProjectionMatrix != nullptr)
				{
					//Set all uniforms
					m_pMeshTrajectory->SetupUniforms(pShaderProgram, &objectMatrix, pViewMatrix, pProjectionMatrix, cameraPosition, m_TextureHandle, Vector4(1, 0, 1, 1), m_UVScale, m_UVOffset);//Set uniforms
					m_pMeshTrajectory->SetColorUniform(pShaderProgram, Vector4(1, 0, 0, 1));

					//Draw the mesh
					m_pMeshTrajectory->SetLineWidth(0.5f);
					m_pMeshTrajectory->Draw(pShaderProgram);

					//Ensure the depth test is turned on after the draw
					glEnable(GL_DEPTH_TEST);
				}
			}
		}
	}
}

void Cannon::RayCastTrajectory(unsigned int aStartingIndex)
{
	//Safety check
	if (aStartingIndex<1 || aStartingIndex>m_trajectoryPoints.size())
	{
		return;
	}

	Box2DWorld* box2DWorld = m_pScene->GetBox2DWorld();

	if (box2DWorld != nullptr)
	{

		b2Vec2 previousPointPosition;
		b2Vec2 nextPointPosition;


		//Start at 1 , since we will check between previous and next point
		for (unsigned int i = aStartingIndex; i < m_trajectoryPoints.size(); i++)
		{

			previousPointPosition = b2Vec2(m_trajectoryPoints.at(i - 1).pos.x, m_trajectoryPoints.at(i - 1).pos.y);//Coordinates point 1
			nextPointPosition = b2Vec2(m_trajectoryPoints.at(i).pos.x, m_trajectoryPoints.at(i).pos.y);//Coordinates point 2

			box2DWorld->RayCast(&m_trajectoryRayCast, previousPointPosition, nextPointPosition);//Do the raycast

			m_numberActiveTrajectoryPoints++;

			if (m_trajectoryRayCast.GetHit() == true)
			{
				break;
			}


		}

		//Set the mesh for the trajectory according to the points that will be used.
		m_pMeshTrajectory->Init(m_trajectoryPoints.data(), m_numberActiveTrajectoryPoints, nullptr, 0, GL_DYNAMIC_DRAW);
	}
}

bool Cannon::HandleInput(InputEvent& aInputevent, double aDelta)
{
	if (aInputevent.type == InputEventType_Key)//Check for keyboard events
	{
		if (aInputevent.state == InputEventState_Down || aInputevent.state == InputEventState_Held)
		{
			if (m_pPhysicsBody != nullptr)
			{

				//Rotation Z axis
				if (aInputevent.keycode == 'D')//Move forward
				{
					m_pPhysicsBody->ApplyTorque(-m_speed, true);
				}
				else if (aInputevent.keycode == 'A')//Move backward
				{
					m_pPhysicsBody->ApplyTorque(m_speed, true);
				}
			}
		}
		
		if (aInputevent.state == InputEventState_Up)
		{
			//Fire
			if (aInputevent.keycode == VK_SPACE)//Move up
			{
				FireProjectile();
			}
		}

	}

    return false;
}

Vector3 Cannon::CalculateLaunchPositionAndDirectionVector(vec2* aDirectionVector,float* aDirectionAngle)
{
	if (m_pPhysicsBody != nullptr)
	{
		*aDirectionAngle = m_pPhysicsBody->GetAngle();

		//Get the direction vector using the angle
		aDirectionVector->x = cos(*aDirectionAngle);
		aDirectionVector->y = sin(*aDirectionAngle);
		aDirectionVector->Normalize();

		//The projectile will be launched from front (right) middle of the player
		vec3 launchPosition;

		vec3 cannonOffset = vec3(0, 0, 0);
		cannonOffset.x = cos(*aDirectionAngle)*GetXLength() / 2;;
		cannonOffset.y = sin(*aDirectionAngle)*GetXLength() / 2;
		//cannonOffset.x = cannonOffset.x*aDirectionVector->x;

		launchPosition = m_Position + cannonOffset;

		return launchPosition;
	}

	aDirectionAngle = 0;
	aDirectionVector = 0;
	return 0;
}

void  Cannon::FireProjectile()
{
	CannonProjectile* projectile = m_poolProjectiles.GetActivateObject();///Get an object from the pool

	if (projectile != nullptr)
	{
		string projectileID = projectile->GetName();
		m_pScene->AddGameObject(projectileID, projectile);//Add projectile to scene

		vec2 directionVector;
		float launchAngle;
		vec3 launchPosition = CalculateLaunchPositionAndDirectionVector(&directionVector,&launchAngle);

		projectile->LaunchProjectile(launchPosition, launchAngle, vec2(directionVector.x*m_launchSpeed, directionVector.y*m_launchSpeed));
	}

}