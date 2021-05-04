#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <map>
#include <string>

struct Size {
	float width;
	float height;
};

struct Padding {
	float left;
	float top;
};

enum class MenuDirections {
	UP,
	DOWN
};

class Menu {
private:
	Size size;
	Padding padding;
	int selectedItemIndex;
	int fontSize;
	sf::Font font;
	std::vector<sf::Text> menu;
	std::map<std::string, std::function<void()>> menuFunctions;
public:
	Menu(std::string, float, float, float, float, float, bool);
	~Menu();
	void draw(sf::RenderWindow*);
	void handleEvents(sf::Event);
	void move(MenuDirections);
	void addNewItem(std::string, std::function<void()>);
	void runSelected();
	void setSelectedItemIndex(int);
	void updateItem(int, std::string);
	void updatePosition(sf::Vector2f);

};
#endif // !MENU