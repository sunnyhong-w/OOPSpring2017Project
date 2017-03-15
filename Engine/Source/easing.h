#pragma once
#include <cmath>

namespace game_engine
{

struct Vector2;

class Easing
{
        //t: current time
        //b: beginning value
        //c: changing value
        //d: duration
    public:
        static double _easeBasic(double a, double dt);
        static double easeInBasic(double t, double b, double c, double d, double a);
        static double easeOutBasic(double t, double b, double c, double d, double a);
        static double easeInOutBasic(double t, double b, double c, double d, double a);
        static double linear(double t, double b, double c, double d);
        static double easeInQuad(double t, double b, double c, double d);
        static double easeOutQuad(double t, double b, double c, double d);
        static double easeInOutQuad(double t, double b, double c, double d);
        static double easeInCubic(double t, double b, double c, double d);
        static double easeOutCubic(double t, double b, double c, double d);
        static double easeInOutCubic(double t, double b, double c, double d);
        static double easeInQuart(double t, double b, double c, double d);
        static double easeOutQuart(double t, double b, double c, double d);
        static double easeInOutQuart(double t, double b, double c, double d);
        static double easeInQuint(double t, double b, double c, double d);
        static double easeOutQuint(double t, double b, double c, double d);
        static double easeInOutQuint(double t, double b, double c, double d);
        static double easeInSine(double t, double b, double c, double d);
        static double easeOutSine(double t, double b, double c, double d);
        static double easeInOutSine(double t, double b, double c, double d);
        static double easeInExpo(double t, double b, double c, double d);
        static double easeOutExpo(double t, double b, double c, double d);
        static double easeInOutExpo(double t, double b, double c, double d);
        static double easeInCirc(double t, double b, double c, double d);
        static double easeOutCirc(double t, double b, double c, double d);
        static double easeInOutCirc(double t, double b, double c, double d);
        static double easeInElastic(double t, double b, double c, double d);
        static double easeOutElastic(double t, double b, double c, double d);
        static double easeInOutElastic(double t, double b, double c, double d);
        static double easeInBack(double t, double b, double c, double d, double s = NAN);
        static double easeOutBack(double t, double b, double c, double d, double s = NAN);
        static double easeInOutBack(double t, double b, double c, double d, double s = NAN);
        static double easeInOutBack_Fix(double t, double b, double c, double d);
        static double easeOutBounce(double t, double b, double c, double d);
        static double easeInBounce(double t, double b, double c, double d);
        static double easeInOutBounce(double t, double b, double c, double d);
        /// <summary>
        /// 正規的Easing Function
        /// </summary>
        /// <param name="func">放入想要執行的easeing Function</param>
        /// <param name="t">現在時間</param>
        /// <param name="b">起始點</param>
        /// <param name="c">位移</param>
        /// <param name="d">總時間</param>
        /// <returns></returns>
        static float easer(double func(double, double, double, double), double t, double b, double c, double d);
        /// <summary>
        /// 正規的Easing Function
        /// </summary>
        /// <param name="func">放入想要執行的easeing Function</param>
        /// <param name="t">現在時間</param>
        /// <param name="b">起始點</param>
        /// <param name="c">位移</param>
        /// <param name="d">總時間</param>
        /// <returns></returns>
        static Vector2 easer(double(*func)(double, double, double, double), double t, Vector2 b, Vector2 c, double d);
        /// <summary>
        /// 從A點到B點的Easing Function
        /// </summary>
        /// <param name="func">放入想要執行的easeing Function</param>
        /// <param name="t">現在時間</param>
        /// <param name="from">起始點</param>
        /// <param name="to">終點</param>
        /// <param name="d">總時間</param>
        /// <returns></returns>
        static float easeFT(double(*func)(double, double, double, double), double t, double from, double to, double d);
        /// <summary>
        /// 從A點到B點的Easing Function
        /// </summary>
        /// <param name="func">放入想要執行的easeing Function</param>
        /// <param name="t">現在時間</param>
        /// <param name="from">起始點</param>
        /// <param name="to">終點</param>
        /// <param name="d">總時間</param>
        /// <returns></returns>
        static Vector2 easeFT(double(*func)(double, double, double, double), double t, Vector2 from, Vector2 to, double d);
        enum easeMode { easeIn, easeOut, easeInOut };
        /// <summary>
        /// 從A點到B點的Easing Function
        /// </summary>
        /// <param name="mode">放入In , Out 或是 InOut的模式</param>
        /// <param name="t">現在時間</param>
        /// <param name="from">起始點</param>
        /// <param name="to">終點</param>
        /// <param name="d">總時間</param>
        /// <param name="a">柔度</param>
        /// <returns></returns>
        static float easeFTBasic(easeMode mode, double t, double from, double to, double d, double a);
        /// <summary>
        /// 從A點到B點的Easing Function
        /// </summary>
        /// <param name="mode">放入In , Out 或是 InOut的模式</param>
        /// <param name="t">現在時間</param>
        /// <param name="from">起始點</param>
        /// <param name="to">終點</param>
        /// <param name="d">總時間</param>
        /// <param name="a">柔度</param>
        /// <returns></returns>
        Vector2 easeFTBasic(easeMode mode, double t, Vector2 from, Vector2 to, double d, double a);
};

//Spring Function By AllenChou(CJCat)
/// <summary>
/// 阻尼平滑系統
/// </summary>
/// <param name="x">value</param>
/// <param name="v">velocity</param>
/// <param name="xt">target value</param>
/// <param name="zeta">damping ratio</param>
/// <param name="omega">angular frequency</param>
/// <param name="h">time step</param>
/// <returns></returns>
inline void Spring(float &x, float &v, float xt, float zeta, float omega, float h = Time::deltaTime);

/// <summary>
/// 阻尼平滑系統(設計者友好)
/// </summary>
/// <param name="x">value</param>
/// <param name="v">velocity</param>
/// <param name="xt">target value</param>
/// <param name="td">specific duration</param>
/// <param name="pd">fraction of oscillation magnitude reduced </param>
/// <param name="f">oscillation frequency</param>
/// <param name="h">time step</param>
/// <returns></returns>
void Damp(float &x, float &v, float xt, float td, float pd, float f, float h = Time::deltaTime);

}