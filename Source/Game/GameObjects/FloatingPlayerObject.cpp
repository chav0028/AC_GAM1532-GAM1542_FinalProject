#include "CommonHeader.h"

FloatingPlayerObject::FloatingPlayerObject()
{
}

FloatingPlayerObject::FloatingPlayerObject(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture)
{
    Init( pScene, name, pos, rot, scale, pMesh, pShader, texture );
    m_numObjectsAttached = 0;
    m_currentOrbitingAngle = 0;
	m_speed = FLOATING_PLAYER_DEFAULT_MOVEMENT_SPEED;
}

FloatingPlayerObject::~FloatingPlayerObject()
{

}

void FloatingPlayerObject::CheckCloseObjects()
{
	// check if we're colliding with any mines.
	for (int i = 0; i<WATER_SCENE_NUMBER_MINE_OBJECT; i++)
	{
		std::string minename = "Mine" + std::to_string(i);
		FloatingMineObject* pMine = dynamic_cast<FloatingMineObject*>(m_pScene->GetGameObject(minename.c_str()));
		assert(pMine != 0);

		if (pMine != nullptr)
		{
			//Set touching to false at the start
			bool touching = false;

			//TODO: check if we're touching mine.      
			if (Calculate3DDistanceBetween(pMine->GetPosition()) <= FLOATING_PLAYER_COLLECTION_DISTANCE)
			{
				touching = true;

				if (touching == true&& pMine->m_AttachedToPlayer == false)//If the player is touching the mine and the object hasn't been attached before
				{
						m_numObjectsAttached++;//Increase the number of objects the player has attached
						pMine->m_AttachedToPlayer = true;//Attached  the mine to palyer
				}
			}
		}

	}

}

void FloatingPlayerObject::OrbitAttachedObjects(double aTimePassed)
{
	float PiDouble = PI * 2;//Circle = 2 PI radians
	float objectDistance = PiDouble / m_numObjectsAttached;//Calculate the angle of each slice
	float orbitSpeed = FLOATING_PLAYER_ORBITING_SPEED*(float)aTimePassed; //Increase the orbit speed according to the angle  position
	m_currentOrbitingAngle += +orbitSpeed;//increase the current angle

	float tempX;
	float tempZ;
	vec3 objectPosition = 0;
	unsigned int attachedObjectIndex = 0;//Tracker of number of objects attached
	float objectCurrentOrbitAngle = 0;//Variable to sotre the current orbitting angle+ the offset the object may have (so that they are place evenly in a circle).

	// make all attached mines circle around us.
	for (int i = 0; i<WATER_SCENE_NUMBER_MINE_OBJECT; i++)
	{
		std::string minename = "Mine" + std::to_string(i);
		FloatingMineObject* pMine = dynamic_cast<FloatingMineObject*>(m_pScene->GetGameObject(minename.c_str()));
		assert(pMine != 0);

		if (pMine != 0 && pMine->m_AttachedToPlayer == true)
		{
			//TODO: position the mine so it's circling us.  

			//"Reset" the angle
			while (m_currentOrbitingAngle > PiDouble)
			{
				m_currentOrbitingAngle -= PiDouble;
			}

			objectCurrentOrbitAngle = m_currentOrbitingAngle + (float)attachedObjectIndex*objectDistance;//Get the object current orbit angle, accounting general rotation angle plus  offset the object may have (so that they are place evenly in a circle). 

																										  //Orbit movement
			tempX = m_Position.x + (FLOATING_PLAYER_ORBITING_RADIUS * cos(objectCurrentOrbitAngle));//Calculates the x position by using the radius and cos, + center of the circle
			tempZ = m_Position.z + (FLOATING_PLAYER_ORBITING_RADIUS * sin(objectCurrentOrbitAngle)); // Calculates the y position by using the radius and cos, + enter of the circle

			objectPosition = pMine->GetPosition();//Get the current object position, used for y value sin wave

			pMine->SetPosition(Vector3(tempX, objectPosition.y, tempZ));//Set the position of the object

			attachedObjectIndex++;//Increase tracker of number objects attached
		}
	}
}

void FloatingPlayerObject::Update(double aTimePassed)
{
    FloatingObject::Update( aTimePassed );

	CheckCloseObjects();
	OrbitAttachedObjects(aTimePassed);

    // move faster depending how many mines we have.
	m_speed =FLOATING_PLAYER_DEFAULT_MOVEMENT_SPEED + (m_numObjectsAttached*FLOATING_PLAYER_OBJECT_ATTACHED_SPEED_BONUS_SCALE);

}

bool FloatingPlayerObject::HandleInput(InputEvent & aInputevent, double aDelta)
{
	if (aInputevent.type == InputEventType_Key)
	{
		if (aInputevent.state == InputEventState_Down || aInputevent.state == InputEventState_Held)
		{
			Vector3 dir(0, 0, 0);

			if (aInputevent.keycode == 'W')
			{
				dir.z += 1;
			}

			if (aInputevent.keycode == 'S')
			{
				dir.z -= 1;
			}	 

			if (aInputevent.keycode == 'A')
			{
				dir.x -= 1;
			}

			if (aInputevent.keycode == 'D')
			{
				dir.x += 1;
			}

			//TODO: make sure direction vector isn't longer than 1.
			dir.Normalize();//Normalize direction

			if (dir.Length() > 1)//Error message is vector hasn't been normalized
				OutputMessage("Player moving too fast (%f, %f, %f)\n", dir.x, dir.y, dir.z);

			m_Position += dir * m_speed * (float)aDelta;
		}

	}

    return false;
}
