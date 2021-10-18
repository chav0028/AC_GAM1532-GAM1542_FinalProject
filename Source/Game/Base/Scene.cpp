#include "CommonHeader.h"
#include <sstream>

using namespace std;

Scene::Scene() :
	m_pSceneWorld(nullptr),
	m_updateable(true),
	m_drawable(true),
    m_pSkyBox(nullptr),
	m_usePostProcessEffects(false),
	m_ambientLightPercentage(0.0f),
    m_sceneIsOpaque(true)
{
}

Scene::~Scene()
{
    SAFE_DELETE(m_pSkyBox);

    for each(auto iterator in m_pGameObjects)
	{
		if (iterator.second != nullptr)
		{
			delete iterator.second;
			iterator.second = nullptr;
		}
	}

	//Delete the particle systems
	while (m_pParticleSystems.empty() == false)
	{
		SAFE_DELETE(m_pParticleSystems.back());
		m_pParticleSystems.pop_back();
	}


	//std::vector<Scene*> m_pLinkedPostProcessingScenes;//DOESN'T DELETE THE SCENES
}

void Scene::OnSurfaceChanged(unsigned int width, unsigned int height)
{
	//m_projectionMatrix.CreateOrtho(-DEFAULT_PROJECTION_SCALE, DEFAULT_PROJECTION_SCALE, -DEFAULT_PROJECTION_SCALE, DEFAULT_PROJECTION_SCALE, 0, 200);//Orthographic
	m_projectionMatrix.CreatePerspectiveVFoV(45, 1, 0.01f, 200);//Perspective

	m_HUDProjectionMatrix.CreateOrtho(-DEFAULT_HUD_PROJECTION_SCALE, DEFAULT_HUD_PROJECTION_SCALE, -DEFAULT_HUD_PROJECTION_SCALE, DEFAULT_HUD_PROJECTION_SCALE, 0, 200);//Orthographic

	//Update the projection matrix on the world debug drawer
	if (m_pSceneWorld != nullptr)
	{
		m_pSceneWorld->SetDebugDrawProjectionMatrix(&m_projectionMatrix);
	}
}

void Scene::LoadContent(b2Vec2 aWorldGravity)
{
	m_pSceneWorld = new Box2DWorld(aWorldGravity);
}

bool Scene::HandleInput(InputEvent& aInputevent, double aDelta)
{
	CheckControlsDebugDrawing(aInputevent);//Check if the debug drawer was set to do something

	bool inputUsed = false;
    bool inputObject = false;

	//Handle the input of all game objects
	for (unsigned int i = 0; i < m_pInputHandlerObjects.size(); i++)
	{
		if (m_pInputHandlerObjects.at(i) != nullptr)
		{
            inputObject=m_pInputHandlerObjects.at(i)->HandleInput(aInputevent, aDelta);

            if (inputObject == true && inputUsed==false)
            {
                inputUsed = true;
            }
		}
	}

	return inputUsed;
}

void Scene::Update(double TimePassed)
{
	if (m_updateable == true)
	{

		CheckForGLErrors();

		if (m_pSceneWorld != nullptr)
		{
			m_pSceneWorld->Update(TimePassed);//Update the physics world, this is done before updating all the visual representation of the objects
		}

        //Update the sky box
        if (m_pSkyBox != nullptr)
        {
            m_pSkyBox->Update(TimePassed);
        }


		map<std::string, GameObject*>::iterator nextItem = m_pGameObjects.begin();//Variable to save the next value since we amay remove element during update

		// update all of the Scene objects in the list.
		for (auto iterator = m_pGameObjects.begin(); iterator != m_pGameObjects.end();)
		{
			nextItem++;//Save the value of the next item

			if (iterator->second != nullptr)
			{
				if (iterator->second->GetEnabled() == true)//Check if the game object is enabled
				{
					iterator->second->Update(TimePassed);//Update the object, it may get deleted from map here

				}
			}

			iterator = nextItem;//Increase the iterator
		}

		//Update all the particle systems
		for (unsigned int i = 0; i < m_pParticleSystems.size(); i++)
		{
			if (m_pParticleSystems.at(i) != nullptr)
			{
				m_pParticleSystems.at(i)->Update(TimePassed);
			}
		}

		CheckForGLErrors();
	}
}

void Scene::Draw()
{
	if (m_drawable == true)
	{

		CheckForGLErrors();

		// render all of the Scene objects in the list.
		for (int i = 0; i < MAX_RENDER_ORDER; i++)
		{

            //Draw the sky box
            if (m_pSkyBox != nullptr)
            {
                //Disable Z writing
                glDepthMask(false);

                //Draw skybox
                m_pSkyBox->Draw(i);

                //Enable Z
                glDepthMask(true);
            }

			//for( std::map<std::string, GameObject*>::iterator iterator = m_pGameObjects.begin(); iterator != m_pGameObjects.end(); iterator++ )
			for each(auto iterator in m_pGameObjects)
			{
				if (iterator.second != nullptr)
				{
					if (iterator.second->GetEnabled() == true)//Check if the game object is enabled
					{
 						iterator.second->Draw(i);
					}
				}
			}
		}

		//Draw all the particle systems
		for (unsigned int i = 0; i < m_pParticleSystems.size(); i++)
		{
			if (m_pParticleSystems.at(i) != nullptr)
			{
				m_pParticleSystems.at(i)->Draw();
			}
		}

		m_pSceneWorld->DrawDebugDrawer();//Draw the debugger

		CheckForGLErrors();
	}
}

void Scene::Reset()
{
    if (m_pSkyBox != nullptr)
    {
        m_pSkyBox->Reset();
    }

	//Go through all the gameobjects in the scene
	for each(auto iterator in m_pGameObjects)
	{
		if (iterator.second != nullptr)
		{
			iterator.second->Reset();//Reset each one
		}
	}
}

