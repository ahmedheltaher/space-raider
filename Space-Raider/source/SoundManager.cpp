#include "../headers/SoundManager.h"
#include <fstream>
#include <iostream>
#include <sstream>


sf::SoundBuffer generateBuffer(std::string filename) {
	sf::SoundBuffer buffer;
	buffer.loadFromFile("./sounds/sfx/" + filename);
	return buffer;
}

// Constructor
SoundManager::SoundManager(std::string musicFileName, std::string sfxFileName) {
	std::ifstream musicFile("./sounds/loaders/" + musicFileName + ".txt");
	if (musicFile.is_open()) {
		std::string line, name, source;
		while (std::getline(musicFile, line)) {
			std::stringstream ss(line);
			ss >> name >> source;
			addMusic(name, source);
		}
	}
	musicFile.close();
	std::ifstream sfxFile("./sounds/loaders/" + sfxFileName + ".txt");
	if (sfxFile.is_open()) {
		std::string line, name, source;
		while (std::getline(sfxFile, line)) {
			std::stringstream ss(line);
			ss >> name >> source;
			addSFX(name, generateBuffer(source));
		}
	}
	sfxFile.close();
	music.setLoop(true);
};

// Destructor
SoundManager::~SoundManager() {};

void SoundManager::addSFX(std::string name, sf::SoundBuffer buffer) {
	SFXBuffers[name] = buffer;
};

void SoundManager::addMusic(std::string name, std::string source) {
	musicBuffers[name] = source;
};

void SoundManager::playSFX(std::string name) {
	sound.setBuffer(SFXBuffers[name]);
	sound.play();
};

void SoundManager::playMusic(std::string name) {
	playingNow = name;
	music.openFromFile("./sounds/music/" + musicBuffers[name]);
	music.play();
};

void SoundManager::pause() {
	music.openFromFile("./sounds/music/" + musicBuffers[playingNow]);
	music.pause();
};

void SoundManager::playBack() {
	music.openFromFile("./sounds/music/" + musicBuffers[playingNow]);
	music.play();
};