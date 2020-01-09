#pragma once
#include "Includes.h"
#include "semaphore.h"
#include "MemoryManager.h"

#define BlockSize 32 //Wielko�� bloku wyrazona w bajtach
#define DiskSize  1024 //Wielko�� dysku wyra�ona w bajtach
#define MaxFileSize  512 //Max wielkosc pliku w bajtach
#define MaxFileNumber  32
#define FileNameLenght 2



#define ERROR_ALREADY_EXISTING_FILE 64
#define ERROR_NO_SPACE_ON_DISK 65
#define ERROR_NO_FILE_WITH_THAT_NAME 66
#define ERROR_FILE_OPENED_BY_OTHER_PROCESS 67
#define ERROR_FILE_IS_NOT_OPENED 68
#define ERROR_UOT_OF_FILE_RANGE 69
#define ERROR_FILE_IS_OPENED_CANT_DELETE 70
#define ERROR_INAPPROPRIATE_FILENAME_LENGHT 71




struct File
{
	std::string name;
	int size = 0, PID = -1;
	std::vector<int> i_node;
	bool isOpen = false;
	Semaphore s;
	File();
};

struct Containers
{

	static std::vector<File> MainFileCatalog; // Katlog g�owny

	static std::array<int, DiskSize / BlockSize> bit_vector; // Mapa bitowa

	static std::array<char, DiskSize> DiskArray; //Tablica reprezentujca dysk 

	static std::vector<int> open_file_table; //Tablic otwartych plik�w

	static std::array<std::string, DiskSize / BlockSize> BitVectorWithFiles;

	static std::vector<std::pair<std::string, unsigned int>> Colors;
};




class FileMenager
{
private:
	unsigned int color = 0;
	int number_of_opened_files = 0, number_of_existing_files = 0;
	std::shared_ptr<Memory> memory;
public:
	FileMenager(std::shared_ptr<Memory> memory);

	int8_t createFile(std::string nazwa_pliku); //funkcja do tworzenia pliku o podanej nazwie w katalogu glownym
	// 64 istnieje plik o danej nazwie
	// 65 brak miejsca na dysku

	int8_t deleteFile(std::string name); //funkcja usuwajaca plik o danej nazwie
	// 66 brak pliku o podanej nazwie
	// 70 plik jest otwarty nie mozna usunac
	//   0 udalo sie usunac plik

	int8_t openFile(std::string name, unsigned int PID); //funkcja do otwierania pliku
	// 67 plik jest juz otawrty przez inny proces
	// 66 brak pliku o podanej nazwie
	//  0 udalo sie otworzyc plik

	int8_t closeFile(std::string name, unsigned int PID); //funkcja zamykajaca plik o danej nazwie
	// 68 plik nie jest otwarty
	//   0 udalo sie zamknac plik

	int8_t writeToEndFile(uint16_t byte, unsigned int PID, std::string name); //funkcja wpisuj�ca 1 bajt do otwartego pliku
	// 65 brak miejsca na dysku
	// 68 plik nie jest otwarty
	//   0 udalo sie otworzyc plik

	int8_t append(std::string name, uint16_t byte);
	// 65 brak miejsca na dysku
	// 68 plik nie jest otwarty
	//   0 udalo sie otworzyc plik

	int8_t writeToFile(uint8_t byte, uint8_t pos, unsigned int PID,std::string name); //funkcja wpisuj�ca bajt do podanego pliku na podana pozycje
	// 69 pos z poza zakresu pliku
	// 68 plik nie jest otwarty
	//   0 udalo sie otworzyc plik

	int8_t rename(std::string nazwa, std::string new_name);  //funkcja zmianiajac nazwe podanego pliku na inna podana 
	// 66 brak pliku o podanej nazwie
	//   0 udalo sie zmienic nazwe pliku

	int8_t readFile(uint8_t addr, uint8_t pos, unsigned int n, unsigned int PID,std::string name); //odczytaj n bajt�w z otwartego pliku(z danej pozycji) i wpisz do kom�rek pami�ci z podanym adresem pierwszej
	// 69 zakres z poza pliku
	// 68 plik nie jst otwarty
	//   0 udalo sie odczytac dane z pliku

	int8_t clearFile(std::string name);//czy�ci zawarto�� pliku ale go nie usuwa
	// 66 brak pliku o podanej nazwie
	// 70 plik jest otwarty nie mozna usunac
	//   0 udalo sie usunac plik

	std::pair<int8_t,int> wc(std::string name);//zwraca liczb� znak�w podanego pliku
	// 66 brak pliku o podanej nazwie
	//  0 uda�o si� uzyskac liczbe

	std::pair<int8_t, int> wc(unsigned int PID);//zwraca liczb� znak�w podanego pliku
	// 66 brak pliku o podanej nazwie
	//  0 uda�o si� uzyskac liczbe

	std::pair<std::vector<int>,int8_t> printInode(std::string node);
	// 66 brak pliku o podanej nazwie
	//  0 udało sie 

	std::vector<std::string> ls(); //zwraca wszystkie nazwy plikow jakie istnieja w folderze glownym

	std::pair<int8_t, std::string> cat(std::string name); //zwraca zawartosc calego pliku jako string

	void closeProcessFiles(unsigned int PID); //zamyka wszystkie pliki u�ywane przez proces o podanym I

	int FindFreeBlock(File* file); //szuka wolengo bloku jesli znajdzie zwraca jego adres logiczny, w przeciwnym wypadku zwraca -1

};

bool isNameColision(std::string name); //sprawdza czy w katalogu nie ma juz pliku o podanej nazwie
	//   1 jest juz plik o podanej nazwie
	//   0 nie ma pliku o podanej nazwie

bool isFileOpen(std::string name, int PID); //sprawdza czy plik jest otwarty 
	//   1 jest juz otwarty
	//   0 nie jest otwarty

void clearBlock(int log); //czysci blok pamieci pod podanym adresem logicznym

void showBitVector();
void ShowMemory();