void Scene::CheckControlsDebugDrawing(InputEvent& aInputevent)
{
	if (m_pSceneWorld != nullptr)
	{
		uint32 debugDrawFlags = m_pSceneWorld->GetDebugDrawFlags();//Get the current state of the flags

		if (aInputevent.state == InputEventState_Up && aInputevent.type == InputEventType_Key)
		{
			//Set all flags
			if (aInputevent.keycode == '9')
			{
				//Set all the flags
				debugDrawFlags |= b2Draw::e_shapeBit;
				debugDrawFlags |= b2Draw::e_jointBit;
				debugDrawFlags |= b2Draw::e_centerOfMassBit;
				debugDrawFlags |= b2Draw::e_aabbBit;
				debugDrawFlags |= b2Draw::e_pairBit;
			}

			//Clear all flags
			if (aInputevent.keycode == '0')
			{
				//Clear all the flags
				debugDrawFlags &= ~b2Draw::e_shapeBit;
				debugDrawFlags &= ~b2Draw::e_jointBit;
				debugDrawFlags &= ~b2Draw::e_centerOfMassBit;
				debugDrawFlags &= ~b2Draw::e_aabbBit;
				debugDrawFlags &= ~b2Draw::e_pairBit;
			}

			//Set Draw shapes flag
			if (aInputevent.keycode == '4')
			{
				debugDrawFlags ^= b2Draw::e_shapeBit;
			}

			//Set Draw joint connections flag	
			if (aInputevent.keycode == '5')
			{
				debugDrawFlags ^= b2Draw::e_jointBit;
			}

			//Set Draw axis aligned bounding boxes flag	
			if (aInputevent.keycode == '6')
			{
				debugDrawFlags ^= b2Draw::e_aabbBit;
			}

			//Set Draw broad-phase pairs flag	
			if (aInputevent.keycode == '7')
			{
				debugDrawFlags ^= b2Draw::e_pairBit;
			}

			//Set Draw a marker at body center of mass flag	
			if (aInputevent.keycode == '8')
			{
				debugDrawFlags ^= b2Draw::e_centerOfMassBit;
			}

		}

		m_pSceneWorld->SetDebugDrawFlags(debugDrawFlags);//Set the flags
	}
}

GameObject* Scene::AddGameObject(std::string aName, GameObject* aObject)
{
    if (aObject != nullptr)
    {
		if (m_pGameObjects[aName] == 0)
		{
			m_pGameObjects[aName] = aObject;//Add object to scene

			return aObject;//Return the game object wished for easier access
		}
	}

    return nullptr;
}

//Only removes an object from the map of gameobjects ,doesn't return it or delete it.
void Scene::RemoveGameObject(std::string aName)
{
	m_pGameObjects.erase(aName);
}

void Scene::SetSkyBox(GameObject* aSkyBoxObject)
{
    if (aSkyBoxObject != nullptr)
    {
        m_pSkyBox = aSkyBoxObject;
    }
}

void Scene::AddInputHandler(GameObject* aInputHandler)
{
	if (aInputHandler != nullptr)
	{
		m_pInputHandlerObjects.push_back(aInputHandler);
	}
}

void Scene::AddLight(LightObject* aLight)
{
	//Check that the lights don't go over their limit.
	if (m_pSceneLights.size() < MAX_NUMBER_OF_LIGHTS)
	{
		if (aLight != nullptr)
		{
			m_pSceneLights.push_back(aLight);
		}
	}
}

void Scene::AddParticleSystem(ParticleSystem * aPartifcleSystem)
{
	if (aPartifcleSystem != nullptr)
	{
		m_pParticleSystems.push_back(aPartifcleSystem);
	}
}

ParticleSystem* Scene::GetParticleSystem(unsigned int aIndex)
{
	if (aIndex < m_pParticleSystems.size())
	{
		return m_pParticleSystems.at(aIndex);
	}

    return nullptr;
}

void Scene::AddPostProcessingShader(ShaderProgram * aPostProcessingShader)
{
	if (aPostProcessingShader != nullptr)
	{
		m_pLinkedPostProcessingShaders.push_back(aPostProcessingShader);
	}
}

void Scene::RemovePostProcessingShader(ShaderProgram * aPostProcessingShader)
{
	//Search for the scene
	if (aPostProcessingShader != nullptr)
	{
		//Go through all the post processing scenes
		for (unsigned int i = 0;i < m_pLinkedPostProcessingShaders.size();i++)
		{
			//Compare the scenes to see if it found it
			if (m_pLinkedPostProcessingShaders.at(i) == aPostProcessingShader)
			{
				//Remove it from vector
				m_pLinkedPostProcessingShaders.erase(m_pLinkedPostProcessingShaders.begin() + i);
			}
		}
	}

}

//Checks a JSON file corresponding to a Box2D scene with all its game objects and creates the game objects accordingly. It creates all the object as the simple, base, game objects.
void Scene::LoadGameObjectsScene(cJSON* ajRoot)
{
	if (ajRoot != nullptr)
	{
		//Get all the game objects
		cJSON* jGameObjects = cJSON_GetObjectItem(ajRoot, "GameObjects");

		if (jGameObjects != 0)
		{

			cJSON* jCurrentObject = nullptr;
			int numberGameObjects = cJSON_GetArraySize(jGameObjects);

			//Load all the game objects
			for (int i = 0; i < numberGameObjects; i++)
			{
				jCurrentObject = cJSON_GetArrayItem(jGameObjects, i);

				//Check the type of the object
				cJSON* jFlags = cJSON_GetObjectItem(jCurrentObject, "Flags");
				cJSON* jCurrentFlag = nullptr;
				string stringCurrentFlag;

				if (jFlags != 0)
				{
					//If there are no flags, just create a gameobject
					if (cJSON_GetArraySize(jFlags) == 0)
					{
						LoadGameObject(jCurrentObject);
					}
					//Check all the flags the object
					for (int i = 0; i < cJSON_GetArraySize(jFlags); i++)
					{
						jCurrentFlag = cJSON_GetArrayItem(jFlags, i);
						{
							if (jCurrentFlag != 0)
							{
								stringCurrentFlag = jCurrentFlag->valuestring;

								//If the object is a GAME camera
								if (stringCurrentFlag == "Camera-Main")
								{
									CameraObject* gameCamera=(CameraObject*)LoadGameObject(jCurrentObject, new CameraObject(GAME_DEFAULT_CAMERA_NAME));

								}
								//If the object is a HUD camera
								else if (stringCurrentFlag == "Camera-HUD")				
								{
                                    CameraObject* hudCamera = (CameraObject*)LoadGameObject(jCurrentObject, new CameraObject(HUD_DEFAULT_CAMERA_NAME));
								}
								else//Load common game object
								{
									LoadGameObject(jCurrentObject);
								}
							}
						}
					}
				}
			}

		}

		//Load the joints, after the game objects have created
		LoadGameObjectsJointsScene(ajRoot);

		////Set the view in the debug draw
		CameraObject* pCamera = dynamic_cast<CameraObject*>(GetGameObject(GAME_DEFAULT_CAMERA_NAME));//Get the camera

		if (m_pSceneWorld != nullptr)
		{

			if (pCamera != nullptr)
			{
				m_pSceneWorld->SetDebugDrawViewMatrix(pCamera->GetViewMatrix());//Pass the debug draw the view matrix of the camera
			}

			m_pSceneWorld->SetDebugDrawShader(g_pGame->GetShader("Color"));//Set which sahder the debug draw will use
		}

	}
}

