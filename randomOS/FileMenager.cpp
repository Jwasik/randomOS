#include "FileMenager.h"

FileMenager::FileMenager()
{
	bit_vector.fill(1);
	DiskArray.fill(0);
}

int8_t FileMenager::createFile(std::string nazwa_pliku)
{
	if (isNameColision(nazwa_pliku))
	{
		return ERROR_ALREADY_EXISTING_FILE; //istnieje plik o danej nazwie
	}
	else
	{

		File file;
		if (FindFreeBlock(&file) == -1)
		{
			return ERROR_NO_SPACE_ON_DISK; //brak miejsca na dysku
		}
		else
		{
			//file.s.wait();
			file.name = nazwa_pliku;
			file.PID = -1;
			file.size = 0;
			MainFileCatalog.push_back(file);
			return 0;
		}

	}
}

int8_t FileMenager::openFile(std::string name, unsigned int PID)
{
	for (int i = 0; i < (int)MainFileCatalog.size(); i++)
	{
		if (MainFileCatalog[i].name == name)
		{
			//Sprawdzenie tego za pomoca semaforow
			if (isFileOpen(name, PID)) return ERROR_FILE_OPENED_BY_OTHER_PROCESS;
			else
			{
				//MainFileCatalog[i].s.wait();
				MainFileCatalog[i].PID = PID;
				MainFileCatalog[i].isOpen = true;
				open_file_table.push_back(i);
				return 0;
			}
		}
	}
	return ERROR_NO_FILE_WITH_THAT_NAME;
}

int8_t FileMenager::writeToEndFile(uint16_t byte, unsigned int PID)
{
	int phycial = 0;

	for (auto i : open_file_table)
	{
		int req = (MainFileCatalog[i].size / BlockSize); //numer logiczny bloku w ktorym bedzie sie znajdowal bajt
		if (MainFileCatalog[i].PID == PID) //sprawdza czy plik jest owtarty przez podany proces
		{
			if (MainFileCatalog[i].size % BlockSize == 0 && MainFileCatalog[i].size != 0)
			{
				if (FindFreeBlock(&MainFileCatalog[i]) == -1) return ERROR_NO_SPACE_ON_DISK;
			}

			if (req == 0)
			{
				phycial = (MainFileCatalog[i].i_node[req] * BlockSize) + (MainFileCatalog[i].size % BlockSize);
				DiskArray[phycial] = byte;
				MainFileCatalog[i].size++;
				return 0;
			}
			else if (req == 1)
			{
				phycial = (MainFileCatalog[i].i_node[req] * BlockSize) + (MainFileCatalog[i].size % BlockSize);
				DiskArray[phycial] = byte;
				MainFileCatalog[i].size++;
				return 0;
			}
			else if (req > 1)
			{
				phycial = MainFileCatalog[i].i_node[2] * BlockSize + (req - 2);
				phycial = DiskArray[phycial];
				phycial = (phycial * BlockSize) + (MainFileCatalog[i].size % BlockSize);
				DiskArray[phycial] = byte;
				MainFileCatalog[i].size++;
				return 0;
			}

		}
	}
	return ERROR_FILE_IS_NOT_OPENED;
}

int8_t FileMenager::writeToFile(uint8_t byte, uint8_t pos, unsigned int PID)
{
	int phycial = 0;

	for (auto i : open_file_table)
	{

		if (MainFileCatalog[i].PID == PID)
		{
			int req = pos / BlockSize; //numer logiczny bloku
			if (pos >= MainFileCatalog[i].size) return ERROR_UOT_OF_FILE_RANGE;
			if (req == 0)
			{
				phycial = (MainFileCatalog[i].i_node[0] * BlockSize) + pos;
				DiskArray[phycial] = byte;
				return 0;
			}
			else if (req == 1)
			{
				phycial = (MainFileCatalog[i].i_node[1] * BlockSize) + (pos % BlockSize);
				DiskArray[phycial] = byte;
				return 0;
			}
			else
			{
				phycial = (MainFileCatalog[i].i_node[2] * BlockSize) + (req - 2);
				phycial = (DiskArray[phycial] * BlockSize) + (pos % BlockSize);
				DiskArray[phycial] = byte;
				return 0;
			}
		}
	}
	return ERROR_FILE_IS_NOT_OPENED;
}

int8_t FileMenager::readFile(uint8_t addr, uint8_t pos, unsigned int n, unsigned int PID)
{
	int phycial = 0;
	int curr_pos = pos;
	int ret = 0;
	for (auto i : open_file_table)
	{
		if (MainFileCatalog[i].PID == PID)
		{
			if ((pos + n) > MainFileCatalog[i].size - pos) return ERROR_UOT_OF_FILE_RANGE;
			ret = 1;
			while (curr_pos < pos + n)
			{
				if (curr_pos < 32)
				{
					phycial = (MainFileCatalog[i].i_node[0] * BlockSize) + (curr_pos % BlockSize);
					std::cout << DiskArray[phycial]; // tu write do ramu
				}
				else if (curr_pos < 64)
				{
					phycial = (MainFileCatalog[i].i_node[1] * BlockSize) + (curr_pos % BlockSize);
					std::cout << DiskArray[phycial]; // tu write do ramu
				}
				else
				{
					phycial = (MainFileCatalog[i].i_node[2] * BlockSize) + (curr_pos / BlockSize) - 2;
					phycial = DiskArray[phycial];
					phycial = (phycial * BlockSize) + (curr_pos % BlockSize);
					std::cout << DiskArray[phycial]; // tu write do ramu
				}
				curr_pos++;
			}
		}
	}
	if (ret == 1) return 0;
	else return ERROR_FILE_IS_NOT_OPENED;
}

