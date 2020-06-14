#pragma once
namespace dae 
{
	class GameObject;
}
#include "Events.h"
class BaseObserver
{
public:
	BaseObserver();
	virtual ~BaseObserver() = default;
	virtual void onNotify(dae::GameObject* entity, Events event) = 0;
};

