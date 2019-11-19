#include "FileMenager.h"

FileMenager::FileMenager()
{
	bit_vector.fill(1);
	DiskArray.fill(0);
}

int FileMenager::createFile(std::string nazwa_pliku)
{
	if (isNameColision(nazwa_pliku)) 
	{
		return -1; //-1 istnieje plik o danej nazwie
	}
	else
	{

		File file;
		if (FindFreeBlock(&file) == -1)
		{
			return -2; //-2 brak miejsca na dysku
		}
		else
		{
			//file.s.wait();
			file.name = nazwa_pliku;
			file.PID = -1;
			file.size = 0;
			MainFileCatalog.push_back(file);
			return 1;
		}

	}
}

int FileMenager::openFile(std::string name, unsigned int PID)
{
	for (int i = 0; i < (int)MainFileCatalog.size(); i++)
	{
		if (MainFileCatalog[i].name == name)
		{
			if(isFileOpen(name,PID)) return -1;
			else
			{
				//MainFileCatalog[i].s.wait();
				MainFileCatalog[i].PID = PID;
				MainFileCatalog[i].isOpen = true;
				open_file_table.push_back(i);
				return 1;
			}
		}
	}
	return -2;
}

int FileMenager::writeToEndFile(uint16_t byte, unsigned int PID)
{	int phycial = 0;
	
	for (auto i : open_file_table)
	{
		int req = (MainFileCatalog[i].size/BlockSize); //numer logiczny bloku w ktorym bedzie sie znajdowal bajt
		if (MainFileCatalog[i].PID == PID) //sprawdza czy plik jest owtarty przez podany proces
		{
				if (MainFileCatalog[i].size % BlockSize == 0 && MainFileCatalog[i].size !=0)
				{
					if(FindFreeBlock(&MainFileCatalog[i]) == -1) return -1;
				}
				
				if (req == 0)
				{
					phycial = (MainFileCatalog[i].i_node[req]*BlockSize) + (MainFileCatalog[i].size%BlockSize);
					DiskArray[phycial] = byte;
					MainFileCatalog[i].size++;
					return 1;
				}
				else if (req == 1)
				{
					phycial = (MainFileCatalog[i].i_node[req] * BlockSize) + (MainFileCatalog[i].size % BlockSize);
					DiskArray[phycial] = byte;
					MainFileCatalog[i].size++;
					return 1;
				}
				else if (req > 1)
				{
					phycial = MainFileCatalog[i].i_node[2] * BlockSize + (req - 2);
					phycial = DiskArray[phycial];
					phycial = (phycial * BlockSize) + (MainFileCatalog[i].size % BlockSize);
					DiskArray[phycial] = byte;
					MainFileCatalog[i].size++;
					return 1;
				}
				
		}
	}
	return -2;
}

int FileMenager::writeToFile(uint8_t byte, uint8_t pos, unsigned int PID)
{
	int phycial = 0;

	for (auto i : open_file_table)
	{
		
		if (MainFileCatalog[i].PID == PID)
		{
			int req = pos / BlockSize; //numer logiczny bloku
			if(pos >= MainFileCatalog[i].size) return -1;
			if (req == 0)
			{
				phycial = (MainFileCatalog[i].i_node[0] * BlockSize) + pos;
				DiskArray[phycial] = byte;
				return 1;
			}
			else if (req == 1)
			{
				phycial = (MainFileCatalog[i].i_node[1] * BlockSize) + (pos % BlockSize);
				DiskArray[phycial] = byte;
				return 1;
			}
			else
			{
				phycial = (MainFileCatalog[i].i_node[2] * BlockSize)+(req-2);
				phycial = (DiskArray[phycial] * BlockSize) + (pos%BlockSize);
				DiskArray[phycial] = byte;
				return 1;
			}
		}
	}
	return -2;
}

int FileMenager::deleteFile(std::string name)
{
	for (int i = 0; i < (int)MainFileCatalog.size(); i++)
	{
		if (MainFileCatalog[i].name == name)
		{
			if(MainFileCatalog[i].isOpen == true) return -2;
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
				for (int k=0;k<2;k++)
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
	return -1;
}

int FileMenager::closeFile(std::string name, unsigned int PID)
{

	for (int i = 0; i < (int)open_file_table.size(); i++)
	{
		int pom = open_file_table[i];
		if (MainFileCatalog[pom].name == name && MainFileCatalog[pom].PID == PID)
		{
			MainFileCatalog[pom].isOpen = false;
			open_file_table.erase(open_file_table.begin()+i);
			//open_file_table[i]->size.signal();
			return 1;
		}
	}
	return -1;
}

int FileMenager::rename(std::string nazwa, std::string new_name)
{
	for (auto i : MainFileCatalog)
	{
		if (i.name == nazwa)
		{
			i.name = new_name;
			return 1;
		}
	}
	return -1;
}

void FileMenager::closeProcessFiles(unsigned int PID)
{
	for (auto i : open_file_table)
	{
		if(MainFileCatalog[i].PID == PID) closeFile(MainFileCatalog[i].name,MainFileCatalog[i].PID);
	}
}

std::string FileMenager::cat(std::string name)
{
	std::string result;
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
					result.push_back(DiskArray[physical]);
				}
				else if (req < 64)
				{
					physical = i.i_node[1] * BlockSize + (req % BlockSize);
					result.push_back(DiskArray[physical]);
				}
				else
				{
					physical = i.i_node[2] * BlockSize + ((req/BlockSize) -2);
					physical = DiskArray[physical];
					physical = (physical * BlockSize) + (req % BlockSize);
					result.push_back(DiskArray[physical]);
				}
				req++;
			}

		}
	}

	return result;
}

std::vector<std::string> FileMenager::ls()
{
	std::vector<std::string> result;
	for(auto i:MainFileCatalog) result.push_back(i.name);
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

int FileMenager::FindFreeBlock(File *file)
{
	if (file->i_node.size() < 2)
	{
		for (int i=0;i<(int)bit_vector.size();i++)
		{
			if (bit_vector[i] == 1)
			{
				file->i_node.push_back(i);
				bit_vector[i] = 0;
				return 1;
			}
		}
	}
	else if(file->i_node.size() == 2)
	{
		for (int i = 0; i <(int)bit_vector.size(); i++)
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
		for (int i = 0; i <(int)bit_vector.size(); i++)
		{
			if (bit_vector[i] == 1)
			{
				int pom = (file->size / BlockSize) - 2;
				pom = (file->i_node[2] * BlockSize) + pom;
				DiskArray[pom] = i;
				bit_vector[i] = 0;
				return 1;
			}
		}
	}
	return -1;
}

bool isNameColision(std::string name)
{
	for (auto i : MainFileCatalog)
	{
		if(i.name == name) return true;
	}
	return false;
}

bool isFileOpen(std::string name, int PID)
{
	for (auto i : open_file_table)
	{
		if(MainFileCatalog[i].name == name && MainFileCatalog[i].PID != PID) return true;
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

	for(auto i:MainFileCatalog)
		std::cout << i.name << "\n";
}