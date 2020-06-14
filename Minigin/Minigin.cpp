#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "vld.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "Locator.h"
#include "Audio.h"

void dae::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	window = SDL_CreateWindow(
		"Bubble Bobble - 2DAE07_Eeckhout_Jens",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		750,
		550,
		SDL_WINDOW_OPENGL
	);
	if (window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}
	Renderer::GetInstance().Init(window);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	NFE::Locator::Initialize();
	SceneManager::GetInstance().InitializeScene("BubbleBobbleScene");
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	dae::ResourceManager::GetInstance().Init("../BubbleBobble/Resources/Textures/");
	
	LoadGame();
	{
		auto t = std::chrono::high_resolution_clock::now();
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		NFE::ConsoleAudio song;
		int bgmID = song.AddMusic("../BubbleBobble/Resources/Audio/bgm.mp3");
		const int volume = 10;
		song.SetVolumeMusic(volume);
		NFE::Locator::Provide(&song);
		NFE::Locator::GetAudio()->PlayMusic(bgmID);
		bool doContinue = true;
		float lag = 0.f;
		const float msPerUpdate(0.02f);
		while (doContinue)
		{
			input.CheckControllers();
			auto current = std::chrono::high_resolution_clock::now();
			float deltaTime(std::chrono::duration<float>(current - t).count());
			t = current;
			lag += deltaTime;
			doContinue = input.ProcessInput();
			while (lag >= msPerUpdate)
			{
				sceneManager.Update(msPerUpdate);
				lag -= msPerUpdate;
			}
			renderer.Render();

		}
	}

	Cleanup();
}
