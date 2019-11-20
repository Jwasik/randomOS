#pragma once
#include "Includes.h"
//#include "semaphore.h"

#define BlockSize 32 //Wielkoœæ bloku wyrazona w bajtach
#define DiskSize  1024 //Wielkoœæ dysku wyra¿ona w bajtach
#define MaxFileSize  512 //Max wielkosc pliku w bajtach
#define MaxFileNumber  32


struct File
{
	std::string name;
	int size, PID;
	std::vector<int> i_node;
	bool isOpen = false;
	//Semaphore s;
};


static std::vector<File> MainFileCatalog; // Katlog g³owny

static std::array<int, DiskSize / BlockSize> bit_vector; // Mapa bitowa

static std::array<char, DiskSize> DiskArray; //Tablica reprezentujca dysk 

static std::vector<int> open_file_table; //Tablic otwartych plików


class FileMenager
{
private:

	int number_of_opened_files = 0, number_of_existing_files = 0;

public:
	FileMenager();

	int createFile(std::string nazwa_pliku); //funkcja do tworzenia pliku o podanej nazwie w katalogu glownym
	// -1 istnieje plik o danej nazwie
	// -2 brak miejsca na dysku

	int deleteFile(std::string name); //funkcja usuwajaca plik o danej nazwie
	//  -1 brak pliku o podanej nazwie
	//   1 udalo sie usunac plik

	int openFile(std::string name, unsigned int PID); //funkcja do otwierania pliku
	// -1 plik jest juz otawrty przez inny proces
	// -2 nie ma takie pliku
	//  1 udalo sie otworzyc plik

	int closeFile(std::string name, unsigned int PID); //funkcja zamykajaca plik o danej nazwie
	//  -1 plik nie jest otwarty
	//   1 udalo sie zamknac plik

	int writeToEndFile(uint16_t byte, unsigned int PID); //funkcja wpisuj¹ca 1 bajt do otwartego pliku
	//  -1 brak miejsca na dysku
	//  -2 plik nie jest otwarty
	//   1 udalo sie otworzyc plik

	int writeToFile(uint8_t byte, uint8_t pos, unsigned int PID); //funkcja wpisuj¹ca bajt do podanego pliku na podana pozycje
	//  -1 pos z poza zakresu pliku
	//  -2 plik nie jest otwarty
	//   1 udalo sie otworzyc plik

	int rename(std::string nazwa, std::string new_name);  //funkcja zmianiajac nazwe podanego pliku na inna podana 
	//  -1 nie ma takiego pliku
	//   1 udalo sie zmienic nazwe pliku

	int readFile(uint8_t addr, uint8_t pos, unsigned int n, unsigned int PID); //odczytaj n bajtów z otwartego pliku(z danej pozycji) i wpisz do komórek pamiêci z podanym adresem pierwszej
	//  -1 zakres z poza pliku
	//  -2 plik nie jst otwarty
	//   1 udalo sie odczytac dane z pliku

	std::vector<std::string> ls(); //zwraca wszystkie nazwy plikow jakie istnieja w folderze glownym
	std::string cat(std::string name); //zwraca zawartosc calego pliku jako string
	void closeProcessFiles(unsigned int PID); //zamyka wszystkie pliki u¿ywane przez proces o podanym I

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



