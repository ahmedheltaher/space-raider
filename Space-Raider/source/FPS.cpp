#include "../headers/FPS.h"

// Constructor
FPS::FPS() : frame(0), fps(0) {};

// Destructor
FPS::~FPS() {};

// Getter for fps
unsigned int FPS::getFPS() {
	return fps;
};

void FPS::update() {
	if (clock.getElapsedTime().asSeconds() >= 1.f) {
		fps = frame;
		frame = 0;
		clock.restart();
	}
	++frame;
};