//Sets the properties of a gameobject pointer (if none is passed it will create a base game object) according to the JSON array object corresponding to a game object.
//It also adds the object to the scene
GameObject* Scene::LoadGameObject(cJSON* ajGameObject, GameObject* pGameObject,bool aAddToScene)
{
	if (ajGameObject != 0)
	{

		if (pGameObject == nullptr)
		{
			pGameObject = new GameObject();//Create a new game object if we don't pass a valid one
		}


		//If the game object is valid
		if (pGameObject != nullptr)
		{
			//Set the scene for the game object
			pGameObject->SetScene(this);

			//Check the name of the object
			LoadGameObjectName(ajGameObject, pGameObject);

			//Check the position of the object
			LoadGameObjectPosition(ajGameObject, pGameObject);

			//Check the rotation of the object
			LoadGameObjectRotation(ajGameObject, pGameObject);

			//Check the scale of the object
			LoadGameObjectScale(ajGameObject, pGameObject);

			//Load all the components of the game object
			LoadGameObjectComponents(ajGameObject, pGameObject);

            //Add object to scene
            if (aAddToScene == true)
            {
                AddGameObject(pGameObject->GetName(), pGameObject);
            }

		} //End of (pGameObject != nullptr)
	}//End of if(ajGameobject!=0)

	return pGameObject;
}

void Scene::LoadGameObjectsJointsScene(cJSON* ajRoot)
{
	if (ajRoot != nullptr)
	{
		//Get all the game objects
		cJSON* jGameObjects = cJSON_GetObjectItem(ajRoot, "GameObjects");

		if (jGameObjects != 0)
		{

			cJSON* jCurrentObject = nullptr;

			//Go  through all the game objects
			for (int i = 0; i < cJSON_GetArraySize(jGameObjects); i++)
			{
				jCurrentObject = cJSON_GetArrayItem(jGameObjects, i);

				//Load the joints for the game 
				LoadGameObjectJoint(jCurrentObject);

			}
		}
	}
}

