#include "FileMenager.h"

std::vector<File> Containers::Containers::MainFileCatalog;
std::array<int, DiskSize / BlockSize> Containers::Containers::bit_vector;
std::array<char, DiskSize> Containers::Containers::DiskArray;
std::vector<int> Containers::Containers::open_file_table;
std::array<std::string, DiskSize / BlockSize> Containers::BitVectorWithFiles;

FileMenager::FileMenager()
{
	Containers::bit_vector.fill(1);
	Containers::DiskArray.fill(0);
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
		file.name = nazwa_pliku;
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
			Containers::MainFileCatalog.push_back(file);
			return 0;
		}

	}
}

int8_t FileMenager::openFile(std::string name, unsigned int PID)
{
	for (int i = 0; i < (int)Containers::MainFileCatalog.size(); i++)
	{
		if (Containers::MainFileCatalog[i].name == name)
		{
			//Sprawdzenie tego za pomoca semaforow
			if (isFileOpen(name, PID)) return ERROR_FILE_OPENED_BY_OTHER_PROCESS;
			else
			{
				//Containers::MainFileCatalog[i].s.wait();
				Containers::MainFileCatalog[i].PID = PID;
				Containers::MainFileCatalog[i].isOpen = true;
				Containers::open_file_table.push_back(i);
				return 0;
			}
		}
	}
	return ERROR_NO_FILE_WITH_THAT_NAME;
}

int8_t FileMenager::writeToEndFile(uint16_t byte, unsigned int PID)
{	int phycial = 0;
	
	for (int i=0;i< Containers::open_file_table.size();i++)
	{
		File* t = &Containers::MainFileCatalog[Containers::open_file_table[i]];
		int req = (t->size/BlockSize); //numer logiczny bloku w ktorym bedzie sie znajdowal bajt
		if (t->PID == PID) //sprawdza czy plik jest owtarty przez podany proces
		{
				if (t->size % BlockSize == 0 && t->size !=0)
				{
					if(FindFreeBlock(t) == -1) return ERROR_NO_SPACE_ON_DISK;
				}
				
				if (req == 0)
				{
					phycial = (t->i_node[req]*BlockSize) + (t->size%BlockSize);
					Containers::DiskArray[phycial] = byte;
					t->size++;
					return 0;
				}
				else if (req == 1)
				{
					phycial = (t->i_node[req] * BlockSize) + (t->size % BlockSize);
					Containers::DiskArray[phycial] = byte;
					t->size++;
					return 0;
				}
				else if (req > 1)
				{
					phycial = t->i_node[2] * BlockSize + (req - 2);
					phycial = Containers::DiskArray[phycial];
					phycial = (phycial * BlockSize) + (t->size % BlockSize);
					Containers::DiskArray[phycial] = byte;
					t->size++;
					return 0;
				}
				
		}
	}
	return ERROR_FILE_IS_NOT_OPENED;
}

int8_t FileMenager::append(std::string name, uint16_t byte)
{
	int8_t ret;
	ret = openFile(name,0);
	if(ret != 0) return ret;
	ret = writeToEndFile(byte,0);
	if (ret != 0) return ret;
	ret = closeFile(name,0);
	return ret;
}

