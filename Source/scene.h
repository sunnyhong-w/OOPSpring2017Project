#pragma once

#include"gamelib.h"

namespace game_engine
{
class GameScene : public game_framework::CGameState
{
    public:
        GameScene(game_framework::CGame* CG): game_framework::CGameState(CG){};
        void OnInit() override {};								// 狀態的初值及圖形設定
        void OnBeginState() override;							// 設定每次進入這個狀態時所需的初值
        void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) override;				// 處理鍵盤Down的動作
        void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) override;				// 處理鍵盤Up的動作
        void OnLButtonDown(UINT nFlags, CPoint point) override;// 處理滑鼠的動作
        void OnLButtonUp(UINT nFlags, CPoint point) override;	// 處理滑鼠的動作
        void OnMouseMove(UINT nFlags, CPoint point) override;  // 處理滑鼠的動作
        void OnRButtonDown(UINT nFlags, CPoint point) override;// 處理滑鼠的動作
        void OnRButtonUp(UINT nFlags, CPoint point) override;	// 處理滑鼠的動作
        void OnCopyData(game_framework::TransferData* TDP) override;          // 處理視窗間的資料傳遞
        void OnMove() override;
        void OnShow() override;
        void ParseJSON(json j);
        void LoadScene(string filename);
    private:
        void LoadSceneData();
        vector<game_framework::TransferData> TDPQueue;
        string filename;
        string loadname = "Main";
};
}