Box2DJointInfo Scene::LoadGameObjectJoint(cJSON * ajGameObject)
{	
	Box2DJointInfo jointInformation;
	b2Joint* joint = nullptr;

	if (ajGameObject != 0)
	{
		string objectName;
		GameObject* pCurrentObject = nullptr;
		b2Body* pGameObjectBody = nullptr;

		//Check the name of the object
		cJSON* jName = cJSON_GetObjectItem(ajGameObject, "Name");
		if (jName != 0)
		{
			objectName = jName->valuestring;
		}

		//Get the game object
		pCurrentObject = m_pGameObjects[objectName];

		if (pCurrentObject != nullptr)
		{

			//Get the body of the game object
			pGameObjectBody = pCurrentObject->GetPhysicsBody();

            if (pGameObjectBody != nullptr)
            {

                //Check the array of components
                cJSON* jComponents = cJSON_GetObjectItem(ajGameObject, "Components");
                cJSON* jCurrentComponent = nullptr;

                if (jComponents != 0)
                {

                    //Go through all the components the object has
                    for (int i = 0; i < cJSON_GetArraySize(jComponents); i++)
                    {
                        //Get the current component in the loop
                        jCurrentComponent = cJSON_GetArrayItem(jComponents, i);

                        if (jCurrentComponent != 0)
                        {
                            //Get which type of component it is.
                            string componentType = cJSON_GetObjectItem(jCurrentComponent, "Type")->valuestring;

                            //Temporary variables used to create the joint
                            b2Vec2 anchorA;
                            b2Vec2 anchorB;
                            bool motorEnabled;
                            float motorSpeed;
                            float motorMaxForce;
                            bool limitEnabled;
                            float limitMin;
                            float limitMax;
                            string otherGameObjectName;
                            GameObject* otherGameObject = nullptr;
                            b2Body* otherGameObjectBody = nullptr;

                            //Only check for joints
                            if (componentType == "2D Joint - Revolute")
                            {
                                //Anchor A
                                cJSON* jAnchorA = cJSON_GetObjectItem(jCurrentComponent, "AnchorA");
                                if (jAnchorA == 0)
                                {
                                    return jointInformation;
                                }
                                else
                                {
                                    anchorA.x = (float)cJSON_GetArrayItem(jAnchorA, 0)->valuedouble;
                                    anchorA.y = (float)cJSON_GetArrayItem(jAnchorA, 1)->valuedouble;
                                }

                                //Anchor B
                                cJSON* jAnchorB = cJSON_GetObjectItem(jCurrentComponent, "AnchorB");
                                if (jAnchorB == 0)
                                {
                                    return jointInformation;
                                }
                                else
                                {
                                    anchorB.x = (float)cJSON_GetArrayItem(jAnchorB, 0)->valuedouble;
                                    anchorB.y = (float)cJSON_GetArrayItem(jAnchorB, 1)->valuedouble;
                                }

                                //Motor Enabled
                                cJSON* jMotorEnabled = cJSON_GetObjectItem(jCurrentComponent, "MotorEnabled");
                                if (jMotorEnabled == 0)
                                {
                                    return jointInformation;
                                }
                                else
                                {
                                    //Motor not enabled
                                    if (jMotorEnabled->valueint == 0)
                                    {
                                        motorEnabled = false;
                                    }
                                    else if (jMotorEnabled->valueint == 1)//Motor enabled
                                    {
                                        motorEnabled = true;
                                    }
                                }

                                //Motor Speed
                                cJSON* jMotorSpeed = cJSON_GetObjectItem(jCurrentComponent, "MotorSpeed");
                                if (jMotorSpeed == 0)
                                {
                                    return jointInformation;
                                }
                                else
                                {
                                    motorSpeed = (float)jMotorSpeed->valuedouble;
                                }

                                //Motor Max Torque
                                cJSON* jMotorMaxTorque = cJSON_GetObjectItem(jCurrentComponent, "MotorMaxTorque");
                                if (jMotorMaxTorque == 0)
                                {
                                    return jointInformation;
                                }
                                else
                                {
                                    motorMaxForce = (float)jMotorMaxTorque->valuedouble;
                                }

                                //L,imit Enabled
                                cJSON* jLimitEnabled = cJSON_GetObjectItem(jCurrentComponent, "LimitEnabled");
                                if (jLimitEnabled == 0)
                                {
                                    return jointInformation;
                                }
                                else
                                {
                                    //limit not enabled
                                    if (jLimitEnabled->valueint == 0)
                                    {
                                        limitEnabled = false;
                                    }
                                    else if (jLimitEnabled->valueint == 1)//limit enabled
                                    {
                                        limitEnabled = true;
                                    }
                                }

                                //Limit Min
                                cJSON* jLimitMin = cJSON_GetObjectItem(jCurrentComponent, "LimitMin");
                                if (jLimitMin == 0)
                                {
                                    return jointInformation;
                                }
                                else
                                {
                                    limitMin = (float)jLimitMin->valuedouble;
                                }

                                //Limit Max
                                cJSON* jLimitMax = cJSON_GetObjectItem(jCurrentComponent, "LimitMax");
                                if (jLimitMax == 0)
                                {
                                    return jointInformation;
                                }
                                else
                                {
                                    limitMax = (float)jLimitMax->valuedouble;
                                }

                                //Get the name of the object that is jointed to
                                cJSON* jOtherObjectName = cJSON_GetObjectItem(jCurrentComponent, "OtherGameObject");
                                if (jOtherObjectName != 0)
                                {
                                    otherGameObjectName = jOtherObjectName->valuestring;

                                    //Get the other game object
                                    otherGameObject = m_pGameObjects[otherGameObjectName];
                                }


                                //Get the body from the other game object
                                if (otherGameObject != nullptr)
                                {
                                    otherGameObjectBody = otherGameObject->GetPhysicsBody();
                                }
                                else if (m_pSceneWorld != nullptr)//Joint to center of the world
                                {
                                    otherGameObjectBody = m_pSceneWorld->GetCenterWorldBody();
                                    anchorB += b2Vec2(pGameObjectBody->GetPosition().x, pGameObjectBody->GetPosition().y);//Adjust the anchors
                                }

                                //Set the joint definition according to the values read from JSON file
                                b2RevoluteJointDef revoluteJointDefinition;
                                revoluteJointDefinition.localAnchorA = anchorA;
                                revoluteJointDefinition.localAnchorB = anchorB;
                                revoluteJointDefinition.enableMotor = motorEnabled;
                                revoluteJointDefinition.motorSpeed = motorSpeed;
                                revoluteJointDefinition.maxMotorTorque = motorMaxForce;
                                revoluteJointDefinition.enableLimit = limitEnabled;
                                revoluteJointDefinition.lowerAngle = limitMin;
                                revoluteJointDefinition.upperAngle = limitMax;

                                if (pGameObjectBody != nullptr&&otherGameObjectBody != nullptr)
                                {
                                    //Set the bodies for the joint
                                    revoluteJointDefinition.bodyA = pGameObjectBody;
                                    revoluteJointDefinition.bodyB = otherGameObjectBody;

                                    //Create the joint
                                    if (m_pSceneWorld != nullptr)
                                    {
                                        joint = m_pSceneWorld->CreateJoint(&revoluteJointDefinition);

                                        //Add the joint to the list of joints in case we use them later
                                        if (joint != nullptr)
                                        {
                                            jointInformation.SetJoinInfo(joint);
                                            m_pSceneJoints.push_back(jointInformation);
                                        }
                                    }
                                }


                            }//End if (componentType == "2D Joint - Revolute")
                            else if (componentType == "2D Joint - Prismatic")\
                            {
                                //Axis
                                b2Vec2 localAxis;
                                cJSON* jAxis = cJSON_GetObjectItem(jCurrentComponent, "Up");
                                if (jAxis == 0)
                                {
                                    return jointInformation;
                                }
                                else
                                {
                                    localAxis.x = (float)cJSON_GetArrayItem(jAxis, 0)->valuedouble;
                                    localAxis.y = (float)cJSON_GetArrayItem(jAxis, 1)->valuedouble;
                                    localAxis *= -1;//Change because of the order the bodies are set
                                }

                                //Anchor A
                                cJSON* jAnchorA = cJSON_GetObjectItem(jCurrentComponent, "AnchorA");
                                if (jAnchorA == 0)
                                {
                                    return jointInformation;
                                }
                                else
                                {
                                    anchorA.x = (float)cJSON_GetArrayItem(jAnchorA, 0)->valuedouble;
                                    anchorA.y = (float)cJSON_GetArrayItem(jAnchorA, 1)->valuedouble;
                                }

                                //Anchor B
                                cJSON* jAnchorB = cJSON_GetObjectItem(jCurrentComponent, "AnchorB");
                                if (jAnchorB == 0)
                                {
                                    return jointInformation;
                                }
                                else
                                {
                                    anchorB.x = (float)cJSON_GetArrayItem(jAnchorB, 0)->valuedouble;
                                    anchorB.y = (float)cJSON_GetArrayItem(jAnchorB, 1)->valuedouble;
                                }

                                //Motor Enabled
                                cJSON* jMotorEnabled = cJSON_GetObjectItem(jCurrentComponent, "MotorEnabled");
                                if (jMotorEnabled == 0)
                                {
                                    return jointInformation;
                                }
                                else
                                {
                                    //Motor not enabled
                                    if (jMotorEnabled->valueint == 0)
                                    {
                                        motorEnabled = false;
                                    }
                                    else if (jMotorEnabled->valueint == 1)//Motor enabled
                                    {
                                        motorEnabled = true;
                                    }
                                }

                                //Motor Speed
                                cJSON* jMotorSpeed = cJSON_GetObjectItem(jCurrentComponent, "MotorSpeed");
                                if (jMotorSpeed == 0)
                                {
                                    return jointInformation;
                                }
                                else
                                {
                                    motorSpeed = (float)jMotorSpeed->valuedouble;
                                }

                                //Motor Max Force
                                cJSON* jMotorMaxForce = cJSON_GetObjectItem(jCurrentComponent, "MotorMaxForce");
                                if (jMotorMaxForce == 0)
                                {
                                    return jointInformation;
                                }
                                else
                                {
                                    motorMaxForce = (float)jMotorMaxForce->valuedouble;
                                }

                                //L,imit Enabled
                                cJSON* jLimitEnabled = cJSON_GetObjectItem(jCurrentComponent, "LimitEnabled");
                                if (jLimitEnabled == 0)
                                {
                                    return jointInformation;
                                }
                                else
                                {
                                    //limit not enabled
                                    if (jLimitEnabled->valueint == 0)
                                    {
                                        limitEnabled = false;
                                    }
                                    else if (jLimitEnabled->valueint == 1)//limit enabled
                                    {
                                        limitEnabled = true;
                                    }
                                }

                                //Limit Min
                                cJSON* jLimitMin = cJSON_GetObjectItem(jCurrentComponent, "LimitMin");
                                if (jLimitMin == 0)
                                {
                                    return jointInformation;
                                }
                                else
                                {
                                    limitMin = (float)jLimitMin->valuedouble;
                                }

                                //Limit Max
                                cJSON* jLimitMax = cJSON_GetObjectItem(jCurrentComponent, "LimitMax");
                                if (jLimitMax == 0)
                                {
                                    return jointInformation;
                                }
                                else
                                {
                                    limitMax = (float)jLimitMax->valuedouble;
                                }

                                //Get the name of the object that is jointed to
                                cJSON* jOtherObjectName = cJSON_GetObjectItem(jCurrentComponent, "OtherGameObject");
                                if (jOtherObjectName != 0)
                                {
                                    otherGameObjectName = jOtherObjectName->valuestring;
                                }

                                //Get the other game object
                                otherGameObject = m_pGameObjects[otherGameObjectName];

                                //Get the body from the other game object
                                if (otherGameObject != nullptr)
                                {
                                    otherGameObjectBody = otherGameObject->GetPhysicsBody();
                                }
                                else if (m_pSceneWorld != nullptr)//Joint to center of the world
                                {
                                    otherGameObjectBody = m_pSceneWorld->GetCenterWorldBody();
                                    anchorB += b2Vec2(pGameObjectBody->GetPosition().x, pGameObjectBody->GetPosition().y);//Adjust the anchors
                                }

                                if (otherGameObjectBody != nullptr)
                                {
                                    //Set the joint definition according to the values read from JSON file
                                    b2PrismaticJointDef prismaticJointDefinition;
                                    prismaticJointDefinition.localAxisA = localAxis;
                                    prismaticJointDefinition.localAnchorA = anchorA;
                                    prismaticJointDefinition.localAnchorB = anchorB;
                                    prismaticJointDefinition.enableMotor = motorEnabled;
                                    prismaticJointDefinition.motorSpeed = motorSpeed;
                                    prismaticJointDefinition.maxMotorForce = motorMaxForce;
                                    prismaticJointDefinition.enableLimit = limitEnabled;
                                    prismaticJointDefinition.lowerTranslation = limitMin;
                                    prismaticJointDefinition.upperTranslation = limitMax;

                                    if (pGameObjectBody != nullptr&&otherGameObjectBody != nullptr)
                                    {
                                        //Set the bodies for the joint
                                        prismaticJointDefinition.bodyA = pGameObjectBody;
                                        prismaticJointDefinition.bodyB = otherGameObjectBody;

                                        //Create the joint
                                        if (m_pSceneWorld != nullptr)
                                        {
                                            joint = m_pSceneWorld->CreateJoint(&prismaticJointDefinition);

                                            //Add the joint to the list of joints in case we use them later
                                            if (joint != nullptr)
                                            {
                                                jointInformation.SetJoinInfo(joint);
                                                m_pSceneJoints.push_back(jointInformation);
                                            }
                                        }
                                    }
                                }
                            }


                        }//End of if (jCurrentComponent != 0)
                    }//End of  for (unsigned int i = 0; i < cJSON_GetArraySize(jComponents); i++)
                }//End of if (jComponents!=0) 
			}//End of if (pGameObjectBody != nullptr)
		}//End of if (pCurrentObject != nullptr)
	} //End of if (ajGameObject!=0)

	//Return the joint information
	return 	  jointInformation;
}

