#include "stdafx.h"
//#include "Klasa_klient.hpp"

/* Domyslna wartosc pola ostatnie_id
Przy wczytywaniu danych z pliku wartosc jest zastepowana numerem ID ostatniego rekordu w bazie danych klientow */
int Klient::ostatnie_id = 0;


Klient::Klient() {
	//Stworzony na potrzeby wczytywania danych z pliku binarnego
}

  Klient::Klient(std::string imie, std::string nazwisko, std::string nrtel, std::string email) {
	//Funkcja sprintf_s zostala uzyta w celu konwersji z typu std::string na typ char
	sprintf(this->imie, "%s", imie.c_str(), _TRUNCATE);
	sprintf(this->nazwisko, "%s", nazwisko.c_str(), _TRUNCATE);
	sprintf(this->nrtelefonu, "%s", nrtel.c_str(), _TRUNCATE);
	sprintf(this->email, "%s", email.c_str(), _TRUNCATE);
	//Nadajemy rekordowi w bazie unikalny numer ID
	id_klienta = ++ostatnie_id;
}

  void Klient::ustawImie(std::string imie) {
	//Setter imienia
	sprintf_s(this->imie, "%s", imie.c_str());
}

  void Klient::ustawNazwisko(std::string nazwisko) {
	//Setter nazwiska
	sprintf_s(this->nazwisko, "%s", nazwisko.c_str());
}

  void Klient::ustawNrTel(std::string nrtel) {
	//Setter numeru telefonu
	sprintf_s(this->nrtelefonu, "%s", nrtel.c_str());
}

void Klient::ustawEmail(std::string email) {
	//Setter adresu email
	sprintf_s(this->email, "%s", email.c_str());
}

DostepnoscKlienta::DostepnoscKlienta() {}

DostepnoscKlienta::DostepnoscKlienta(int id_klienta, int dzien, int start, int koniec) : id_klienta(id_klienta), dzien(dzien), start(start), koniec(koniec) {
	/* Konstruktor tworzy obiekt na podstawie dostarczonych do niego danych*/
	//Nadajemy rekordowi w bazie unikalny numer ID
	id_dostepnosci = ++ostatnie_id;
}
double DostepnoscKlienta::ObliczCzasPrzedzialu() {

	double CzasPrzedzialu = this->koniec - this->start + 1;
	return CzasPrzedzialu = CzasPrzedzialu*Parametry::zaokraglenie / Parametry::jednostkaczasowa;

}
/* Domyslna wartosc pola ostatnie_id
Przy wczytywaniu danych z pliku wartosc jest zastepowana numerem ID ostatniego rekordu w bazie danych dostepnosci klientow */
int DostepnoscKlienta::ostatnie_id = 0;
