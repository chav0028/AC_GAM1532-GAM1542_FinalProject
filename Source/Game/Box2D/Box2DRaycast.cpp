#include "CommonHeader.h"

Box2DRayCast::Box2DRayCast(Box2DRayCastType aRayCastType)
{
    m_hit = false;
    m_rayCastType = aRayCastType;
}

float32 Box2DRayCast::ReportFixture(b2Fixture* aFixture, const b2Vec2& aPoint, const b2Vec2 &aNormal, float32 aFraction)
{
	if (m_rayCastType == Box2DRayCastType_Ignore)
	{
		return -1;
	}

	if (aFixture != nullptr)
	{
		//Copy all the information
		m_rayCastInformation.push_back(new RayCastInformation());//Push empty value
		m_rayCastInformation.back()->m_pFixture = aFixture;
		m_rayCastInformation.back()->m_pBody = aFixture->GetBody();
		m_rayCastInformation.back()->m_point = aPoint;
		m_rayCastInformation.back()->m_normal = aNormal;
		m_rayCastInformation.back()->m_fraction = aFraction;


		//Check for fixtures to ignore
		for (unsigned int i = 0; i < m_bodyTypesToIgnore.size(); i++)
		{
			if (m_rayCastInformation.back()->m_pBody->GetType() == m_bodyTypesToIgnore.at(i))
			{
				return -1;//Ignore the fixture shape and continue finding other fixtures.
			}
		}

		m_hit = true;//Report a hit

		switch (m_rayCastType)
		{
		case Box2DRayCastType_Closest:
			return aFraction;//Find the closest fixture
			break;

		case Box2DRayCastType_AllFixtures:
			return 1;//Continue finding fixtures
			break;

		case Box2DRayCastType_Anything:
			return 0;//Stop the raycast, only used to check if it collided with something, anything
			break;

		default:
			return aFraction;
			break;
		}
	}

	return aFraction;

}

RayCastInformation* Box2DRayCast::GetRayCastInformation(unsigned int aIndex)
{
    if (aIndex < m_rayCastInformation.size())
    {
        return m_rayCastInformation.at(aIndex);
    }
	
	return nullptr;
}

std::vector<RayCastInformation*> Box2DRayCast::GetAllRayCastInformation()
{
    return m_rayCastInformation;
}

void Box2DRayCast::ClearAllRayCastInformation()
{
	m_rayCastInformation.clear();
}

void Box2DRayCast::AddBodyTypeToIgnore(b2BodyType aBodyType)
{
	bool bodyTypeInList = false;

	//Check that the body type hasn't been already added
	for (unsigned int i = 0; i < m_bodyTypesToIgnore.size(); i++)
	{
		//If the body type is already in the list
		if (m_bodyTypesToIgnore.at(i) == aBodyType)
		{
			bodyTypeInList = true;//Break out of the loop
			break;
		}
	}

	//If the body type is not in the list
	if (bodyTypeInList == false)
	{
	   //Add the body type
		m_bodyTypesToIgnore.push_back(aBodyType);
	}

}

void Box2DRayCast::RemoveBodyTypeToIgnore(b2BodyType aBodyType)
{							  
	//Search for the body type in the lsit
	for (unsigned int i = 0; i < m_bodyTypesToIgnore.size(); i++)
	{
		if (m_bodyTypesToIgnore.at(i) == aBodyType)
		{
			m_bodyTypesToIgnore.erase(m_bodyTypesToIgnore.begin(), m_bodyTypesToIgnore.begin() + i);
		}
	}

}

//Merely clears all information and set hit to false
void Box2DRayCast::ResetRayCast()
{
	ClearAllRayCastInformation();
	m_hit = false;
}
