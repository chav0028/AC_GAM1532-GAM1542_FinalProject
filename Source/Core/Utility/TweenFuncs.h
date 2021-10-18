#ifndef __TweenFuncs_H__
#define __TweenFuncs_H__

#include "CommonHeader.h"

//// t: current time, b: begInnIng value, c: change In value, d: duration
template <typename T>
T BounceEaseIn(double t, T b, T c, double d);
template <typename T>
T BounceEaseOut(double t, T b, T c, double d);
template <typename T>
T BounceEaseInOut(double t, T b, T c, double d);
template <typename T>
T ElasticEaseIn(double t, T b, T c, double d, float elasticity);
template <typename T>
T ElasticEaseOut(double t, T b, T c, double d, float elasticity);
template <typename T>
T ElasticEaseInOut(double t, T b, T c, double d, float elasticity);

template <typename T>
T TweenFunc_Linear(T startvalue, T valuerange, double timepassed, double totaltime)
{
	float timeperc = (float)(timepassed / totaltime);
	if (timeperc > 1)
		timeperc = 1;
	return startvalue + valuerange*timeperc;
}

template <typename T>
T TweenFunc_SineEaseIn(T startvalue, T valuerange, double timepassed, double totaltime)
{
	float timeperc = (float)(timepassed / totaltime);
	if (timeperc > 1)
		timeperc = 1;

	T inverValueRange = valuerange*-1;//Changed so that the code works with template
	return startvalue + inverValueRange * cos(timeperc * PI / 2) + valuerange;
}

template <typename T>
T TweenFunc_SineEaseOut(T startvalue, T valuerange, double timepassed, double totaltime)
{
	float timeperc = (float)(timepassed / totaltime);
	if (timeperc > 1)
		timeperc = 1;
	return startvalue + valuerange * sin(timeperc * PI / 2);
}

template <typename T>
T TweenFunc_SineEaseInOut(T startvalue, T valuerange, double timepassed, double totaltime)
{
	float timeperc = (float)(timepassed / totaltime);
	if (timeperc > 1)
		timeperc = 1;

	T inverValueRange = valuerange*-1;//Changed so that the code works with template

	return startvalue + inverValueRange / 2 * (cos(timeperc * PI) - 1);
}

template <typename T>
T TweenFunc_BounceEaseIn(T startvalue, T valuerange, double timepassed, double totaltime)
{
	return (T)BounceEaseIn(timepassed, startvalue, valuerange, totaltime);
}

template <typename T>
T TweenFunc_BounceEaseOut(T startvalue, T valuerange, double timepassed, double totaltime)
{
	return (T)BounceEaseOut(timepassed, startvalue, valuerange, totaltime);
}

template <typename T>
T TweenFunc_BounceEaseInOut(T startvalue, T valuerange, double timepassed, double totaltime)
{
	return (T)BounceEaseInOut(timepassed, startvalue, valuerange, totaltime);
}
template <typename T>
T TweenFunc_ElasticEaseIn(T startvalue, T valuerange, double timepassed, double totaltime)
{
	return (T)ElasticEaseIn(timepassed, startvalue, valuerange, totaltime, 3);
}

template <typename T>
T TweenFunc_ElasticEaseOut(T startvalue, T valuerange, double timepassed, double totaltime)
{
	return (T)ElasticEaseOut(timepassed, startvalue, valuerange, totaltime, 3);
}

template <typename T>
T TweenFunc_ElasticEaseInOut(T startvalue, T valuerange, double timepassed, double totaltime)
{
	return (T)ElasticEaseInOut(timepassed, startvalue, valuerange, totaltime, 3);
}


//http://www.kirupa.com/forum/archive/index.php/t-76799.html
//// t: current time, b: begInnIng value, c: change In value, d: duration

template <typename T>
T BounceEaseIn(double t, T b, T c, double d)
{
	return c - BounceEaseOut(d - t, (T)0, c, d) + b;
}

template <typename T>
T BounceEaseOut(double t, T b, T c, double d)
{
	if ((t /= d) < (1 / 2.75f))
	{
		return (T)(c*(7.5625f*(float)t*(float)t) + b);
	}
	else if (t < (2 / 2.75f))
	{
		double postFix = t -= (1.5f / 2.75f);
        return (T)(c*(7.5625f*(float)(postFix)*(float)t + 0.75f) + b);
	}
	else if (t < (2.5 / 2.75))
	{
		double postFix = t -= (2.25f / 2.75f);
        return (T)(c*(7.5625f*(float)(postFix)*(float)t + 0.9375f) + b);
	}
	else
	{
		double postFix = t -= (2.625f / 2.75f);
        return (T)(c*(7.5625f*(float)(postFix)*(float)t + 0.984375f) + b);
	}
}

template <typename T>
T BounceEaseInOut(double t, T b, T c, double d)
{
	if (t < d / 2)
		return BounceEaseIn(t * 2, (T)0, c, d) * .5f + b;
	else
		return BounceEaseOut(t * 2 - d, (T)0, c, d) * .5f + c*.5f + b;
}

template <typename T>
T ElasticEaseIn(double t, T b, T c, double d, float elasticity)
{
	if (t == 0)
		return b;
	if (t > d)
		return b + c;
	if ((t /= d) == 1)
		return b + c;

	float p = (float)d * 0.3f;
	T a = c;
	float s = p / 4;

    t -= 1;
    float poW = elasticity*(float)t;
    float tempPostFix = (float)pow(10, poW);
    T postFix = (T)(a * tempPostFix);
    T result = (T)((postFix * sin(((float)t*(float)d - s) * (2 * PI) / p)) + b);

	return result*-1;
}

template <typename T>
T ElasticEaseOut(double t, T b, T c, double d, float elasticity)
{
	if (t == 0)
		return b;
	if (t > d)
		return b + c;
	if ((t /= d) == 1)
		return b + c;

    float p = (float)d * 0.3f;
	T a = c;
	float s = p / 4;

    return (T)((a * (float)pow(10, -elasticity*(float)t) * sin(((float)t*(float)d - s) * (2 * PI) / p) + c + b));
}

template <typename T>
T ElasticEaseInOut(double t, T b, T c, double d, float elasticity)
{
	if (t == 0)
		return b;
	if (t > d)
		return b + c;
	if ((t /= d / 2) == 2)
		return b + c;

    float p = (float)d * (0.3f * 1.5f);
	T a = c;
	float s = p / 4;

	if (t < 1)
	{
        T postFix = (T)(a * (float)pow(10, elasticity*(t -= 1)));
        T calculatedValue = (T)((postFix * sin(((float)t*(float)d - s) * (2 * PI) / p)));
		calculatedValue *= -0.5f;
		calculatedValue += b;
		return calculatedValue;
	}

    T postFix = (T)(a * (float)pow(10, -elasticity*(t -= 1)));
    return (T)(postFix * sin(((float)t*(float)d - s) * (2 * PI) / p) * 0.5f + c + b);
}

#endif //__TweenFuncs_H__