int8_t FileMenager::writeToFile(uint8_t byte, uint8_t pos, unsigned int PID)
{
	int phycial = 0;

	for (auto i : Containers::open_file_table)
	{
		File *t = &Containers::MainFileCatalog[Containers::open_file_table[i]];
		if (t->PID == PID)
		{
			int req = pos / BlockSize; //numer logiczny bloku
			if(pos >= t->size) return ERROR_UOT_OF_FILE_RANGE;
			if (req == 0)
			{
				phycial = (t->i_node[0] * BlockSize) + pos;
				Containers::DiskArray[phycial] = byte;
				return 0;
			}
			else if (req == 1)
			{
				phycial = (t->i_node[1] * BlockSize) + (pos % BlockSize);
				Containers::DiskArray[phycial] = byte;
				return 0;
			}
			else
			{
				phycial = (t->i_node[2] * BlockSize)+(req-2);
				phycial = (Containers::DiskArray[phycial] * BlockSize) + (pos%BlockSize);
				Containers::DiskArray[phycial] = byte;
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
	for (auto i : Containers::open_file_table)
	{
		if (Containers::MainFileCatalog[i].PID == PID)
		{
			if((pos + n) > Containers::MainFileCatalog[i].size - pos) return ERROR_UOT_OF_FILE_RANGE;
			ret = 1;
			while (curr_pos < pos + n)
			{
				if (curr_pos < 32)
				{
					phycial = (Containers::MainFileCatalog[i].i_node[0] * BlockSize) + (curr_pos%BlockSize);
					std::cout << Containers::DiskArray[phycial]; // tu write do ramu
				}
				else if (curr_pos < 64)
				{
					phycial = (Containers::MainFileCatalog[i].i_node[1] * BlockSize) + (curr_pos % BlockSize);
					std::cout << Containers::DiskArray[phycial]; // tu write do ramu
				}
				else
				{
					phycial = (Containers::MainFileCatalog[i].i_node[2] * BlockSize) + (curr_pos/BlockSize) - 2;
					phycial = Containers::DiskArray[phycial];
					phycial = (phycial * BlockSize) + (curr_pos % BlockSize);
					std::cout << Containers::DiskArray[phycial]; // tu write do ramu
				}
				curr_pos++;
			}
		}
	}
	if(ret == 1) return 0;
	else return ERROR_FILE_IS_NOT_OPENED;
}

int8_t FileMenager::deleteFile(std::string name)
{
	for (int i = 0; i < (int)Containers::MainFileCatalog.size(); i++)
	{
		if (Containers::MainFileCatalog[i].name == name)
		{
			if(Containers::MainFileCatalog[i].isOpen == true) return ERROR_FILE_IS_OPENED_CANT_DELETE;
			int req = Containers::MainFileCatalog[i].size / BlockSize;
			
			if (Containers::MainFileCatalog[i].i_node.size() < 3)
			{
				for (auto k : Containers::MainFileCatalog[i].i_node)
				{
					clearBlock(k);
				}
				if (i > 0) Containers::MainFileCatalog.erase(Containers::MainFileCatalog.begin() + i - 1);
				else Containers::MainFileCatalog.erase(Containers::MainFileCatalog.begin());
				return 0;
			}
			else
			{
				int physical = Containers::MainFileCatalog[i].i_node[2] * BlockSize;
				for (int k=0;k<2;k++)
				{
					clearBlock(Containers::MainFileCatalog[i].i_node[k]);
				}
				for (int k = 0; k < req - 1; k++)
				{
					clearBlock(Containers::DiskArray[physical]);
					physical++;
				}
				clearBlock(Containers::MainFileCatalog[i].i_node[2]);
				if(i > 0) Containers::MainFileCatalog.erase(Containers::MainFileCatalog.begin() + i - 1);
				else Containers::MainFileCatalog.erase(Containers::MainFileCatalog.begin());
				
				return 0;
			}

		}
		return ERROR_NO_FILE_WITH_THAT_NAME;
	}
	return 0;
}

int8_t FileMenager::closeFile(std::string name, unsigned int PID)
{

	for (int i = 0; i < (int)Containers::open_file_table.size(); i++)
	{
		int pom = Containers::open_file_table[i];
		if (Containers::MainFileCatalog[pom].name == name && Containers::MainFileCatalog[pom].PID == PID)
		{
			Containers::MainFileCatalog[pom].isOpen = false;
			Containers::open_file_table.erase(Containers::open_file_table.begin()+i);
			//Containers::open_file_table[i]->size.signal();
			return 0;
		}
	}
	return ERROR_FILE_IS_NOT_OPENED;
}

int8_t FileMenager::rename(std::string nazwa, std::string new_name)
{
	for (auto & i : Containers::MainFileCatalog)
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
	for (auto i : Containers::open_file_table)
	{
		if(Containers::MainFileCatalog[i].PID == PID) closeFile(Containers::MainFileCatalog[i].name,Containers::MainFileCatalog[i].PID);
	}
}

std::pair<int8_t, std::string> FileMenager::cat(std::string name)
{
	std::pair<int8_t, std::string> result;
	for (auto i : Containers::MainFileCatalog)
	{
		if (i.name == name)
		{
			int req = 0;
			int physical;
			while (req < i.size)
			{
				if (req < 32)
				{
					physical = i.i_node[0] * BlockSize + (req % BlockSize);
					result.second.push_back(Containers::DiskArray[physical]);
				}
				else if (req < 64)
				{
					physical = i.i_node[1] * BlockSize + (req % BlockSize);
					result.second.push_back(Containers::DiskArray[physical]);
				}
				else
				{
					physical = i.i_node[2] * BlockSize + ((req/BlockSize) -2);
					physical = Containers::DiskArray[physical];
					physical = (physical * BlockSize) + (req % BlockSize);
					result.second.push_back(Containers::DiskArray[physical]);
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
	for(auto i:Containers::MainFileCatalog) result.push_back(i.name);
	return result;
}

int8_t FileMenager::clearFile(std::string name)
{
	int8_t ret;
	ret = deleteFile(name);
	if(ret !=0) return ret;
	ret = createFile(name);
	return ret;
}

std::pair <int8_t, int> FileMenager::wc(std::string name)
{
	std::pair <int8_t, int> res;
	for (int i = 0; i < (int)Containers::MainFileCatalog.size(); i++)
	{
		if (Containers::MainFileCatalog[i].name == name)
		{
			res.second = Containers::MainFileCatalog[i].size;
			res.first = 0;
			return res;
		}
	}
	res.first = ERROR_NO_FILE_WITH_THAT_NAME;
	return res;
}


void clearBlock(int log)
{
	Containers::bit_vector[log] = 1;
	Containers::BitVectorWithFiles[log] = "";
	int physical = log * BlockSize;
	int pom;
	for (int i = 0; i < 32; i++)
	{
		pom = physical + i;
		Containers::DiskArray[pom] = 0;
	}
}

int FileMenager::FindFreeBlock(File* file)
{
	if (file->i_node.size() < 2)
	{
		for (int i=0;i<(int)Containers::bit_vector.size();i++)
		{
			if (Containers::bit_vector[i] == 1)
			{
				file->i_node.push_back(i);
				Containers::bit_vector[i] = 0;
				Containers::BitVectorWithFiles[i] = file->name;
				return 0;
			}
		}
	}
	else if (file->i_node.size() == 2)
	{
		for (int i = 0; i <(int)Containers::bit_vector.size(); i++)
		{
			if (Containers::bit_vector[i] == 1)
			{
				file->i_node.push_back(i);
				Containers::bit_vector[i] = 0;
				Containers::BitVectorWithFiles[i] = file->name;
				FindFreeBlock(file);
				return 2;
			}
		}
	}
	else if (file->i_node.size() > 2)
	{
		for (int i = 0; i <(int)Containers::bit_vector.size(); i++)
		{
			if (Containers::bit_vector[i] == 1)
			{
				int pom = (file->size / BlockSize) - 2;
				pom = (file->i_node[2] * BlockSize) + pom;
				Containers::DiskArray[pom] = i;
				Containers::bit_vector[i] = 0;
				Containers::BitVectorWithFiles[i] = file->name;
				return 0;
			}
		}
	}
	return -1;
}

bool isNameColision(std::string name)
{
	for (auto i : Containers::MainFileCatalog)
	{
		if (i.name == name) return true;
	}
	return false;
}

bool isFileOpen(std::string name, int PID)
{
	for (auto i : Containers::open_file_table)
	{
		if(Containers::MainFileCatalog[i].name == name && Containers::MainFileCatalog[i].PID != PID) return true;
	}
	return false;
}

void ShowMemory()
{
	int licz = 0;
	for (auto i : Containers::DiskArray)
	{

		std::cout << licz << " " << i << "\n";
		licz++;
	}
}

void showBitVector()
{
	int licz = 0;
	for (auto i : Containers::bit_vector)
	{
		std::cout << " " << i << " ";
		licz++;
	}
	std::cout << "\n\n\n";
}