int8_t FileMenager::deleteFile(std::string name)
{
	for (int i = 0; i < (int)MainFileCatalog.size(); i++)
	{
		if (MainFileCatalog[i].name == name)
		{
			if (MainFileCatalog[i].isOpen == true) return ERROR_FILE_IS_OPENED_CANT_DELETE;
			int req = MainFileCatalog[i].size / BlockSize;
			int physical = MainFileCatalog[i].i_node[2] * BlockSize;
			if (MainFileCatalog[i].i_node.size() < 3)
			{
				for (auto k : MainFileCatalog[i].i_node)
				{
					clearBlock(k);
				}
			}
			else
			{
				for (int k = 0; k < 2; k++)
				{
					clearBlock(MainFileCatalog[i].i_node[k]);
				}
				for (int k = 0; k < req - 1; k++)
				{
					clearBlock(DiskArray[physical]);
					physical++;
				}
				clearBlock(MainFileCatalog[i].i_node[2]);
			}

			MainFileCatalog.erase(MainFileCatalog.begin() + i);
		}
	}
	return 0;
}

int8_t FileMenager::closeFile(std::string name, unsigned int PID)
{

	for (int i = 0; i < (int)open_file_table.size(); i++)
	{
		int pom = open_file_table[i];
		if (MainFileCatalog[pom].name == name && MainFileCatalog[pom].PID == PID)
		{
			MainFileCatalog[pom].isOpen = false;
			open_file_table.erase(open_file_table.begin() + i);
			//open_file_table[i]->size.signal();
			return 0;
		}
	}
	return ERROR_FILE_IS_NOT_OPENED;
}

int8_t FileMenager::rename(std::string nazwa, std::string new_name)
{
	for (auto i : MainFileCatalog)
	{
		if (i.name == nazwa)
		{
			i.name = new_name;
			return 0;
		}
	}
	return ERROR_NO_FILE_WITH_THAT_NAME;
}

void FileMenager::closeProcessFiles(unsigned int PID)
{
	for (auto i : open_file_table)
	{
		if (MainFileCatalog[i].PID == PID) closeFile(MainFileCatalog[i].name, MainFileCatalog[i].PID);
	}
}

std::pair<int8_t, std::string> FileMenager::cat(std::string name)
{
	std::pair<int8_t, std::string> result;
	for (auto i : MainFileCatalog)
	{
		if (i.name == name)
		{
			int req = 0;
			int physical;
			while (req < i.size)
			{
				if (req < 32)
				{
					physical = i.i_node[1] * BlockSize + (req % BlockSize);
					result.second.push_back(DiskArray[physical]);
				}
				else if (req < 64)
				{
					physical = i.i_node[1] * BlockSize + (req % BlockSize);
					result.second.push_back(DiskArray[physical]);
				}
				else
				{
					physical = i.i_node[2] * BlockSize + ((req / BlockSize) - 2);
					physical = DiskArray[physical];
					physical = (physical * BlockSize) + (req % BlockSize);
					result.second.push_back(DiskArray[physical]);
				}
				req++;
			}

		}
		else result.first = ERROR_NO_FILE_WITH_THAT_NAME;
	}
	result.first = 0;
	return result;
}

std::vector<std::string> FileMenager::ls()
{
	std::vector<std::string> result;
	for (auto i : MainFileCatalog) result.push_back(i.name);
	return result;
}

void clearBlock(int log)
{
	bit_vector[log] = 1;
	int physical = log * BlockSize;
	int pom;
	for (int i = 0; i < 32; i++)
	{
		pom = physical + i;
		DiskArray[pom] = 0;
	}
}

int FileMenager::FindFreeBlock(File* file)
{
	if (file->i_node.size() < 2)
	{
		for (int i = 0; i < (int)bit_vector.size(); i++)
		{
			if (bit_vector[i] == 1)
			{
				file->i_node.push_back(i);
				bit_vector[i] = 0;
				return 0;
			}
		}
	}
	else if (file->i_node.size() == 2)
	{
		for (int i = 0; i < (int)bit_vector.size(); i++)
		{
			if (bit_vector[i] == 1)
			{
				file->i_node.push_back(i);
				bit_vector[i] = 0;
				FindFreeBlock(file);
				return 2;
			}
		}
	}
	else if (file->i_node.size() > 2)
	{
		for (int i = 0; i < (int)bit_vector.size(); i++)
		{
			if (bit_vector[i] == 1)
			{
				int pom = (file->size / BlockSize) - 2;
				pom = (file->i_node[2] * BlockSize) + pom;
				DiskArray[pom] = i;
				bit_vector[i] = 0;
				return 0;
			}
		}
	}
	return -1;
}

bool isNameColision(std::string name)
{
	for (auto i : MainFileCatalog)
	{
		if (i.name == name) return true;
	}
	return false;
}

bool isFileOpen(std::string name, int PID)
{
	for (auto i : open_file_table)
	{
		if (MainFileCatalog[i].name == name && MainFileCatalog[i].PID != PID) return true;
	}
	return false;
}

void ShowMemory()
{
	int licz = 0;
	for (auto i : DiskArray)
	{

		std::cout << licz << " " << i << "\n";
		licz++;
	}
}

void showBitVector()
{
	int licz = 0;
	for (auto i : bit_vector)
	{
		std::cout << " " << i << " ";
		licz++;
	}
	std::cout << "\n\n\n";

	for (auto i : MainFileCatalog)
		std::cout << i.name << "\n";
}