#ifndef __Utility_H__
#define __Utility_H__

#define PI 3.1415926535897932384626433832795f

const float FEQUALEPSILON = 0.00001f;

inline bool fequal(const float a, const float b, const float epsilon = FEQUALEPSILON)
{
    return fabs(a - b) <= epsilon;
}

inline bool fnotequal(const float a, const float b, const float epsilon = FEQUALEPSILON)
{
    return fabs(a - b) > epsilon;
}

template <class MyType> void MySwap(MyType &v1, MyType &v2)
{
    MyType temp = v1;
    v1 = v2;
    v2 = temp;
}

template <class MyType> void MyClamp(MyType &value, MyType min, MyType max)
{
    if( value < min )
        value = min;

    if( value > max )
        value = max;
}

template <class MyType> MyType MyClamp_Return(MyType value, MyType min, MyType max)
{
    MyType temp = value;

    if( temp < min )
        temp = min;

    if( temp > max )
        temp = max;

    return temp;
}

template <class MyType> void IncreaseIfBigger(MyType &value, MyType newvalue)
{
    if( newvalue > value )
        value = newvalue;
}

template <class MyType> void DecreaseIfLower(MyType &value, MyType newvalue, bool treatzeroasinfinite = false)
{
    if( treatzeroasinfinite && value == 0 )
        value = newvalue;

    if( newvalue < value )
        value = newvalue;
}

#endif //__Utility_H__
