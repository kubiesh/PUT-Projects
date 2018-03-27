#ifndef KLASA_LISTAZAMOWIEN_HPP
#define KLASA_LISTAZAMOWIEN_HPP


//KOMENTARZE DO ZROBIENIA
//#include "Klasa_ZamowionyPrzedmiot.h"

//#include <iostream>
//#include <string>
//#include <vector>
//#include <fstream>
//#include <algorithm>
//using std::cout;
//using std::cin;


class ListaZamowien {
	//Nadanie uprawnien klasie ListaKlientow
friend class ListaKlientow;
friend class ListaPrzedmiotow;

public: static std::vector<ZamowionyPrzedmiot>Lista;
protected: static std::fstream Plik_Lista;
protected: static std::string Plik_Lista_Nazwa;

//////////////////////////////////////////////////////////////////////////////////////////

public: ListaZamowien();

public: void menu();

protected: static void wczytajdane();

protected: static void zapiszzmiany();
public: static int getIDKlienta(int id_wpisu);
public: static std::string getNazwiskoKlienta(int id_wpisu);

public: static std::string getImieKlienta(int id_wpisu);

public: static std::string getNazwaPrzedmiotu(int id_wpisu);
public: static std::string getPoziomPrzedmiotu(int id_wpisu);
public: static int getCenaPrzedmiotu(int id_wpisu);

protected: static std::vector<ZamowionyPrzedmiot>::iterator znajdz(int id_wpisu);

protected: void wyswietlwiersz(std::vector<ZamowionyPrzedmiot>::iterator &it);

protected: void wyswietlzamowienia(bool wszystkie = true, bool pokliencie = false, int id = 0);
protected: void wyswietlzamowieniamenu();

protected: bool czyistniejerekord(int id_wpisu);

protected: void usunwpis(int id_wpisu);


protected: static void usunwszystkiewpisy_idklienta(int id_klienta);

protected: static void usunwszystkiewpisy_idprzedmiotu(int id_przedmiotu);

protected: void przylaczklienta();

protected: void przylaczklientamenu();

protected: void odlaczklientamenu();

};



#endif