#pragma once
class BaseButton
{
public:
	BaseButton();
	virtual ~BaseButton();

protected:
	bool m_Pressed{};
};

