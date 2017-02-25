#pragma once
#include "gamebehavior.h"
using namespace game_engine;

class TextRenderer : public GameBehaviour
{
    public:
        TextRenderer(GameObject* gobj) : GameBehaviour(gobj) {}
        ~TextRenderer() {}
        void ParseJSON(json j) override;
        void Start() override;
        void Update() override;
        void OnRecivedBoardcast(json j) override;
        void Draw();
        void SetPosition(Vector2I pos);
        void LoadText(string fileName);
    private:
        vector<wstring> line;
};