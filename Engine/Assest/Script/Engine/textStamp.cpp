#include"StdAfx.h"
#include"Engine.h"
#include "textStamp.h"
#include<fstream>
#include <codecvt>
using namespace std;

void TextStamp::ParseJSON(json j)
{
    SR = this->gameObject->AddComponentOnce<SpriteRenderer>();

    if (j.find("fontsetting") != j.end())
    {
        for (json::iterator it = j["fontsetting"].begin(); it != j["fontsetting"].end(); it++)
        {
            fontInfo[it.key()] = it.value();
            SR->LoadBitmapData(R"(Font\)" + it.key());
            fontInfo[it.key()]["SurfaceID"] = SR->GetSurfaceID();
        }
    }
}

void TextStamp::Start()
{
    wifstream data;
    data.imbue(locale(locale::empty(), new std::codecvt_utf8<wchar_t>));
    data.open(R"(.\Assest\Prefrab\Engine\TextRenderer\font_sample_data.txt)");
    wstring str;
    Vector2I pos = Vector2I::zero;

    while (!data.eof())
    {
        data >> str;

        for (wstring::iterator it = str.begin(); it != str.end(); it++)
        {
            if (*it != '\n')
            {
                charPos[*it] = pos;
                pos.x++;
            }
        }

        pos.y++;
        pos.x = 0;
    }

    data.close();
    Setfont("font_sample");
}

void TextStamp::Update()
{
}

void TextStamp::OnRecivedBoardcast(json j)
{
}

void TextStamp::Stamp(wchar_t character, Vector2I pos)
{
    this->transform->position = pos.GetV2();
    SR->SetSourcePos(charPos[character] * size);
    SR->Draw();
}

void TextStamp::Setfont(string fontName)
{
    if (fontInfo.find(fontName) != fontInfo.end())
    {
        SR->SetSurfaceID(fontInfo[fontName]["SurfaceID"]);
        SR->SetSize(fontInfo[fontName]);
        size = fontInfo[fontName];
    }
}
