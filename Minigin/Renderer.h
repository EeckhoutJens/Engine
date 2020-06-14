#pragma once
#include "Singleton.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Rect;

namespace dae
{
	class Texture2D;
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* mRenderer = nullptr;

	public:
		void Init(SDL_Window* window);
		void Render();
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, SDL_Rect& textureData) const;

		SDL_Renderer* GetSDLRenderer() const { return mRenderer; }
	};
}

