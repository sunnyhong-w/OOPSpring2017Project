#include"StdAfx.h"
#include"_setting.h"

namespace game_engine {

void from_json(json &j, Layer &l)
{
    l = Layer::Other;
}

void from_json(json &j, Tag &t)
{
    t = Tag::Other;
}

void from_json(json &j, RenderDepth &rd)
{
    if (j.get<string>() == "BACKGROUND")
        rd = RenderDepth::BACKGROUND;
    else if (j == "FOREGROUND")
        rd = RenderDepth::FOREGROUND;
    else if (j == "MAINGROUND")
        rd = RenderDepth::MAINGROUND;
    else if (j == "GUI")
        rd = RenderDepth::GUI;
    else
        rd = RenderDepth::MAINGROUND;
}

}