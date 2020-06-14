#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "Font.h"


NFE::FPSComponent::FPSComponent(dae::GameObject * parent) : BaseComponent(parent)
, m_Font(nullptr)
, m_TextObj(nullptr)
{
}

NFE::FPSComponent::~FPSComponent()
{
	delete m_TextObj;
}

void NFE::FPSComponent::Update(const float& deltaTime, float,float,float,float)
{
	m_FPSTimer += deltaTime;
	if (m_FPSTimer >= 1)
	{
		int fps = int(1 / deltaTime);
		m_NumFPS = std::to_string(fps);
		m_TextObj->SetText(m_NumFPS);
		m_TextObj->Update(deltaTime);
		m_FPSTimer -= 1;
	}

}

void NFE::FPSComponent::Render()
{
	m_TextObj->Render();
}

void NFE::FPSComponent::MakeFont(const std::string& fontPath, unsigned fontSize)
{
	m_Font = std::make_shared<dae::Font>(fontPath, fontSize);
	m_TextObj = new dae::TextObject{ m_NumFPS,m_Font };
	m_TextObj->SetPosition(10, 100);
}

void NFE::FPSComponent::MakeFont(const std::shared_ptr<dae::Font> font)
{
	m_Font = font;
	m_TextObj = new dae::TextObject{ m_NumFPS,m_Font };
	m_TextObj->SetPosition(10, 100);
}
