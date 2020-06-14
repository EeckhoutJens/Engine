#pragma once
#include "SceneManager.h"

namespace dae
{
	class SceneObject;
	class Scene
	{
		friend void SceneManager::CreateScene(const std::string& name, SceneType type);
	public:
		void Add(const std::shared_ptr<SceneObject>& object);
		void Remove(const std::shared_ptr<SceneObject>& object);
		virtual void Initialize();
		virtual void Update(float deltaTime);
		void Render() const;

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		std::string GetName() { return mName; }
	protected:
		std::string mName{};
		explicit Scene(const std::string& name);
		std::vector <std::shared_ptr<SceneObject>> mObjects{};
		static unsigned int idCounter;
	};

}
