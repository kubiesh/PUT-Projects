#ifndef KLASA_LISTAPRZEDMIOTOW_HPP
#define KLASA_LISTAPRZEDMIOTOW_HPP

/*Plik Klasa_ListaPrzedmiotow.h zawiera:
- deklaracja klasy ListaPrzedmiotow*/

//Import plikow naglowkowych

//Import bibliotek
//#include <iostream>
//#include <string>
//#include <vector>
//#include <fstream>
//using std::cout;
//using std::cin;
//#include "Jadro_programu.hpp"
//extern class Przedmiot;
//extern class ListaZamowien;
class ListaPrzedmiotow {

/* Klasa odpowiadajaca za obsluge bazy danych dostepnych przedmiotow */

public: static std::vector<Przedmiot> Lista; //Kolekcja pelniaca role bazy danych przedmiotow
protected: std::fstream Plik_Lista; //Obiekt obslugujacy plik z baza danych przedmiotow
protected: std::string Plik_Lista_Nazwa = "Lista_Przedmiotow.bin"; //Lancuch przechowujacy nazwe pliku z baza danych przedmiotow

//////////////////////////////////////////////////////////////////////////////////////////

/*KONSTRUKTOR*/
public: ListaPrzedmiotow();

public: void menu();


protected: void wczytajdane();

protected: void zapiszzmiany();

public: static void wyswietlliste();

protected: std::vector<Przedmiot>::iterator znajdz(int id_przedmiotu);

protected: void dodajprzedmiot();


protected: void usunprzedmiot();

//
//FUNKCJE POMOCNICZE (NIE WIADOMO CZY BEDA UZYTE)
//

public: static std::string getNazwaPrzedmiotu(int indeks);
public: static std::string getPoziomPrzedmiotu(int indeks);
public: static int getCenaPrzedmiotu(int indeks);
public: static bool czyistniejrekord(int indeks);

};



#endif