//Check for valid CJSON and pointer done in LoadGameObject Fucntion
void Scene::LoadGameObjectName(cJSON * ajGameObject, GameObject* apGameObject)
{
	//Check the name of the object
	cJSON* jName = cJSON_GetObjectItem(ajGameObject, "Name");
	if (jName != 0)
	{
		string objectName = jName->valuestring;

		//Check that the object doesn't have the name of a camera, since they are set/created as camera objects when loading all game objects
		if (apGameObject->GetName() != GAME_DEFAULT_CAMERA_NAME && apGameObject->GetName()!= HUD_DEFAULT_CAMERA_NAME)
		{
			apGameObject->SetName(objectName);
		}
	}
	else
	{
		apGameObject->SetName("CANT_READ_NAME_FROM_FILE");
	}
}

//Check for valid CJSON and pointer done in LoadGameObject Fucntion
void Scene::LoadGameObjectPosition(cJSON * ajGameObject, GameObject* apGameObject)
{
	//Check the position of the object
	cJSON* jPosition = cJSON_GetObjectItem(ajGameObject, "Pos");
	if (jPosition != 0)
	{
		apGameObject->SetPositionX((float)cJSON_GetArrayItem(jPosition, 0)->valuedouble);
		apGameObject->SetPositionY((float)cJSON_GetArrayItem(jPosition, 1)->valuedouble);
		apGameObject->SetPositionZ((float)cJSON_GetArrayItem(jPosition, 2)->valuedouble);
	}
	else
	{
		apGameObject->SetPosition(vec3(0, 0, 0));
	}

    apGameObject->SetDefaultPosition(apGameObject->GetPosition());//Set the default position the object will have
}

//Check for valid CJSON and pointer done in LoadGameObject Fucntion
void Scene::LoadGameObjectRotation(cJSON * ajGameObject, GameObject* apGameObject)
{
	//Check the rotation of the object
	cJSON* jRotation = cJSON_GetObjectItem(ajGameObject, "Rot");
	if (jRotation != 0)
	{
		apGameObject->SetRotationX((float)cJSON_GetArrayItem(jRotation, 0)->valuedouble);
		apGameObject->SetRotationY((float)cJSON_GetArrayItem(jRotation, 1)->valuedouble);
		apGameObject->SetRotationZ((float)cJSON_GetArrayItem(jRotation, 2)->valuedouble);
	}
	else
	{
		apGameObject->SetRotation(vec3(0, 0, 0));
	}

    apGameObject->SetDefaultRotation(apGameObject->GetRotation());//Set the default rotation the object will have
}

//Check for valid CJSON and pointer done in LoadGameObject Fucntion
void Scene::LoadGameObjectScale(cJSON * ajGameObject, GameObject* apGameObject)
{
	//Check the scale of the object
	cJSON* jScale = cJSON_GetObjectItem(ajGameObject, "Scale");
	if (jScale != 0)
	{
		apGameObject->SetScaleX((float)cJSON_GetArrayItem(jScale, 0)->valuedouble);
		apGameObject->SetScaleY((float)cJSON_GetArrayItem(jScale, 1)->valuedouble);
		apGameObject->SetScaleZ((float)cJSON_GetArrayItem(jScale, 2)->valuedouble);
	}
	else
	{
		apGameObject->SetScale(vec3(1, 1, 1));
	}

    apGameObject->SetDefaultScale(apGameObject->GetScale());//Set the default scale the object will have
}

