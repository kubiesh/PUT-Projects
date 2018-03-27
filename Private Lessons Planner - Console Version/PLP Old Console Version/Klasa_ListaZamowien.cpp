#include "stdafx.h"
//#include "Klasa_ListaZamowien.hpp"


std::vector<ZamowionyPrzedmiot>ListaZamowien::Lista;
std::fstream ListaZamowien::Plik_Lista;
std::string ListaZamowien::Plik_Lista_Nazwa = "Lista_ZamowionePrzedmioty.bin";

ListaZamowien::ListaZamowien() {
	//Ladowanie listy zamowien z pliku
	this->wczytajdane();

}

  void ListaZamowien::menu() {
	/* Funkcja odpowiedzialna za obsluge glownego menu do interfejsu zarzadzania zamowieniami*/

	//Flaga ktora warunkuje wyswietlanie sie glownego menu modulu
	bool warunekpetli = true;
	while (warunekpetli) {
		//Wyswietlenie dostepnych opcji
		cout << "\n::ZAMOWIONE PRZEDMIOTY - MENU::\n1 - Wyswietl zamowione przedmioty\n"
			<< "2 - Przylacz przedmiot do klienta\n3 - Odlacz przedmiot od klienta\n0 - Wroc do menu glownego\n";
		int wybor = -1;
		cin >> wybor;
		switch (wybor) {
		case 1: //Wyswietl zamowione przedmioty
			this->wyswietlzamowieniamenu();
			break;
		case 2: //Przylacz przedmiot do klienta
			this->przylaczklientamenu();
			break;
		case 3: //Odlacz przedmiot od klienta
			this->odlaczklientamenu();
			break;
		case 4:

			break;
		case 5:

			break;
		default: //Wroc do menu glownego
			warunekpetli = false;
			break;
		}
	}
}

  void ListaZamowien::wczytajdane() {
	/*Funkcja odpowiedzialna za wczytywanie danych z pliku binarnego do kolekcji bazy zamowien*/
	//Proba otwarcia pliku binarnego z baza danych zamowien
	Plik_Lista.open(Plik_Lista_Nazwa, std::ios::binary | std::ios::in);
	//Sprawdzenie czy plik udalo sie otworzyc
	if (Plik_Lista.is_open()) {
		//Utworzenie obiektu klasy ZamowionyPrzedmiot, ktory bedzie sluzyl wczytywaniu danych z pliku
		ZamowionyPrzedmiot odczytany;
		while (Plik_Lista.read(reinterpret_cast<char*>(&odczytany), sizeof(odczytany))) {
			Lista.push_back(odczytany); //Dodajemy rekord do bazy danych
		}
		//Sprawdzamy czy byly w pliku jakiekolwiek dane aby ustalic jakie bylo ostatnie ID klienta
		if (Lista.size() > 0) {
			int ostatnie_id = Lista[Lista.size() - 1].id_wpisu;
			//Jezeli w pliku byly dane ustawiamy w polu statycznym ostatnie ID klienta aby zachowac unikalnosc kluczy
			ZamowionyPrzedmiot::ostatnie_id = ostatnie_id;
		}
	}
	else {
		//Komunikat na wypadek niepowodzenia
		cout << "Nie udalo sie otworzyc Plik_ListaZamowien\n";
	}
	//Zamkniecie pliku
	Plik_Lista.close();
}

  void ListaZamowien::zapiszzmiany() {
	/*Funkcja odpowiedzialna za zapisywanie zmian w bazie danych zamowien do pliku binarnego*/

	//Proba otwarcia pliku binarnego z baza danych zamowien
	Plik_Lista.open(Plik_Lista_Nazwa, std::ios::binary | std::ios::out);
	//Sprawdzenie czy plik udalo sie otworzyc
	if (Plik_Lista.is_open()) {
		for (auto it = Lista.begin(); it != Lista.end(); ++it) {
			//Iterujac po calej bazie danych zapisujemy rekord po rekordzie do pliku binarnego
			Plik_Lista.write(reinterpret_cast<char*>(&(*it)), sizeof((*it)));
		}
		cout << "Zapisano zmiany w pliku ListaZamowien\n";
	}
	else {
		//Komunikat na wypadek niepowodzenia
		cout << "Nie udalo sie zapisac Plik_ListaZamowien\n";
	}
	//Zamkniecie pliku
	Plik_Lista.close();
}

  int ListaZamowien::getIDKlienta(int id_wpisu)
  {
	  auto it = znajdz(id_wpisu);
	  if (it != Lista.end()) {
		  return (*it).id_klienta;
	  }
	  else {
		  return int();
	  }
  }

