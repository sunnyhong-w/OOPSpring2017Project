#define _USE_MATH_DEFINES
#include <cmath>
#include "enginelib.h"

namespace game_engine
{

class Easing
{
        //t: current time
        //b: beginning value
        //c: changing value
        //d: duration

        static double _easeBasic(double a, double dt)
        {
            a = a + 1;
            return pow(dt, a) / (pow(dt, a) + pow(1 - dt, a));
        }

        static double easeInBasic(
            double t, double b, double c, double d, double a)
        {
            return c * _easeBasic(a, (t / d) / 2) / 0.5 + b;
        }

        static double easeOutBasic(
            double t, double b, double c, double d, double a)
        {
            return c * (_easeBasic(a, 0.5 + (t / d) / 2) - 0.5) / 0.5 + b;
        }

        static double easeInOutBasic(
            double t, double b, double c, double d, double a)
        {
            return c * _easeBasic(a, t / d) + b;
        }

        static double linear(
            double t, double b, double c, double d)
        {
            return c * t / d + b;
        }

        static double easeInQuad(
            double t, double b, double c, double d)
        {
            return c * (t /= d) * t + b;
        }

        static double easeOutQuad(
            double t, double b, double c, double d)
        {
            return -c * (t /= d) * (t - 2) + b;
        }

        static double easeInOutQuad(
            double t, double b, double c, double d)
        {
            if ((t /= d / 2) < 1) return c / 2 * t * t + b;

            return -c / 2 * ((--t) * (t - 2) - 1) + b;
        }

        static double easeInCubic(
            double t, double b, double c, double d)
        {
            return c * (t /= d) * t * t + b;
        }

        static double easeOutCubic(
            double t, double b, double c, double d)
        {
            return c * ((t = t / d - 1) * t * t + 1) + b;
        }

        static double easeInOutCubic(
            double t, double b, double c, double d)
        {
            if ((t /= d / 2) < 1) return c / 2 * t * t * t + b;

            return c / 2 * ((t -= 2) * t * t + 2) + b;
        }

        static double easeInQuart(
            double t, double b, double c, double d)
        {
            return c * (t /= d) * t * t * t + b;
        }

        static double easeOutQuart(
            double t, double b, double c, double d)
        {
            return -c * ((t = t / d - 1) * t * t * t - 1) + b;
        }

        static double easeInOutQuart(
            double t, double b, double c, double d)
        {
            if ((t /= d / 2) < 1) return c / 2 * t * t * t * t + b;

            return -c / 2 * ((t -= 2) * t * t * t - 2) + b;
        }

        static double easeInQuint(
            double t, double b, double c, double d)
        {
            return c * (t /= d) * t * t * t * t + b;
        }

        static double easeOutQuint(
            double t, double b, double c, double d)
        {
            return c * ((t = t / d - 1) * t * t * t * t + 1) + b;
        }

        static double easeInOutQuint(
            double t, double b, double c, double d)
        {
            if ((t /= d / 2) < 1) return c / 2 * t * t * t * t * t + b;

            return c / 2 * ((t -= 2) * t * t * t * t + 2) + b;
        }

        static double easeInSine(
            double t, double b, double c, double d)
        {
            return -c * cos(t / d * M_PI_2) + c + b;
        }

        static double easeOutSine(
            double t, double b, double c, double d)
        {
            return c * sin(t / d * M_PI_2) + b;
        }

        static double easeInOutSine(
            double t, double b, double c, double d)
        {
            return -c / 2 * (cos(M_PI * t / d) - 1) + b;
        }

        static double easeInExpo(
            double t, double b, double c, double d)
        {
            return (t == 0) ? b : c * pow(2, 10 * (t / d - 1)) + b;
        }

        static double easeOutExpo(
            double t, double b, double c, double d)
        {
            return (t == d) ? b + c : c * (-pow(2, -10 * t / d) + 1) + b;
        }

        static double easeInOutExpo(
            double t, double b, double c, double d)
        {
            if (t == 0) return b;

            if (t == d) return b + c;

            if ((t /= d / 2) < 1) return c / 2 * pow(2, 10 * (t - 1)) + b;

            return c / 2 * (-pow(2, -10 * --t) + 2) + b;
        }

        static double easeInCirc(
            double t, double b, double c, double d)
        {
            return -c * (sqrt(1 - (t /= d) * t) - 1) + b;
        }

        static double easeOutCirc(
            double t, double b, double c, double d)
        {
            return c * sqrt(1 - (t = t / d - 1) * t) + b;
        }

        static double easeInOutCirc(
            double t, double b, double c, double d)
        {
            if ((t /= d / 2) < 1) return -c / 2 * (sqrt(1 - t * t) - 1) + b;

            return c / 2 * (sqrt(1 - (t -= 2) * t) + 1) + b;
        }

