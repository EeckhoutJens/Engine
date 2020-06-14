#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include "vector"
#include "BinaryWriter.h"
#include <SDL.h>

namespace NFE
{
	class BaseComponent;
	class FPSComponent;
	class TextureComponent;
	class StateComponent;
}


namespace dae
{
	class Font;
	class GameObject final : public SceneObject
	{
	public:
		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
		void Update(float deltaTime) override;
		void Render() const override;
		void SetPosition(float x, float y);
		const float3& GetPosition();
		void AddComponentToVector(NFE::BaseComponent* componentToAdd);
		void AddChild(GameObject* child);
		void DeleteChildren();
		void WriteToFile(BinaryWriter& writer, const char* filename);
		std::vector<NFE::BaseComponent*>& GetComponents();
		template <class T>
		T* GetComponent()
		{
			const type_info& ti = typeid(T);
			for (auto* component : m_pComponents)
			{
				if (component && typeid(*component) == ti)
					return static_cast<T*>(component);
			}

			return nullptr;
		}

	private:
		Transform mTransform;
		std::vector<NFE::BaseComponent*> m_pComponents;
		std::vector<GameObject*> m_pChildren;
	};
}
