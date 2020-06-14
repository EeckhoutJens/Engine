#pragma once
#include <memory>
#include <string>
namespace dae
{
	class TextObject;
	class Font;

}
class ScoreObserver;
class Score
{
public:
	Score();
	~Score();
	ScoreObserver* GetObserver() { return m_pObserver; }
	std::shared_ptr<dae::TextObject> GetScore() { return m_pScoreObject; }
	void Update();
	void Reset();

private:
	std::shared_ptr<dae::TextObject> m_pScoreObject{};
	std::shared_ptr<dae::Font> m_pFont{};
	ScoreObserver* m_pObserver{};
};

