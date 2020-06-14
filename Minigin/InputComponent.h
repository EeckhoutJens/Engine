#pragma once
#include "BaseComponent.h"

enum class ObjectType
{
	Player,
	ZenChan,
	Maita
};

namespace NFE
{
	
	class Command;
	class InputComponent final : public BaseComponent
	{
	public:
		InputComponent(dae::GameObject *parent, int ID, ObjectType type);
		~InputComponent();
		void Update(const float& deltaTime, float x, float y, float width, float height) override;
		void Render() override{;};
		void WriteToFile(BinaryWriter&, const char*) override {};
		Command* GetLastTriggeredCommand() { return prevTriggeredCommand; };
		void AssignCommand(Command* command);

	private:
		int m_ControllerID;
		ObjectType m_Type;
		std::vector<Command*> triggeredCommands;
		std::vector<Command*> m_VecCommands{};
		Command* prevTriggeredCommand;
	};
}


