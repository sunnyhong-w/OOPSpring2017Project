#pragma once
#include "gamebehavior.h"
#include "textStamp.h"
using namespace game_engine;

class TextRenderer : public GameBehaviour
{
    public:
        TextRenderer(GameObject* gobj) : GameBehaviour(gobj), paragraphIndex(0), textStampIndex(0),defaultFraps(5) {}
        ~TextRenderer();
        void ParseJSON(json j) override;
        void Start() override;
        void Update() override;
		void OnRecivedBroadcast(BoardcastMessageData bmd) override;
        void Draw(Vector2I cameraPos) override;
        void SetPosition(Vector2I pos = Vector2I::zero);
        void LoadText(string fileName);
        void NextLine();
        void setFont();
    private:
		const int defaultFraps;
        SpriteRenderer* SR = nullptr;
        vector<wstring> paragraph;
        vector<TextStamp*> textStampObj;
        map<wchar_t, Vector2I>charPos;
		wstring line;
        int paragraphIndex = 0;
        int textStampIndex = 0;
        int frapsPerWord;
        int remainFraps;
        int drawIndex = 0;
        json fontInfo;
};