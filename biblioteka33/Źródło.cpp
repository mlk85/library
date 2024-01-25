#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Czytelnik
{
public:
	string imie;
	string nazwisko;
	string nrId;
	bool status = false;

	Czytelnik() {}
	Czytelnik(const string& imie, const string& nazwisko, const string& nrId)
		:imie(imie), nazwisko(nazwisko), nrId(nrId) {};

	bool czyPosiada()const { return status; }
	void ustawStatus(bool stat) { status = stat; }
	
	void zapisz(const string& nazwaPliku) {
		ofstream plik(nazwaPliku, ios::app);

		if (!plik.is_open()) {
			cout << "Nie udalo sie otworzyc pliku do zapisu." << endl;
		}

		plik << imie << " " << nazwisko << "\n\n";
	}
};

class Ksiazka
{
public:
	string nazwa;
	string autor;
	bool dostepna;

	Ksiazka() {}
	Ksiazka(const string& nazwa, const string& autor)
		: nazwa(nazwa), autor(autor), dostepna(true) {}

	bool czyDostepna()const { return dostepna; }
	void ustawDostepnosc(bool dostepnosc) { dostepna = dostepnosc; }

};

class Biblioteka
{
public:
	vector<Ksiazka> listaKsiazek;
	vector<Czytelnik> listaOczekujacych;

	//
	/*void odczytajKsiazkiZPliku(const string& nazwaPliku) {
		ifstream plik(nazwaPliku);
		if (!plik.is_open()) {
			cout << "Nie udalo sie otworzyc pliku do odczytu." << endl;
			return;
		}

		while (!plik.eof()) {
			Ksiazka ksiazka;
			getline(plik, ksiazka.nazwa);
			plik >> ksiazka.dostepna;
			plik.ignore(numeric_limits<streamsize>::max(), '\n');
			listaKsiazek.push_back(ksiazka);
		}

		plik.close();
	}*/
	//
	void podanieCzytelnika(const Czytelnik& czytelnik) {
		cout << "Czytelnik " << czytelnik.imie << " " << czytelnik.nazwisko << " zarejestrowany." << endl;
	}

	bool wyswietlenieDostepnosci(const Ksiazka& ksiazka) const {
		cout << "Dostepnosc ksiazki \"" << ksiazka.nazwa << "\" autorstwa " << ksiazka.autor << ": " << (ksiazka.czyDostepna() ? "Dostepna" : "Niedostepna") << endl;
		return ksiazka.czyDostepna();
	}

	void zapisNaListe(const Czytelnik& czytelnik, const Ksiazka& ksiazka) {
		cout << "Czytelnik " << czytelnik.imie << " " << czytelnik.nazwisko << " zapisal sie na liste oczekujacych" << endl;
		cout << "na ksiazke \"" << ksiazka.nazwa << "\"." << endl;
		listaOczekujacych.push_back(czytelnik);
	}

	void wypozycz(Czytelnik& czytelnik, Ksiazka& ksiazka) {
		if (ksiazka.czyDostepna()) {
			cout << "Ksiazka \"" << ksiazka.nazwa << "\" wypozyczona przez " << czytelnik.imie << " " << czytelnik.nazwisko << "." << endl;
			ksiazka.ustawDostepnosc(false);
			czytelnik.ustawStatus(true);
		}
		else {
			cout << "Ksiazka niedostepna. Zapis czytelnika " << czytelnik.imie << " " << czytelnik.nazwisko << " na liste oczekujacych." << endl;
			zapisNaListe(czytelnik, ksiazka);
		}
	}

	void zwrocKsiazke(Czytelnik& czytelnik, Ksiazka& ksiazka) {
		cout << "Ksiazka \"" << ksiazka.nazwa << "\" zwrocona przez " << czytelnik.imie << " " << czytelnik.nazwisko << "." << endl;
		ksiazka.ustawDostepnosc(true);
		powiadomODostepnosciKsiazki(ksiazka);
	}

	void powiadomODostepnosciKsiazki(const Ksiazka& ksiazka) {
		cout << "Powiadomienie o dostepnosci ksiazki \"" << ksiazka.nazwa << "\" wyslane do:" << endl;
		for (const auto& czytelnik : listaOczekujacych) {
			cout << czytelnik.nrId << ", ";
		}
		cout << endl;
	}

};

Czytelnik wczytajDane() {
	string imie, nazwisko, id;
	cout << "Podaj imie: ";
	cin >> imie;
	cout << "Podaj nazwisko: ";
	cin >> nazwisko;
	cout << "Podaj ID: ";
	cin >> id;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	return Czytelnik(imie, nazwisko, id);
}

Ksiazka podajKsiazke() {
	string nazwa, autor;
	cout << "Podaj nazwe ksiazki ktora chcesz wypozyczyc: ";
	getline(cin, nazwa);
	cout << "Podaj autora: ";
	getline(cin, autor);
	return Ksiazka(nazwa, autor);
}

void wyswietlKsiazke(const Ksiazka& ksiazka) {
	cout << "Nazwa: \"" << ksiazka.nazwa << "\"" << endl;
	cout << "Autor: " << ksiazka.autor << endl;
	cout << "Dostepnosc: " << (ksiazka.czyDostepna() ? "Dostepna" : "Niedostepna") << endl;
}

void menu(Ksiazka& ksiazka, Czytelnik& czytelnik, Biblioteka& biblioteka) {
	int k;
	do {
		cout << "Wybierz co chcesz zrobic:" << endl;
		cout << "1. Dodaj uzytkownika" << endl;
		cout << "2. Zwroc ksiazke" << endl;
		cout << "3. Sprawdz dostepnosc ksiazki" << endl;
		cout << "4. Zakoncz program" << endl;
		cout << "5. Wypozycz ksiazke" << endl << endl;

		cin >> k;

		switch (k) {
		case 1:
			czytelnik = wczytajDane();
			czytelnik.zapisz("uzytkownicy.txt");
			biblioteka.podanieCzytelnika(czytelnik);
			break;

		case 2:
			if (czytelnik.czyPosiada()) {
				biblioteka.zwrocKsiazke(czytelnik, ksiazka);
			}
			else {
				cout << "Nie mozesz zwrocic ksiazki, ktorej nie posiadasz." << endl;
			}
			break;

		case 3:
			cin.get();
			ksiazka = podajKsiazke();
			biblioteka.wyswietlenieDostepnosci(ksiazka);
			break;

		case 4:
			cout << "Koniec programu." << endl;
			break;

		case 5:
			cin.get();
			ksiazka = podajKsiazke();
			if (ksiazka.czyDostepna()) {
				biblioteka.wypozycz(czytelnik, ksiazka);
			}
			break;

		default:
			cout << "Bledna instrukcja." << endl;
			break;
		}
	} while (k != 4);
}

int main()
{
	Biblioteka biblioteka;
	Czytelnik czytelnik1;
	Ksiazka ksiazka1;

	menu(ksiazka1, czytelnik1, biblioteka);
}