#include "FileMenager.h"

std::vector<File> Containers::Containers::MainFileCatalog;
std::array<int, DiskSize / BlockSize> Containers::Containers::bit_vector;
std::array<char, DiskSize> Containers::Containers::DiskArray;
std::vector<int> Containers::Containers::open_file_table;
std::array<std::string, DiskSize / BlockSize> Containers::BitVectorWithFiles;
std::vector<std::pair<std::string, unsigned int>> Containers::Colors;

FileMenager::FileMenager(std::shared_ptr<Memory> memory) : memory(memory)
{
	Containers::bit_vector.fill(1);
	Containers::DiskArray.fill(0);
}

File::File()
{
	s = Semaphore(0);
}

int8_t FileMenager::createFile(std::string nazwa_pliku)
{
	if (nazwa_pliku.size() != 2) return ERROR_BAD_NAME_SIZE;
	if (isNameColision(nazwa_pliku))//sprawdzam czy nie istnieje ju� plik o podanej nazwie
	{
		return ERROR_ALREADY_EXISTING_FILE; //istnieje plik o danej nazwie
	}
	else//je�eli nie istnieje plik o podanej nazwie przechodz� do tworzenia
	{

		File file;
		file.name = nazwa_pliku;
		if (FindFreeBlock(&file) == -1)//wywo�uj� funkcj� kt�ra dla danego pliku przydziela wolny blok a w przypadku braku wolnych blok�w zwraca -1
		{
			return ERROR_NO_SPACE_ON_DISK; //brak miejsca na dysku
		}
		else//je�eli blok zosta� przydzielony ustawiam domy�lne warto�ci FCB i dodaje utworzony plik do katalogu plik�w
		{
			Containers::Colors.push_back({ nazwa_pliku,color });
			this->color++;
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
	for (int i = 0; i < (int)Containers::MainFileCatalog.size(); i++)//iterujemy po g�owynm katalogu
	{
		if (Containers::MainFileCatalog[i].name == name)//szukamy pliku o podanej nazwie
		{
			if (isFileOpen(name, PID)) return ERROR_FILE_OPENED_BY_OTHER_PROCESS;//sprawdza czy plik nie jest otwarty przez inny proes
			else//je�eli nie to ustawiam PID na te jakie odsta�em i dodaje plik do tabilcy otwartych plik�w
			{
				Containers::MainFileCatalog[i].s.wait();
				Containers::MainFileCatalog[i].PID = PID;
				Containers::MainFileCatalog[i].isOpen = true;
				Containers::open_file_table.push_back(i);
				return 0;
			}
		}
	}
	return ERROR_NO_FILE_WITH_THAT_NAME;
}

int8_t FileMenager::writeToEndFile(uint16_t byte, unsigned int PID, std::string name)
{
	int phycial = 0; //zmienna przechowuj�ca adres pod ktory zapisze bajt

	for (int i = 0; i < Containers::open_file_table.size(); i++)//iteracja po tablicy otwartych plik�w
	{
		File* t = &Containers::MainFileCatalog[Containers::open_file_table[i]];//pomocniczne
		
		int req = ((t->size) / BlockSize); //numer logiczny bloku w ktorym bedzie sie znajdowal bajt 
		int pom = ((t->size-1) / BlockSize);//numer logiczny poprzedniego zapisanego bajtu
		if (t->PID == PID && t->name == name) //sprawdza czy plik jest owtarty przez podany proces
		{
			if (req!=pom)//sprawdzam czy b�d� potrzebowa� nowego bloku
			{
				if (FindFreeBlock(t) == -1)//szukam wolnego bloku przekazuj�c *plik dla ktorego ma by� znaleziony blok w przypadku braku blok�w wyrzucam b��d
				{
					t->size--;
					return ERROR_NO_SPACE_ON_DISK;
				}

			}
			if (req == 0)//je�eli baj b�dzie si� znajdowa� w 0 bloku (blok bezpo�redni)
			{
				phycial = (t->i_node[req] * BlockSize) + ((t->size)%BlockSize);
				Containers::DiskArray[phycial] = byte;
				t->size++;
				return 0;
			}
			else if (req == 1)//je�eli bajt b�dzie si� znajdwa� w 1 bloku (blok bezpo�redni)
			{
				phycial = (t->i_node[req] * BlockSize) + (t->size % BlockSize);
				Containers::DiskArray[phycial] = byte;
				t->size++;
				return 0;
			}
			else if (req > 1)//je�eli bajt b�dzie si� znajdowa� w 2 lub wi�kszym bloku
			{
				phycial = t->i_node[2] * BlockSize + (req - 2); //pobieram z i_node[2] numer bloku indeksowego, przeliczam numer na adres fizyczny i obliczam nr_indexu w bloku indexowym
				phycial = Containers::DiskArray[phycial];//pobieram numer bloku do kt�rego b�d� zapisywa� dane
				phycial = (phycial * BlockSize) + (t->size % BlockSize);//zamieniam go na adres i dodaje pzresuni�cie (kt�ry bajt danego bloku)
				Containers::DiskArray[phycial] = byte;//zapisuj� na dysku bajt
				t->size++;
				return 0;
			}

		}
		//je�eli nie to wywalam b��d
	}
	return ERROR_FILE_IS_NOT_OPENED;
}

int8_t FileMenager::append(std::string name, uint16_t byte)
{
	int8_t ret;
	ret = openFile(name, 0);
	if (ret != 0) return ret;
	ret = writeToEndFile(byte, 0,name);
	if (ret != 0)
	{
		closeFile(name, 0);
		return ret;
	}
	ret = closeFile(name, 0);
	return ret;
}

int8_t FileMenager::writeToFile(uint8_t byte, uint8_t pos, unsigned int PID,std::string name)
{
	int phycial = 0;

	for (auto i : Containers::open_file_table)//iteruje po tablicy otawrtych plik�w
	{
		File *t = &Containers::MainFileCatalog[Containers::open_file_table[i]];
		if (t->PID == PID && t->name == name)
		{
			int req = pos / BlockSize; //numer  bloku
			if (pos >= t->size) return ERROR_UOT_OF_FILE_RANGE;//sprawdzam czy nie wyszlismy poza zakres pliku
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
				phycial = (t->i_node[2] * BlockSize) + (req - 2);
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
	for (auto i : Containers::open_file_table)//itereacja po tablicy otawrtych plik�w
	{
		File* t = &Containers::MainFileCatalog[Containers::open_file_table[i]];//pomoc
		if (t->PID == PID)//sprawdzenie czy plik jest otwarty przez podany proces
		{
			if ((pos + n) > t->size - pos) return ERROR_UOT_OF_FILE_RANGE;//sprawdzenie czy nie wychodzimy poza zakres pliku
			ret = 1;//zmienia pomocnicza do stwierdzenia czy plik jest otawrty czy nie
			while (curr_pos < pos + n)
			{
				if (curr_pos < 32)
				{
					phycial = (t->i_node[0] * BlockSize) + (curr_pos%BlockSize);
					memory->writeInMem(PID, addr, Containers::DiskArray[phycial]);
				}
				else if (curr_pos < 64)
				{
					phycial = (t->i_node[1] * BlockSize) + (curr_pos % BlockSize);
					memory->writeInMem(PID, addr, Containers::DiskArray[phycial]);
				}
				else
				{
					phycial = (t->i_node[2] * BlockSize) + (curr_pos / BlockSize) - 2;
					phycial = Containers::DiskArray[phycial];
					phycial = (phycial * BlockSize) + (curr_pos % BlockSize);
					memory->writeInMem(PID, addr, Containers::DiskArray[phycial]);
				}
				curr_pos++;//obecna pozycja
			}
		}
	}
	if (ret == 1) return 0;
	else return ERROR_FILE_IS_NOT_OPENED;
}

int8_t FileMenager::deleteFile(std::string name)
{
	for (unsigned int i = 0; i < Containers::MainFileCatalog.size(); i++)
	{
		if (Containers::MainFileCatalog[i].name == name)//szukamy pliku w katalogu
		{
			if (Containers::MainFileCatalog[i].isOpen == true) return ERROR_FILE_IS_OPENED_CANT_DELETE;//sprawdzamy czy plik nie jest otwarty
			int req = Containers::MainFileCatalog[i].size / BlockSize;//liczba blok�w do usuni�cia

			if (Containers::MainFileCatalog[i].i_node.size() < 3)//je�eli nie ma bloku indeksowego
			{
				for (auto k : Containers::MainFileCatalog[i].i_node)
				{
					clearBlock(k);//funkcja czyszczaca pojedy�czy blok
				}
				for (unsigned int k = 0; k < Containers::Colors.size(); k++)
				{
					if (Containers::Colors[k].first == name)
					{
						Containers::Colors.erase(Containers::Colors.begin() + k);
						break;
					}
				}
				if (i > 0) Containers::MainFileCatalog.erase(Containers::MainFileCatalog.begin() + i);//usuwanie pliku z katalogu 
				else Containers::MainFileCatalog.erase(Containers::MainFileCatalog.begin());
				return 0;
			}
			else//rozmiar wi�kszy ni� 2
			{
				int physical = Containers::MainFileCatalog[i].i_node[2] * BlockSize;
				for (int k = 0; k < 2; k++)
				{
					clearBlock(Containers::MainFileCatalog[i].i_node[k]);
				}
				for (int k = 0; k < req - 1; k++)
				{
					clearBlock(Containers::DiskArray[physical]);
					physical++;
					
				}
				clearBlock(Containers::MainFileCatalog[i].i_node[2]);//usuniecie bloku indeksowego

				if(i > 0) Containers::MainFileCatalog.erase(Containers::MainFileCatalog.begin() + i);
				else Containers::MainFileCatalog.erase(Containers::MainFileCatalog.begin());

				return 0;
			}

		}
		
	}
	return ERROR_NO_FILE_WITH_THAT_NAME;
}

int8_t FileMenager::closeFile(std::string name, unsigned int PID)
{

	for (int i = 0; i < (int)Containers::open_file_table.size(); i++)
	{
		int pom = Containers::open_file_table[i];
		if (Containers::MainFileCatalog[pom].name == name && Containers::MainFileCatalog[pom].PID == PID)
		{
			Containers::MainFileCatalog[pom].isOpen = false;
			Containers::MainFileCatalog[pom].s.signal();
			Containers::open_file_table.erase(Containers::open_file_table.begin() + i);
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
		if (Containers::MainFileCatalog[i].PID == PID) closeFile(Containers::MainFileCatalog[i].name, Containers::MainFileCatalog[i].PID);
	}
}

std::pair<int8_t, std::string> FileMenager::cat(std::string name)
{
	std::pair<int8_t, std::string> result;
	result.first = ERROR_NO_FILE_WITH_THAT_NAME;

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
					physical = i.i_node[2] * BlockSize + ((req / BlockSize) - 2);
					physical = Containers::DiskArray[physical];
					physical = (physical * BlockSize) + (req % BlockSize);
					result.second.push_back(Containers::DiskArray[physical]);
				}
				req++;
			}
			result.first = 0;
			break;
		}
	}
	return result;
}

std::vector<std::string> FileMenager::ls()
{
	std::vector<std::string> result;
	for (auto i : Containers::MainFileCatalog) result.push_back(i.name);
	return result;
}

int8_t FileMenager::clearFile(std::string name)
{
	int8_t ret;
	ret = deleteFile(name);
	if (ret != 0) return ret;
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


std::pair<int8_t, int> FileMenager::wc(unsigned int PID)
{
	std::pair <int8_t, int> res;
	for (int i = 0; i < (int)Containers::open_file_table.size(); i++)
	{
		if (Containers::MainFileCatalog[Containers::open_file_table[i]].PID == PID)
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
		for (int i = 0; i < (int)Containers::bit_vector.size(); i++)
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
		for (int i = 0; i < (int)Containers::bit_vector.size(); i++)
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
		for (int i = 0; i < (int)Containers::bit_vector.size(); i++)
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
		if (Containers::MainFileCatalog[i].name == name && Containers::MainFileCatalog[i].PID != PID) return true;
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