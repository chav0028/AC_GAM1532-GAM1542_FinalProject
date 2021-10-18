#include "CommonHeader.h"

using namespace std;


NumberFontSystem::NumberFontSystem(unsigned int aNumber, Scene * apScene, string aName, vec3 aPos, vec3 aRot, vec3 aScale, Mesh * apMesh, ShaderProgram * apShader, GLuint aTexture, 
    const char* aCameraName, unsigned int aDrawRenderOrder) :
GameObject(apScene, aName, aPos, aRot, aScale, apMesh, apShader, aTexture, vec2(1,1),vec2(0,0),aCameraName,aDrawRenderOrder),
m_number(aNumber)
{
}

NumberFontSystem::~NumberFontSystem()
{
    //Delete all objects
    while (m_DigitBoxes.empty() == false)
    {
        //Delete element
        delete m_DigitBoxes.back();
        m_DigitBoxes.back() = nullptr;

        //Removed element from vector
        m_DigitBoxes.pop_back();
    }
}

void NumberFontSystem::LoadContent()
{
    //Create all the digit boxes
    for (unsigned int i = 0; i < NUMBER_FONT_SYSTEM_MAX_DIGITS; i++)
    {
        GameObject* tempRectangle = new GameObject();
        tempRectangle->Init(m_pScene, "Digit Box" + to_string(i), vec3(0, 0, 0), vec3(0, 0, 0), vec3(m_Scale.x / NUMBER_FONT_SYSTEM_MAX_DIGITS, m_Scale.y, m_Scale.z), m_pMesh, m_pShaderProgram, m_TextureHandle);//Create a rectangle for each digit
        tempRectangle->SetRenderOrder(GetRenderOrder());
        tempRectangle->SetCameraDrawerName(GetCameraDrawerName());
        tempRectangle->SetUVScale(Vector2(10, 1));//Set the scale of the to match the number of digits (from 0-9) in the number font sprite strip
        m_DigitBoxes.push_back(tempRectangle);//Add it to the list
    }

    SetNumber(m_number);//Set the number their will display
    SetPosition(m_Position);//Set their position   
}

void NumberFontSystem::Draw(int aRenderOrder)
{
    //check if the render order being drawn matches the one of the object
    if (GetRenderOrder() == aRenderOrder)
    {
        if (m_numberOfDigits - 1 < m_DigitBoxes.size())//Num digits -1 , because the number of digits index starts at 1 and not 0
        {

            //Draw the digits according to the ones that are in the number
            for (unsigned int i = 0; i < m_numberOfDigits; i++)
            {
                if (m_DigitBoxes.at(i) != nullptr)
                {            
                    m_DigitBoxes.at(i)->Draw(aRenderOrder);
                }
            }
        }
    }
}

//Sets the number variable and calls the function to change the number displayed
void NumberFontSystem::SetNumber(unsigned int aNumber)
{
    if (aNumber <= NUMBER_FONT_SYSTEM_MAX_NUMBER)
    {
        unsigned int singleDigit = 0;
        unsigned int numberDigits = 0;

        // Display digit in hundred thousand spot
        if (aNumber >= HUNDRED_THOUSAND)
        {
            if (m_DigitBoxes.size() > 5)
            {
                singleDigit = aNumber / HUNDRED_THOUSAND % 10;//Get the value of the digit in the hundred thousan spot

                ChangeNumberDisplay(singleDigit, m_DigitBoxes.at(5));//Change the number that will be displayed
                numberDigits++;//Increase the digit count
            }
        }

        // Display digit in ten thousand spot
        if (aNumber >= TEN_THOUSAND)
        {
            if (m_DigitBoxes.size() > 4)
            {
                singleDigit = aNumber / TEN_THOUSAND % 10;//Get the value of the digit in the ten thousand spot

                ChangeNumberDisplay(singleDigit, m_DigitBoxes.at(4));//Change the number that will be displayed
                numberDigits++;//Increase the digit count
            }
        }

        // Display digit in thousand spot
        if (aNumber >= THOUSAND)
        {
            if (m_DigitBoxes.size() > 3)
            {
                singleDigit = aNumber / THOUSAND % 10;//Get the value of the digit in the thousand spot

                ChangeNumberDisplay(singleDigit, m_DigitBoxes.at(3));//Change the number that will be displayed
                numberDigits++;//Increase the digit count
            }

        }

        // Display digit in hundred spot
        if (aNumber >= HUNDRED)
        {
            if (m_DigitBoxes.size() > 2)
            {
                singleDigit = aNumber / HUNDRED % 10;//Get the value of the digit in the hundred spot


                ChangeNumberDisplay(singleDigit, m_DigitBoxes.at(2));//Change the number that will be displayed
                numberDigits++;//Increase the digit count
            }
        }

        // Display digit in tens spot
        if (aNumber >= TEN)
        {
            if (m_DigitBoxes.size() > 1)
            {
                singleDigit = aNumber / TEN % 10;//Get the value of the digit in the ten  spot


                ChangeNumberDisplay(singleDigit, m_DigitBoxes.at(1));//Change the number that will be displayed
                numberDigits++;//Increase the digit count
            }
        }


        //Display digit in ones spot
        singleDigit = aNumber % 10;//Get the value of the digit in the ten thousand spot

        //Always display something in the ones spot, even 0.
        if (m_DigitBoxes.empty() == false)
        {
            ChangeNumberDisplay(singleDigit, m_DigitBoxes.at(0));//Change the number that will be displayed
            numberDigits++;//Increase the digit count
        }

        //Save the number of digits
        m_numberOfDigits = numberDigits;

        //Save the current number
        m_number = aNumber;

        //Reset numbers position
        SetPosition(m_Position);
    }
}

