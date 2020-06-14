#pragma once
#include "BaseObserver.h"
namespace dae
{
	class GameObject;
}
class ScoreObserver final : public BaseObserver
{
public:
	ScoreObserver();
	virtual ~ScoreObserver() {};
	virtual void onNotify(dae::GameObject* entity, Events event) override;
	int GetScore() { return m_Score; } 
	void ResetScore() { m_Score = 0; }
private:
	int m_Score{};
};



