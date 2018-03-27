#ifndef ALGORYTMZACHLANNY_HPP
#define ALGORYTMZACHLANNY_HPP


extern std::vector<std::string> DniTygodnia;


class Generator {
	friend class Program;
public: Plan WygenerowanyPlan;
protected: std::vector<ZamowieniezPriorytetem> Zamowienia;

public: Generator();
public: void FunkcjaGlowna();
protected: void GenerujPlan();
protected: bool ProbaDodaniaZamowienia(std::vector<ZamowieniezPriorytetem>::iterator &iterator);
protected: void UmiescZamowienieNaPlanie(int id_wpisu,int dzien, int start, int koniec);
protected: bool SprawdzDanyDzien(const std::vector<ZamowieniezPriorytetem>::iterator & iterator, int dzien);
protected: int getIloscZamowionychPrzedmiotow(std::vector<ZamowionyPrzedmiot>::iterator iterator);
protected: double CzasDostepnosciKlienta(ZamowieniezPriorytetem & Zamowienie);
protected: void TworzenieKolekcjiZamowienPriorytetowych();
protected: void SortujZamowieniaPriorytetowo();
protected: void PlanDoPliku();
protected:std::vector<ZamowieniezPriorytetem>::iterator Generator::CzyDanyKlientJestJuzOpriorytetowany(std::vector<ZamowionyPrzedmiot>::iterator iterator);


};

class ZamowieniezPriorytetem {
	friend class Generator;
protected: std::vector<ZamowionyPrzedmiot>::iterator Zamowienie;
protected: std::vector<DostepnoscKlienta> DostepnoscDanegoKlienta;
protected: double Priorytet;
public: ZamowieniezPriorytetem(std::vector<ZamowionyPrzedmiot>::iterator it);
public: ZamowieniezPriorytetem(std::vector<ZamowionyPrzedmiot>::iterator it, std::vector<DostepnoscKlienta> DostepnoscDanegoKlienta,double Priorytet);

protected: void nadajPriorytet(double CzasDostepnosciKlienta, int IloscZamowionychPrzedmiotow);

};



#endif // !ALGORYTMZACHLANNY_HPP

