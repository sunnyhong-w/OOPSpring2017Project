#pragma once
#include "gamebehavior.h"
using namespace game_engine;
class TextRenderer: public GameBehaviour
{
    public:
        TextRenderer(GameObject* gobj) : GameBehaviour(gobj) {}
        void ParseJSON(json j) override;
        void Start() override;
        void Update() override;
        void OnRecivedBoardcast(json j) override;
        void Stamp(wchar_t character, Vector2I pos);
        void Setfont(string fontName);
    private:
        SpriteRenderer* SR = nullptr;
        Vector2I size;
        map<wchar_t, Vector2I> charPos;
        json fontInfo;
};