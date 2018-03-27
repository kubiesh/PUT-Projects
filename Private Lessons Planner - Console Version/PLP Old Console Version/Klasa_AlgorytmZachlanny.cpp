#include "stdafx.h"
#include <random>

int randomInt(int min, int max)
{
	static random_device r;
	static default_random_engine e(r());
	uniform_int_distribution<int> d(min, max);
	return d(e);
}

Generator::Generator() {
	Plan *Temp = new Plan(Parametry::Dostepnosc);
	WygenerowanyPlan = (*Temp);
}

void Generator::FunkcjaGlowna()
{
	this->TworzenieKolekcjiZamowienPriorytetowych();
	this->GenerujPlan();
}

void Generator::GenerujPlan()
{
	for (auto it = Zamowienia.begin(); it != Zamowienia.end(); ++it) {
		bool CzyDodanoZamowienie = this->ProbaDodaniaZamowienia(it);
		if (CzyDodanoZamowienie) {
			this->WygenerowanyPlan.przychod = this->WygenerowanyPlan.przychod + ListaZamowien::getCenaPrzedmiotu((*(*it).Zamowienie).id_wpisu);
			this->WygenerowanyPlan.iloscZajec = this->WygenerowanyPlan.iloscZajec + 1;
			this->WygenerowanyPlan.dodaneZamowieniaID.push_back((*(*it).Zamowienie).id_wpisu);
			cout << "Zamowienie " << (*(*it).Zamowienie).id_wpisu << " umieszczono na planie.\n";
		}
		else {
			this->WygenerowanyPlan.niezrealizowaneZamowieniaID.push_back((*(*it).Zamowienie).id_wpisu);
			cout << "Zamowienia " << (*(*it).Zamowienie).id_wpisu << " nie umieszczono na planie.\n";
		}
	}
	cout << "Wygenerowano plan.";
}

bool CzyWszystkieMajaWartoscTrue(const std::vector<bool>&CzySprawdzonoPrzedzial) {
	for (auto el : CzySprawdzonoPrzedzial) {
		if (el == false) {
			return false;
		}
	}
	return true;
}

bool Generator::ProbaDodaniaZamowienia(std::vector<ZamowieniezPriorytetem>::iterator & iterator)
{
	//Flaga dodania zamowienia do planu
	bool Dodano = false;
	//Tablica bool o takim samym rozmiarze co tablica dostepnosci dla danego klienta
	std::vector<bool>CzySprawdzonoPrzedzial((*iterator).DostepnoscDanegoKlienta.size());
	//Nadawanie domyslnych wartosci
	for (auto el : CzySprawdzonoPrzedzial) {
		el = false;
	}
	


	while (!CzyWszystkieMajaWartoscTrue(CzySprawdzonoPrzedzial) && Dodano==false) {
		//Losowanie indeksu
		int WylosowanyIndeksDostepnosci = randomInt(0, CzySprawdzonoPrzedzial.size() - 1);
		if (CzySprawdzonoPrzedzial[WylosowanyIndeksDostepnosci] == false) {
			int dzien = (*iterator).DostepnoscDanegoKlienta[WylosowanyIndeksDostepnosci].dzien;
			//Sprawdzenie czy w danym dniu cos bylo
			if (!this->SprawdzDanyDzien(iterator, dzien)) {

				int start = (*iterator).DostepnoscDanegoKlienta[WylosowanyIndeksDostepnosci].start;
				int dolna_granica = start;
				int gorna_granica = start + 11;
				int koniec = (*iterator).DostepnoscDanegoKlienta[WylosowanyIndeksDostepnosci].koniec;

				while (gorna_granica != koniec + 1) {
					int WarunekKonieczny = (gorna_granica + dolna_granica) / 2;
					if (this->WygenerowanyPlan.rozkladZajec[dzien][dolna_granica] == 0 && this->WygenerowanyPlan.rozkladZajec[dzien][gorna_granica] == 0 && this->WygenerowanyPlan.rozkladZajec[dzien][WarunekKonieczny] == 0) {
						int id_zamowienia = (*(*iterator).Zamowienie).id_wpisu;
						this->UmiescZamowienieNaPlanie(id_zamowienia, dzien, dolna_granica, gorna_granica);
						Dodano = true;
						return true;
					}



					dolna_granica = dolna_granica + 1;
					gorna_granica = gorna_granica + 1;
				}
				if (Dodano == false) {
					CzySprawdzonoPrzedzial[WylosowanyIndeksDostepnosci] = true;
				}
			}
			else {
				//W tej wersji nie robimy zajec tego samego dnia
				CzySprawdzonoPrzedzial[WylosowanyIndeksDostepnosci] = true;
			}
		}
	
	}
	

	return false;
}

