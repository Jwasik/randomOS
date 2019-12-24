#define ERROR_PG_INDEX_OUT_OF_PAGE 50
#define PAGE_SIZE 16
	//writes given byte on a given index of the page, return 0 fror successfull writing, 50 for outOfPage index
	int8_t writeToPage(const unsigned int& index, int8_t byte); 