//Check for valid CJSON and pointer done in LoadGameObject Fucntion
void Scene::LoadGameObjectComponents(cJSON * ajGameObject, GameObject* apGameObject)
{
	//Check the array of components
	cJSON* jComponents = cJSON_GetObjectItem(ajGameObject, "Components");
	cJSON* jCurrentComponent = nullptr;
	string componentType;

	if (jComponents != 0)
	{

		vector<cJSON*> jBox2Components;

		//Go through all the components the object has
		for (int i = 0; i < cJSON_GetArraySize(jComponents); i++)
		{
			//Get the current component in the loop
			jCurrentComponent = cJSON_GetArrayItem(jComponents, i);

			if (jCurrentComponent != 0)
			{
				//Get which type of component it is.
				componentType = cJSON_GetObjectItem(jCurrentComponent, "Type")->valuestring;

				//If the component is a sprite
				if (componentType == "Sprite")
				{
					LoadGameObjectSpriteComponent(jCurrentComponent, apGameObject);
				}
				else if (componentType == "2D Collision Object")
				{
					//Make the bodies after they have make a mesh
					jBox2Components.push_back(jCurrentComponent);

				}
				//Joints checked in a second pass
				//Type of mesh
				else if (componentType == "Mesh-OBJ")
				{
					LoadGameObjectOBJMesh(jCurrentComponent, apGameObject);
				}

			}//End of if (jCurrentComponent != 0)
		}	//End of for (unsigned int i = 0; i < cJSON_GetArraySize(jComponents); i++)

		//Load all the bodies/fixtures of the game object
		LoadGameObjectsBodiesComponent(jBox2Components, apGameObject);

	}//End of if (jComponents != 0)
}

//Check for valid CJSON and pointer done in LoadGameObject Fucntion
void Scene::LoadGameObjectSpriteComponent(cJSON * ajSpriteComponent, GameObject* apGameObject)
{
	//Shader
	cJSON* jShader = cJSON_GetObjectItem(ajSpriteComponent, "Shader");
	if (jShader != 0)
	{
		ShaderProgram* objectShader = LoadOrCreateShader(jShader->valuestring);

		if (objectShader != nullptr)
		{
			apGameObject->SetShader(objectShader);
		}
	}

	//Color
	cJSON* jColor = cJSON_GetObjectItem(ajSpriteComponent, "Color");
	if (jColor != 0)
	{
		//Get each individual RGBA value
		Vector4 objectRGBAColor;
		objectRGBAColor.x = (float)cJSON_GetArrayItem(jColor, 0)->valuedouble;
		objectRGBAColor.y = (float)cJSON_GetArrayItem(jColor, 1)->valuedouble;
		objectRGBAColor.z = (float)cJSON_GetArrayItem(jColor, 2)->valuedouble;
		objectRGBAColor.w = (float)cJSON_GetArrayItem(jColor, 3)->valuedouble;

		apGameObject->SetColor(objectRGBAColor);
	}

	//Box (sprite) mesh
	apGameObject->SetMesh(g_pGame->GetMesh("Box"));
}

//Check for valid CJSON and pointer done in LoadGameObject Fucntion
void Scene::LoadGameObjectsBodiesComponent(std::vector<cJSON*> aBodyComponents, GameObject* apGameObject)
{
	//Make all the box2d bodies
	for (unsigned int i = 0; i < aBodyComponents.size(); i++)
	{
		cJSON* jCurrentComponent = aBodyComponents.at(i);
		if (jCurrentComponent != 0)
		{
			//Get which type of component it is.
			string componentType = cJSON_GetObjectItem(jCurrentComponent, "Type")->valuestring;

			if (componentType == "2D Collision Object")
			{

				cJSON* jFixtureShape = cJSON_GetObjectItem(jCurrentComponent, "PrimitiveType");
				if (jFixtureShape != 0)
				{
					string fixtureShape = jFixtureShape->valuestring;
					bool fixtureCreated = false;//Tracks whether a fixture has been created or not

												//Check which corresponding fixture shape the Box2D body will have
					if (fixtureShape == "Box")
					{
						LoadGameObjectBoxBody(apGameObject);
						fixtureCreated = true;
					}
					else if (fixtureShape == "Circle")
					{
						LoadGameObjectCircleBody(apGameObject);
						fixtureCreated = true;
					}
					else if (fixtureShape == "Edge")
					{
						LoadGameObjectEdgeBody(apGameObject);
						fixtureCreated = true;
					}
					else if (fixtureShape == "Chain")
					{
						fixtureCreated = LoadGameObjectChainBody(jCurrentComponent, apGameObject);
					}

					//If a fixture was created, customize it.
					if (fixtureCreated == true)
					{
						LoadGameObjectPhysicsBody(jCurrentComponent, apGameObject);//Customize the fixture/physics body
					}
				}
			}
		}
	}
}

//Check for valid CJSON and pointer done in LoadGameObject Fucntion
void Scene::LoadGameObjectBoxBody(GameObject * apGameObject)
{
	vec2 physicsBodySize;

	//Get the size the physics body (except physics chain), will have.
	physicsBodySize.x = apGameObject->GetXLength();
	physicsBodySize.y = apGameObject->GetYLength();

	//If there is not a mesh use the scale
	if (physicsBodySize.x <= 0)
	{
		physicsBodySize.x = apGameObject->GetScale().x;
	}

	if (physicsBodySize.y <= 0)
	{
		physicsBodySize.y = apGameObject->GetScale().y;
	}

	//Double check that the values are greater than 0
	if (physicsBodySize.x <= 0)
	{
		physicsBodySize.x = 1;
	}

	if (physicsBodySize.y <= 0)
	{
		physicsBodySize.y = 1;
	}

	apGameObject->CreatePhysicsBox(physicsBodySize);//Create the box fixture

}

//Check for valid CJSON and pointer done in LoadGameObject Fucntion
void Scene::LoadGameObjectCircleBody(GameObject * apGameObject)
{
	float radius;
	radius = apGameObject->GetXYRadius();

	//If there is not a mesh use the scale
	if (radius <= 0)
	{
		radius = (apGameObject->GetScale().x + apGameObject->GetScale().y) / 2;//Add the Y and X length divided by 2 to get the average diameter
		radius /= 2;
	}

	//Double check that the values are greater than 0
	if (radius <= 0)
	{
		radius = 0.5f;
	}

	apGameObject->CreatePhysicsCircle(radius);
}

//Check for valid CJSON and pointer done in LoadGameObject Fucntion
void Scene::LoadGameObjectEdgeBody(GameObject * apGameObject)
{
	vec3 objectPostion = apGameObject->GetPosition();
	float objectHalfWidth = apGameObject->GetXLength() / 2.0f;

	//If there is not a mesh use the scale
	if (objectHalfWidth <= 0)
	{
		objectHalfWidth = apGameObject->GetScale().x / 2.0f;
	}

	//Double check that the values are greater than 0
	if (objectHalfWidth <= 0)
	{
		objectHalfWidth = 0.5f;
	}

	//The start of the line will be at the center of the object, minus half its width
	vec2 startEdge;
	startEdge.y = objectPostion.y;
	startEdge.x = objectPostion.x - objectHalfWidth;

	//The end of the line will be at the center of the object, plus half its width
	vec2 endEdge;
	endEdge.y = objectPostion.y;
	endEdge.x = objectPostion.x + objectHalfWidth;

	apGameObject->CreatePhysicsLine(startEdge, endEdge);
}

