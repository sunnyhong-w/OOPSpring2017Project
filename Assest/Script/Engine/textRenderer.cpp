#include "StdAfx.h"
#include "Engine.h"
#include "textRenderer.h"
#include <fstream>
#include <codecvt>
using namespace std;
void TextRenderer::ParseJSON(json j)
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

void TextRenderer::Start()
{
}

void TextRenderer::Update()
{
}

void TextRenderer::OnRecivedBoardcast(json j)
{
}

void TextRenderer::Draw(Vector2I cameraPos)
{
    Vector2I stampPos = this->transform->position.GetV2I();
    //TextStamp* tStamp = GameObject::findGameObjectByName("TextStamp")->GetComponent<TextStamp>();
    json tStampJson = GameObject::GetPrefrabs(R"(Engine\TextRenderer\TextStamp)");
    bool doNOTDestoryOnChangeScene = tStampJson.find("doNOTDestoryOnChangeScene") != tStampJson.end() ? tStampJson["doNOTDestoryOnChangeScene"] : false;
    bool isPureScript = tStampJson.find("isPureScript") != tStampJson.end() ? tStampJson["isPureScript"] : false;
    GameObject* tStampObj = new GameObject(doNOTDestoryOnChangeScene, isPureScript);
    tStampObj->ParseJSON(tStampJson);

    /*if (tStamp != nullptr && line.size() != 0)
    {
        wstring it = line[lineIndex];

        for (auto c : it)
        {
            tStamp->Stamp(c, stampPos - cameraPos);
            stampPos.x += tStamp->size.x;
        }
    }*/
    if (tStampObj != nullptr && line.size() != 0)
    {
        wstring it = line[lineIndex];
        tStampObj->GetComponent<TextStamp>()->Setfont(fontInfo["font_sample"]["SurfaceID"], fontInfo["font_sample"]);
        tStampObj->GetComponent<TextStamp>()->Stamp(it[0], stampPos - cameraPos);
    }
}

void TextRenderer::SetPosition(Vector2I pos)
{
    this->transform->position = pos.GetV2();
}

void TextRenderer::LoadText(string fileName)
{
    wifstream file;
    file.imbue(locale(locale::empty(), new std::codecvt_utf8<wchar_t>));
    string filePath = R"(.\Assest\Scenario\)" + fileName + ".story";
    file.open(filePath);
    GAME_ASSERT(file, ("can't find " + fileName + ".story. \n#[Engine]TextRenderer::LoadText").c_str());
    wstring str;
    line.clear();

    while (getline(file, str))
    {
        line.push_back(str);
    }

    file.close();
}

void TextRenderer::NextLine()
{
    if (lineIndex + 1 < (int)line.size())
        lineIndex++;
    else
    {
        line.clear();
        lineIndex = 0;
    }
}
