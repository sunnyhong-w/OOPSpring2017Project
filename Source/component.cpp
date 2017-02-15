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

namespace game_engine
{

//////////////////////////////////////////////////////////////////
// Component實作
//////////////////////////////////////////////////////////////////
Component::Component(GameObject* gobj, bool isGB) : isGameBehavior(isGB)
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

void from_json(const json &j, Transform &t)
{
    t.position = j["position"];
    t.scale = j["scale"];
    t.depth = j["depth"];
    t.zindex = j["zindex"];
    t.zcode = t.zindex + (int)t.depth;
}

//////////////////////////////////////////////////////////////////
// SpriteRenderer實作
//////////////////////////////////////////////////////////////////
SpriteRenderer::SpriteRenderer(GameObject* gobj) : Component(gobj)
{
    srcpos = Vector2I(-1, -1);
    size = Vector2I(-1, -1);
}

void SpriteRenderer::Draw()
{
    GAME_ASSERT(transform != nullptr, "You need transform to render sprite. #[Engine]SpriteRenderer->Draw");
    this->ShowBitmap(transform->position.GetV2I(), transform->scale, srcpos, size, cutSrc);
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

void SpriteRenderer::LoadBitmapData(char* filename, short r, short g, short b)
{
    this->LoadBitmapA(filename, RGB(r, g, b));
    this->ResetSize();
    this->ResetSourcePos();
}

int SpriteRenderer::GetSurfaceID()
{
    return this->SurfaceID;
}

void SpriteRenderer::SetSurfaceID(int SID)
{
    GAME_ASSERT(CheckExist(SID), "SurfaceID not found. #[Engine]SpriteRenderer->SetSurfaceID");
    this->SurfaceID = SID;
    this->ResetSize();
    this->ResetSourcePos();
    this->isBitmapLoaded = true;
}


//////////////////////////////////////////////////////////////////
// Collision實作
//////////////////////////////////////////////////////////////////
Collider::Collider(GameObject* gobj, Vector2I dP, Vector2I sz) : Component(gobj)
{
    this->deltaPoint = dP;
    this->size = sz;
}

bool Collider::PointCollision(Vector2I point)
{
    GAME_ASSERT(transform != nullptr, (string("transform not found. #[Engine]Collision::PointCollision | Object : ") + gameObject->GetName()).c_str());
    return point.x >= (transform->position.x + deltaPoint.x)
        && point.x <= (transform->position.x + deltaPoint.x + size.x)
        && point.y >= (transform->position.y + deltaPoint.y)
        && point.y <= (transform->position.y + deltaPoint.y + size.y);
}

}