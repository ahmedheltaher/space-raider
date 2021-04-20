#ifndef FPS_H
#define FPS_H
#include <SFML/Graphics.hpp>

class FPS {
private:
	unsigned int frame;
	unsigned int fps;
	sf::Clock clock;

public:
	FPS();
	~FPS();
	unsigned int getFPS();
	void update();
};

#endif // !FPS