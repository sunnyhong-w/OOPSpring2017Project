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

void TextRenderer::Draw()
{
}

void TextRenderer::SetPosition(Vector2I pos)
{
    this->transform->position = pos.GetV2();
}

void TextRenderer::LoadText(string fileName)
{
    wifstream file;
    string filePath = R"(.\Assest\Scenario)" + fileName + ".story";
    file.open(filePath);

    if (file)
    {
        wstring str;

        while (getline(file, str))
        {
            line.push_back(str);
        }
    }

    file.close();
}
