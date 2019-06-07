#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <conio.h>
#include <climits>

using namespace std;

/// klasa do algorytmu rpq
class RPQ {
public:
	int r;      ///czas przygotowania
	int p;      ///czas wykonania
	int q;      ///czas dostarczen
	int nr;     ///iloscZadan
	RPQ(int r, int p, int q, int nr) : r(r), p(p), q(q), nr(nr) {}
	RPQ() { p = r = q = nr = 0; }
	friend ostream & operator << (ostream & os, const RPQ & r);
};

ostream & operator << (ostream & os, const RPQ & r)
{
	os << r.nr + 1 << "  ";
	return os;
}

///struktury dla dwoch czasow - przygotowania i dostarczania
struct R { bool operator ()(const RPQ & a, const RPQ & b) { return a.r > b.r; } };
struct Q { bool operator ()(const RPQ & a, const RPQ & b) { return a.q < b.q; } };


/// funkcja max
int max(int a, int b)
{
	if (a < b) return b;
	else
		return a;
}


int main()
{
	priority_queue<RPQ, vector<RPQ>, Q> wektorG;    ///zbior zadan gotowych do uszeregowania - kolejka priorytetowa
	priority_queue<RPQ, vector<RPQ>, R> wektorN;    ///zbior zadan nieuszeregowanych - kolejka priorytetowa


	ifstream plik;      ///wczytywanie pliku
	plik.open("in200.txt");
	int liczbaDanych = 0;
	int r, p, q;
	int iloscZadan = 0;
	int t = 0, Cmax = 0;
	plik >> liczbaDanych;
	plik >> r;

/** wyswietlanie danych z pliku **/
	for (int i = 0; i < liczbaDanych; ++i)
	{
		plik >> r >> p >> q;
		///cout << r << "   " << p << "   " << q << endl;
		RPQ Dane(r, p, q, iloscZadan);
		wektorN.push(Dane);
		iloscZadan++;
	}
///	cout << "\n\n";
    cout << "Kolejnosc wykonywanych zadan : \n";

	/**Zmienna posiadająca bardzo dużą wartość (czyt. nieskończoną) podstawiana
	do zmiennej 'q' w celu zabezpieczenia zadania przed przerwaniem */
	int liczba_zabezpieczajaca_przerwanie = INT_MAX;
	RPQ Maszyna(0, 0, liczba_zabezpieczajaca_przerwanie, 0);

	while (wektorG.empty() == false || wektorN.empty() == false) { ///dopoki wektor G lub wektor N nie sa puste
                                                                ///budowanie zbioru zadan gotowych do uszeregwania
		while (wektorN.empty() == false && wektorN.top().r <= t) { ///dopoki wektor N nie jest pusty oraz czas przygotowania pierwszego zadania

			RPQ tmp = wektorN.top();   /**tworzymy tmp o strukturze elementu klasy RPQ
                                        i przypisujemy jej ostatnio dodany element stosu wektora N */

			wektorG.push(wektorN.top()); ///do wektora G dodajemy pierwszy element z wektora N
			wektorN.pop(); ///usuwamy pierwszy element z wektora N


            /** porownywanie czasu dostarczania ostatnio dodanego zadania na maszyne(wektorG)
            z czasem zadania wykonujacego sie na maszynie. Gdy czas dodanego jest wiekszy to zadanie
            jest przerywane a pozostala czesc zadania - o czasie t - tmp.r jest wkladana do
            zbioru zadan gotowych do szeregowania */

			if (tmp.q > Maszyna.q)
			{
				Maszyna.p = t - tmp.r;
				t = tmp.r;
				wektorG.push(Maszyna);
			}
		}

		/// dla pustego wektora G - brak zadan do uszeregowania - aktualizujemy pomocniczy czas
		/// jesli wektor G nie jest pusty to aktualizujemy 't' o czas przygotowania
		if (wektorG.empty() == true) {
			t = wektorN.top().r;
			continue;
		}
		else {
            RPQ kolejnosc(0, 0, 0, 0);
			kolejnosc = wektorG.top();
			cout << kolejnosc;
			wektorG.pop();
			Maszyna = kolejnosc; //dodane
			t += kolejnosc.p;
			Cmax = max(Cmax, t + kolejnosc.q);
		}
	}

	cout << "\n\nNasz Cmax wynosi: " << Cmax << endl;
	_getch();
}
