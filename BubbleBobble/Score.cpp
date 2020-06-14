#include "pch.h"
#include "Score.h"
#include "ScoreObserver.h"
#include "TextObject.h"
#include "ResourceManager.h"
Score::Score()
{
	m_pObserver = new ScoreObserver();
	m_pFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf",30);
	m_pScoreObject = std::make_shared<dae::TextObject>(std::to_string(m_pObserver->GetScore()), m_pFont);
	m_pScoreObject->SetPosition(625, 300);
}

Score::~Score()
{
	delete m_pObserver;
	m_pObserver = nullptr;
}

void Score::Update()
{
	m_pScoreObject->SetText(std::to_string(m_pObserver->GetScore()));
}

void Score::Reset()
{
	m_pObserver->ResetScore();
}
