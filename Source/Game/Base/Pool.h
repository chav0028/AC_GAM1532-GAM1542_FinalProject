#ifndef __Pool__H__
#define __Pool__H__

#include <vector>

/*Template classs to easily manage object/memory pooling.*/
template<class T>
class Pool
{

public:
	Pool(unsigned int numberObjects);
	~Pool();

	void DeactivateObject(T* object);
	T* GetActivateObject();
	void AddNewObjectsToPool(unsigned int numberObjects);

    T* GetInactiveObject(unsigned int aIndex);

	int GetActiveObjectIndex(T* object);
    int GetInactiveObjectIndex(T * object);

	std::vector<T*> GetCurrentlyActiveObjects();
    std::vector<T*> GetCurrentlyInactiveObjects();

    void AddExistingObject(T* object);
    
private:
	std::vector<T*>m_activeObjects;
	std::vector <T*>m_inactiveObjects;
};

template<class T>
inline Pool<T>::Pool(unsigned int numberObjects)
{
	AddNewObjectsToPool(numberObjects);//Create the number of objects
}

template<class T>
inline Pool<T>::~Pool()
{
	//Go through all objects
	while (m_activeObjects.empty() == false)
	{
        if (m_activeObjects.back() != nullptr)
        {
            delete m_activeObjects.back();//Delete last object
            m_activeObjects.pop_back();//Remove it from vector
        }
	}

	while (m_inactiveObjects.empty() == false)
	{
        if (m_inactiveObjects.back() != nullptr)
        {
            delete m_inactiveObjects.back();//Delete last object
            m_inactiveObjects.pop_back();//Remove it from vector
        }
	}
}

template<class T>
inline void Pool<T>::DeactivateObject(T * object)
{
	m_inactiveObjects.push_back(object);//Add object back to inactive list


	int index = GetActiveObjectIndex(object);
	if (index != -1)//if the object was find in the active objects
	{
		m_activeObjects.erase(m_activeObjects.begin() + index);//Remove element from active objects
	}
}

template<class T>
inline void Pool<T>::AddExistingObject(T* aObject)
{
    if (aObject != nullptr)
    {
        m_inactiveObjects.push_back(aObject);//Add object back to inactive list
    }

}

template<class T>
inline T * Pool<T>::GetActivateObject()
{
	//If we  have aN inactive object left
	if (m_inactiveObjects.empty() == false)
	{
		T* object = m_inactiveObjects.back();//Get the object from the back of the inactive list
		m_inactiveObjects.pop_back();//Remove the object from inactive list

		if (object != nullptr)
		{
			m_activeObjects.push_back(object);//Add the object to active list

			return object;//Return the object
		}
	}
	//We could create more objects here if the inactive list is empty

	return nullptr;//Return nullptr
}

//Function do return an inactive object, doesn't remove it from inactive list or activate the object
template<class T>
inline T * Pool<T>::GetInactiveObject(unsigned int aIndex)
{
    //If we  have aN inactive object left
    if (m_inactiveObjects.empty() == false)
    {
        if (m_inactiveObjects.size() < aIndex)
        {
            T* object = m_inactiveObjects.at(aIndex);//Get the object from the back of the inactive list

            if (object != nullptr)//If it is a valid object
            {
                return object;//Return the object
            }
        }
    }
    //We could create more objects here if the inactive list is empty

    return nullptr;//Return nullptr
}

template<class T>
inline void Pool<T>::AddNewObjectsToPool(unsigned int numberObjects)
{
	for (unsigned int i = 0; i < numberObjects; i++)
	{
		m_inactiveObjects.push_back(new T(this));//Create an object and set that it belongs to this pool
	}
}

template<class T>
inline int Pool<T>::GetActiveObjectIndex(T * object)
{
	//Check that there is a valid object
	if (object != nullptr)
	{
		//Look for the object in the active projectiles
		for (unsigned int i = 0; i < m_activeObjects.size(); i++)
		{
			//If the object is find
			if (m_activeObjects.at(i) == object)
			{
				return i;//Return the position at it was find
			}
		}
	}

	return -1;//If it wasn't find , return -1 to indicate an error
}

template<class T>
inline int Pool<T>::GetInactiveObjectIndex(T * object)
{
    //Check that there is a valid object
    if (object != nullptr)
    {
        //Look for the object in the active projectiles
        for (unsigned int i = 0; i < m_inactiveObjects.size(); i++)
        {
            //If the object is find
            if (m_inactiveObjects.at(i) == object)
            {
                return i;//Return the position at it was find
            }
        }
    }

    return -1;//If it wasn't find , return -1 to indicate an error
}


template<class T>
inline std::vector<T*> Pool<T>::GetCurrentlyActiveObjects()
{
	return m_activeObjects;
}

template<class T>
inline std::vector<T*> Pool<T>::GetCurrentlyInactiveObjects()
{
    return m_inactiveObjects;
}

#endif