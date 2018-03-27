#ifndef KLASA_PLAN_HPP
#define KLASA_PLAN_HPP

class Plan {

	friend class Generator;
	friend class Program;

protected: int rozkladZajec[7][720];
protected: bool czySaZajeciaDzien[7];
public: int przychod;
protected: int iloscZajec;
protected: std::vector<int> dodaneZamowieniaID;
protected: std::vector<int> niezrealizowaneZamowieniaID;

//////////////////////////////////////////////////////////////////////////////////////////
		

public: Plan();
public: Plan(int rozkladZajec[7][720]);;
};

#endif