void Generator::UmiescZamowienieNaPlanie(int id_wpisu,int dzien, int start, int koniec)
{
	if (!this->WygenerowanyPlan.czySaZajeciaDzien[dzien]) {
		this->WygenerowanyPlan.czySaZajeciaDzien[dzien] = true;
	}
	//Wiemy ze mozna wiec dodajemy
	for (int i = start; i <= koniec; i++) {
		this->WygenerowanyPlan.rozkladZajec[dzien][i] = id_wpisu;
	}
	////Oznaczanie obrzezy PRZERWA
	////Dol
	//for (int i = 1; i <= 2; i++) {
	//	if (WygenerowanyPlan.rozkladZajec[dzien][start - i] == 0) {
	//		WygenerowanyPlan.rozkladZajec[dzien][start - i] = -2;
	//	}
	//}
	////Gora
	//for (int i = 1; i <= 2; i++) {
	//	if (WygenerowanyPlan.rozkladZajec[dzien][koniec + i] == 0) {
	//		WygenerowanyPlan.rozkladZajec[dzien][koniec + i] = -2;
	//	}
	//}
}

bool Generator::SprawdzDanyDzien(const std::vector<ZamowieniezPriorytetem>::iterator & iterator, int dzien)
{
	for (int j = 0; j < 720; j++) {
		if (this->WygenerowanyPlan.rozkladZajec[dzien][j] > 0) {
			int id_zamowienia = this->WygenerowanyPlan.rozkladZajec[dzien][j];
			if (ListaZamowien::getIDKlienta(id_zamowienia) == (*(*iterator).Zamowienie).id_klienta) {
				return true;
			}
		}

	}

	return false;
}

int Generator::getIloscZamowionychPrzedmiotow(std::vector<ZamowionyPrzedmiot>::iterator iterator) {
	int id_klienta = (*iterator).id_klienta;
	int ilosc_wystapien = 0;
	for (; iterator != ListaZamowien::Lista.end(); ++iterator) {
		if ((*iterator).id_klienta == id_klienta) {
			ilosc_wystapien++;
		}
	}
	return ilosc_wystapien;
}
double Generator::CzasDostepnosciKlienta(ZamowieniezPriorytetem &Zamowienie) {
	double lacznyCzasPrzedzialu{};
	for (auto it = ListaKlientow::Lista_dostepnosc.begin(); it != ListaKlientow::Lista_dostepnosc.end(); ++it) {
		if ((*it).id_klienta == (*Zamowienie.Zamowienie).id_klienta) {
			//Dodanie rekordu do mini-bazy rekordow dla danego zamowienia
			Zamowienie.DostepnoscDanegoKlienta.push_back(*it);
			lacznyCzasPrzedzialu=lacznyCzasPrzedzialu+(*it).ObliczCzasPrzedzialu();
		}
	}
	return lacznyCzasPrzedzialu;

}

void Generator::TworzenieKolekcjiZamowienPriorytetowych()
{
	for (auto it = ListaZamowien::Lista.begin(); it != ListaZamowien::Lista.end(); ++it) {
		auto OdpowiednikzPriorytetem = CzyDanyKlientJestJuzOpriorytetowany(it);
		ZamowieniezPriorytetem *Nowy;
		if (OdpowiednikzPriorytetem ==this->Zamowienia.end()) {
			Nowy = new ZamowieniezPriorytetem(it);
			int IloscZamowionychPrzedmiotow = this->getIloscZamowionychPrzedmiotow(it);
			double CzasDostepnosciKlienta = this->CzasDostepnosciKlienta((*Nowy));
			if (CzasDostepnosciKlienta < 1) {
				this->WygenerowanyPlan.niezrealizowaneZamowieniaID.push_back((*it).id_wpisu);
				cout << "Klient niedyspozycjny. Zamowienie " << ((*it).id_wpisu) << " nie dodane do harmonogramu.\n";
				continue;
			}
			Nowy->nadajPriorytet(CzasDostepnosciKlienta, IloscZamowionychPrzedmiotow);
		}
		else {
			//Kopia danych oprocz iteratora
			Nowy = new ZamowieniezPriorytetem(it, (*OdpowiednikzPriorytetem).DostepnoscDanegoKlienta, (*OdpowiednikzPriorytetem).Priorytet);
			
		}
		cout << "Do kolekcji zamowien dodano: " << (*(*Nowy).Zamowienie).id_wpisu << '\n';
		this->Zamowienia.push_back((*Nowy));
	}
	cout << "Zadano wszystkim zamowieniom priorytety.\n";
	this->SortujZamowieniaPriorytetowo();
}

