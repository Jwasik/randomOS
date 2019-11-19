#include "FileSystem.h"

FileMenager::FileMenager()
{
	bit_vector.fill(1);
}

int FileMenager::touch(std::string sciezka, std::string nazwa_pliku)
{
	std::string catalog;
	int pom = 0;
	for (auto i : sciezka)
	{
		if (pom == 1) catalog = catalog + i;
		if(i == '/') pom =1;	
	}

	if (catalog.size() == 0) //Katalog glowny
	{

	}
	else //Podkatalog
	{

	}
	return 0;
}
