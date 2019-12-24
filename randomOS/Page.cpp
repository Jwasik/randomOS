
int8_t Page::writeToPage(const unsigned int & index, int8_t byte)
{
	if (index > PAGE_SIZE-1 || index<0) { return ERROR_PG_INDEX_OUT_OF_PAGE; }
	this->data[index] = byte;
	return 0;
}