void Generator::PlanDoPliku() {

	BasicExcel e;
	e.New(1);
	BasicExcelWorksheet * arkusz = e.GetWorksheet(0);
	size_t Row = 0;
	size_t Column = 0;
	BasicExcelCell * komorka;
	for (int i = 0; i < 7; i++) {
		if (this->WygenerowanyPlan.czySaZajeciaDzien[i] || i==0) {
			if (WygenerowanyPlan.czySaZajeciaDzien[i]) {


				//Wypisanie dnia
				komorka = arkusz->Cell(Row, Column);
				komorka->Set(DniTygodnia[i].c_str());
				komorka = arkusz->Cell(Row, Column + 1);
				komorka->Set(DniTygodnia[i].c_str());
				Row++;


				int ostatniaWartosc = this->WygenerowanyPlan.rozkladZajec[i][0]; //Pobieramy pierwsza wartosc
				int indeksPoczatkuPrzedzialu = 0;
				int indeksKoncaPrzedzialu;
				bool koniecPrzedzialu = false;
				for (int j = 1; j < 720; j++) {

					if (ostatniaWartosc != this->WygenerowanyPlan.rozkladZajec[i][j] || j == 719) {

						if (j == 719) {
							indeksKoncaPrzedzialu = j;
						}
						else {
							indeksKoncaPrzedzialu = j;
						}


						if (ostatniaWartosc != -1) {

							//Sekwencja Excelowa
							//Jezeli -1 to nic nie rob
							//Jezeli -2 to przerwa
							//Jezeli 0 top niewykorzystanyczas
							std::string Przedzial = Parametry::przekazWartoscPrzedzialu(indeksPoczatkuPrzedzialu, indeksKoncaPrzedzialu);
							komorka = arkusz->Cell(Row, Column);
							komorka->Set(Przedzial.c_str());
							if (ostatniaWartosc == 0 || ostatniaWartosc == -2) {
								std::string komunikat;
								if (ostatniaWartosc == 0) {
									komunikat = "NIEWYKORZYSTANY CZAS";
								}
								if (ostatniaWartosc == -2) {
									komunikat = "PRZERWA";
								}

								komorka = arkusz->Cell(Row, Column + 1);
								komorka->Set(komunikat.c_str());
								Row++;
							}
							else {
								//NazwiskoImie
								komorka = arkusz->Cell(Row, Column + 1);
								int id_wpisu = ostatniaWartosc;
								std::string NazwiskoImie = ListaZamowien::getNazwiskoKlienta(id_wpisu) + ' ' + ListaZamowien::getImieKlienta(id_wpisu);
								komorka->Set(NazwiskoImie.c_str());
								Row++;
								//Nazwa przedmiotu
								komorka = arkusz->Cell(Row, Column + 1);
								std::string NazwaPrzedmiotu = ListaZamowien::getNazwaPrzedmiotu(id_wpisu);
								komorka->Set(NazwaPrzedmiotu.c_str());
								Row++;
								//Poziom przedmiotu
								komorka = arkusz->Cell(Row, Column + 1);
								std::string PoziomPrzedmiotu = ListaZamowien::getPoziomPrzedmiotu(id_wpisu);
								komorka->Set(PoziomPrzedmiotu.c_str());
								Row++;
								//Cena
								komorka = arkusz->Cell(Row, Column + 1);
								int CenaPrzedmiotu = ListaZamowien::getCenaPrzedmiotu(id_wpisu);
								std::ostringstream CenaOSS;
								CenaOSS << CenaPrzedmiotu<<"PLN/" << Parametry::jednostkaczasowa << "min";
								std::string CenaPrzedmiotuString = CenaOSS.str();
								komorka->Set(CenaPrzedmiotuString.c_str());
								Row++;
								//ID_klienta
								komorka = arkusz->Cell(Row, Column + 1);
								std::ostringstream IDklientaOSS;
								int id_klienta = ListaZamowien::getIDKlienta(id_wpisu);
								IDklientaOSS << "ID klienta: " << id_klienta;
								std::string IDklientaString = IDklientaOSS.str();
								komorka->Set(IDklientaString.c_str());
								Row++;
								//ID_zamowienia
								komorka = arkusz->Cell(Row, Column + 1);
								std::ostringstream IDzamowieniaOSS;
								IDzamowieniaOSS << "ID zamowienia: " << id_wpisu;
								std::string IDzamowieniaString = IDzamowieniaOSS.str();
								komorka->Set(IDzamowieniaString.c_str());
								Row++;

							}


						}


						//Na sam koniec:
						indeksPoczatkuPrzedzialu = j;
						ostatniaWartosc = this->WygenerowanyPlan.rozkladZajec[i][j];

					}

				}
			}
			if (WygenerowanyPlan.czySaZajeciaDzien[i] && i == 0)
			{
				Row = Row + 2;
			}
				if (i == 0) {
					//Wyswietlenie przychodu tygodniowego
				
					komorka = arkusz->Cell(Row, Column);
					std::string Przychod = "Przychod tygodniowy:";
					komorka->Set(Przychod.c_str());
					komorka = arkusz->Cell(Row, Column + 1);
					std::ostringstream PrzychodOSS;
					PrzychodOSS << this->WygenerowanyPlan.przychod << " PLN";
					Przychod = PrzychodOSS.str();
					komorka->Set(Przychod.c_str());
					//Wyswietlenie niezrealizowanych zamowien
					Row++;
					komorka = arkusz->Cell(Row, Column);
					std::string komunikat = "Niezrealizowane Zamowienia(ID):";
					komorka->Set(komunikat.c_str());
					for (auto element : this->WygenerowanyPlan.niezrealizowaneZamowieniaID) {
						komorka = arkusz->Cell(Row, Column + 1);
						komorka->Set(element);
						Row++;
					}

				}
			
			
			Column = Column + 3;
			Row = 0;
		}



	}





	//Funkcja aktualnej daty
	time_t czas;
	struct tm * data;
	char akt_data[20];
	time(&czas);
	data = localtime(&czas);
	strftime(akt_data, 40, "%Y-%m-%d_%H_%M", data);
	std::string nazwapliku = akt_data;
	nazwapliku = nazwapliku + ".xls";
	if (e.SaveAs(nazwapliku.c_str())) {
		cout << "Zapisano wygenerowany plan do pliku!\n";
	}
	else {
		cout << "Nie udalo sie zapisac wygenerowanego planu do pliku\n";
	}


	}

	std::vector<ZamowieniezPriorytetem>::iterator Generator::CzyDanyKlientJestJuzOpriorytetowany(std::vector<ZamowionyPrzedmiot>::iterator iterator)
	{
		auto it = this->Zamowienia.begin();
		for (; it != this->Zamowienia.end(); ++it) {
			if (iterator == (*it).Zamowienie) {
				return it;
			};
		}
		return it;
	}

	void Generator::SortujZamowieniaPriorytetowo()
	{
		std::sort(this->Zamowienia.begin(), this->Zamowienia.end(), [](const ZamowieniezPriorytetem &a, const ZamowieniezPriorytetem &b) -> bool { return a.Priorytet > b.Priorytet;  });
		cout << "Posortowano zamowienia wedlug priorytetow.\n";
	}

	ZamowieniezPriorytetem::ZamowieniezPriorytetem(std::vector<ZamowionyPrzedmiot>::iterator it) {
		Zamowienie = it;
		/*this->nadajPriorytet(CzasDostepnosciKlienta, IloscZamowionychPrzedmiotow);*/
	}

	ZamowieniezPriorytetem::ZamowieniezPriorytetem(std::vector<ZamowionyPrzedmiot>::iterator  it, std::vector<DostepnoscKlienta> DostepnoscDanegoKlienta, double Priorytet) {
		Zamowienie = it;
		this->DostepnoscDanegoKlienta = DostepnoscDanegoKlienta;
		this->Priorytet = Priorytet;
	}

	 void ZamowieniezPriorytetem::nadajPriorytet(double CzasDostepnosciKlienta, int IloscZamowionychPrzedmiotow) {
		this->Priorytet = CzasDostepnosciKlienta / (double)IloscZamowionychPrzedmiotow;
		this->Priorytet = this->Priorytet / ListaPrzedmiotow::getCenaPrzedmiotu((*Zamowienie).id_wpisu);
	}