std::string ListaZamowien::getNazwiskoKlienta(int id_wpisu) {
	  auto it = znajdz(id_wpisu);
	  if (it != Lista.end()) {
		  return ListaKlientow::getNazwiskoKlienta((*it).id_klienta);
	  }
	  else {
		  return std::string();
	  }
  }

std::string ListaZamowien::getImieKlienta(int id_wpisu) {
	  auto it = znajdz(id_wpisu);
	  if (it != Lista.end()) {
		  return ListaKlientow::getImieKlienta((*it).id_klienta);
	  }
	  else {
		  return std::string();
	  }
  }

 std::string ListaZamowien::getNazwaPrzedmiotu(int id_wpisu) {
	  auto it = znajdz(id_wpisu);
	  if (it != Lista.end()) {
		  return ListaPrzedmiotow::getNazwaPrzedmiotu((*it).id_przedmiotu);
	  }
	  else {
		  return std::string();
	  }
  }

 std::string ListaZamowien::getPoziomPrzedmiotu(int id_wpisu) {
	  auto it = znajdz(id_wpisu);
	  if (it != Lista.end()) {
		  return ListaPrzedmiotow::getPoziomPrzedmiotu((*it).id_przedmiotu);
	  }
	  else {
		  return std::string();
	  }
  }

 int ListaZamowien::getCenaPrzedmiotu(int id_wpisu) {
	  auto it = znajdz(id_wpisu);
	  if (it != Lista.end()) {
		  return ListaPrzedmiotow::getCenaPrzedmiotu((*it).id_przedmiotu);
	  }
	  else {
		  return int();
	  }
  }

  std::vector<ZamowionyPrzedmiot>::iterator ListaZamowien::znajdz(int id_wpisu) {
	/*Funkcja pomocnicza zwracajaca iterator zamowienia o podanym numerze ID
	W przypadku gdy nie ma zamowienia o podanym numerze ID funkcja zwraca iterator Lista.end()*/

	//Ustawienie iteratora na poczatek bazy danych
	std::vector<ZamowionyPrzedmiot>::iterator it = Lista.begin();
	//Iteracja po calej bazie danych w celu znalezienia rekordu o szukanym numerze ID
	while (it != Lista.end()) {
		if ((*it).id_wpisu == id_wpisu) {
			//W razie powodzenia przerwij dzialanie petli
			break;
		}
		++it;
	}
	//Zwroc iterator
	return it;
}

  void ListaZamowien::wyswietlwiersz(std::vector<ZamowionyPrzedmiot>::iterator & it) {
	/* Pomocnicza funkcja wyswietlajaca wiersz przekazanego iteratora rekordu bazy*/
	cout << (*it).id_wpisu << '\t' << (*it).id_przedmiotu << '\t' << ListaPrzedmiotow::getNazwaPrzedmiotu((*it).id_przedmiotu)
		<< ',' << ListaPrzedmiotow::getPoziomPrzedmiotu((*it).id_przedmiotu) << " - " << ListaKlientow::getNazwiskoKlienta((*it).id_klienta) << ' ' << ListaKlientow::getImieKlienta((*it).id_klienta) << " [" << (*it).id_klienta << "]\n";
}

  void ListaZamowien::wyswietlzamowienia(bool wszystkie, bool pokliencie, int id) {
	/* Pomocnicza funkcja wyswietlajaca liste zamowien przedmiotow.
	Dostepne opcje wyswietlania:
	- Wszystkie zamowienia (jesli flaga 'wszystkie' ma wartosc true)
	- Wszystkie zamowienia dla danego klienta (jesli flaga 'wszystkie' ma wartosc false i flaga 'pokliencie' ma wartosc true)
	- Wszystkie zamowienia dla danego przedmiotu (jesli flaga 'wszystkie' ma wartosc false i flaga 'pokliencie' ma wartosc false)

	W zaleznosci od konfiguracji id przekazywane do funkcji odnosi do do id_przedmiotu lub id_klienta*/

	//Wyswietlenie naglowka tabeli
	cout << "ID zamowienia\tID przedmiotu\tNazwa Przedmiotu, Poziom - Nazwisko Imie [ID klienta]\n";

	int wszystkieIlosc = 0;
	int pokliencieIlosc = 0;
	int poprzedmiocieIlosc = 0;

	//Petla iterujaca po bazie danych
	for (auto it = this->Lista.begin(); it != this->Lista.end(); ++it) {

		if (!wszystkie) {
			//Jezeli flaga 'wszystkie' ma wartosc false
			if (pokliencie) {
				//Jezeli flaga 'pokliencie' ma wartosc true
				if ((*it).id_klienta == id) {
					this->wyswietlwiersz(it);
					pokliencieIlosc++;
				}
			}
			else {
				//Jezeli flaga 'pokliencie' ma wartosc false
				if ((*it).id_przedmiotu == id) {
					this->wyswietlwiersz(it);
					poprzedmiocieIlosc++;
				}
			}
		}
		else {
			//Jezeli flaga 'wszystkie' ma wartosc true wyswietlamy bezwarunkowo kazdy wiersz
			this->wyswietlwiersz(it);
			wszystkieIlosc++;
		}
	}
	cout << "\nLaczna liczba ";
	if (!wszystkie) {
		if (pokliencie) {
			cout << "zamowien dla tego klienta wynosi: " << pokliencieIlosc;
		}
		else {
			cout << "zamowien dla tego przedmiotu wynosi: " << poprzedmiocieIlosc;
		}
	}
	else {
		cout << "wszystkich zamowien wynosi: " << wszystkieIlosc;
	}
	cout << "\n\n";

}

  void ListaZamowien::wyswietlzamowieniamenu() {
	/* Funkcja obslugujaca podmenu 'Wyswietl zamowienia' */

	//Flaga ktora warunkuje wyswietlanie sie podmenu
	bool warunekpetli = true;
	while (warunekpetli) {
		//Wyswietlenie dostepnych opcji
		cout << "::Ustal sposob wyswietlania zamowien::\n";
		cout << "1 - Wyswietl wszystkie\n2 - Wyswietl po ID przedmiotu\n3 - Wyswietl po ID klienta\n"
			<< "4 - Wyswietl liste przedmiotow\n5 - Wyswietl liste klientow\n0 - Wroc do poprzedniego menu\n";
		int wybor = -1;
		cin >> wybor;
		//Zmienna pomocnicza dla niektorych z opcji
		int id_dostepnosci;
		switch (wybor) {
		case 1: //Wyswietl wszystkie
			this->wyswietlzamowienia();
			break;
		case 2: //Wyswietl po ID przedmiotu
			cout << "Wprowadz ID przedmiotu:\n";
			cin >> id_dostepnosci;
			this->wyswietlzamowienia(false, false, id_dostepnosci);
			break;
		case 3: //Wyswietl po ID klienta
			cout << "Wprowadz ID klienta:\n";
			cin >> id_dostepnosci;
			this->wyswietlzamowienia(false, true, id_dostepnosci);
			break;
		case 4: //Wyswietl liste przedmiotow
			ListaPrzedmiotow::wyswietlliste();
			break;
		case 5: //Wyswietl liste klientow
			ListaKlientow::wyswietlliste();
			break;
		default: //Wroc do poprzedniego menu
			warunekpetli = false;
			break;
		}
	}
}

  bool ListaZamowien::czyistniejerekord(int id_wpisu) {
	/* Funkcja przeznaczona do poinformowania o istnieju badz nieistnieniu rekordu w bazie o podanym 'id_wpisu' */
	for (auto it = Lista.begin(); it != Lista.end(); ++it) {
		if ((*it).id_wpisu == id_wpisu) {
			return true;
		}
	}
	return false;
}

  void ListaZamowien::usunwpis(int id_wpisu) {
	/* Funkcja przeznaczona do usuwania rekordu z bazy na podstawie 'id_wpisu' */
	for (auto it = Lista.begin(); it != Lista.end(); ++it) {
		if ((*it).id_wpisu == id_wpisu) {
			it = Lista.erase(it);
			break;
		}
	}
}

  void ListaZamowien::usunwszystkiewpisy_idklienta(int id_klienta) {
	/* Funkcja przeznaczona do usuwania wszystkich rekordow z bazy od danym 'id_klienta'
	Funkcji uzywa modul ListaKlientow do usuwania wszystkich zamowien dla danego klienta jezeli klient istnieje*/
	for (auto it = Lista.begin(); it != Lista.end(); ++it) {
		if ((*it).id_klienta == id_klienta) {
			it = Lista.erase(it);
		}
	}
}

  void ListaZamowien::usunwszystkiewpisy_idprzedmiotu(int id_przedmiotu) {
	/* Funkcja przeznaczona do usuwania wszystkich rekordow z bazy od danym 'id_przedmiotu'
	Funkcji uzywa modul ListaPrzedmiotow do usuwania wszystkich zamowien dla danego przedmiotu jezeli przedmiot istnieje*/
	for (auto it = Lista.begin(); it != Lista.end(); ++it) {
		if ((*it).id_przedmiotu == id_przedmiotu) {
			it = Lista.erase(it);
		}
	}
}

  void ListaZamowien::przylaczklienta() {
	/*Funkcja pomocnicza dla menu przylaczania klienta
	Interfejs nie oferuje opcji wycofania sie z wprowadzonych zmian, poniewaz latwo je wycofac recznie*/
	//Wyswietlenie komunikatu
	cout << "Wprowadz ID klienta:\n";
	//Zmienna przechowujaca wprowadzone ID_klienta przez strumien wejsciowy
	int id_klienta;
	//Wczytanie ID klienta ze strumienia wejsciowego
	cin >> id_klienta;
	//Sprawdzamy czy w bazie danych klientow istnieje rekord o podanym numerze ID
	if (ListaKlientow::czyistniejrekord(id_klienta))
	{
		cout << "Wybrano: " << ListaKlientow::getNazwiskoKlienta(id_klienta) << " " << ListaKlientow::getImieKlienta(id_klienta) << '[' << id_klienta << "]\n";
		
		//Flaga ktora warunkuje dzialanie interfejsu
		bool warunekpetli = true;
		while (warunekpetli) {
			//Wyswietlenie dostepnych opcji
			cout << "::PRZYLACZANIE KLIENTA DO PRZEDMIOTU::\n";
			cout << "1 - Dalej\n2 - Wyswietl liste przedmiotow\n0 - Wroc do poprzedniego menu\n";
			//Zmienna przechowujaca wybor uzytkownika
			int wybor = -1;
			//Wczytanie wyboru ze strumienia wejsciowego
			cin >> wybor;
			//Zmienna przechowujaca wprowadzone ID przedmiotu
			int id_przedmiotu;
			//Zmienna przechowujaca ilosc jednostek czasowych dla danego przedmiotu
			int ilosc_jednostek;
			ZamowionyPrzedmiot *Nowy;
			switch (wybor) {
			case 1: //Dalej
					//Wyswietlenie instrukcji
				cout << "Wprowadz ID przedmiotu:\n";
				//Wczytanie ID przedmiotu ze strumienia wejsciowego
				cin >> id_przedmiotu;
				if (ListaPrzedmiotow::czyistniejrekord(id_przedmiotu)) {
					//Jezeli przedmiot istnieje w bazie danych wykonujemy dalszy ciag instrukcji
					//Wyswietlamy powiadomienie o wybranym przedmiocie
					cout << "Wybrano " << ListaPrzedmiotow::getNazwaPrzedmiotu(id_przedmiotu) << ", " << ListaPrzedmiotow::getPoziomPrzedmiotu(id_przedmiotu) << ". Cena: " << ListaPrzedmiotow::getCenaPrzedmiotu(id_przedmiotu) << " PLN\n";
					//Wyswietlenie instrukcji
					cout << "Podaj ilosc jednostek czasowych:\n";
					//Wczytanie liczby jednostek czasowych ze strumienia wejsciowego
					cin >> ilosc_jednostek;
					//Sprawdzamy czy podana wartosc nie jest przypadkiem mniejsza od 0
					if (ilosc_jednostek > 0) {
						for (int i = 0; i < ilosc_jednostek; i++) {
							//Petla, poniewaz dla wielu jednostek czasowych bedzie odpowiednia ilosc wpisow w bazie
							//Tworzymy nowy rekord bazy
							Nowy = new ZamowionyPrzedmiot(id_klienta, id_przedmiotu); //MOZLIWY WYCIEK PAMIECI??
							this->Lista.push_back((*Nowy)); //Dodajemy rekord do bazy danych
							cout << "Dodano nowy wpis:\n";
							//Wyswietlenie dodanego wpisu
							this->wyswietlwiersz(this->znajdz(ZamowionyPrzedmiot::ostatnie_id));
						}
						//Zapis zmian do pliku
						ListaZamowien::zapiszzmiany();
						//Zadanie wykonane, wiec zmieniamy flage na false powodujac wyjscie z interfejsu
						warunekpetli = false;
					}
				}
				else {
					//Komunikat w razie wpisania nieprawidlowego ID przedmiotu
					cout << "Podano nieprawidlowe ID przedmiotu. Sprobuj jeszcze raz.\n";
				}
				break;
			case 2: //Wyswietl liste przedmiotow
				ListaPrzedmiotow::wyswietlliste();
				break;
			default: //Wroc do poprzedniego menu
				warunekpetli = false;
				break;
			}
		}
	}
	else {
		//Komunikat w razie wpisania ID ktore nie zawiera sie w bazie klientow
		cout << "Podane ID nie istnieje w bazie klientow. Sprobuj jeszcze raz.\n";
	}

}

  void ListaZamowien::przylaczklientamenu() {
	/* Funkcja obslugujaca podmenu 'Przylacz klienta' */

	//Flaga ktora warunkuje wyswietlanie sie podmenu
	bool warunekpetli = true;
	while (warunekpetli) {
		//Wyswietlenie dostepnych opcji
		cout << "::PRZYLACZANIE KLIENTA DO PRZEDMIOTU::\n";
		cout << "1 - Przylacz\n2 - Wyswietl liste klientow\n0 - Wroc do poprzedniego menu\n";
		int wybor = -1;
		cin >> wybor;
		switch (wybor) {
		case 1: //Przylacz
				//Uruchomienie specjalnego interfejsu do przylaczania klienta
			this->przylaczklienta();
			break;
		case 2: //Wyswietl liste klientow
			ListaKlientow::wyswietlliste();
			break;
		default: //Wroc do poprzedniego menu
			warunekpetli = false;
			break;
		}
	}
}

  void ListaZamowien::odlaczklientamenu() {
	/* Funkcja obslugujaca podmenu 'Odlacz klienta' */

	//Wyswietlenie instrukcji
	cout << "\n::ODLACZANIE KLIENTA - MENU::\n"
		<< "Wprowadz ID zamowienia w bazie\nWpisz 0 aby wrocic do poprzedniego menu\n";
	//Zmienna przechowujaca wprowadzony ID zamowienia
	int id_zamowienia;
	//Wczytanie wartosci ID zamowienia ze strumienia wejsciowego
	cin >> id_zamowienia;
	if (id_zamowienia != 0) {
		//Sprawdzamy czy rekord o danym ID istnieje w bazie
		if (czyistniejerekord(id_zamowienia)) {
			//Jezeli istnieje usuwamy rekord z bazy
			auto it = this->znajdz(id_zamowienia);
			cout << "Usunieto wpis:\n";
			this->wyswietlwiersz(it);
			this->usunwpis(id_zamowienia);
			ListaZamowien::zapiszzmiany();
		}
		else {
			//Jezeli rekord nie istnieje wyswietlamy odpowiedni komunikat
			cout << "Podane ID zamowienia nie istnieje w bazie.\n";
		}
	}

}
