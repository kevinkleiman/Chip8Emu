#pragma once

#include <stdint.h>

namespace display {

	bool init();

	void updateDisplay();
	unsigned int fastCeil(unsigned int x, unsigned int y);
	void setColor(int r, int g, int b, int a);
	void cleanup();
}
