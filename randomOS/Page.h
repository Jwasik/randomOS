#pragma once
#include "Includes.h"

class Page {
public:
	int8_t data[16]{ 0 }; // page data, initialized with zeroes
	Page();
	explicit Page(const int8_t data[]);
	void print() const;
};

