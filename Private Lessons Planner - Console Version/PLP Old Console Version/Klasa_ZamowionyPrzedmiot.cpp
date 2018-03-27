#include "stdafx.h"


/* Domyslna wartosc pola ostatnie_id
Przy wczytywaniu danych z pliku wartosc jest zastepowana numerem ID ostatniego rekordu w bazie danych zamowionych przedmiotow */
int ZamowionyPrzedmiot::ostatnie_id = 0;

ZamowionyPrzedmiot::ZamowionyPrzedmiot() {
	//Stworzony na potrzeby wczytywania danych z pliku binarnego
}

ZamowionyPrzedmiot::ZamowionyPrzedmiot(int id_klienta, int id_przedmiotu) : id_klienta(id_klienta), id_przedmiotu(id_przedmiotu) {
	//Nadajemy rekordowi w bazie unikalny numer ID	
	id_wpisu = ++ostatnie_id;
}
