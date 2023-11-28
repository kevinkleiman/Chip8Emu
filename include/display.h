#pragma once

#include <stdint.h>

namespace display {

	bool init();

	void updateDisplay();
	void setFBColor(uint32_t rgba, int layer);
	void cleanup();
}
