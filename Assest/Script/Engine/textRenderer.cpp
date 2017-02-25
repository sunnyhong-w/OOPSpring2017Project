#include "StdAfx.h"
#include "Engine.h"
#include "textRenderer.h"
#include <fstream>
#include <codecvt>
using namespace std;
void TextRenderer::ParseJSON(json j)
{
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
    GameObject* goj = GameObject::findGameObjectByName("TextStamp");

    if (goj != nullptr)
        for (vector<wstring>::iterator it = line.begin(); it != line.end(); it++)
        {
            for (wstring::iterator c = it->begin(); c != it->end(); c++)
            {
                goj->GetComponent<TextStamp>()->Stamp(*c, stampPos - cameraPos);
                stampPos.x += 20;
            }

            stampPos.x = 0;
            stampPos.y += 20;
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
