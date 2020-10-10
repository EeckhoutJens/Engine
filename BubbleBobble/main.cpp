#pragma comment(lib,"xinput.lib")

// ReSharper disable once CppUnusedIncludeDirective
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "BubbleBobbleScene.h"
int main(int, char* []) 
{
	std::shared_ptr<BubbleBobbleScene> pScene = std::make_shared<BubbleBobbleScene>("BubbleBobbleScene");
	dae::SceneManager::GetInstance().AddScene(pScene);
	dae::Minigin engine;
	engine.Run();
	return 0;
}
