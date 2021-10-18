#include "CommonHeader.h"

FGSlidingDoor::FGSlidingDoor(double waitTime, double warningTime, Vector4 stationaryColor, Vector4 warningColor, Vector4 movingColor)
{
    m_pJoint = nullptr;

}

FGPlayerWheel::~FGPlayerWheel()
{
}

void FGPlayerWheel::Update(double TimePassed)
{

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

    GameObject::Update(TimePassed);
}