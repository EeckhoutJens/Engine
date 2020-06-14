#pragma once
#include "Singleton.h"
#include "vector"
#include <string>
#include <memory>
namespace dae
{
	enum class SceneType
	{
		Standard,
		MainMenu,
		BubbleBobbleLevel,
		GameOverMenu,
	};
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		void CreateScene(const std::string& name, SceneType type);
		void AddScene(std::shared_ptr<Scene> sceneToAdd);
		void Update(float deltaTime);
		void Render();
		void InitializeScene(const std::string& name);

	private:
		std::vector<std::shared_ptr<Scene>> mScenes;
	};

}
