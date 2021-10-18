#ifndef __Box2DJointInfo__H__
#define  __Box2DJointInfo__H__

#include "CommonHeader.h"

class GameObject;

//Struct to store the information about a box2D joint. This struct is not intended to be used in actual game logic, but rather as a way to easily pass the information
// to be used in the game. The struct contains pointers to the joint, which type of joint it is, and pointers to the gameobjects and box2d physics body linked to it.
struct Box2DJointInfo
{
	b2Joint* m_pJoint;
	b2JointType  m_pJointType;
	GameObject* m_pGameObjectA;
	b2Body* m_pBodyA;
	GameObject* m_pGameObjectB;
	b2Body* m_pBodyB;

	//Constructor to set all values to null by default
	Box2DJointInfo()
	{
		m_pJoint = nullptr;
		m_pJointType = e_unknownJoint;
		m_pGameObjectA = nullptr;
		m_pBodyA = nullptr;
		m_pGameObjectB = nullptr;
		m_pBodyB = nullptr;

	}

	void SetJoinInfo(b2Joint* aJoint)
	{
		if (aJoint != nullptr)
		{
			m_pJoint = aJoint;
			m_pJointType = aJoint->GetType();
	
			m_pBodyA= aJoint->GetBodyA();
			m_pGameObjectA = (GameObject*)m_pBodyA->GetUserData();

			m_pBodyB = aJoint->GetBodyB();
			m_pGameObjectB = (GameObject*)m_pBodyB->GetUserData();
		}
	}

};

#endif