#pragma once
#include "Includes.h"
#include "semaphore.h"

#define BlockSize 32 //Wielko�� bloku wyrazona w bajtach
#define DiskSize  1024 //Wielko�� dysku wyra�ona w bajtach
#define MaxFileSize  512 //Max wielkosc pliku w bajtach
#define MaxFileNumber  32
#define MaxOpenedFiles  15



struct File
{
	std::string name;
	int size, PID;
	std::vector<char*> i_node;
	//Kontrola dostepu
};




static std::array<File, MaxFileNumber> MainFileCatalog; // Katlog g�owny

static std::array<int,DiskSize/BlockSize> bit_vector; // Mapa bitowa

static std::array<char, DiskSize> DiskArray; //Tablica reprezentujca dysk 

static std::vector<int> open_file_table; //Tablic otwartych plik�w


class FileMenager
{
private:

	int number_of_opened_files = 0, number_of_existing_files = 0;
	
public:
	FileMenager();

	void closeProcessFiles(unsigned int PID); // zamyka wszystkie pliki u�ywane przez proces o podanym ID

	int deleteFile(std::string name); // usuwa plik(Marcin)
	int openFile(std::string name, unsigned int PID);// otw�rz plik(Marcin)
	int closeFile(std::string name, unsigned int PID); // zamknij plik(Marcin)
	int writeToEndFile(uint16_t byte, unsigned int PID);// wpisz bajt do otwartego pliku(na koniec) (Marcin)
	int writeToFile(uint8_t byte, uint8_t pos, unsigned int PID); // wpisz bajt do otwartego pliku(na danej pozycji) (Marcin)
	int readFile(uint8_t addr, uint8_t pos, unsigned int n, unsigned int PID); // odczytaj n bajt�w z otwartego pliku(z danej pozycji) i wpisz do kom�rek pami�ci z podanym adresem pierwszej
	
	
	int removeFile(std::string name);//usuwa podany plik, je�li si� nie da to zwraca kod b��du
	int rename(std::string nazwa);  //zmienia nazw� pliku lub katalogu na podan�.Ko�c�wka.txt oznacza plik
	std::pair<int,std::string> cat(std::string name); 
	int createFile(std::string nazwa_pliku); // tworzy plik o podanej nazwie w podanej lokalizacji
	std::vector<std::string> ls(); // zwraca zawartos� folderu(pliki)


	char* FindFreeBlock(File* file);
	bool isNameColision(std::string name);
	bool isFileOpen(std::string name);
};

bool isFreeSpace(int number_of_blocks);
bool checkFileName(std::string file_name,std::string catalog_name);





