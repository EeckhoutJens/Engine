#pragma once
class BaseObserver;
#include "Events.h"
namespace dae 
{
	class GameObject;
}
class BaseSubject
{
public:
	BaseSubject();
	~BaseSubject();
	void AddObserver(BaseObserver* observer);
	void RemoveObserver(BaseObserver* observer);
	void Notify(dae::GameObject* object, Events event);
private:
	std::vector<BaseObserver*> m_VecObservers{};
	int m_NumObservers{};
};

