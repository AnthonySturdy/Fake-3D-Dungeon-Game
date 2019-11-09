#include "Pacman.h"
#include "DungeonCrawler.h";
#include <iostream>

//Entry Point for Application
int main(int argc, char* argv[]) {
	int input;
	std::cout << "1. Pacman (Demo project)\n2. Dungeon Ninja (Main game)\n\n> ";
	std::cin >> input;

	if (input == 1)
		Pacman* game = new Pacman(argc, argv);
	else if (input == 2)
		DungeonCrawler* game = new DungeonCrawler(argc, argv);
}