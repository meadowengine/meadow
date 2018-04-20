#ifndef MEADOW_GAME_H_
#define MEADOW_GAME_H_

#include <vector>
#include <string>

namespace Meadow {
	class Game {
	public:
		const char *title = "Untitled Game";
		int width = 800;
		int height = 600;
		std::vector<std::string> args;
	};
}

extern void setup(Meadow::Game &game);

#endif