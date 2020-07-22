#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include "Timer.h"
class Music {
public:
	Music(std::string fileName) {
		
		/*SDL_Init(SDL_INIT_AUDIO);
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
			std::cout << "error\n";
		}*/
		music = Mix_LoadMUS(fileName.c_str());
	}
	void play() {
		Mix_PlayMusic(music, -1);
	}
	void stop() {
		Mix_HaltMusic();
	}
	void setFile(std::string file) {
		music = Mix_LoadMUS(file.c_str());
	}
	~Music() {
		Mix_FreeMusic(music);
		music = nullptr;
		Mix_Quit();
	}
private:
	Mix_Music* music;
};
class Sound {
public:
	Sound(std::string fileName) {
		Timer t("creating sound");
		chunk = Mix_LoadWAV(fileName.c_str());

	}
	void setFile(std::string file) {
		chunk = Mix_LoadWAV(file.c_str());
		
	}
	Sound() {}
	void play() {
		Mix_PlayChannel(-1, chunk, 0);
	}
	~Sound() {
		Mix_FreeChunk(chunk);
		chunk = nullptr;
		Mix_Quit();
	}
private:
	Mix_Chunk* chunk;
};