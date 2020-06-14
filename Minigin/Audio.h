#pragma once
#include <iostream>
#include <SDL_mixer.h>
#include <vector>

namespace NFE
{
	class Audio
{
public:
	virtual ~Audio() = default;
	virtual void PlaySound(int SoundID) = 0;
	virtual void PauseSound() = 0;
	virtual void ResumeSound() = 0;
	virtual void PlayMusic(int SoundID) = 0;
	virtual void PauseMusic() = 0;
	virtual void ResumeMusic() = 0;
	virtual int AddSFX(const std::string& fileName) = 0;
	virtual int AddMusic(const std::string& fileName) = 0;
};

class ConsoleAudio final : public Audio
{
	public:
		ConsoleAudio()
		{
			Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,2,4096);
		}
		~ConsoleAudio()
		{
			Mix_CloseAudio();
			for(Mix_Chunk* pChunk : m_pSFX)
			{
				Mix_FreeChunk(pChunk);
				pChunk = nullptr;
			}
			for(Mix_Music* pMusic : m_pMusic)
			{
				Mix_FreeMusic(pMusic);
				pMusic = nullptr;
			}

		}
		void PlaySound(int SoundID) override
		{
			Mix_PlayChannel(-1,m_pSFX.at(SoundID),0);
		}
		void PauseSound() override
		{
			if (Mix_Playing(1) != 0)
			{
				Mix_Pause(1);
			}
		};
		void SetVolumeSFX(int value)
		{
			for(Mix_Chunk* chunk : m_pSFX)
			{
				Mix_VolumeChunk(chunk,value);
			}
		}
		void SetVolumeMusic(int value)
		{
			Mix_VolumeMusic(value);
		}
		void ResumeSound() override
		{
			if (Mix_Paused(1) != 0)
			{
				Mix_Resume(1);
			}
		};
		void PlayMusic(int SoundID) override
		{
			Mix_PlayMusic(m_pMusic.at(SoundID),-1);
		};
		void PauseMusic() override
		{
			if (Mix_PlayingMusic() != 0)
			{
				Mix_PauseMusic();
			}
		};
		void ResumeMusic() override
		{
			if (Mix_PausedMusic() != 0)
			{
				Mix_ResumeMusic();
			}
		};
		int AddSFX(const std::string& fileName) override
		{
			Mix_Chunk* sfxToAdd = Mix_LoadWAV(fileName.c_str());
			m_pSFX.push_back(sfxToAdd);
			return int(m_pSFX.size() - 1);
		};
		int AddMusic(const std::string& fileName) override
		{
			Mix_Music* musicToAdd = Mix_LoadMUS(fileName.c_str());
			m_pMusic.push_back(musicToAdd);
			return int(m_pMusic.size() - 1);	
		};

	private:
		std::vector<Mix_Chunk*> m_pSFX;
		std::vector<Mix_Music*> m_pMusic;
	};

	class NullAudio final : public Audio
	{
	public:
		void PlaySound(int) override{}
		void PauseSound() override{}
		void ResumeSound() override{}
		void PlayMusic(int) override{};
		void PauseMusic() override{};
		void ResumeMusic() override{};
		int AddSFX(const std::string& fileName) override
		{
			Mix_Chunk* sfxToAdd = Mix_LoadWAV(fileName.c_str());
			m_pSFX.push_back(sfxToAdd);
			return int(m_pSFX.size());
		};
		int AddMusic(const std::string& fileName) override
		{
			Mix_Music* musicToAdd = Mix_LoadMUS(fileName.c_str());
			m_pMusic.push_back(musicToAdd);
			return int(m_pMusic.size());	
		};

		private:
		std::vector<Mix_Chunk*> m_pSFX;
		std::vector<Mix_Music*> m_pMusic;
	};
}
