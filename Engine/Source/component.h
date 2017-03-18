/*  component.h: 本檔案儲存遊戲引擎所使用的component的interface和basic component
Copyright (C) 2017 Guo Xiang, Hong <sunnyhong.w@gmail.com>

基本上是把Unity那種組件式引擎移植到現在的game framework上面讓未來能夠加速開發的邏輯。
請不要把自己的Script也一起塞到這一堆裡面，這邊只放所有人共有的基礎類別。

HISTORY :
2017-02-09 v0.1.0
*/

#pragma once
#include"enginelib.h"
#include"gamelib.h"
#include"_setting.h"

namespace game_engine
{

class GameObject;
class Transform;

/////////////////////////////////////////////////////////////////////////////
// 所有Component的模板
/////////////////////////////////////////////////////////////////////////////

class Component
{
        friend class GameObject;

    public:
        Component(GameObject* gobj, bool skip = false);
        virtual ~Component() {};
        virtual void ParseJSON(json j) {};
        ///<summary>獲得skipTriverse的資料，確認這個Component能不能被Skip</summary>
        bool isBehavior();
        bool enable;
    protected:
        GameObject* gameObject;
        Transform* transform;
    private:
        ///<summary>在Scene處理Object Component Triverse的時候，跳過這個Component</summary>
        const bool isGameBehavior = false;
};


/////////////////////////////////////////////////////////////////////////////
// 儲存Gameobject位置用的component，沒有的話相關的東西不會運作
/////////////////////////////////////////////////////////////////////////////
class Transform : public Component
{
    public:
        Transform(GameObject* gobj, Vector2 pos = Vector2::zero, int z = 0, RenderDepth rd = RenderDepth::MAINGROUND);
        ~Transform() {};
        void ParseJSON(json j) override;
        void SetRenderDepth(int z);
        void SetRenderDepth(RenderDepth rd);
        void SetRenderDepth(int z, RenderDepth rd);
        int GetZCode();
        Vector2 position;
        Vector2 scale;


    private:
        int zindex;
        RenderDepth depth;
        int zcode;
};

/////////////////////////////////////////////////////////////////////////////
// 進行圖像繪製的Component
/////////////////////////////////////////////////////////////////////////////
class SpriteRenderer : public Component, private game_framework::CMovingBitmap
{
    public:
        SpriteRenderer(GameObject* gobj);
        ~SpriteRenderer() {};
        void ParseJSON(json j) override;
        void Draw(Vector2I cameraPos = Vector2I::zero);
        ///<summary>設定Sprite的圖源剪位置</summary>
        void SetSourcePos(Vector2I pos);
        ///<summary>取消Sprite的圖片裁剪功能</summary>
        void ResetSourcePos();
        ///<summary>設定Sprite的輸出大小</summary>
        void SetSize(Vector2I size);
        ///<summary>將Size重置成最後一次Load的圖片大小</summary>
        void ResetSize();
        ///<summary>取代原本的LoadBitmap機能，注意在讀檔之後會重置Size和SrcPos的參數</summary>
        ///<param name="filename">檔案目錄</param>
        void LoadBitmapData(string filename, bool unsafe = false, short r = 255, short g = 0, short b = 255);
        ///<summary>取得目前的SurfaceID</summary>
        int GetSurfaceID();
        ///<summary>設定已存在的SurfaceID，注意在設定之後會重置Size和SrcPos的參數</summary>
        ///<param name="SID">要設定的SurfaceID</param>
        void SetSurfaceID(int SID);
        void UnsafeSetSurfaceID(int SID);
        void Reset();
        void SetAnchorRaito(Vector2 pos);
        void SetOffset(Vector2I offset);
        Vector2I GetAnchorPoint();

    private:
        Vector2I size;
        Vector2I srcpos;
        Vector2I offset;
        Vector2 anchorRaito;

        bool cutSrc = false;
        static map<string, unsigned int> fileInfo;
};

struct CollisionLayer
{
	CollisionLayer();
	Layer layer;
	bool block;
};

void from_json(const json &j, CollisionLayer &cl);

class Collider : public Component
{
    public:
        Collider(GameObject* gobj, Vector2I dP = Vector2I::zero, Vector2I sz = Vector2I::zero);
		void OnDrawGismos();
        bool PointCollision(Vector2I point);
		bool BoxCollision(Collider* box, Vector2 &velocityOffse, bool block = false);
        void ParseJSON(json j) override;
		CollisionInfo collisionInfo;
		vector<CollisionLayer> collisionLayer;
};

class Animation : public Component
{
public:
    Animation(GameObject* gobj) : Component(gobj) {};
    void LoadAnimation(json jsonobj);
    void ParseJSON(json j) override;
    void Update();
    void ResetAnimation();
private:
    int animateCount;
    int duration;
    DWORD timeStamp;
    json animationInfo;
    SpriteRenderer *SR;
};

class AnimationController : public Component
{
public:
    AnimationController(GameObject* gobj) : Component(gobj) {};
    void ParseJSON(json j) override;
    void Update();
	void JumpState(string state);
    json varibleMap;
private:
	bool CheckCondition(json j);
	string jumpState = "";
	json data;
	Animation* animation;
};

class Rigidbody : public Component
{
public:
	Rigidbody(GameObject* gobj) : Component(gobj) {};
	Vector2 velocity;
	void ParseJSON(json j) override;
	void Update();
private:

};

}