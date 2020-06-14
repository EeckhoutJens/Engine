#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "TextObject.h"
#include "TextureComponent.h"
#include "FPSComponent.h"
#include "ResourceManager.h"

unsigned int dae::Scene::idCounter = 0;

dae::Scene::Scene(const std::string& name) : mName(name)
{
}

void dae::Scene::Add(const std::shared_ptr<SceneObject>& object)
{
	mObjects.push_back(object);
}

void dae::Scene::Remove(const std::shared_ptr<SceneObject>& object)
{
	if (object)
	{
		auto objLoc = std::find(mObjects.begin(), mObjects.end(), object);
		if (objLoc != mObjects.end())
		{
			mObjects.erase(objLoc);
		}
	}
	
	
}

void dae::Scene::Initialize()
{
	auto go = std::make_shared<GameObject>();
	auto text = new NFE::TextureComponent{go.get(),"logo.png"};
	go->AddComponentToVector(text);
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto test = new NFE::FPSComponent{go.get()};
	test->MakeFont(font);
	go->AddComponentToVector(test);
	go->SetPosition(216, 180);
	this->Add(go);

	auto to = std::make_shared<TextObject>("Programming 4 Assignment", font);
	to->SetPosition(80, 20);
	this->Add(to);
}

void dae::Scene::Update(float deltaTime)
{
	for(auto gameObject : mObjects)
	{
		gameObject->Update(deltaTime);
	}
}

void dae::Scene::Render() const
{
	for (const auto gameObject : mObjects)
	{
		gameObject->Render();
	}
}

