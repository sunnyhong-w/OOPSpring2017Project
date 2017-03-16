#include "StdAfx.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "enginelib.h"
#include "easing.h"

namespace game_engine
{
double Easing::_easeBasic(double a, double dt)
{
    a = a + 1;
    return pow(dt, a) / (pow(dt, a) + pow(1 - dt, a));
}

double Easing::easeInBasic(
    double t, double b, double c, double d, double a)
{
    return c * _easeBasic(a, (t / d) / 2) / 0.5 + b;
}

double Easing::easeOutBasic(
    double t, double b, double c, double d, double a)
{
    return c * (_easeBasic(a, 0.5 + (t / d) / 2) - 0.5) / 0.5 + b;
}

double Easing::easeInOutBasic(
    double t, double b, double c, double d, double a)
{
    return c * _easeBasic(a, t / d) + b;
}

double Easing::linear(
    double t, double b, double c, double d)
{
    return c * t / d + b;
}

double Easing::easeInQuad(
    double t, double b, double c, double d)
{
    return c * (t /= d) * t + b;
}

double Easing::easeOutQuad(
    double t, double b, double c, double d)
{
    return -c * (t /= d) * (t - 2) + b;
}

double Easing::easeInOutQuad(
    double t, double b, double c, double d)
{
    if ((t /= d / 2) < 1) return c / 2 * t * t + b;

    return -c / 2 * ((--t) * (t - 2) - 1) + b;
}

double Easing::easeInCubic(
    double t, double b, double c, double d)
{
    return c * (t /= d) * t * t + b;
}

double Easing::easeOutCubic(
    double t, double b, double c, double d)
{
    return c * ((t = t / d - 1) * t * t + 1) + b;
}

double Easing::easeInOutCubic(
    double t, double b, double c, double d)
{
    if ((t /= d / 2) < 1) return c / 2 * t * t * t + b;

    return c / 2 * ((t -= 2) * t * t + 2) + b;
}

double Easing::easeInQuart(
    double t, double b, double c, double d)
{
    return c * (t /= d) * t * t * t + b;
}

double Easing::easeOutQuart(
    double t, double b, double c, double d)
{
    return -c * ((t = t / d - 1) * t * t * t - 1) + b;
}

double Easing::easeInOutQuart(
    double t, double b, double c, double d)
{
    if ((t /= d / 2) < 1) return c / 2 * t * t * t * t + b;

    return -c / 2 * ((t -= 2) * t * t * t - 2) + b;
}

double Easing::easeInQuint(
    double t, double b, double c, double d)
{
    return c * (t /= d) * t * t * t * t + b;
}

double Easing::easeOutQuint(
    double t, double b, double c, double d)
{
    return c * ((t = t / d - 1) * t * t * t * t + 1) + b;
}

double Easing::easeInOutQuint(
    double t, double b, double c, double d)
{
    if ((t /= d / 2) < 1) return c / 2 * t * t * t * t * t + b;

    return c / 2 * ((t -= 2) * t * t * t * t + 2) + b;
}

double Easing::easeInSine(
    double t, double b, double c, double d)
{
    return -c * cos(t / d * M_PI_2) + c + b;
}

double Easing::easeOutSine(
    double t, double b, double c, double d)
{
    return c * sin(t / d * M_PI_2) + b;
}

double Easing::easeInOutSine(
    double t, double b, double c, double d)
{
    return -c / 2 * (cos(M_PI * t / d) - 1) + b;
}

double Easing::easeInExpo(
    double t, double b, double c, double d)
{
    return (t == 0) ? b : c * pow(2, 10 * (t / d - 1)) + b;
}

double Easing::easeOutExpo(
    double t, double b, double c, double d)
{
    return (t == d) ? b + c : c * (-pow(2, -10 * t / d) + 1) + b;
}

double Easing::easeInOutExpo(
    double t, double b, double c, double d)
{
    if (t == 0) return b;

    if (t == d) return b + c;

    if ((t /= d / 2) < 1) return c / 2 * pow(2, 10 * (t - 1)) + b;

    return c / 2 * (-pow(2, -10 * --t) + 2) + b;
}

double Easing::easeInCirc(
    double t, double b, double c, double d)
{
    return -c * (sqrt(1 - (t /= d) * t) - 1) + b;
}

double Easing::easeOutCirc(
    double t, double b, double c, double d)
{
    return c * sqrt(1 - (t = t / d - 1) * t) + b;
}

double Easing::easeInOutCirc(
    double t, double b, double c, double d)
{
    if ((t /= d / 2) < 1) return -c / 2 * (sqrt(1 - t * t) - 1) + b;

    return c / 2 * (sqrt(1 - (t -= 2) * t) + 1) + b;
}

double Easing::easeInElastic(
    double t, double b, double c, double d)
{
    double s = 1.70158;
    double p;
    double a = c;

    if (t == 0) return b;

    if ((t /= d) == 1) return b + c;

    p = d * 0.3;

    if (a < abs(c))
    {
        a = c;
        s = p / 4;
    }
    else s = p / (2 * M_PI) * asin(c / a);

    return -(a * pow(2, 10 * (t -= 1)) * sin((t * d - s) * (2 * M_PI) / p)) + b;
}

double Easing::easeOutElastic(
    double t, double b, double c, double d)
{
    double s = 1.70158;
    double p;
    double a = c;

    if (t == 0) return b;

    if ((t /= d) == 1) return b + c;

    p = d * 0.3;

    if (a < abs(c))
    {
        a = c;
        s = p / 4;
    }
    else s = p / (2 * M_PI) * asin(c / a);

    return a * pow(2, -10 * t) * sin((t * d - s) * (2 * M_PI) / p) + c + b;
}

double Easing::easeInOutElastic(
    double t, double b, double c, double d)
{
    double s = 1.70158;
    double p;
    double a = c;

    if (t == 0) return b;

    if ((t /= d / 2) == 2) return b + c;

    p = d * (0.3 * 1.5);

    if (a < abs(c))
    {
        a = c;
        s = p / 4;
    }
    else s = p / (2 * M_PI) * asin(c / a);

    if (t < 1) return -.5 * (a * pow(2, 10 * (t -= 1)) * sin((t * d - s) * (2 * M_PI) / p)) + b;

    return a * pow(2, -10 * (t -= 1)) * sin((t * d - s) * (2 * M_PI) / p) * .5 + c + b;
}

double Easing::easeInBack(
    double t, double b, double c, double d, double s)
{
    double s2 = isnan(s) ? (s2 = 1.70158) : (double)s;
    return c * (t /= d) * t * ((s2 + 1) * t - s2) + b;
}

double Easing::easeOutBack(
    double t, double b, double c, double d, double s)
{
    double s2 = isnan(s) ? (s2 = 1.70158) : (double)s;
    return c * ((t = t / d - 1) * t * ((s2 + 1) * t + s2) + 1) + b;
}

double Easing::easeInOutBack(
    double t, double b, double c, double d, double s)
{
    double s2 = isnan(s) ? (s2 = 1.70158) : (double)s;

    if ((t /= d / 2) < 1) return c / 2 * (t * t * (((s2 *= (1.525)) + 1) * t - s2)) + b;

    return c / 2 * ((t -= 2) * t * (((s2 *= (1.525)) + 1) * t + s2) + 2) + b;
}

double Easing::easeInOutBack_Fix(
    double t, double b, double c, double d)
{
    double s2 = 1.70158;

    if ((t /= d / 2) < 1) return c / 2 * (t * t * (((s2 *= (1.525)) + 1) * t - s2)) + b;

    return c / 2 * ((t -= 2) * t * (((s2 *= (1.525)) + 1) * t + s2) + 2) + b;
}

double Easing::easeOutBounce(
    double t, double b, double c, double d)
{
    if ((t /= d) < (1 / 2.75))
    {
        return c * (7.5625 * t * t) + b;
    }
    else if (t < (2 / 2.75))
    {
        return c * (7.5625 * (t -= (1.5 / 2.75)) * t + .75) + b;
    }
    else if (t < (2.5 / 2.75))
    {
        return c * (7.5625 * (t -= (2.25 / 2.75)) * t + .9375) + b;
    }
    else
    {
        return c * (7.5625 * (t -= (2.625 / 2.75)) * t + .984375) + b;
    }
}

double Easing::easeInBounce(
    double t, double b, double c, double d)
{
    return c - Easing::easeOutBounce(d - t, 0, c, d) + b;
}

double Easing::easeInOutBounce(
    double t, double b, double c, double d)
{
    if (t < d / 2) return Easing::easeInBounce(t * 2, 0, c, d) * .5 + b;

    return Easing::easeOutBounce(t * 2 - d, 0, c, d) * .5 + c * .5 + b;
}

double Easing::backForth(double& t, double d, double dt)
{
    double ret = d - abs(d - t);
    t = (t + dt);

    if (t >= (d * 2))
        t = t - (d * 2);

    return ret;
}

int Easing::backForth(int& t, int d, int dt)
{
    int ret = d - abs(d - t);
    t = (t + dt) % (d * 2);
    return ret;
}

float Easing::easer(double func(double, double, double, double), double t, double b, double c, double d)
{
    t = t > d ? d : t;
    return (float)func(t, b, c, d);
}

Vector2 Easing::easer(double(*func)(double, double, double, double), double t, Vector2 b, Vector2 c, double d)
{
    return Vector2((float)(*func)(t, b.x, c.x, d), (float)(*func)(t, b.x, c.x, d));
}

float Easing::easeFT(double(*func)(double, double, double, double), double t, double from, double to, double d)
{
    t = t > d ? d : t;
    return (float)(*func)(t, from, to - from, d);
}

Vector2 Easing::easeFT(double(*func)(double, double, double, double), double t, Vector2 from, Vector2 to, double d)
{
    return Vector2((float)(*func)(t, from.x, to.x - from.x, d), (float)(*func)(t, from.y, to.y - from.y, d));
}

float Easing::easeFTBasic(easeMode mode, double t, double from, double to, double d, double a)
{
    t = t > d ? d : t;

    switch (mode)
    {
        case easeMode::easeIn:
            return (float)easeInBasic(t, from, to - from, d, a);

        case easeMode::easeOut:
            return (float)easeOutBasic(t, from, to - from, d, a);

        case easeMode::easeInOut:
            return (float)easeInOutBasic(t, from, to - from, d, a);

        default:
            return 0;
    }
}

Vector2 Easing::easeFTBasic(easeMode mode, double t, Vector2 from, Vector2 to, double d, double a)
{
    switch (mode)
    {
        case easeMode::easeIn:
            return Vector2((float)easeInBasic(t, from.x, to.x - from.x, d, a), (float)easeInBasic(t, from.y, to.y - from.y, d, a));

        case easeMode::easeOut:
            return Vector2((float)easeOutBasic(t, from.x, to.x - from.x, d, a), (float)easeInBasic(t, from.y, to.y - from.y, d, a));

        case easeMode::easeInOut:
            return Vector2((float)easeInOutBasic(t, from.x, to.x - from.x, d, a), (float)easeInBasic(t, from.y, to.y - from.y, d, a));

        default:
            return Vector2::zero;
    }
}

void Spring(float & x, float & v, float xt, float zeta, float omega, float h)
{
	const float f = 1.0f + 2.0f * h * zeta * omega;
	const float oo = omega * omega;
	const float hoo = h * oo;
	const float hhoo = h * hoo;
	const float detInv = 1.0f / (f + hhoo);
	const float detX = f * x + h * v + hhoo * xt;
	const float detV = v + hoo * (xt - x);
	x = detX * detInv;
	v = detV * detInv;
}

void Damp(float & x, float & v, float xt, float td, float pd, float f, float h)
{
	const float omega = 2 * M_PI * f;
	const float zeta = log(pd) / (-1 * omega * td);
	Spring(x, v, xt, zeta, omega, h);
}

}