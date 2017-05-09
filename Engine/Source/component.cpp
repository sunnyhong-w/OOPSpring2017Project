/*  component.h: 本檔案儲存遊戲引擎所使用的component的interface和basic component的實作
Copyright (C) 2017 Guo Xiang, Hong <sunnyhong.w@gmail.com>

基本上是把Unity那種組件式引擎移植到現在的game framework上面讓未來能夠加速開發的邏輯。
請不要把自己的Script也一起塞到這一堆裡面，這邊只放所有人共有的基礎類別。

HISTORY :
2017-02-09 v0.1.0
*/

#include"stdafx.h"
#include <ddraw.h>
#include"component.h"
#include"gameobject.h"
#include"enginelib.h"
#include "gamebehavior.h"
#include<fstream>

namespace game_engine
{

//////////////////////////////////////////////////////////////////
// Component實作
//////////////////////////////////////////////////////////////////
Component::Component(GameObject* gobj, bool isGB) : isGameBehavior(isGB) , enable(true)
{
    this->gameObject = gobj;
    this->transform = gobj->transform;
}

bool Component::isBehavior()
{
    return this->isGameBehavior;
}

void Component::SetEnable(bool enable)
{
    if (this->enable ^ enable)
    {
        this->enable = enable;

        if (this->isGameBehavior)
        {
            if (enable)
                static_cast<GameBehaviour*>(this)->Awake();
            else
                static_cast<GameBehaviour*>(this)->Sleep();
        }
    }
}

bool Component::GetEnable()
{
    return this->enable;
}

//////////////////////////////////////////////////////////////////
// Transform實作
//////////////////////////////////////////////////////////////////
Transform::Transform(GameObject* gobj, Vector2 v2, int z) : Component(gobj)
{
    this->scale = Vector2::one;
    this->worldzindex = 0;
    this->zindex = z;
    this->parent = nullptr;
    this->worldposition = Vector2::zero;
    this->SetPosition(v2);
    this->transform = this;
}

void Transform::SetZIndex(int z)
{
    this->zindex = z;
    this->worldzindex = ((this->parent != nullptr) ? z + this->parent->worldzindex : z);
    UpdateWorldZIndex();
}

int Transform::GetWorldZIndex()
{
    return worldzindex;
}

void Transform::SetWorldZIndex(int z)
{
    this->worldzindex = z;
    this->zindex = ((this->parent != nullptr) ? z - this->parent->worldzindex : z);
    UpdateWorldZIndex();
}

void Transform::UpdateWorldZIndex()
{
    GameObject::gameObjectRenderOrderUpdatePool.insert(this->gameObject);

    for (auto c : child)
    {
        c->worldzindex = this->worldzindex + c->zindex;
        c->UpdateWorldZIndex();
    }
}

Vector2 Transform::GetPostion()
{
    return position;
}

void Transform::SetPosition(Vector2 newpos)
{
    this->position = newpos;
    this->worldposition = ((this->parent != nullptr) ? newpos + this->parent->worldposition : newpos);
    UpdateWorldPosition();
}

Vector2 Transform::GetWorldPosition()
{
    return worldposition;
}

void Transform::SetWorldPosition(Vector2 newpos)
{
    this->worldposition = newpos;
    this->position = ((this->parent != nullptr) ? newpos - this->parent->worldposition : newpos);
    UpdateWorldPosition();
}

void Transform::UpdateWorldPosition()
{
    if (gameObject->spriteRenderer != nullptr)
        gameObject->spriteRenderer->UpdateRealRenderPostion();

    for (auto c : child)
    {
        c->worldposition = this->worldposition + c->position;
        c->UpdateWorldPosition();
    }
}

void Transform::Translate(Vector2 dpos)
{
    SetPosition(position + dpos);
}

void Transform::Translate(float dx, float dy)
{
    SetPosition(position + Vector2(dx, dy));
}

int Transform::GetZIndex()
{
    return zindex;
}

int Transform::GetSortingLayer()
{
    SpriteRenderer *SR = this->gameObject->spriteRenderer;
    if (SR == nullptr)
        return -1;
    else
        return (int)SR->GetSortingLayer();
}

void Transform::SetParent(Transform *target)
{
    if(this->parent != nullptr)
        this->parent->RemoveChild(this);

    if(target != nullptr)
        target->AddChild(this);

	this->parent = target;
	this->SetPosition(position);
    this->gameObject->UpdateEnable();
}

void Transform::SetParentAbsolute(Transform* target)
{
	if (this->parent != nullptr)
		this->parent->RemoveChild(this);

	if (target != nullptr)
		target->AddChild(this);

	Vector2 myPos = GetWorldPosition();
	this->parent = target;
	this->SetWorldPosition(myPos);
    this->gameObject->UpdateEnable();
}

Transform* Transform::GetParent()
{
    return parent;
}

vector<Transform*> Transform::GetChild()
{
    return child;
}

void Transform::AddChild(Transform * target)
{
    this->child.push_back(target);
}

void Transform::RemoveChild(Transform * target)
{
    auto it = find(this->child.begin(), this->child.end(), target);
    GAME_ASSERT(it != this->child.end(), "Remove Child Not Found,\nSomething went wrong.\nPlease connect engine dev......");
    this->child.erase(it);
}

void Transform::ParseJSON(json j)
{
    bool doUpdateRenderOrder = false;

    if (j.find("position") != j.end())
        this->SetPosition(j["position"]);

    if (j.find("scale") != j.end())
        this->scale = j["scale"];

    if (j.find("zindex") != j.end())
    {
        this->SetZIndex(j["zindex"]);
    }
}

//////////////////////////////////////////////////////////////////
// SpriteRenderer實作
//////////////////////////////////////////////////////////////////

map<string, unsigned int> SpriteRenderer::fileInfo;
SpriteRenderer::SpriteRenderer(GameObject* gobj) : Component(gobj)
{
    this->gameObject->spriteRenderer = this;

    srcpos = Vector2I::null;
    size = Vector2I::null;
    offset = Vector2I::zero;
    anchorRaito = Vector2::zero;
	sortingLayer = SortingLayer::Default;
    realRenderPosition = Vector2I::zero;
}

SpriteRenderer::~SpriteRenderer()
{
    this->gameObject->spriteRenderer = nullptr;
}

void SpriteRenderer::ParseJSON(json j)
{
    if (j.find("Bitmap") != j.end())
    {
        int r, g, b;
        r = b = 255;
        g = 0;

        if (j["Bitmap"].find("colorkey") != j["Bitmap"].end())
        {
            if (j["Bitmap"]["colorkey"].find("r") != j["Bitmap"]["colorkey"].end())
                r = j["Bitmap"]["colorkey"]["r"];

            if (j["Bitmap"]["colorkey"].find("g") != j["Bitmap"]["colorkey"].end())
                g = j["Bitmap"]["colorkey"]["g"];

            if (j["Bitmap"]["colorkey"].find("b") != j["Bitmap"]["colorkey"].end())
                b = j["Bitmap"]["colorkey"]["b"];
        }

        string name = j["Bitmap"]["name"].get<string>();
        LoadBitmapData(name, false, r, g, b);
    }

    if (j.find("SrcPosition") != j.end())
        this->SetSourcePos(j["SrcPosition"]);

    if (j.find("SrcSize") != j.end())
        this->SetSize(j["SrcSize"]);

	if (j.find("AnchorRaito") != j.end())
		this->SetAnchorRaito(j["AnchorRaito"]);

    if (j.find("SortingLayer") != j.end())
    {
        this->sortingLayer = j["SortingLayer"];
        GameObject::UpdateRenderOrder(this->gameObject);
    }
}

void SpriteRenderer::Draw(Vector2I cameraPos)
{
    GAME_ASSERT(transform != nullptr, "You need transform to render sprite. #[Engine]SpriteRenderer->Draw");
    if(CameraTest(cameraPos))
        this->ShowBitmap(this->GetRealRenderPostion() - cameraPos, transform->scale, srcpos, size, cutSrc);
}

void SpriteRenderer::SetSourcePos(Vector2I pos)
{
    this->srcpos = pos;
    this->cutSrc = true;
}

void SpriteRenderer::ResetSourcePos()
{
    this->srcpos = Vector2I(-1, -1);
    this->cutSrc = false;
}

Vector2I SpriteRenderer::GetSourcePos()
{
    return this->srcpos;
}

void SpriteRenderer::SetSourceData(Rect src)
{
    this->size = Vector2I(src.w, src.h);
    this->srcpos = Vector2I(src.x, src.y);
    this->cutSrc = true;
}

void SpriteRenderer::SetSize(Vector2I size)
{
    this->size = size;
}

Vector2I SpriteRenderer::GetSize()
{
	return size;
}

void SpriteRenderer::ResetSize()
{
    this->size = Vector2I(this->Width(), this->Height());
}

void SpriteRenderer::LoadBitmapData(string filename, bool unsafe, short r, short g, short b)
{
    string name = R"(.\Assest\Bitmap\)" + filename + ".bmp";

    if (fileInfo.find(name) == fileInfo.end())
    { 
        int length = strlen(name.c_str());
        char* cname = new char[length + 1]();
        strncpy(cname, name.c_str(), length);
        this->LoadBitmapA(cname, RGB(r, g, b));
        this->ResetSize();
        this->ResetSourcePos();
        delete[] cname;

        fileInfo[name] = this->SurfaceID;
    }
    else
    {
        if(unsafe)
            this->UnsafeSetSurfaceID(fileInfo[name]);
        else
            this->SetSurfaceID(fileInfo[name]);
    }
}

int SpriteRenderer::GetSurfaceID()
{
    return this->SurfaceID;
}

void SpriteRenderer::SetSurfaceID(int SID)
{
    GAME_ASSERT(CheckExist(SID), "SurfaceID not found. #[Engine]SpriteRenderer->SetSurfaceID");
    this->SurfaceID = SID;
    this->isBitmapLoaded = true;
    this->ResetSize();
    this->ResetSourcePos();
}

void SpriteRenderer::UnsafeSetSurfaceID(int SID)
{
    this->isBitmapLoaded = true;
    this->SurfaceID = SID;
}

void SpriteRenderer::Reset()
{
    ResetSize();
    ResetSourcePos();
}

void SpriteRenderer::SetAnchorRaito(Vector2 pos)
{
    this->anchorRaito = pos;
    UpdateRealRenderPostion();
}

void SpriteRenderer::SetOffset(Vector2I dp)
{
    this->offset = dp;
    UpdateRealRenderPostion();
}

Vector2I SpriteRenderer::GetOffset()
{
	return offset;
}

Vector2I SpriteRenderer::GetAnchorPoint()
{
    return (this->size.GetV2() * this->anchorRaito).GetV2I();
}

SortingLayer SpriteRenderer::GetSortingLayer()
{
    return sortingLayer;
}

void SpriteRenderer::SetSortingLayer(SortingLayer SL)
{
    this->sortingLayer = SL;
    GameObject::UpdateRenderOrder(this->gameObject);
}

inline Vector2I SpriteRenderer::GetRealRenderPostion()
{
    return realRenderPosition;
}

inline void SpriteRenderer::UpdateRealRenderPostion()
{
    realRenderPosition = transform->GetWorldPosition().round().GetV2I() - GetAnchorPoint() + offset;
}

bool SpriteRenderer::CameraTest(Vector2I cameraPos)
{
    Vector2 aw = (this->size.GetV2() - Vector2::one) / 2, bw = (Vector2(SIZE_X, SIZE_Y) - Vector2::one) / 2;
    return ((aw + GetRealRenderPostion()) - (bw + cameraPos)).abs() <= aw + bw;
}

void SpriteRenderer::SetFlip(bool x, bool y)
{
    flipCode = (x == true ? 1 : 0) + (y == true ? 2 : 0);
}

void SpriteRenderer::SetFlipX(bool x)
{
    flipCode = (flipCode & 2) + (x == true ? 1 : 0);
}

void SpriteRenderer::SetFlipY(bool y)
{
    flipCode = (flipCode & 1) + (y == true ? 2 : 0);
}

bool SpriteRenderer::GetFlipX()
{
    return (flipCode & 1) == 1;
}

bool SpriteRenderer::GetFlipY()
{
    return (flipCode & 2) == 2;
}




//////////////////////////////////////////////////////////////////
// Collision實作
//////////////////////////////////////////////////////////////////
Collider::Collider(GameObject* gobj, Vector2I dP, Vector2I sz) : Component(gobj)
{
    this->gameObject->collider = this;

    this->collisionInfo.offset = dP;
    this->collisionInfo.size = sz;
}

Collider::~Collider()
{
    this->gameObject = nullptr;
}

void Collider::OnDrawGismos(CDC *pDC, Vector2I cameraPos)
{
	Vector2I w = collisionInfo.size;
	SpriteRenderer *SR = gameObject->spriteRenderer;
	Vector2 SpriteOffset = SR != nullptr ? SR->GetAnchorPoint().GetV2() : Vector2::zero;
	Vector2 pos = transform->GetWorldPosition() + collisionInfo.offset.GetV2()- SpriteOffset;
    game_framework::CDDraw::DrawRect(pDC, pos.GetV2I() - cameraPos, w, RGB(0, 255, 0));
}

bool Collider::PointCollision(Vector2I point)
{
    GAME_ASSERT(transform != nullptr, (string("transform not found. #[Engine]Collision::PointCollision | Object : ") + gameObject->GetName()).c_str());
    return point >= (transform->GetWorldPosition().GetV2I() + collisionInfo.offset) && point <= (transform->GetWorldPosition().GetV2I() + collisionInfo.offset + collisionInfo.size);
}

bool Collider::BoxCollision(Collider* box, Vector2 &velocityOffset, bool block)
{
    Vector2 aw = (collisionInfo.size.GetV2() - Vector2::one) / 2, bw = (box->collisionInfo.size.GetV2() - Vector2::one) / 2;
	SpriteRenderer *aSR = gameObject->spriteRenderer, *bSR = box->gameObject->spriteRenderer;
	Vector2 aSpriteOffset = aSR != nullptr ? aSR->GetAnchorPoint().GetV2() : Vector2::zero;
	Vector2 bSpriteOffset = bSR != nullptr ? bSR->GetAnchorPoint().GetV2() : Vector2::zero;
	Vector2 apos = transform->GetWorldPosition() + collisionInfo.offset.GetV2() + velocityOffset - aSpriteOffset;
	Vector2 bpos = box->transform->GetWorldPosition() + box->collisionInfo.offset.GetV2() - bSpriteOffset;
	Vector2 amid = apos + aw;
	Vector2 bmid = bpos + bw;
	Vector2 w = aw + bw;
    Vector2 l = (amid - bmid).abs();
	bool ret = l <= w;

	if (ret && block)
	{
		Vector2 collideLength = w - l +Vector2::one;
		velocityOffset = velocityOffset - collideLength * velocityOffset.side();
	}

	return ret;
}

void Collider::Update()
{
	OnEnter.clear();
	OnStay.clear();

    for (auto c : collidedCollider)
    {
        auto it = lastCollidedCollder.find(c);
        if (it == lastCollidedCollder.end())
        {
            OnEnter.push_back(c);
        }
        else
        {
            OnStay.push_back(c);
            lastCollidedCollder.erase(it);
        }
    }

    for (auto c : OnEnter)
        for (auto it = gameObject->gamebehaviorSetBegin; it != gameObject->gamebehaviorSetEnd; ++it)
            if ((*it)->GetEnable())
                (*it)->OnCollisionEnter(c);

	for (auto c : OnStay)
        for (auto it = gameObject->gamebehaviorSetBegin; it != gameObject->gamebehaviorSetEnd; ++it)
            if ((*it)->GetEnable())
                (*it)->OnCollisionStay(c);

	for (auto c : lastCollidedCollder) //最後剩在lastCollidedCollder的就是OnExit的Collider
        for (auto it = gameObject->gamebehaviorSetBegin; it != gameObject->gamebehaviorSetEnd; ++it)
            if ((*it)->GetEnable())
                (*it)->OnCollisionExit(c);

	lastCollidedCollder = collidedCollider;
	collidedCollider.clear();
}

void Collider::ParseJSON(json j)
{
    if (j.find("offset") != j.end())
    {
		collisionInfo.offset = j["offset"];
    }

    if (j.find("size") != j.end())
    {
		collisionInfo.size = j["size"];
    }

	if (j.find("collisionLayer") != j.end())
	{
		for (CollisionLayer cl : j["collisionLayer"])
			collisionLayer.push_back(cl);
	}
		
}

Animation::Animation(GameObject * gobj) : Component(gobj)
{
    this->gameObject->animation = this;
    this->gameObject->AddComponentOnce<SpriteRenderer>();
    startAtReminder = true;
	frameRemider = -1;
}

Animation::~Animation()
{
    this->gameObject->animation = nullptr;
}

void Animation::NextFrame()
{
	int size = 0;
	AnimationPlaytype ap;

	if (this->frameRemider != -1)
	{
		size = animationOneShot.frameList.size();
		ap = animationOneShot.playtype;

		if (ap == AnimationPlaytype::Forward)
		{
            if (this->animateFrame + 1 == size)
            {
                this->animateCount = this->frameRemider;
                EndAnimation();
                this->frameRemider = -1;
            }
			else
			{
				this->animateCount++;
				SetFrame(this->animateCount);
			}
		}
		else if (ap == AnimationPlaytype::Reverse)
		{
			if (this->animateFrame - 1 == -1)
			{
				this->animateCount = this->frameRemider;
                EndAnimation();
				this->frameRemider = -1;
			}
			else
			{
				this->animateCount++;
				SetFrame(size - this->animateCount - 1);
			}
		}
		else if (ap == AnimationPlaytype::Pingpong)
		{
			if (this->animateCount + 1 == (size - 1) * 2 + 1)
			{
				this->animateCount = this->frameRemider;
                EndAnimation();
				this->frameRemider = -1;
			}
			else
			{
				this->animateCount++;
				SetFrame(size - 1 - abs(size - 1 - animateCount));
			}
		}
	}

	if (this->frameRemider == -1)
	{
		size = animationData.frameList.size();
		ap = animationData.playtype;

		if (ap == AnimationPlaytype::Forward)
		{
            if (this->animateFrame + 1 == size)
                EndAnimation();

			this->animateCount++;
			SetFrame(this->animateCount);
		}
		else if (ap == AnimationPlaytype::Reverse)
		{
            if (this->animateFrame - 1 == -1)
                EndAnimation();

			this->animateCount++;
			SetFrame(size - this->animateCount - 1);
		}
		else if (ap == AnimationPlaytype::Pingpong)
		{
            if (this->animateCount + 1 == (size - 1) * 2)
                EndAnimation();

			this->animateCount = (this->animateCount + 1) % ((size - 1) * 2);
			SetFrame(size - 1 - abs(size - 1 - animateCount));
		}
	}
}

void Animation::SetFrame(int i)
{
    AnimationSetting data;
	if (this->frameRemider != -1)
	{
		int size = animationOneShot.frameList.size();
		this->animateFrame = ((i % size) + size) % size;
		data = animationOneShot.frameList[this->animateFrame];
	}
	else
    {
		int size = animationData.frameList.size();
		this->animateFrame = ((i % size) + size) % size;
        data = animationData.frameList[this->animateFrame];
    }

    this->gameObject->spriteRenderer->LoadBitmapData(data.filename, true);
    this->gameObject->spriteRenderer->SetSourceData(data.frame);
    this->duration = data.duration;
    this->timeStamp = clock();
}

void Animation::Play(AnimationData newAnim)
{
    this->animationData.frameList.clear();
    this->animationData.frameList = newAnim.frameList;
    this->animationData.playtype = newAnim.playtype;
	this->animationData.name = newAnim.name;

    GAME_ASSERT(animationData.frameList.size() != 0, "Animation Size ERROR");

	ResetAnimation();
}

void Animation::PlayOneShot(AnimationData newAnim)
{
    if (this->frameRemider != -1)
        this->animationOneShot.frameList.clear();
    else
        frameRemider = this->animateCount;

    this->animationOneShot.frameList = newAnim.frameList;
    this->animationOneShot.playtype = newAnim.playtype;
	this->animationOneShot.name = newAnim.name;

    GAME_ASSERT(animationOneShot.frameList.size() != 0, "Animation Size ERROR");

	ResetAnimation();
}

void Animation::EndAnimation()
{
    if (this->frameRemider != -1)
    {
        for (auto it = gameObject->gamebehaviorSetBegin; it != gameObject->gamebehaviorSetEnd; ++it)
            if ((*it)->GetEnable())
                (*it)->OnAnimationEnd(animationOneShot);
    }
    else
    {
        for (auto it = gameObject->gamebehaviorSetBegin; it != gameObject->gamebehaviorSetEnd; ++it)
            if ((*it)->GetEnable())
                (*it)->OnAnimationEnd(animationData);
    }
}

void Animation::Update()
{
	if (this->animateCount != -1 && clock() - this->timeStamp >= (DWORD)this->duration)
		NextFrame();
}

void Animation::ResetAnimation()
{
	this->animateCount = 0;
    SetFrame(0);
}

void Animation::SetAnimationPlaytype(AnimationPlaytype ap)
{
    if (this->frameRemider != -1)
        animationOneShot.playtype = ap;
    else
        animationData.playtype = ap;
}

AnimationController::AnimationController(GameObject * gobj) : Component(gobj)
{
    this->gameObject->animationController = this;
    this->gameObject->AddComponentOnce<Animation>();
    jumpState = "";
    laststate = "";
}

AnimationController::~AnimationController()
{
    this->gameObject->animationController = nullptr;
}

void AnimationController::ParseJSON(json j)
{
    //animation name
    //init

    if(j.find("animation") != j.end())
    {
        string name = R"(.\Assest\Animation\)" + j["animation"].get<string>() + ".anim";

        ifstream file;
        file.open(name);
        if (file.good())
        {
            stringstream buffer;
            buffer << file.rdbuf();
            json aespriteJSONobj = json::parse(buffer);
            this->ParseAespriteJSON(aespriteJSONobj);
            file.close();
        }
        else
        {
            file.close();
            string str = "ERROR : Animation NOT FOUND when parse Animation JSON :\n";
            str += "GameObject : " + gameObject->GetName() + "\n";
            str += "Animation : " + name + " => NOT FOUND";
            GAME_ASSERT(false, str.c_str());
        }
    }

    if (j.find("init") != j.end())
        Play(j["init"].get<string>());
    else
        Play(0);
    
    Update();
}

void AnimationController::ParseAespriteJSON(json j)
{
    this->animationList.clear();
    this->animationData.clear();
    this->frames.clear();

    string filename = j["meta"]["image"];

    filename = filename.substr(filename.find(R"(Assest\Bitmap\)") + 14);
    filename = filename.substr(0, filename.find(".bmp"));

    for (AnimationSetting as : j["frames"])
    {
        as.filename = filename;
        this->frames.push_back(as);
    }

    for (auto jobj : j["meta"]["frameTags"])
    {
        this->animationList.push_back(jobj["name"]);

        AnimationData animationData;
        
        string direction = jobj["direction"];

        if(direction == "forward")
            animationData.playtype = AnimationPlaytype::Forward;
        else if (direction == "reverse")
            animationData.playtype = AnimationPlaytype::Reverse;
        else if (direction == "pingpong")
            animationData.playtype = AnimationPlaytype::Pingpong;

        if (jobj.find("from") != jobj.end() && jobj.find("to") != jobj.end())
        {
            int from = jobj["from"];
            int to = jobj["to"];

            for (int i = from; i <= to; i++)
            {
                animationData.frameList.push_back(frames[i]);
            }
        }
        else if (jobj.find("list") != jobj.end())
        {
            for (int i : jobj["list"])
            {
                animationData.frameList.push_back(frames[i]);
            }
        }

        animationData.name = jobj["name"].get<string>();
        this->animationData[jobj["name"]] = animationData;
    }
}

void AnimationController::Update()
{
	if (jumpState != "")
	{
        this->gameObject->animation->Play(animationData[jumpState]);
        laststate = jumpState;
		jumpState = "";
	}
}

bool AnimationController::Play(string state, bool force)
{
    if (animationData.find(state) != animationData.end())
    {
        if (state != laststate || force)
        {
            jumpState = state;
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

bool AnimationController::Play(int state, bool force)
{
    if (state >= 0 && state < (int)animationList.size())
         return Play(animationList[state], force);
    else
        return false;
}

void AnimationController::PlayOneShot(string state)
{
    if (animationData.find(state) != animationData.end())
        this->gameObject->animation->PlayOneShot(animationData[state]);
}

void AnimationController::PlayOneShot(int state)
{
    if (state >= 0 && state < (int)animationList.size())
        this->gameObject->animation->PlayOneShot(animationData[animationList[state]]);
}

Rigidbody::Rigidbody(GameObject * gobj) : Component(gobj)
{
    this->gameObject->rigidbody = this;
}

Rigidbody::~Rigidbody()
{
    this->gameObject->rigidbody = nullptr;
}

void Rigidbody::ParseJSON(json j)
{
    if (j.find("TimeSliceCollision") != j.end())
        this->TimeSliceCollision = j["TimeSliceCollision"];
}

void Rigidbody::OnCollision(Collider *tgcollider)
{
	Collider* c = this->gameObject->collider;

	tgcollider->collidedCollider.insert(c);
	c->collidedCollider.insert(tgcollider);
}

bool Rigidbody::DoCollision(Collider *collider, set<GameObject*>& gobjset, Vector2 &tempVelocity, bool block, bool resetVX)
{
	float vx = tempVelocity.x;
    bool ret = false;
    for (auto gobj : gobjset)
    {
		if (gobj == this->gameObject || !gobj->enable)
			continue;

        Collider* tgcollider = gobj->collider;
        if (tgcollider != nullptr && tgcollider->GetEnable())
        {
            if (collider->BoxCollision(tgcollider, tempVelocity, block))
            {
                OnCollision(tgcollider);
                ret = true;
            }
			if (resetVX)
				tempVelocity.x = vx;
        }
    }
    return ret;
}

void Rigidbody::CollisionDetection(Vector2& invelocity)
{
    Collider* collider = this->gameObject->collider;
    if (collider != nullptr)
    {
        for (CollisionLayer cl : collider->collisionLayer)
        {
            auto gobjvec = GameObject::findGameObjectsByLayer(cl.layer);

            if (cl.block)
            {
                //Horizontal Collision
                Vector2 vHorizontal(invelocity.x, 0);
                if (DoCollision(collider, gobjvec, vHorizontal, cl.block))
                    invelocity.x = vHorizontal.x;

                //Vertical Collision
                Vector2 vVertical(invelocity.x, invelocity.y);
                if (DoCollision(collider, gobjvec, vVertical, cl.block, true))
                    invelocity.y = vVertical.y;
            }
            else
                DoCollision(collider, gobjvec, invelocity, cl.block);
        }
    }
}

void Rigidbody::CollisionDetectionSlice(Vector2 & invelocity)
{
    Collider* collider = this->gameObject->collider;
    if (collider != nullptr)
    {
        for (CollisionLayer cl : collider->collisionLayer)
        {
            vector<Vector2> sliceVelocity;
            Vector2 sliceUnit = Vector2(16, 16);
            Vector2 vslicenum = invelocity / sliceUnit;
            vslicenum = vslicenum.abs();

            auto &gobjvec = GameObject::findGameObjectsByLayer(cl.layer);

            if (cl.block)
            {
                Vector2I sliceNum(ceil(vslicenum.x), ceil(vslicenum.y));

                //Horizontal Collision
                if (sliceNum.x != 0)
                {
                    Vector2 slice = invelocity / sliceNum.x;
                    for (int i = 1; i <= sliceNum.x; i++)
                    {
                        Vector2 v;
                        if (i != sliceNum.x)
                            v = (slice * i).round();
                        else
                            v = invelocity;
                        Vector2 vHorizontal(v.x, 0);
                        if (DoCollision(collider, gobjvec, vHorizontal, cl.block))
                        {
                            invelocity.x = vHorizontal.x;
                            break;
                        }
                    }
                }

                if (sliceNum.y != 0)
                {
                    Vector2 slice = invelocity / sliceNum.y;
                    for (int i = 1; i <= sliceNum.y; i++)
                    {
                        Vector2 v;
                        if (i != sliceNum.y)
                            v = (slice * i).round();
                        else
                            v = invelocity;
                        Vector2 vVertical(invelocity.x, v.y);
                        if (DoCollision(collider, gobjvec, vVertical, cl.block, true))
                        {
                            invelocity.y = vVertical.y;
                            break;
                        }
                    }
                }

				if (sliceNum == Vector2I::zero)
					DoCollision(collider, gobjvec, invelocity, cl.block);
            }
            else
            {
                Vector2I temp(ceil(vslicenum.x), ceil(vslicenum.y));
                int sliceNum = temp.x > temp.y ? temp.x : temp.y;

                if (sliceNum != 0)
                {
                    Vector2 slice = invelocity / sliceNum;
                    for (int i = 1; i <= sliceNum; i++)
                    {
                        Vector2 v;
                        if (i != sliceNum)
                            v = (slice * i).round();
                        else
                            v = invelocity;
                        
                        DoCollision(collider, gobjvec, v, cl.block);
                    }
                }
				else
					DoCollision(collider, gobjvec, invelocity, cl.block);
            }
        }
    }
}

void Rigidbody::Update()
{
    colliderInfo.Reset();

    Vector2 originalVelocity = velocity;
    velocity = velocity.ceilSpecial();

    Vector2 originalFloorVelocity = velocity;

    if (TimeSliceCollision)
        CollisionDetectionSlice(velocity);
    else
        CollisionDetection(velocity);

    //Set ColliderInfo
    if (velocity.x != originalFloorVelocity.x)
    {
        if (originalFloorVelocity.x > 0)
            colliderInfo.right = true;
        else if (originalFloorVelocity.x < 0)
            colliderInfo.left = true;
    }

    if (velocity.y != originalFloorVelocity.y)
    {
        if (originalFloorVelocity.y > 0)
            colliderInfo.bottom = true;
        else if (originalFloorVelocity.y < 0)
            colliderInfo.top = true;
    }
    
    //Move Object
    this->transform->Translate(velocity);

    //Set velocity accoriding to original velocity(Avoiding weird number problem when adding float velocity < 1)
    velocity = originalVelocity + (velocity - originalFloorVelocity);

}

CollisionLayer::CollisionLayer()
{
	layer = Layer::Default;
	block = false;
}

void from_json(const json & j, CollisionLayer & cl)
{
	if (j.find("Block") != j.end())
		cl.block = j["Block"];

	if (j.find("Layer") != j.end())
		cl.layer = (Layer)j["Layer"];
}

string ColliderInfo::toString()
{
	return "top : " + to_string(top) + " | bottom : " + to_string(bottom) + " | left : " + to_string(left) + " | right : " + to_string(right);
}

void ColliderInfo::Reset()
{
    top = bottom = left = right = false;
}

}