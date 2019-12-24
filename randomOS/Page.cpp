#include "Page.h"

Page::Page() = default;

Page::Page(const int8_t data[]) {
	for (int i = 0; i < 16; i++)
		this->data[i] = data[i];
}

void Page::print() const {
	for (auto& x : data) {
		if (x == 0) std::cout << "_";
		else std::cout << std::to_string(x);
	}
	std::cout << std::endl;
}