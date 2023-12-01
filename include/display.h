#pragma once

#include <stdint.h>

namespace display {

	bool init();

	void updateDisplay();
	void setColor(int r, int g, int b, int a);
	void cleanup();
}
