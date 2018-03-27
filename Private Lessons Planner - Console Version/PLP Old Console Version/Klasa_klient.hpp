#ifndef KLASA_KLIENT_HPP
#define KLASA_KLIENT_HPP

/*Plik Klasa_klient.h zawiera:
- deklaracja klasy Klient
- deklaracja klasy DostepnoscKlienta */

//Import bibliotek
//#include <string>

class Klient {
	/* Klasa odpowiedzialna za ustalenie informacji o danym kliencie */

	//Przekazanie uprawnien klasie ListaKlientow, ktora tworzy z obiektow klasy Klient baze danych
friend class ListaKlientow;
friend class Generator;

protected: int id_klienta; //Unikalny numer ID klienta
	/*Lancuchy znakow sa reprezentowane przez typ char, poniewaz sa problemy z odczytem lancuchow klasy 
	std::string z pliku binarnego */
protected: char imie[20]; //Lancuch przechowujacy imie klienta
protected: char nazwisko[40]; //Lancuch przechowujacy nazwisko klienta
protected: char nrtelefonu[13]; //Lancuch numer telefonu imie klienta
protected: char email[30]; //Lancuch przechowujacy adres email klienta
	//Pole statyczne przechowujace ostatni numer ID utworzone w celu zachowania unikalnosci rekordow w bazie:
private: static int ostatnie_id; 

//////////////////////////////////////////////////////////////////////////////////////////

	/* KONSTRUKTOR DOMYŒLNY*/
public: Klient();
	/* KONSTRUKTOR */	
public: Klient(std::string imie, std::string nazwisko, std::string nrtel, std::string email);;
public: void ustawImie(std::string imie);
public: void ustawNazwisko(std::string nazwisko);
public: void ustawNrTel(std::string nrtel);
public: void ustawEmail(std::string email);

};

class DostepnoscKlienta {
	/* Klasa odpowiedzialna za przechowywanie danych o przedzialach czasowych, w ktorych klient moze uczeszczac
	w zajeciach*/


	//Przekazanie uprawnien klasie ListaKlientow, ktora tworzy z obiektow klasy DostepnoscKlienta baze danych
	friend class ListaKlientow;
	friend class Generator;

protected: int id_dostepnosci; //Unikalne ID wpisow w bazie dostepnosci klientow
protected: int id_klienta; //Pole przechowujace ID klienta, ktorego dotyczy wpis w bazie

	/* Ponizsze pola dzien,start,koniec sluza do reprezentowania informacji o przedzialach czasowych
	dostepnosci klienta zgodnie z ustalonym sposobem przechowywania informacji o przedzialach czasowych*/
protected: int dzien;
protected: int start;
protected: int koniec;
	//Pole statyczne przechowujace ostatni numer ID utworzone w celu zachowania unikalnosci rekordow w bazie:
private: static int ostatnie_id;

	/* KONSTRUKTOR DOMYSLNY */
public: DostepnoscKlienta();
	/* KONSTRUKTOR */
public: DostepnoscKlienta(int id_klienta, int dzien, int start, int koniec);
protected: double ObliczCzasPrzedzialu();
};


#endif