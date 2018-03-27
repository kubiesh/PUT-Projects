#ifndef KLASA_ZAMOWIONY_PRZEDMIOT_HPP
#define KLASA_ZAMOWIONY_PRZEDMIOT_HPP

/*Plik Klasa_ZamowionyPrzedmiot.h zawiera:
- deklaracja klasy ZamowionyPrzedmiot*/

class ZamowionyPrzedmiot {
	/* Klasa odpowiedzialna za przechowywanie informacji o zamowieniu klienta na dany przedmiot */
	//Przekazanie uprawnien do modulu obslugujacego baze danych zamowien
	friend class ListaZamowien;
	friend class ZamowieniezPriorytetem;
	friend class Generator;

protected: int id_wpisu; //Unikalne ID rekordu zamowien
protected: int id_klienta; //ID klienta zamawiajacego przedmiot
protected: int id_przedmiotu; //ID zamowionego przedmiotu

					   //Pole statyczne przechowujace ostatni numer ID utworzone w celu zachowania unikalnosci rekordow w bazie:
protected: static int ostatnie_id;

/* KONSTRUKTOR DOMYŒLNY*/
public: ZamowionyPrzedmiot();

/* KONSTRUKTOR*/
public: ZamowionyPrzedmiot(int id_klienta, int id_przedmiotu);

};

#endif