//Check for valid CJSON and pointer done in LoadGameObject Fucntion
bool Scene::LoadGameObjectChainBody( cJSON* ajBodyComponent,GameObject * apGameObject)
{
	bool fixtureMade = false;

	//Get the vertices needed to make the chain
	cJSON* jVertices = cJSON_GetObjectItem(ajBodyComponent, "Vertices");
	if (jVertices != 0)
	{
		b2Vec2* vertices2DPosition = nullptr;
		cJSON* jCurrentXOrYValue = nullptr;

		//Set the size of the array of vertices position. The JSON files stores the 
		//positions individually as an X and Y value. So the total size will be the size of the array
		//divided by 2
		unsigned int numVertices = cJSON_GetArraySize(jVertices) / 2;
		vertices2DPosition = new b2Vec2[numVertices];

		int unsigned currentVertex = 0;//Tracker of current vertex being modified

									   //Go through all the positions, increase the count by 2. Since it has X and Y position for 1 b2Vec vertex position.
		for (int j = 0; j < cJSON_GetArraySize(jVertices); j += 2)
		{
			//Get the x value
			jCurrentXOrYValue = cJSON_GetArrayItem(jVertices, j);
			if (jCurrentXOrYValue != 0)
			{
				vertices2DPosition[currentVertex].x = (float)jCurrentXOrYValue->valuedouble;
			}

			//Get the x value
			jCurrentXOrYValue = cJSON_GetArrayItem(jVertices, j + 1);
			if (jCurrentXOrYValue != 0)
			{
				vertices2DPosition[currentVertex].y = (float)jCurrentXOrYValue->valuedouble;
			}

			//Increase the count of the current vertex
			currentVertex++;
		}

		if (vertices2DPosition != nullptr && numVertices >= 2)
		{
			//Create the line strips using the vertices positions and the number of vertices divided by 2. Because one line is made of 2 vertices.
			apGameObject->CreatePhysicsLineStrip(vertices2DPosition, numVertices, false);
			fixtureMade = true;//Return that the body was successfully created
		}

		//Delete the array of vertices
		delete[] vertices2DPosition;

	}

	return fixtureMade;//Return if the fixtuere wasn made
}

//Check for valid CJSON and pointer done in LoadGameObject Fucntion
void Scene::LoadGameObjectPhysicsBody(cJSON * aBodyComponent, GameObject * apGameObject)
{
	if (aBodyComponent != 0 && apGameObject != nullptr)
	{
		//Body Type
		cJSON* jStatic = cJSON_GetObjectItem(aBodyComponent, "Static");
		if (jStatic != 0)
		{

			//Dynamic body
			if (jStatic->valueint == 0)
			{
				apGameObject->SetPhysicsBodyType(b2_dynamicBody);
			}
			else if (jStatic->valueint == 1)//Static body		
			{
				apGameObject->SetPhysicsBodyType(b2_staticBody);
			}
		}

		//Density
		cJSON* jDensity = cJSON_GetObjectItem(aBodyComponent, "Density");
		if (jDensity != 0)
		{
			apGameObject->SetPhysicsBodyDensity((float)jDensity->valuedouble);
		}

		//IsSensor
		cJSON* jSensor = cJSON_GetObjectItem(aBodyComponent, "IsSensor");
		if (jSensor != 0)
		{
			//Is not a sensor
			if (jSensor->valueint == 0)
			{
				apGameObject->SetPhysicsBodyIsSensor(false);
			}
			else if (jSensor->valueint == 1)//Is a sensor
			{
				apGameObject->SetPhysicsBodyIsSensor(true);
			}
		}

		//Friction
		cJSON* jFriction = cJSON_GetObjectItem(aBodyComponent, "Friction");
		if (jFriction != 0)
		{
			apGameObject->SetPhysicsBodyFriction((float)jFriction->valuedouble);
		}

		//Restitution
		cJSON* jRestitution = cJSON_GetObjectItem(aBodyComponent, "Restitution");
		if (jRestitution != 0)
		{
			apGameObject->SetPhysicsBodyRestitution((float)jRestitution->valuedouble);
		}
	}
}