//Sets which number will be displayed
void NumberFontSystem::ChangeNumberDisplay(unsigned int aNumber, GameObject* aDigitBox)
{
    float offsetX = 0.0f;
    float offsetY = 0.0f;

    //Calculate the UV offset using math
    if (aNumber < NUMBERS_DISPLAYED_IN_TEXTURE_STRIP)
    {
        offsetX = (float)aNumber / (float)NUMBERS_DISPLAYED_IN_TEXTURE_STRIP;
        offsetY = 0.0f;
    }
    else
    {
        //Set the UV to display empty space
        offsetX = 0.0f;
        offsetY = 0.5f;
    }

    aDigitBox->SetUVOffset(Vector2(offsetX, offsetY));//Set the offset

}


//Setters, merely ensure that the properties is set in this system, AND in ALL the digit boxes this system has
void NumberFontSystem::SetEnabled(bool aEnabledStatus)
{
    GameObject::SetEnabled(aEnabledStatus);

    //Go through all the digits
    for (unsigned int i = 0; i < m_DigitBoxes.size(); i++)
    {
        if (m_DigitBoxes.at(i) != nullptr)//If they are not null
        {
            m_DigitBoxes.at(i)->SetEnabled(aEnabledStatus);// Set its status
        }
    }
}

void NumberFontSystem::SetPosition(vec3 aPosition)
{
    m_Position = aPosition;//Set the status in this system

    //Calculate how wide a single digit box is
    float singleDigitWidth = 0;
    if (m_DigitBoxes.empty() == false)
    {
        if (m_DigitBoxes.at(0) != nullptr)//if we have digit boxes
        {
            singleDigitWidth = m_DigitBoxes.at(0)->GetScale().x;
        }
    }

    //ANCHOR POINT is in the middle left

    //Go through all the digits, from left to right
    unsigned int digitModified = 0;//Get a variable so that we modify the values from right to left
    for (unsigned int i = m_numberOfDigits - 1; i < m_DigitBoxes.size(); i--)
    {
        if (m_DigitBoxes.at(digitModified) != nullptr)//If they are not null
        {
            float xValue = m_Position.x + (singleDigitWidth*i);
            m_DigitBoxes.at(digitModified)->SetPosition(vec3(xValue, m_Position.y, m_Position.z));//Set the position, according to the number of digits
            //between the anchor point on the left and the current digit being modified
        }
        digitModified++;
    }
}

void NumberFontSystem::SetScale(vec3 aScale)
{
    m_Scale = aScale;

    //Go through all the digits
    for (unsigned int i = 0; i < m_DigitBoxes.size(); i++)
    {
        if (m_DigitBoxes.at(i) != nullptr)//If they are not null
        {
            m_DigitBoxes.at(i)->SetScale(aScale);
            SetPosition(m_Position);//Place the number again, so that the distance between them is reset
        }
    }
}

void NumberFontSystem::SetRotation(vec3 aRotation)
{
    m_Rotation = aRotation;

    //Go through all the digits
    for (unsigned int i = 0; i < m_DigitBoxes.size(); i++)
    {
        if (m_DigitBoxes.at(i) != nullptr)//If they are not null
        {
            m_DigitBoxes.at(i)->SetRotation(aRotation);
        }
    }
}

void NumberFontSystem::SetColor(Vector4 aColor)
{
    m_Color = aColor;

    //Go through all the digits
    for (unsigned int i = 0; i < m_DigitBoxes.size(); i++)
    {
        if (m_DigitBoxes.at(i) != nullptr)//If they are not null
        {
            m_DigitBoxes.at(i)->SetColor(aColor);
        }
    }
}

void NumberFontSystem::Reset()
{
    GameObject::Reset();

    //Go through all the digits
    for (unsigned int i = 0; i < m_DigitBoxes.size(); i++)
    {
        if (m_DigitBoxes.at(i) != nullptr)//If they are not null
        {
            m_DigitBoxes.at(i)->Reset();
        }
    }

}