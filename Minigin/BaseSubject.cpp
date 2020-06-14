#include "MiniginPCH.h"
#include "BaseSubject.h"
#include "BaseObserver.h"
#include "GameObject.h"
BaseSubject::BaseSubject()
{
}


BaseSubject::~BaseSubject()
{

}

void BaseSubject::AddObserver(BaseObserver * observer)
{
	m_VecObservers.push_back(observer);
	++m_NumObservers;
}

void BaseSubject::RemoveObserver(BaseObserver* observer)
{
	for(int i{}; i < int(m_VecObservers.size()); ++i)
		{
			if (observer == m_VecObservers[i])
			{
				std::swap(m_VecObservers[i],m_VecObservers[m_VecObservers.size()]);
				delete m_VecObservers[m_VecObservers.size()];
				m_VecObservers.pop_back();
				--m_NumObservers;
			}
		}
}

void BaseSubject::Notify(dae::GameObject* object, Events event)
{
  for(BaseObserver* pObserver : m_VecObservers)
  {
	  pObserver->onNotify(object,event);
  }
}
