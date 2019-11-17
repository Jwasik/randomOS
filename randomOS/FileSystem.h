#pragma once
#include "Includes.h"
#include "semaphore.h"

#define BlockSize 32 //Wielkoœæ bloku wyrazona w bajtach
#define DiskSize  1024 //Wielkoœæ dysku wyra¿ona w bajtach
#define MaxFileSize  512 //Max wielkosc pliku w bajtach
#define MaxFileNumber  32
#define MaxOpenedFiles  15



struct File
{
	std::string name;
	int size, process_id;
	char* reading_pointer = nullptr;
	char* writing_pointer = nullptr;
	std::vector<char*> i_node;
	//Kontrola dostepu
};




static std::array<File, MaxFileNumber> MainFileCatalog; // Katlog g³owny

static std::array<int,DiskSize/BlockSize> bit_vector; // Mapa bitowa

static std::array<char, DiskSize> DiskArray; //Tablica reprezentujca dysk 

static std::vector<int> open_file_table; //Tablic otwartych plików


class FileMenager
{
private:

	int number_of_opened_files = 0, number_of_existing_files = 0;
	
public:
	FileMenager();

	void closeProcessFiles(unsigned int PID); // zamyka wszystkie pliki u¿ywane przez proces o podanym ID


	int writeFile(int PID,);
	int readFile(int PID,int adress,int position);
	int closeFile(std::string name); // Gdy nikt nie uzywa usuwa z TABLICY OTWARTYCH PLIKOW oraz wywoluje funkcje 
	int openFile(std::string name,int PID); // Przed pierwszym wywolaniem pliku dodaje go do TABLICY OTWARTYCH PLIKOW //Przekazuje wskanizk do tablic otwartych plikow
	
	
	int removeFile(std::string name);//usuwa podany plik, jeœli siê nie da to zwraca kod b³êdu
	int rename(std::string nazwa);  //zmienia nazwê pliku lub katalogu na podan¹.Koñcówka.txt oznacza plik
	std::pair<int,std::string> cat(std::string name);
	int createFile(std::string nazwa_pliku); // tworzy plik o podanej nazwie w podanej lokalizacji
	std::vector<std::string> ls(); // zwraca zawartosæ folderu(pliki)
	int deleteFolder();

	char* FindFreeBlock(File* file);
	bool isNameColision(std::string name);
	bool isFileOpen(std::string name);
};

bool isFreeSpace(int number_of_blocks);
bool checkFileName(std::string file_name,std::string catalog_name);





