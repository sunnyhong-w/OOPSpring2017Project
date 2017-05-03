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
        friend void Destroy(GameObject &gobj);
    public:
        Component(GameObject* gobj, bool skip = false);
        virtual ~Component() {};
        virtual void ParseJSON(json j) {};
        ///<summary>獲得skipTriverse的資料，確認這個Component能不能被Skip</summary>
        bool isBehavior();
        void SetEnable(bool enable);
        bool GetEnable();
        GameObject* gameObject;
        Transform* transform;
    private:
        ///<summary>在Scene處理Object Component Triverse的時候，跳過這個Component</summary>
        const bool isGameBehavior = false;
        bool enable;
};


/////////////////////////////////////////////////////////////////////////////
// 儲存Gameobject位置用的component，沒有的話相關的東西不會運作
/////////////////////////////////////////////////////////////////////////////
class Transform : public Component
{
    public:
        Transform(GameObject* gobj, Vector2 pos = Vector2::zero, int z = 0);
        ~Transform() {};
        void ParseJSON(json j) override;
        int  GetSortingLayer();
        int  GetZIndex();
        void SetZIndex(int z);
        int  GetWorldZIndex();
        void SetWorldZIndex(int z);
        void UpdateWorldZIndex();
        Vector2 GetPostion();
        void SetPosition(Vector2 newpos);
        Vector2 GetWorldPosition();
        void SetWorldPosition(Vector2 newpos);
        void UpdateWorldPosition();
        void Translate(Vector2 dpos);
        void Translate(float dx,float dy);

        Vector2 scale;

		void SetParent(Transform* target);
		void SetParentAbsolute(Transform* target);
        Transform* GetParent();
		vector<Transform*> GetChild();

    private:
        int zindex;
        int worldzindex;
        Vector2 position;
        Vector2 worldposition;

		Transform *parent;
		vector<Transform*> child;
        void AddChild(Transform* target);
        void RemoveChild(Transform* target);
};

/////////////////////////////////////////////////////////////////////////////
// 進行圖像繪製的Component
/////////////////////////////////////////////////////////////////////////////
class SpriteRenderer : public Component, private game_framework::CMovingBitmap
{
    public:
		friend class GameScene;

        SpriteRenderer(GameObject* gobj);
        ~SpriteRenderer();
        void ParseJSON(json j) override;
        void Draw(Vector2I cameraPos = Vector2I::zero);
        ///<summary>設定Sprite的圖源剪位置</summary>
        void SetSourcePos(Vector2I pos);
        ///<summary>取消Sprite的圖片裁剪功能</summary>
        void ResetSourcePos();
        ///<summary>設定Sprite的輸出大小</summary>
        Vector2I GetSourcePos();
        void SetSourceData(Rect src);
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
		Vector2I GetOffset();
        Vector2I GetAnchorPoint();
        SortingLayer GetSortingLayer();
        void SetSortingLayer(SortingLayer SL);
        inline Vector2I GetRealRenderPostion();
        inline void UpdateRealRenderPostion();
    private:
        bool CameraTest(Vector2I cameraPos);

        Vector2I size;
        Vector2I srcpos;
        Vector2I offset;
        Vector2 anchorRaito;
        Vector2I realRenderPosition;
        SortingLayer sortingLayer;
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
	friend class Rigidbody;
    public:
        Collider(GameObject* gobj, Vector2I dP = Vector2I::zero, Vector2I sz = Vector2I::zero);
        ~Collider();
        void OnDrawGismos(CDC *pDC, Vector2I cameraPos);
        bool PointCollision(Vector2I point);
		bool BoxCollision(Collider* box, Vector2 &velocityOffse, bool block = false);
		void Update();
        void ParseJSON(json j) override;
		CollisionInfo collisionInfo;
		vector<CollisionLayer> collisionLayer;
	private:
		set<Collider*> collidedCollider;
        set<Collider*> lastCollidedCollder;

        vector<Collider*> OnEnter;
        vector<Collider*> OnStay;
};

enum class AnimationPlaytype : int { Forward, Reverse, Pingpong };

struct AnimationData {
    vector<AnimationSetting> frameList;
    AnimationPlaytype playtype;
};

class AnimationController : public Component
{
public:
    AnimationController(GameObject* gobj);
    ~AnimationController();
    void ParseJSON(json j) override;
    void Update();
    bool JumpState(string state);
    bool JumpState(int state);
    void PlayOneShot(string state);
    void PlayOneShot(int state);
private:
    void ParseAespriteJSON(json j);

    string jumpState = "";
    map<string, AnimationData> animationData;
    vector<string> animationList;
    vector<AnimationSetting> frames;
};

class Animation : public Component
{
public:
    friend class AnimationController;

    Animation(GameObject* gobj);
    ~Animation();
	void NextFrame();
    void SetFrame(int i);
    void Update();
    void ResetAnimation();
    void SetAnimationPlaytype(AnimationPlaytype ap);

    bool startAtReminder;

private:
    void LoadAnimation(AnimationData newAnim);
    void PlayOneShot(AnimationData oneShot);

	int animateFrame;
    int animateCount;
    int duration;
    int frameRemider;
    DWORD timeStamp;
    AnimationData animationData;
    AnimationData animationOneShot;
};

struct ColliderInfo
{
    bool top, bottom, left, right;
	string toString();
    void Reset();
};

class Rigidbody : public Component
{
public:
	Rigidbody(GameObject* gobj);
    ~Rigidbody();
	Vector2 velocity;
    ColliderInfo colliderInfo;
    bool TimeSliceCollision = false;
	void ParseJSON(json j) override;
	void Update();
private:
    typedef pair<multimap<Layer, GameObject*>::iterator, multimap<Layer, GameObject*>::iterator> LayerPair;

    void OnCollision(Collider *tgcollider);
    bool DoCollision(Collider *collider, set<GameObject*>& gobjset, Vector2 &tempVelocity, bool block, bool resetVX = false);
    void CollisionDetection(Vector2& invelocity);
    void CollisionDetectionSlice(Vector2& invelocity);
};

}