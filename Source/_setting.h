#pragma once
#include"json_include.h"
namespace game_engine
{
///////////////////////////////////
//      Object Layer define      //
///////////////////////////////////
enum class Layer : int { Other };
///////////////////////////////////
//      Object  Tag  define      //
///////////////////////////////////
enum class Tag : int { Other };
///////////////////////////////////
//Render Z-index position define //
///////////////////////////////////
enum class RenderDepth : int
{
    BACKGROUND = -1000,
    MAINGROUND = 0,
    FOREGROUND = 1000,
    GUI = 10000
};
}