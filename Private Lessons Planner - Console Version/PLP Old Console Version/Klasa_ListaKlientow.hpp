#ifndef KLASA_LISTAKLIENTOW_HPP
#define KLASA_LISTAKLIENTOW_HPP

/*Plik Klasa_ListaKlientow.h zawiera:
- deklaracja klasy ListaKlientow (modul obslugujacy baze klientow i baze dyspozycyjnosci klientow)*/

//Import plikow naglowkowych


//#include "Klasa_Parametry.h"
//Import bibliotek
//#include <iostream>
//#include <string>
//#include <vector>
//#include <fstream>
//#include <algorithm>
//using std::cout;
//using std::cin;

//extern std::vector<std::string> DniTygodnia;

//extern class Klient;
//extern class DostepnoscKlienta;
//extern class Parametry;
class ListaKlientow {
	
/* Klasa odpowiadajaca za obsluge bazy danych klientow oraz bazy danych dyspozycyjnosci klientow */

//
//BAZA KLIENTOW
//
public: static std::vector<Klient>Lista; //Kolekcja pelniaca role bazy danych klientow
protected: std::fstream Plik_Lista; //Obiekt obslugujacy plik z baza danych klientow
protected: std::string Plik_Lista_Nazwa = "Lista_Klientow.bin"; //Lancuch przechowujacy nazwe pliku z baza danych klientow

//
//BAZA DYSPOZYCYJNOSCI KLIENTOW
//
public: static std::vector<DostepnoscKlienta>Lista_dostepnosc; //Kolekcja pelniaca role bazy dyspozycyjnosci klientow
protected:	std::fstream Plik_DostepnoscKlientow; //Obiekt obslugujacy plik z baza dyspozycyjnosci klientow
protected:	std::string Plik_DostepnoscKlientow_Nazwa = "Dostepnosc_Klientow.bin"; //Lancuch przechowujacy nazwe pliku z baza dyspozycyjnosci klientow

//////////////////////////////////////////////////////////////////////////////////////////


/*KONSTRUKTOR*/
public: ListaKlientow();


public: void menu();

//
////
////// FUNKCJE OBSLUGUJACE BAZE KLIENTOW
////
//

protected: void wczytajLista();



protected: void zapiszzmiany();

public: static void wyswietlliste();

protected: std::vector<Klient>::iterator znajdz(int id_klienta);

protected: void dodajklienta();

protected: void usunklienta();

protected: void modyfikacjadaneosobowe();


	

	//
	//FUNKCJE POMOCNICZE do uzytku dla innych klas
	//

public: static std::string getNazwiskoKlienta(int id_klienta);

public: static std::string getImieKlienta(int id_klienta);

public: static bool czyistniejrekord(int id_klienta);

	//
	////
	////// FUNKCJE OBSLUGUJACE BAZE DYSPOZYCYJNOSCI KLIENTOW
	////
	//


protected: void wczytajDostepnoscKlientow();

protected: void zapiszzmiany_dostepnosc();

protected: void usunprzedzial(int id_dostepnosci);
protected: void dodajprzedzial(int id_klienta);
protected: void wyswietldyspozycyjnosc(int id_klienta);
protected: void dyspozycyjnosc();

protected: void usunwszystkiedostepnosci_idklienta(int id_klienta);

};



#endif