void Scene::LoadGameObjectOBJMesh(cJSON * aOBJComponent, GameObject * apGameObject)
{
	if (aOBJComponent != 0 && apGameObject != nullptr)
	{
		Mesh* objMesh;
		string objName;
		string objFilepath;
		GLenum primitiveType= GL_TRIANGLES;
		float pointLineSize = -1.0f;

		//Primitive Type
		cJSON* jPrimitiveType = cJSON_GetObjectItem(aOBJComponent, "PrimitiveType");
		if (jPrimitiveType != 0)
		{
			string primitiveTypeFound = jPrimitiveType->valuestring;//Convert to string to ensure it works correctly (it takes values as char* otherwise, and only compares first letter)

			//Triangles
			if (primitiveTypeFound == "Triangles")
			{
				primitiveType=GL_TRIANGLES;
			}
			else if (jPrimitiveType->valuestring == "Points")//Points
			{
				primitiveType = GL_POINTS;
			}
			else if (jPrimitiveType->valuestring == "Lines")//Lines
			{
				primitiveType = GL_LINES;
			}
			else if (jPrimitiveType->valuestring == "LineLoop")//Line Loop
			{
				primitiveType = GL_LINE_LOOP;
			}
			else if (jPrimitiveType->valuestring == "TriangleStrip")//Triangle Strip
			{
				primitiveType = GL_TRIANGLE_STRIP;
			}
			else if (jPrimitiveType->valuestring == "TriangleFan")//Triangle Fan
			{
				primitiveType = GL_TRIANGLE_FAN;
			}
		}

		//Point size or line width
		cJSON* jPointSizeAndLineWidth = cJSON_GetObjectItem(aOBJComponent, "PointSize");
		if (jPointSizeAndLineWidth != 0)
		{
			pointLineSize=(float)jPointSizeAndLineWidth->valuedouble;
		}

		//OBJ
		cJSON* jOBJName = cJSON_GetObjectItem(aOBJComponent, "OBJFilename");
		if (jOBJName != 0)
		{
			string nameWithFileExtension = jOBJName->valuestring;

			//Remove the file extension from the file
			objName = nameWithFileExtension.substr(0, nameWithFileExtension.find(".", 0));//Remove everything after the dot


			char * objCharName = _strdup(objName.c_str());//Convert a const char* to char*
			objMesh = LoadOrCreateOBJ(objCharName);
		}

		//Shader
		cJSON* jShader = cJSON_GetObjectItem(aOBJComponent, "Shader");
		if (jShader != 0)
		{
			ShaderProgram* objectShader = LoadOrCreateShader(jShader->valuestring);

			//Set shader in game object
			if (objectShader != nullptr)
			{
				apGameObject->SetShader(objectShader);
			}
		}

		//Color
		cJSON* jColor = cJSON_GetObjectItem(aOBJComponent, "Color");
		if (jColor != 0)
		{
			//Get each individual RGBA value
			Vector4 objectRGBAColor;
			objectRGBAColor.x = (float)cJSON_GetArrayItem(jColor, 0)->valuedouble;
			objectRGBAColor.y = (float)cJSON_GetArrayItem(jColor, 1)->valuedouble;
			objectRGBAColor.z = (float)cJSON_GetArrayItem(jColor, 2)->valuedouble;
			objectRGBAColor.w = (float)cJSON_GetArrayItem(jColor, 3)->valuedouble;

            //Conver colors to a 0 to 1 range
            objectRGBAColor.x /= 255.0f;
            objectRGBAColor.y /= 255.0f;
            objectRGBAColor.z /= 255.0f;
            objectRGBAColor.w /= 255.0f;

			apGameObject->SetColor(objectRGBAColor);
		}

		//Texture
		cJSON* jTexture = cJSON_GetObjectItem(aOBJComponent, "Texture");
		if (jTexture != 0)
		{
			
			string textureFilePath = jTexture->valuestring;
			string textureFileName = GetFileNameFromPath(textureFilePath);//This reads from file the texture and its path from the EDITOR , not the project. So the file directory has to be removed.

			//Create or add the texture
			char * textureCharName = _strdup(textureFileName.c_str());//Convert a const char* to char*
			GLuint textureHandle = LoadOrCreateTexture(textureCharName);

			if (textureHandle != 0)
			{
				apGameObject->SetTexture(textureHandle);
			}
		}

		//If the mesh is not null set all the properties that were read
		if (objMesh != nullptr)
		{
			objMesh->SetPrimitiveType(primitiveType);

			if (pointLineSize > 0.0f)//If the point line size is vale
			{
				objMesh->SetPointSize(pointLineSize);
				objMesh->SetLineWidth(pointLineSize);
			}

			//Set it for use by the gameobject
			apGameObject->SetMesh(objMesh);
		}
	}
}

ShaderProgram* Scene::LoadOrCreateShader(char* aShaderName)
{
	ShaderProgram* returnedShaderProgram = nullptr;

	//Check if the shader already exists in the game
	returnedShaderProgram = g_pGame->GetShader(aShaderName);
	
	//If the shader is not in the program add it
	if (returnedShaderProgram == nullptr)
	{
		//Convert the char to const char*
		stringstream shaderFileNameSS;
		shaderFileNameSS << aShaderName;
		
		//Joint together all the file names, and extensions to make the complete file path
		//Vertex shader
		std::string shaderFilePathVert(DEFAULT_FILE_PATH_SHADERS);
		shaderFilePathVert.append(shaderFileNameSS.str());
		shaderFilePathVert.append(".vert");

		//Fragment shdaer
		std::string shaderFilePathFrag(DEFAULT_FILE_PATH_SHADERS);
		shaderFilePathFrag.append(shaderFileNameSS.str());
		shaderFilePathFrag.append(".frag");

        //Add the shader to the gamee
        returnedShaderProgram = new ShaderProgram(shaderFilePathVert.c_str(), shaderFilePathFrag.c_str());//Create shader
        g_pGame->AddShader(aShaderName, returnedShaderProgram);
	}

	return returnedShaderProgram;
}

Mesh * Scene::LoadOrCreateOBJ(char * aOBJName)
{
	Mesh* returnedMesh = nullptr;

	//Check if the mesh already exists in the game
	returnedMesh = g_pGame->GetMesh(aOBJName);

	//If the shader is not in the program add it
	if (returnedMesh == nullptr)
	{
		//Convert the char to const char*
		stringstream meshFileNameSS;
		meshFileNameSS << aOBJName;

		//Joint together all the file names, and extensions to make the complete file path
		std::string meshFilePath(DEFAULT_FILE_PATH_OBJS);
		meshFilePath.append(meshFileNameSS.str());
		meshFilePath.append(".obj");

		//Add the shader to the gamee
		returnedMesh = Mesh::LoadObj(meshFilePath.c_str());//Safety check to see if file exists done in here
		g_pGame->AddMesh(aOBJName, returnedMesh);
	}

	return returnedMesh;
}

GLuint Scene::LoadOrCreateTexture(char * aTextureName)
{
	GLuint returnedTexture = 0;

	//Check if the texture already exists in the game
	returnedTexture = g_pGame->GetTexture(aTextureName);

	//If the shader is not in the program add it
	if (returnedTexture == 0)
	{
		//Convert the char to const char*
		stringstream textureFileNameSS;
		textureFileNameSS << aTextureName;

		string textureName = GetFileNameFromPath(textureFileNameSS.str());//Get the texture name from the file name, no path or extensions

		//Joint together all the file names, and extensions to make the complete file path
		std::string textureFilePath(DEFAULT_FILE_PATH_TEXTURES);
		textureFilePath.append(textureName);
		textureFilePath.append(".png");

		//Add the testure to the gamee
		returnedTexture = LoadTexture(textureFilePath.c_str());
	
		g_pGame->AddTexture(textureName.c_str(), returnedTexture);
	}

	return returnedTexture;
}

//Function modified from Cplusplus http://www.cplusplus.com/reference/string/string/find_last_of/
std::string Scene::GetFileNameFromPath(std::string filePath,bool withFileExtension)
{
	//ORIGINAL from Cplusplus
	/*std::cout << "Splitting: " << str << '\n';
	std::size_t found = str.find_last_of("/\\");
	std::cout << " path: " << str.substr(0, found) << '\n';
	std::cout << " file: " << str.substr(found + 1) << '\n';*/

	string fileName = filePath;

	std::size_t lastSlashFound = filePath.find_last_of("/\\"); //Find in the file path the last forward slash (/)
	fileName=fileName.substr(lastSlashFound + 1);//Get the characters after the last slash was found 

	if (withFileExtension == false)
	{
		fileName = fileName.substr(0, fileName.find(".", 0));//Remove everything after the file extension (marked by a .)
	}

	return fileName;
}

