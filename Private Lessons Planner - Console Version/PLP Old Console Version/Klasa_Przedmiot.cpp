#include "stdafx.h"

Przedmiot::Przedmiot() {
	//Stworzony na potrzeby wczytywania danych z pliku binarnego
}

Przedmiot::Przedmiot(std::string nazwa_przedmiotu, std::string poziom, int cena) : cena(cena) {
	//Funkcja sprintf_s zostala uzyta w celu konwersji z typu std::string na typ char
	sprintf_s(this->nazwa_przedmiotu, "%s", nazwa_przedmiotu.c_str());
	sprintf_s(this->poziom, "%s", poziom.c_str());
	//Nadajemy rekordowi w bazie unikalny numer ID
	id_przedmiotu = ++ostatnie_id;
}
/* Domyslna wartosc pola ostatnie_id
Przy wczytywaniu danych z pliku wartosc jest zastepowana numerem ID ostatniego rekordu w bazie danych przedmiotow */
int Przedmiot::ostatnie_id = 0;
