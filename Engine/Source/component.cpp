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
    this->transform = gobj->GetComponent<Transform>();
}

bool Component::isBehavior()
{
    return this->isGameBehavior;
}

//////////////////////////////////////////////////////////////////
// Transform實作
//////////////////////////////////////////////////////////////////
Transform::Transform(GameObject* gobj, Vector2 v2, int z, RenderDepth rd) : Component(gobj)
{
    this->position = v2;
    this->scale = Vector2::one;
    this->zindex = z;
    this->depth = rd;
    this->zcode = this->zindex + (int)this->depth;
}

void Transform::SetRenderDepth(int z)
{
    this->zindex = z;
    this->zcode = this->zindex + (int)this->depth;
    GameObject::UpdateRenderOrder(this->gameObject);
}

void Transform::SetRenderDepth(RenderDepth rd)
{
    this->depth = rd;
    this->zcode = this->zindex + (int)this->depth;
    GameObject::UpdateRenderOrder(this->gameObject);
}

void Transform::SetRenderDepth(int z, RenderDepth rd)
{
    this->zindex = z;
    this->depth = rd;
    this->zcode = this->zindex + (int)this->depth;
    GameObject::UpdateRenderOrder(this->gameObject);
}

int Transform::GetZCode()
{
    return zcode;
}

void Transform::ParseJSON(json j)
{
    if (j.find("position") != j.end())
        this->position = j["position"];

    if (j.find("scale") != j.end())
        this->scale = j["scale"];

    if (j.find("depth") != j.end())
        this->depth = j["depth"];

    if (j.find("zindex") != j.end())
        this->zindex = j["zindex"];

    this->zcode = this->zindex + (int)this->depth;

    if (j.find("depth") != j.end() || j.find("zindex") != j.end())
        GameObject::UpdateRenderOrder(this->gameObject);
}

//////////////////////////////////////////////////////////////////
// SpriteRenderer實作
//////////////////////////////////////////////////////////////////

map<string, unsigned int> SpriteRenderer::fileInfo;
SpriteRenderer::SpriteRenderer(GameObject* gobj) : Component(gobj)
{
    srcpos = Vector2I::null;
    size = Vector2I::null;
    offset = Vector2I::zero;
    anchorRaito = Vector2::zero;
}

