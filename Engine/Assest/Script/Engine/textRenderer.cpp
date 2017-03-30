#include "StdAfx.h"
#include "Engine.h"
#include "textRenderer.h"
#include <fstream>
#include <codecvt>
using namespace std;
TextRenderer::~TextRenderer()
{
    for (TextStamp* tSObj : textStampObj)
        delete tSObj->gameObject;
}

void TextRenderer::ParseJSON(json j)
{
    this->setFont();
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
}

void TextRenderer::Start()
{
	frapsPerWord = defaultFraps;
}

void TextRenderer::Update()
{
	Vector2 pos = this->transform->GetPostion();

	for (wchar_t word : line)
	{
		json tStampJson = GameObject::GetPrefrabs(R"(Engine\TextRenderer\TextStamp)");
		GameObject* gobj = new GameObject();
		gobj->ParseJSON(tStampJson, true);
		if (gobj != nullptr && paragraph.size() != 0)
		{
			TextStamp* tsObj = gobj->GetComponent<TextStamp>();
			tsObj->Setfont(fontInfo["font_sample"]["SurfaceID"], fontInfo["font_sample"], charPos[word]);
			tsObj->setPosition(pos);
			textStampObj.push_back(tsObj);
			pos.x += 16;
		}
	}
	line.clear();
    if ((unsigned)textStampIndex < textStampObj.size())
    {
        textStampIndex++;
    }
	
    for (TextStamp* tobj : textStampObj)
    {
        tobj->Update();
    }
	
    Vector2I stampPos = this->transform->GetPostion().GetV2I();
   
	remainFraps--;
    if (remainFraps <= 0)
    {
        if ((unsigned)(drawIndex) < textStampObj.size())
            drawIndex++;

        remainFraps = frapsPerWord;
    }
}

void TextRenderer::OnRecivedBoardcast(BoardcastMessageData bmd)
{
}

void TextRenderer::Draw(Vector2I cameraPos)
{
    for (int i = 0; i < drawIndex; i++)
    {
        textStampObj[i]->Stamp();
    }
}

void TextRenderer::SetPosition(Vector2I pos)
{
    //this->transform->position = pos.GetV2();
}

void TextRenderer::LoadText(string fileName)
{
    wifstream file;
    file.imbue(locale(locale::empty(), new std::codecvt_utf8<wchar_t>));
    string filePath = R"(.\Assest\Scenario\)" + fileName + ".story";
    file.open(filePath);
    GAME_ASSERT(file, ("can't find" + fileName + ".story. \n#[Engine]TextRenderer::LoadText").c_str());
    wstring str;
    paragraph.clear();
    textStampObj.clear();
    //
   

    while (getline(file, str))
    {
        paragraph.push_back(str);
    }

    file.close();
	remainFraps = 0;
	line = paragraph[0];
}

void TextRenderer::NextLine()
{
	
	if ((unsigned)(drawIndex) < textStampObj.size()) 
	{
		frapsPerWord = 1;
		remainFraps = frapsPerWord;
	}
	else if(paragraphIndex + 1 < (int)paragraph.size())
	{
		paragraphIndex++;
		line = paragraph[paragraphIndex];
		frapsPerWord = defaultFraps;
		remainFraps = 0;
		for (TextStamp* tSObj : textStampObj)
			delete tSObj->gameObject;
		textStampObj.clear();
		drawIndex = 0;
	}
	else
	{
		paragraph.clear();
		paragraphIndex = 0;
		for (TextStamp* tSObj : textStampObj)
			delete tSObj->gameObject;
		textStampObj.clear();
		frapsPerWord = defaultFraps;
		remainFraps = 0;
		drawIndex = 0;
	}
}

void TextRenderer::setFont()
{

}