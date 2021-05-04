#ifndef STORAGE_H
#define STORAGE_H
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

class Storage {
private:
	std::vector<std::pair<std::string, int>> leaderBoard;
public:
	Storage();
	~Storage();
	void addData(std::string);
	std::string getStringLeaderBoard();
};
#endif // !STORAGE