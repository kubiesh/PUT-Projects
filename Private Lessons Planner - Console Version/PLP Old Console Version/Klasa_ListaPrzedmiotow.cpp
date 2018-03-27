#include "stdafx.h"
//#include "Klasa_ListaPrzedmiotow.hpp"

//#include <iostream>

std::vector<Przedmiot> ListaPrzedmiotow::Lista;

ListaPrzedmiotow::ListaPrzedmiotow() {
	//WCZYTYWANIE BAZY PRZEDMIOTOW Z PLIKU
	this->wczytajdane();
}

  void ListaPrzedmiotow::menu() {
	/* Funkcja odpowiedzialna za obsluge glownego menu do interfejsu zarzadzania przedmiotami*/

	//Flaga ktora warunkuje wyswietlanie sie glownego menu modulu
	bool warunekpetli = true;
	while (warunekpetli) {
		//Wyswietlenie dostepnych opcji
		cout << "\n::DOSTEPNE PRZEDMIOTY - MENU::\n1 - Wyswietl liste dostepnych przedmiotow\n"
			<< "2 - Dodaj przedmiot\n3 - Usun przedmiot\n0 - Wroc do menu glownego\n";
		int wybor = -1;
		cin >> wybor;
		switch (wybor) {
		case 1: //Wyswietl liste dostepnych przedmiotow
			this->wyswietlliste();
			break;
		case 2: //Dodaj przedmiot
			this->dodajprzedmiot();
			break;
		case 3: //Usun przedmiot
			this->usunprzedmiot();
			break;
		default: //Wroc do menu glownego
			warunekpetli = false;
			break;
		}
	}
}

  void ListaPrzedmiotow::wczytajdane() {
	/*Funkcja odpowiedzialna za wczytywanie danych z pliku binarnego do kolekcji bazy przedmiotow*/
	//Proba otwarcia pliku binarnego z baza danych przedmiotow
	this->Plik_Lista.open(this->Plik_Lista_Nazwa, std::ios::binary | std::ios::in);
	//Sprawdzenie czy plik udalo sie otworzyc
	if (this->Plik_Lista.is_open()) {

		//Utworzenie obiektu klasy Przedmiot, ktory bedzie sluzyl wczytywaniu danych z pliku
		Przedmiot odczytany;
		while (Plik_Lista.read(reinterpret_cast<char*>(&odczytany), sizeof(odczytany))) {
			this->Lista.push_back(odczytany); //Dodajemy rekord do bazy danych
		}

		//Sprawdzamy czy byly w pliku jakiekolwiek dane aby ustalic jakie bylo ostatnie ID przedmiotu
		if (Lista.size() > 0) {
			//Jezeli w pliku byly dane ustawiamy w polu statycznym ostatnie ID przedmiotu aby zachowac unikalnosc kluczy
			int ostatnie_id = Lista[Lista.size() - 1].id_przedmiotu;
			Przedmiot::ostatnie_id = ostatnie_id;
		}
	}
	else {
		//Komunikat na wypadek niepowodzenia
		cout << "Nie udalo sie otworzyc Plik_ListaPrzedmiotow\n";
	}
	//Zamkniecie pliku
	this->Plik_Lista.close();
}

  void ListaPrzedmiotow::zapiszzmiany() {
	/*Funkcja odpowiedzialna za zapisywanie zmian w bazie danych przedmiotow do pliku binarnego*/

	//Proba otwarcia pliku binarnego z baza danych przedmiotow
	this->Plik_Lista.open(this->Plik_Lista_Nazwa, std::ios::binary | std::ios::out);
	//Sprawdzenie czy plik udalo sie otworzyc
	if (this->Plik_Lista.is_open()) {
		for (auto it = this->Lista.begin(); it != this->Lista.end(); ++it) {
			//Iterujac po calej bazie danych zapisujemy rekord po rekordzie do pliku binarnego
			this->Plik_Lista.write(reinterpret_cast<char*>(&(*it)), sizeof((*it)));
		}
		cout << "Zapisano zmiany w pliku ListaPrzedmiotow\n";
	}
	else {
		//Komunikat na wypadek niepowodzenia
		cout << "Nie udalo sie zapisac Plik_ListaPrzedmiotow\n";
	}
	//Zamkniecie pliku
	this->Plik_Lista.close();
}

  void ListaPrzedmiotow::wyswietlliste() {
	/*Funkcja odpowiedzialna wyswietlenie wszystkich rekordow w bazie przedmiotow*/

	//Wyswietlenie naglowkow tabeli
	cout << "::LISTA DOSTEPNYCH PRZEDMIOTOW::\n";
	cout << "ID\tNazwa przedmiotu\tPoziom\tCena\n";
	//Petla for iterujaca po bazie danych
	for (auto element : Lista) {
		cout << element.id_przedmiotu << '\t' << element.nazwa_przedmiotu << '\t' << element.poziom << '\t' << element.cena << '\n';
	}
	cout << '\n';
	cout << "Laczna ilosc dostepnych przedmiotow wynosi: " << Lista.size()<<'\n';
}

  std::vector<Przedmiot>::iterator ListaPrzedmiotow::znajdz(int id_przedmiotu) {
	/*Funkcja pomocnicza zwracajaca iterator przedmiotu o podanym numerze ID
	W przypadku gdy nie ma przedmiotu o podanym numerze ID funkcja zwraca iterator Lista.end()*/

	//Ustawienie iteratora na poczatek bazy danych
	auto it = Lista.begin();
	//Iteracja po calej bazie danych w celu znalezienia rekordu o szukanym numerze ID
	while (it != Lista.end()) {
		if ((*it).id_przedmiotu == id_przedmiotu) {
			//W razie powodzenia przerwij dzialanie petli
			break;
		}
		++it;
	}
	//Zwroc iterator
	return it;
}

  void ListaPrzedmiotow::dodajprzedmiot() {
	/*Funkcja odpowiedzialna za obsluge interfejsu dodawania przedmiotu do bazy danych*/

	//Wyswietlenie komunikatu
	cout << "::DODAWANIE PRZEDMIOTU::\n";
	//Wywolanie metody ignorujacej bialy znak/ znak konca linii
	cin.ignore();

	////WPROWADZANIE DANYCH PRZEZ UZYTKOWNIKA
	//Nazwa przedmiotu
	cout << "Wprowadz nazwe przedmiotu:\n";
	std::string nazwa_przedmiotu;
	std::getline(cin, nazwa_przedmiotu);

	//Poziom przedmiotu
	std::string poziom_przedmiotu;
	cout << "Wprowadz poziom przedmiotu:\n";
	std::getline(cin, poziom_przedmiotu);

	//Cena za jednostke czasowa
	cout << "Wprowadz cene za jednostke czasowa:\n";
	int cena_przedmiotu;
	try {
		//Wczytanie ceny przedmiotu ze strumienia wejsciowego
		cin >> cena_przedmiotu;

		if (cena_przedmiotu < 0) {
			//Wyrzucenie wyjatku jezeli wartosc jest nieprawidlowa
			throw std::out_of_range("Cena przedmiotu nie moze byc mniejsza od 0");
		}
		else {
			//Utworzenie nowego rekordu 
			Przedmiot Nowy(nazwa_przedmiotu, poziom_przedmiotu, cena_przedmiotu);
			//Dodanie rekordu do bazy danych
			Lista.push_back(Nowy);
			//Wyswietlenie komunikatu
			cout << "Pomyslnie dodano przedmiot.\n";
			//Zapisanie zmian
			this->zapiszzmiany();
		}
	}
	//Obsluga wyjatku
	catch (std::exception &a) {
		cout << a.what() << '\n';
		cout << "Sprobuj jeszcze raz.\n";
	}
}

  void ListaPrzedmiotow::usunprzedmiot() {
	/*Funkcja odpowiedzialna za obsluge interfejsu usuwania przedmiotu z bazy danych*/

	//Wyswietlenie komunikatu
	cout << "::USUWANIE PRZEDMIOTU Z LISTY::\n";
	cout << "Wprowadz ID przedmiotu, ktory chcesz usunac (Wpisz 0 jesli chcesz sie wycofac):\n";

	//Zmienna przechowujaca wpisane ID
	int id_dousuniecia;
	//Wczytanie ID ze strumienia wejsciowego
	cin >> id_dousuniecia;

	if (id_dousuniecia != 0) {
		//Wywolanie funkcji pomocniczej zwracajacej iterator przedmiotu o podanym numerze id
		auto it = this->znajdz(id_dousuniecia);
		//Jezeli istnieje rekord o podanym numerze id to usuwamy przedmiot z bazy
		//Sprawdzamy czy iterator nam nie zwrocil konca kolekcji
		if (it != Lista.end()) {
			//Jezeli numer ID jest prawidlowy wyswietlamy komunikat z informacja o usuwanym przedmiocie
			cout << "Usunieto przedmiot " << (*it).nazwa_przedmiotu << ", Poziom " << (*it).poziom << '\n';
			//Usuwanie wszystkich zamowien dla usuwanego przedmiotu
			ListaZamowien::usunwszystkiewpisy_idprzedmiotu(id_dousuniecia);
			//Usuniecie z bazy przedmiotow
			it = Lista.erase(it);
			//Jezeli jednak uzytkownik sie pomylil i nie chcialby dokonac usuniecia trzeba dac mu szanse wyboru
			//Wyswietlenie komunikatu z zapytaniem o zapisanie zmian
			cout << "Czy zapisac zmiany? (Usuniecie przedmiotu spowoduje usuniecie wszystkich zwiazanych"
				<< "z nim informacji w bazach danych przedmiotow, listy zamowien)\n1 - Tak\n0 - Nie\n";
			//Zmienna przechowujaca wybor zapisu zmian
			int czyzapisaczmiany = 0;
			//Wczytanie wyboru ze strumienia wejsciowego
			cin >> czyzapisaczmiany;
			if (czyzapisaczmiany == 1) {
				//Jezeli uzytkownik zgadza sie zapisac zmiany wywolujemy funkcje zapisujace zmiany:
				ListaZamowien::zapiszzmiany();
				this->zapiszzmiany();
			}
			else {
				//Jezeli uzytkownik nie chce zapisac zmian informujemy go wyswietleniem komunikatu
				cout << "Nie zapisano zmian.\n";

				//Wczytujemy ostatnia zapisana wersje danych
				this->wczytajdane();
				ListaZamowien::wczytajdane();
			}

		}
		else {
			//W przypadku zle wpisanego numeru ID wyswietlamy komunikat:
			cout << "Nie znaleziono przedmiotu o podanym numerze ID.\n";
		}
	}
}

  std::string ListaPrzedmiotow::getNazwaPrzedmiotu(int indeks) {
	auto it = Lista.begin();
	for (; it != Lista.end(); ++it) {
		if ((*it).id_przedmiotu == indeks) {
			break;
		}
	}
	return (*it).nazwa_przedmiotu;
}

  std::string ListaPrzedmiotow::getPoziomPrzedmiotu(int indeks) {
	auto it = Lista.begin();
	for (; it != Lista.end(); ++it) {
		if ((*it).id_przedmiotu == indeks) {
			break;
		}
	}
	return (*it).poziom;
}

  int ListaPrzedmiotow::getCenaPrzedmiotu(int indeks) {
	auto it = Lista.begin();
	for (; it != Lista.end(); ++it) {
		if ((*it).id_przedmiotu == indeks) {
			return (*it).cena;
		}
	}
	return 0;
}

  bool ListaPrzedmiotow::czyistniejrekord(int indeks) {
	for (auto it = Lista.begin(); it != Lista.end(); ++it) {
		if ((*it).id_przedmiotu == indeks) {
			return true;
		}
	}
	return false;
}
