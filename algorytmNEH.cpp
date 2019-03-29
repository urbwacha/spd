#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <conio.h>
#include <climits>
#include <cstdlib>
#include <ctime>


using namespace std;

/**********************************/
/******* klasa algorytmu NEH ******/
/**********************************/
class NEH {
public:
	int nrMaszyny;                 ///numer maszyny
    int czasZadan;               ///czas wykonywania zadan
    vector<int> czasyNaJednejMaszynie;                ///wektor czasow danej maszyny, tak aby wczytywac kolumnami
	bool operator < (const NEH & zad) const { return (czasZadan < zad.czasZadan); }
	void liczTCalkowity();
};

void::NEH::liczTCalkowity()             ///funkcja liczenia czasu calkowitego
{
	czasZadan = 0;
	for (int i = 0; i < czasyNaJednejMaszynie.size(); i++)
		czasZadan += czasyNaJednejMaszynie[i];
}

/******************************************/
/******* liczenie czasu minimalnego  ******/
/******************************************/
int obliczCmax( vector<NEH> wektorZadan,  vector<int> tabPI, int ileZadan, int ileMaszyn)         ///liczenie Cmax czyli czasu minimalnego
{
	int *tabMaszyn = new int[ileMaszyn];
	for (int i = 0; i < ileMaszyn; i++)
		tabMaszyn[i] = 0;
	for (int i = 0; i < tabPI.size(); i++)
	{
		for (int j = 0; j < ileMaszyn; j++)
		{
			if (j > 0 && tabMaszyn[j - 1] > tabMaszyn[j])
				tabMaszyn[j] = tabMaszyn[j - 1];
			tabMaszyn[j] += wektorZadan[tabPI[i]].czasyNaJednejMaszynie[j];
		}
	}
	int Cmax = tabMaszyn[ileMaszyn - 1];
	delete[] tabMaszyn;
	return Cmax;
}

int main()
{
    float czas;
	clock_t start, koniec;
    start = clock();
    vector<NEH> wektorZadan;
    ifstream file;                  ///wczytywanie pliku
	file.open("data.txt");          ///otwieranie pliku

	if (!file.is_open())
	{
		cout << "Nie udalo sie otworzyc pliku z danymi!\n";
		exit(EXIT_FAILURE);
	}


	int ileZadan = 0;
	int ileMaszyn = 0;
	int sumaCzasow = 0;

	/**********************************************************************/
	/** 1) ladowanie danych do dwóch wektorów - 1: czasyNaJednejMaszynie  */
    /**    2: wektorZadan oraz podliczanie czasZadan na kazdej maszynie   */
    /**********************************************************************/
	file >> ileZadan >> ileMaszyn;      ///pierwsza linijka pliku

	for (int i = 0; i < ileZadan; i++)
	{
		NEH tym;            ///nowa zmienna tymczasowa
		for (int j = 0; j < ileMaszyn; j++)
		{
			int czasTym;
			file >> czasTym;
			tym.czasyNaJednejMaszynie.push_back(czasTym);
			sumaCzasow += czasTym;
		}
		tym.nrMaszyny = i;
		tym.liczTCalkowity();
		wektorZadan.push_back(tym);
	}

	/*********************************************************************/
	/**                        KROK 2                                    */
	/**  Sorowanie zadan nierosnaco po priorytetach (sumie czasów        */
    /**  wszystkich operacji na wszystkich maszynach dla danego zadania) */
    /*********************************************************************/
	sort(wektorZadan.rbegin(), wektorZadan.rend());

	 vector<int> tabPI;         ///wektory tablic Pi
	 vector<int> tabPItym;
	tabPI.push_back(0);
	int Cmax = 0;

    /**********************************************************************/
    /**                         KROK 3                                    */
    /**   Wybieranie zadania o najwiekszym piorytecie   i kolejno powrot  */
    /**   do poczatku petli, dzialanie w niej dopoki nie dojdziemy do     */
    /**   ulozenia liczby wszystkich zadan                                */
    /**********************************************************************/
	for (int i = 1; i < wektorZadan.size(); i++)
	{
		Cmax = INT_MAX; ///Cmax jest domyslnie najwiekszy

		/*************************************************************************/
		/**                            KROK 4                                    */
        /** Wstawianie zadania k na k pozycjach, wzgledem poprzednich dzialan    */
        /** Wszystkie dzialania wykonywanie na tablicy PI, pokazujacej kolejnosc */
        /*************************************************************************/
		for (int j = 0; j < i + 1; j++)
		{
			tabPI.insert(tabPI.begin() + j, i);
			int c = obliczCmax(wektorZadan, tabPI, ileZadan, ileMaszyn);
			/// warunek sprawdzania czy aktualny Cmax jest mniejszy od poprzedniego
			/// jesli tak, przypisanie wartosci aktualnego
            if (c < Cmax)
			{
				Cmax = c;
				tabPItym = tabPI;
			}
			tabPI.erase(tabPI.begin() + j);
		}
		tabPI = tabPItym;
	}

	cout << "Wartosc Cmax : " << Cmax <<  endl;         ///wyswietlanie ostatecznego Cmax

    cout << "\n\nKolejnosc zadan po wykonaniu algorytmu NEH:  ";
	for(int z = tabPI.size() - 1 ; z >= 0; z--)
		 cout << tabPI[z] + 1 << "  ";
	koniec = clock();
    czas = (float)(koniec - start) / CLOCKS_PER_SEC;
    cout <<  "\n\n\nProgram liczyl: " << czas  << " s" << endl;
	_getch();
	return 0;
}


