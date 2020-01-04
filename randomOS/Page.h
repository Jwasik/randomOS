#pragma once
#include "Includes.h"

#define ERROR_PG_INDEX_OUT_OF_PAGE 50
#define PAGE_SIZE 16

class Page {
public:
	int8_t data[PAGE_SIZE]{ 0 }; // page data, initialized with zeroes
	Page();
	Page(std::vector<int8_t> data);
	explicit Page(const int8_t data[]);
	void print() const;

	//writes given byte on a given index of the page, return 0 fror successfull writing, 50 for outOfPage index
	int8_t writeToPage(const unsigned int& index, int8_t byte);
};



	
	 
