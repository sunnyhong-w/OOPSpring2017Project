#pragma once
#include"json_include.h"
namespace game_engine
{
///////////////////////////////////
//      Object Layer define      //
///////////////////////////////////
enum class Layer : int { Default, Tile, Button };
///////////////////////////////////
//      Object  Tag  define      //
///////////////////////////////////
enum class Tag : int { Untagged, Tile };
///////////////////////////////////////////////
//    Sprite Renderer Render Order define    //
//  The Layer in front will be render first  //
///////////////////////////////////////////////
enum class SortingLayer : int { Default, Player };

////////////////////////////
//    FromJSON Declear    //
////////////////////////////
void from_json(const json &j, Layer &l);
void from_json(const json &j, Tag &t);
void from_json(const json &j, SortingLayer &l);
}