#include"StdAfx.h"
#include"_setting.h"

namespace game_engine {

void from_json(const json &j, Layer &l)
{
    l = (Layer)j.get<int>();
}

void from_json(const json &j, Tag &t)
{
    t = (Tag)j.get<int>();
}

void from_json(const json &j, SortingLayer &rd)
{
    rd = (SortingLayer)j.get<int>();
}

}