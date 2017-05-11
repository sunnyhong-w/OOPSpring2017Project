#include "StdAfx.h"
#include <string>
#include <ddraw.h>
#include "component.h"
#include "gamebehavior.h"
#include "gameobject.h"
#include "_GameScripts.h"

namespace game_engine
{

    ///// Add Component String Parser

    //Use define to make sure class won't have spelling mistake
#define RegisterComponent(v) else if (ComponentName == #v) return this->AddComponentOnce<v>();

    Component* GameObject::AddComponentOnce(string ComponentName)
    {
        if (ComponentName == "Component") return nullptr;

        //Register Basic Component
		RegisterComponent(Transform)
			RegisterComponent(SpriteRenderer)
			RegisterComponent(Collider)
			RegisterComponent(Rigidbody)
			RegisterComponent(Animation)
			RegisterComponent(AnimationController)
			//Register Script
			RegisterComponent(Tutorial)
			RegisterComponent(TextStamp)
			RegisterComponent(TextRenderer)
			RegisterComponent(MapReader)
			RegisterComponent(Slide)
			RegisterComponent(BoxParent)
			RegisterComponent(Player)
			RegisterComponent(SpawnPlayer)
			RegisterComponent(Spring)
			RegisterComponent(Button)
			RegisterComponent(GreenBox)
			RegisterComponent(YellowBox)
			RegisterComponent(MapEdge)
			RegisterComponent(Camera)
			RegisterComponent(TransportController)
			RegisterComponent(Gem)
			RegisterComponent(CollectedGem)
			RegisterComponent(Smoke)
			RegisterComponent(Background)
			RegisterComponent(Title)
        else return nullptr;
    }
}