        static double easeInElastic(
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

        static double easeOutElastic(
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

        static double easeInOutElastic(
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

        static double easeInBack(
            double t, double b, double c, double d, double s = NAN)
        {
            double s2 = isnan(s) ? (s2 = 1.70158) : (double)s;
            return c * (t /= d) * t * ((s2 + 1) * t - s2) + b;
        }

        static double easeOutBack(
            double t, double b, double c, double d, double s = NAN)
        {
            double s2 = isnan(s) ? (s2 = 1.70158) : (double)s;
            return c * ((t = t / d - 1) * t * ((s2 + 1) * t + s2) + 1) + b;
        }

        static double easeInOutBack(
            double t, double b, double c, double d, double s = NAN)
        {
            double s2 = isnan(s) ? (s2 = 1.70158) : (double)s;

            if ((t /= d / 2) < 1) return c / 2 * (t * t * (((s2 *= (1.525)) + 1) * t - s2)) + b;

            return c / 2 * ((t -= 2) * t * (((s2 *= (1.525)) + 1) * t + s2) + 2) + b;
        }

        static double easeInOutBack_Fix(
            double t, double b, double c, double d)
        {
            double s2 = 1.70158;

            if ((t /= d / 2) < 1) return c / 2 * (t * t * (((s2 *= (1.525)) + 1) * t - s2)) + b;

            return c / 2 * ((t -= 2) * t * (((s2 *= (1.525)) + 1) * t + s2) + 2) + b;
        }

        static double easeOutBounce(
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

        static double easeInBounce(
            double t, double b, double c, double d)
        {
            return c - Easing::easeOutBounce(d - t, 0, c, d) + b;
        }

        static double easeInOutBounce(
            double t, double b, double c, double d)
        {
            if (t < d / 2) return Easing::easeInBounce(t * 2, 0, c, d) * .5 + b;

            return Easing::easeOutBounce(t * 2 - d, 0, c, d) * .5 + c * .5 + b;
        }

        /// <summary>
        /// 正規的Easing Function
        /// </summary>
        /// <param name="func">放入想要執行的easeing Function</param>
        /// <param name="t">現在時間</param>
        /// <param name="b">起始點</param>
        /// <param name="c">位移</param>
        /// <param name="d">總時間</param>
        /// <returns></returns>
        static float easer(double func(double, double, double, double), double t, double b, double c, double d)
        {
            t = t > d ? d : t;
            return (float)func(t, b, c, d);
        }

        /// <summary>
        /// 正規的Easing Function
        /// </summary>
        /// <param name="func">放入想要執行的easeing Function</param>
        /// <param name="t">現在時間</param>
        /// <param name="b">起始點</param>
        /// <param name="c">位移</param>
        /// <param name="d">總時間</param>
        /// <returns></returns>
        static Vector2 easer(double (*func)(double, double, double, double), double t, Vector2 b, Vector2 c, double d)
        {
            return Vector2((float)(*func)(t, b.x, c.x, d), (float)(*func)(t, b.x, c.x, d));
        }

        /// <summary>
        /// 從A點到B點的Easing Function
        /// </summary>
        /// <param name="func">放入想要執行的easeing Function</param>
        /// <param name="t">現在時間</param>
        /// <param name="from">起始點</param>
        /// <param name="to">終點</param>
        /// <param name="d">總時間</param>
        /// <returns></returns>
        static float easeFT(double (*func)(double, double, double, double), double t, double from, double to, double d)
        {
            t = t > d ? d : t;
            return (float)(*func)(t, from, to - from, d);
        }

        /// <summary>
        /// 從A點到B點的Easing Function
        /// </summary>
        /// <param name="func">放入想要執行的easeing Function</param>
        /// <param name="t">現在時間</param>
        /// <param name="from">起始點</param>
        /// <param name="to">終點</param>
        /// <param name="d">總時間</param>
        /// <returns></returns>
        static Vector2 easeFT(double (*func)(double, double, double, double), double t, Vector2 from, Vector2 to, double d)
        {
            return Vector2((float)(*func)(t, from.x, to.x - from.x, d), (float)(*func)(t, from.y, to.y - from.y, d));
        }

        enum easeMode { easeIn, easeOut, easeInOut };

        /// <summary>
        /// 從A點到B點的Easing Function
        /// </summary>
        /// <param name="mode【">放入In , Out 或是 InOut的模式</param>
        /// <param name="t">現在時間</param>
        /// <param name="from">起始點</param>
        /// <param name="to">終點</param>
        /// <param name="d">總時間</param>
        /// <param name="a">柔度</param>
        /// <returns></returns>
        static float easeFTBasic(easeMode mode, double t, double from, double to, double d, double a)
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

        /// <summary>
        /// 從A點到B點的Easing Function
        /// </summary>
        /// <param name="mode【">放入In , Out 或是 InOut的模式</param>
        /// <param name="t">現在時間</param>
        /// <param name="from">起始點</param>
        /// <param name="to">終點</param>
        /// <param name="d">總時間</param>
        /// <param name="a">柔度</param>
        /// <returns></returns>
        Vector2 easeFTBasic(easeMode mode, double t, Vector2 from, Vector2 to, double d, double a)
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
};

}