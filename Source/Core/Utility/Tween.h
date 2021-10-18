#ifndef __Tween_H__
#define __Tween_H__

#include "TweenFuncs.h"
#include "CommonHeader.h"

enum TweeningType
{
    TweeningType_Linear,
    TweeningType_SineEaseIn,
    TweeningType_SineEaseout,
    TweeningType_SineEaseInOut,
    TweeningType_BounceEaseIn,
    TweeningType_BounceEaseout,
    TweeningType_BounceEaseInOut,
    TweeningType_ElasticEaseIn,
    TweeningType_ElasticEaseout,
    TweeningType_ElasticEaseInOut
};

template<class T>
class Tween
{
public:
    typedef T(*TweenFunction)(T startValue, T valueRange, double timeElapse, double totalTime);

private:
    std::string m_TweenName;//Used for identifyng
    T m_startValue;
    T m_endValue;
    double m_endTime;
    double m_timeElapsed;
    double m_startDelayTime;
    TweeningType m_tweeningType;
    T* m_tweenedValue;
    T m_valueRange;
    bool m_tweenStarted;
    bool m_tweenOver;

    TweenFunction m_pTweenFunction;

public:
    Tween();
    ~Tween();

    virtual void Tween::SetupTween(T* apTweenedValue, T aEndValue, double aEndTime, TweeningType aType = TweeningType_Linear, double startDelay = 0.0, std::string tweenName = "Tween");
    virtual void Update(double aDelta);
    std::string GetTweenName() { return m_TweenName; }
    bool GetTweenOver(){ return m_tweenOver; }
};


template<class T>
inline Tween<T>::Tween()
{
    m_timeElapsed = 0;
    m_endTime = 0;
    m_startDelayTime = 0;
    m_tweeningType = TweeningType_Linear;

    m_tweenedValue = nullptr;
    m_pTweenFunction = nullptr;
    m_tweenOver = true;

}

template<class T>
inline Tween<T>::~Tween()
{

}


template<class T>
inline void Tween<T>::SetupTween(T* apTweenedValue, T aEndValue, double aEndTime, TweeningType aType, double aStartDelay, std::string aTweenName)
{
    if (apTweenedValue != nullptr)
    {
        m_TweenName = aTweenName;
        m_startValue = *apTweenedValue;
        m_tweenedValue = apTweenedValue;
        m_endValue = aEndValue;
        m_endTime = aEndTime;
        m_tweeningType = aType;
        m_valueRange = m_endValue - m_startValue;
        m_timeElapsed = 0.0;
        m_tweeningType = aType;
        m_startDelayTime = aStartDelay;
        m_tweenStarted = true;
        if (m_startDelayTime > 0)
        {
            m_tweenStarted = false;
        }

        m_tweenOver = false;

        //Assign the type of easing
        switch (m_tweeningType)
        {
        case TweeningType_Linear:
            m_pTweenFunction = TweenFunc_Linear;
            break;
        case TweeningType_SineEaseIn:
            m_pTweenFunction = TweenFunc_SineEaseIn;
            break;
        case TweeningType_SineEaseout:
            m_pTweenFunction = TweenFunc_SineEaseOut;
            break;
        case TweeningType_SineEaseInOut:
            m_pTweenFunction = TweenFunc_SineEaseInOut;
            break;
        case TweeningType_BounceEaseIn:
            m_pTweenFunction = TweenFunc_BounceEaseIn;
            break;
        case TweeningType_BounceEaseout:
            m_pTweenFunction = TweenFunc_BounceEaseOut;
            break;
        case TweeningType_BounceEaseInOut:
            m_pTweenFunction = TweenFunc_BounceEaseInOut;
            break;
        case TweeningType_ElasticEaseIn:
            m_pTweenFunction = TweenFunc_ElasticEaseIn;
            break;
        case TweeningType_ElasticEaseout:
            m_pTweenFunction = TweenFunc_ElasticEaseOut;
            break;
        case TweeningType_ElasticEaseInOut:
            m_pTweenFunction = TweenFunc_ElasticEaseInOut;
            break;
        default:
            break;
        }
    }
}

template<class T>
inline void Tween<T>::Update(double aDelta)
{
    if (m_tweenedValue != nullptr && m_pTweenFunction != nullptr)
    {
        m_timeElapsed += aDelta;

        //Delayed start
        if (m_tweenStarted == false)
        {
            if (m_timeElapsed >= m_startDelayTime)
            {
                m_tweenStarted = true;
                m_timeElapsed = 0;
            }
        }
        else
        {
            //Normal tweening
            if (m_timeElapsed < m_endTime)
            {
                T currentValue = m_pTweenFunction(m_startValue, m_valueRange, m_timeElapsed, m_endTime);
                *m_tweenedValue = currentValue;//Change the value in the object
            }
            else
            {
                //If time is over reset pointer
                m_tweenedValue = nullptr;

                m_tweenOver = true;
            }
        }
    }
}


#endif