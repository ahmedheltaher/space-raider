#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <vector>
#include <string>

class SoundManager
{
private:
	std::map<std::string, sf::SoundBuffer> SFXBuffers;
	std::map<std::string, std::string> musicBuffers;
	sf::Sound sound;
	sf::Music music;
	std::string playingNow;
public:
	SoundManager(std::string, std::string);
	~SoundManager();
	void addSFX(std::string, sf::SoundBuffer);
	void addMusic(std::string, std::string);
	void playSFX(std::string);
	void playMusic(std::string);
	void pause();
	void playBack();
};

#endif // !SOUNDMANAGER
