#pragma once
#include "BaseButton.h"
#include <Xinput.h>
class ControllerButton final : public BaseButton
{
public:
	ControllerButton(DWORD chosenButton);
	~ControllerButton() override = default;
	DWORD GetValue()const{return value;}
private:
	DWORD value{};
};

