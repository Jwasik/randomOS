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