void SpriteRenderer::ParseJSON(json j)
{
    if (j.find("Bitmap") != j.end())
    {
        int r, g, b;
        r = g = b = 0;

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
}

void SpriteRenderer::Draw(Vector2I cameraPos)
{
    GAME_ASSERT(transform != nullptr, "You need transform to render sprite. #[Engine]SpriteRenderer->Draw");
    this->ShowBitmap(transform->position.round().GetV2I() - cameraPos - GetAnchorPoint() + offset, transform->scale, srcpos, size, cutSrc);
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

void SpriteRenderer::SetSize(Vector2I size)
{
    this->size = size;
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
}

void SpriteRenderer::SetOffset(Vector2I dp)
{
    this->offset = offset;
}

Vector2I SpriteRenderer::GetAnchorPoint()
{
    return (this->size.GetV2() * this->anchorRaito).GetV2I();
}




//////////////////////////////////////////////////////////////////
// Collision實作
//////////////////////////////////////////////////////////////////
Collider::Collider(GameObject* gobj, Vector2I dP, Vector2I sz) : Component(gobj)
{
    this->collisionInfo.offset = dP;
    this->collisionInfo.size = sz;
}

void Collider::OnDrawGismos()
{
	Vector2I w = collisionInfo.size;
	SpriteRenderer *SR = gameObject->GetComponent<SpriteRenderer>();
	Vector2 SpriteOffset = SR != nullptr ? SR->GetAnchorPoint().GetV2() : Vector2::zero;
	Vector2 pos = transform->position + collisionInfo.offset.GetV2()- SpriteOffset;
	game_framework::CDDraw::DrawRect(pos.GetV2I(), w, RGB(0, 255, 0));
}

bool Collider::PointCollision(Vector2I point)
{
    GAME_ASSERT(transform != nullptr, (string("transform not found. #[Engine]Collision::PointCollision | Object : ") + gameObject->GetName()).c_str());
    return point >= (transform->position.GetV2I() + collisionInfo.offset) && point <= (transform->position.GetV2I() + collisionInfo.offset + collisionInfo.size);
}

bool Collider::BoxCollision(Collider* box, Vector2 &velocityOffset, bool block)
{
    Vector2 aw = (collisionInfo.size.GetV2() - Vector2::one) / 2, bw = (box->collisionInfo.size.GetV2() - Vector2::one) / 2;
	SpriteRenderer *aSR = gameObject->GetComponent<SpriteRenderer>(), *bSR = box->gameObject->GetComponent<SpriteRenderer>();
	Vector2 aSpriteOffset = aSR != nullptr ? aSR->GetAnchorPoint().GetV2() : Vector2::zero;
	Vector2 bSpriteOffset = bSR != nullptr ? bSR->GetAnchorPoint().GetV2() : Vector2::zero;
	Vector2 apos = transform->position + collisionInfo.offset.GetV2() + velocityOffset - aSpriteOffset;
	Vector2 bpos = box->transform->position + box->collisionInfo.offset.GetV2() - bSpriteOffset;
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



void Animation::LoadAnimation(json jsonobj)
{
    this->animationInfo.clear();

    Vector2 gAnchor = Vector2::one * -1;
    int gDuration = -1;

    AnimationSetting global;

    if (jsonobj.find("setting") != jsonobj.end())
        global = jsonobj["setting"];
    

    for (AnimationSetting data : jsonobj["data"])
    {
        AnimationSetting setting(global);
        bool safe = setting.Build(data);
        GAME_ASSERT(!safe, ("Animation Building ERROR : \n Animation : " + jsonobj["filename"].get<string>()).c_str());
        
        json j = setting;
        this->animationInfo.push_back(j);

        SR->LoadBitmapData(setting.filename);
    }
}

void Animation::ParseJSON(json j)
{
    this->SR = gameObject->AddComponentOnce<SpriteRenderer>(); //Sprite Renderer Require
	json empty;
	SR->ParseJSON(empty);
} 

void Animation::Update()
{
    if (this->animateCount != -1 && clock() - this->timeStamp >= (DWORD)this->duration)
    {
        this->animateCount = (this->animateCount + 1) % this->animationInfo.size();
        AnimationSetting data = this->animationInfo[this->animateCount];
        SR->LoadBitmapData(data.filename, true);
        SR->SetSize(data.size);
        SR->SetSourcePos(data.position);
        SR->SetAnchorRaito(data.anchor);
        this->duration = data.duration;
        this->timeStamp = clock();
    }
}

void Animation::ResetAnimation()
{
    this->animateCount = this->animationInfo.size() - 1;
    this->timeStamp = clock();
    this->duration = 0;
}

void AnimationController::ParseJSON(json j)
{
	animation = this->gameObject->AddComponentOnce<Animation>(); //Animation require
	json empty;
	animation->ParseJSON(empty);

	GAME_ASSERT(j.find("init") != j.end(), ("Animation State Init Not Found! | \nGameObject : " + gameObject->GetName()).c_str());
	JumpState(j["init"]);

	GAME_ASSERT(j.find("state") != j.end(), ("Animation State Data Not Found! | \nGameObject : " + gameObject->GetName()).c_str());

	for (auto &jdat : j["state"])
	{
		string name = R"(.\Assest\Animation\)" + jdat.get<string>() + ".anim";

		ifstream file;
		file.open(name);
		if (file.good())
		{
			stringstream buffer;
			buffer << file.rdbuf();
			json jsonobj = json::parse(buffer);
			jsonobj["filename"] = name;
			animation->LoadAnimation(jsonobj);
			jdat = jsonobj;
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

	data = j;

	Update();
}

void AnimationController::Update()
{
	if (jumpState != "")
	{
		GAME_ASSERT(data["state"].find(jumpState) != data["state"].end(), ("Animation State Define Not Found! | \nGameObject : " 
																			+ gameObject->GetName() + "\nAnimationState : " + jumpState).c_str());
		animation->LoadAnimation(data["state"][jumpState]);
		animation->ResetAnimation();
		jumpState = "";
	}
}

void AnimationController::JumpState(string state)
{
	jumpState = state;
}

void Rigidbody::ParseJSON(json j)
{

}

void Rigidbody::OnCollision(Collider *tgcollider)
{
    for (GameObject::ComponentData::iterator it = gameObject->componentData.begin(); it != gameObject->componentData.end(); it++)
    {
        if (it->second->isBehavior())
        {
            GameBehaviour* gb = static_cast<GameBehaviour*>(it->second);

            if (gb->enable)
                gb->OnCollisionEnter(tgcollider);
        }
    }
}

bool Rigidbody::DoCollision(Collider *collider, vector<GameObject*> gobjvec, Vector2 &tempVelocity, bool block)
{
    bool ret = false;
    for (auto gobj : gobjvec)
    {
        Collider* tgcollider = gobj->GetComponent<Collider>();
        if (tgcollider != nullptr)
        {
            if (collider->BoxCollision(tgcollider, tempVelocity, block))
            {
                OnCollision(tgcollider);
                ret = true;
            }
        }
    }
    return ret;
}

void Rigidbody::Update()
{
	Collider* collider = this->gameObject->GetComponent<Collider>();
	if (collider != nullptr)
	{
		for (CollisionLayer cl : collider->collisionLayer)
		{
			auto gobjvec = GameObject::findGameObjectsByLayer(cl.layer);

            if (cl.block)
            {
                //Horizontal Collision
                Vector2 vHorizontal(velocity.x, 0);
                if (DoCollision(collider, gobjvec, vHorizontal, cl.block))
                    velocity.x = vHorizontal.x;

                //Vertical Collision
                Vector2 vVertical(velocity.x, velocity.y);
                if (DoCollision(collider, gobjvec, vVertical, cl.block))
                    velocity.y = vVertical.y;
            }
            else
                DoCollision(collider, gobjvec, velocity, cl.block);
		}
	}

	this->transform->position = this->transform->position + velocity;
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

}