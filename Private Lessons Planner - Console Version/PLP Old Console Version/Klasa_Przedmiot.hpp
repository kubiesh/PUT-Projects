#ifndef Klasa_Przedmiot
#define Klasa_Przedmiot

/*Plik Klasa_Przedmiot.h zawiera:
- deklaracja klasy Przedmiot*/

//Import bibliotek
//#include <string>

class Przedmiot {
	/* Klasa odpowiedzialna za przechowywanie informacji o danym przedmiocie */

	//Przekazanie uprawnien klasie ListaPrzedmiotow, ktora tworzy z obiektow klasy Przedmiot baze danych
	friend class ListaPrzedmiotow;

protected:  int id_przedmiotu; //Unikalny numer ID klienta
	 /*Lancuchy znakow sa reprezentowane przez typ char, poniewaz sa problemy z odczytem lancuchow klasy
	 std::string z pliku binarnego */
protected:  char nazwa_przedmiotu[20]; //Lancuch przechowujacy nazwe danego przedmiotu
protected:  char poziom[50]; //Lancuch przechowujacy poziom danego przedmiotu
protected: int cena; //Zmienna przechowujaca cene za jednostke czasowa danego przedmiotu
	//Pole statyczne przechowujace ostatni numer ID utworzone w celu zachowania unikalnosci rekordow w bazie:
protected: static int ostatnie_id;

//////////////////////////////////////////////////////////////////////////////////////////


	/* KONSTRUKTOR DOMYŒLNY*/
public: Przedmiot();
	/* KONSTRUKTOR*/
public: Przedmiot(std::string nazwa_przedmiotu, std::string poziom, int cena);
};


#endif