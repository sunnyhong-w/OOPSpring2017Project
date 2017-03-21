#pragma once

#include"gamelib.h"

namespace game_engine
{
    class GameObject;

class GameScene : public game_framework::CGameState
{
        friend class game_framework::CGame;
    public:
        GameScene(game_framework::CGame* CG, string loadname = "Main", bool lock = false) : game_framework::CGameState(CG)
        {
            this->loadname = loadname;
            this->lock = lock;
        };
        void OnInit() override {};								// 狀態的初值及圖形設定
        void OnBeginState() override;							// 設定每次進入這個狀態時所需的初值
        void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) override;				// 處理鍵盤Down的動作
        void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) override;				// 處理鍵盤Up的動作
        void OnLButtonDown(UINT nFlags, CPoint point) override;// 處理滑鼠的動作
        void OnLButtonUp(UINT nFlags, CPoint point) override;	// 處理滑鼠的動作
        void OnMouseMove(UINT nFlags, CPoint point) override;  // 處理滑鼠的動作
        void OnRButtonDown(UINT nFlags, CPoint point) override;// 處理滑鼠的動作
        void OnRButtonUp(UINT nFlags, CPoint point) override;	// 處理滑鼠的動作
        void OnCopyData(json j) override;          // 處理視窗間的資料傳遞
        void OnMove() override;
        void OnShow() override;
        void ParseJSON(json j);
        void LoadScene(string filename);

        bool lock = false;
        
        static Vector2I& CameraPosition();
        static GameScene* NowScene();
    private:
        void LoadSceneData();
        vector<GameObject*> InstantiateGameObject(json objArray, map<string, string> prefrabmap);
        GameObject* CreateGameObject(json jsonobj, map<string, string> prefrabmap);
        void IncludePrefrabs(json prefrabObject, map<string, string> &prefrabmap);
        void ReadPrefrab(string filename, string includename, map<string, string> &prefrabmap);

        vector<json> TDPQueue;
        string filename;
        string loadname;
        Vector2I cameraPosition;

};
}

