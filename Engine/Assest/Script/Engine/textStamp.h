#pragma once
#include "gamebehavior.h"
using namespace game_engine;
class TextStamp: public GameBehaviour
{
        friend class TextRenderer;
    public:
        TextStamp(GameObject* gobj) : GameBehaviour(gobj) {}
        ~TextStamp() {}
        void ParseJSON(json j) override;
        void Start() override;
        void Update() override;
		void OnRecivedBroadcast(BroadcastMessageData bmd) override;
        void Stamp();
        void Setfont(UINT surfaceID, Vector2I size, Vector2I bitmapPos);
        void shake();
        void randomShake();
        void setPosition(Vector2 stampPos);
    private:

        SpriteRenderer* SR = nullptr;
        Vector2I size;
        Vector2I stampPos;
        Vector2I bitmapPos;
        map<wchar_t, Vector2I> charPos;
        json fontInfo;
        wchar_t character;
        int dp = 0;
};