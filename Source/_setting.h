#pragma once

namespace game_engine
{
///////////////////////////////////
//      Object Layer define      //
///////////////////////////////////
enum Layer {};
///////////////////////////////////
//Render Z-index position define //
///////////////////////////////////
enum RenderDepth
{
    BACKGROUND = -1000,
    MAINGROUND = 0,
    FOREGROUND = 1000,
    GUI = 10000
};
}