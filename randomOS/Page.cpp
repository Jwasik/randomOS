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
Page::Page(std::vector<uint8_t> data)
{
	for (int i = 0; i < data.size() && i<PAGE_SIZE; i++)
		this->data[i] = data[i];
}

int8_t Page::writeToPage(const unsigned int & index, int8_t byte)
{
	if (index > PAGE_SIZE-1 || index<0) { return ERROR_PG_INDEX_OUT_OF_PAGE; }
	this->data[index] = byte;
	return 0;
}

