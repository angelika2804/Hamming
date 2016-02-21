#include <cstdlib>
#include <iostream>
#include <cmath>
#include <list>
#include <vector>

using namespace std;

typedef	struct 
{
	int numer;
	vector<int> sasiedzi;
	int odleglosc;
	int odwiedzony;
	vector<int> wspolrzedne;
	//etykiety
} wierzcholek;

vector<wierzcholek> adjacency_list;
vector<wierzcholek> hamming_adjacency_list;
vector<int> skladowe_ilosc;
vector<wierzcholek*> tablica_wskaznikow;
wierzcholek temp;

void wczytaj(void)
{
	int tmp;
	cin >> tmp;
	int a = 0;
	while (cin.eof() == false) //koniec danych
	{
		while ((cin.peek() != '\n'))  //koniec linii
		{
			cin >> tmp;
			temp.sasiedzi.push_back(tmp); //wpisuje s¹siada
		}
		temp.numer = a;
		a++;
		adjacency_list.push_back(temp);
		temp.sasiedzi.clear();
		cin >> tmp;
	}
}

void sortuj()
{
	temp.sasiedzi.clear();
	vector<wierzcholek> temp_adjacency_list(adjacency_list.size(), temp);
	for (vector<wierzcholek>::iterator iter = adjacency_list.begin(); iter != adjacency_list.end(); iter++)
	{
		int numer = (iter - adjacency_list.begin());
		temp_adjacency_list[numer].numer = numer;
		for (vector<int>::iterator iter1 = iter->sasiedzi.begin(); iter1 != iter->sasiedzi.end(); iter1++)
		{
			temp_adjacency_list[*iter1].sasiedzi.push_back(numer);
		}
	}
	adjacency_list = temp_adjacency_list;
}

void sortuj_po_etykietach(int iloczyn)
{
	temp.sasiedzi.clear();
	int numer;
	vector<wierzcholek> temp_adjacency_list(adjacency_list.size(), temp);
	for (int i = 0; i < iloczyn; i++)
	{
		temp_adjacency_list[i].wspolrzedne = adjacency_list[i].wspolrzedne;
		//iter = tablica_wskaznikow[i];
		numer = tablica_wskaznikow[i]->numer;
		for (vector<int>::iterator iter1 = tablica_wskaznikow[i]->sasiedzi.begin(); iter1 != tablica_wskaznikow[i]->sasiedzi.end(); iter1++)
		{
			temp_adjacency_list[*iter1].sasiedzi.push_back(numer);
		}
	}
	adjacency_list = temp_adjacency_list;
}

void bfs(void)
{
	list<int> kolejka;
	kolejka.push_back(0);
	adjacency_list[0].odleglosc = 0;
	adjacency_list[0].odwiedzony = 1;

	while (!kolejka.empty())
	{
		for (vector<int>::iterator iter1 = adjacency_list[kolejka.front()].sasiedzi.begin(); iter1 != adjacency_list[kolejka.front()].sasiedzi.end(); iter1++)
		{
			if (adjacency_list[*iter1].odwiedzony == 0)
			{
				adjacency_list[*iter1].odwiedzony = 1;
				adjacency_list[*iter1].odleglosc = (adjacency_list[kolejka.front()].odleglosc + 1);
				kolejka.push_back(*iter1);
			}
		}
		kolejka.pop_front();
	}
}

int przelicz(vector<int> &etykieta, int components)
{
	int wartosc = 0;
	for (int i = 0; i < components; i++)
	{
		wartosc *= skladowe_ilosc[i];
		wartosc += etykieta[i];
	}
	return wartosc;
}

vector<int> na_odwrot(int wartosc, int components)
{
	vector<int> label(components, 0);
	for (int i = (components)-1; i >= 0; i--)
	{
		label[i]=(wartosc%skladowe_ilosc[i]);
		wartosc /= skladowe_ilosc[i];
	}
	return label;
}

void wypisz(int components)
{
	for (vector<wierzcholek>::iterator iter = adjacency_list.begin(); iter != adjacency_list.end(); iter++)
	{
		cout << iter - adjacency_list.begin() << " odleglosc: " << iter->odleglosc << " sasiedzi: ";
		for (vector<int>::iterator iter1 = iter->sasiedzi.begin(); iter1 != iter->sasiedzi.end(); iter1++)
		{
			cout << *iter1 << " ";
		}
		cout << "etykiety: ";
		for (vector<int>::iterator iter1 = iter->wspolrzedne.begin(); iter1 != iter->wspolrzedne.end(); iter1++)
		{
			cout << *iter1 << " ";
		}
		cout << " Numer: " << przelicz(iter->wspolrzedne, components) << endl;
	}
}

void wypisz2(int components)
{
	for (vector<wierzcholek>::iterator iter = hamming_adjacency_list.begin(); iter != hamming_adjacency_list.end(); iter++)
	{
		cout << iter - hamming_adjacency_list.begin() << " odleglosc: " << iter->odleglosc << " sasiedzi: ";
		for (vector<int>::iterator iter1 = iter->sasiedzi.begin(); iter1 != iter->sasiedzi.end(); iter1++)
		{
			cout << *iter1 << " ";
		}
		cout << "etykiety: ";
		for (vector<int>::iterator iter1 = iter->wspolrzedne.begin(); iter1 != iter->wspolrzedne.end(); iter1++)
		{
			cout << *iter1 << " ";
		}
		cout << " Numer: " << przelicz(iter->wspolrzedne, components) << endl;
	}
}

int skladowe_spojnosci()
{
	temp.sasiedzi.clear();
	vector<wierzcholek> temp_adjacency_list(adjacency_list.size(), temp);
	int components = 0;
	for (vector<int>::iterator iter = adjacency_list[0].sasiedzi.begin(); iter != adjacency_list[0].sasiedzi.end(); iter++)
	{
		if (adjacency_list[*iter].wspolrzedne.empty())
		{
			list<int> kolejka;
			kolejka.push_back(*iter);
			adjacency_list[*iter].wspolrzedne.resize(components, 0);
			adjacency_list[*iter].wspolrzedne.push_back(1);
			int numer = 2;

			while (!kolejka.empty())
			{
				for (vector<int>::iterator iter1 = adjacency_list[kolejka.front()].sasiedzi.begin(); iter1 != adjacency_list[kolejka.front()].sasiedzi.end(); iter1++)
				{
					if (adjacency_list[*iter1].odleglosc == 1 && adjacency_list[*iter1].wspolrzedne.empty())
					{
						adjacency_list[*iter1].wspolrzedne.resize(components, 0);
						adjacency_list[*iter1].wspolrzedne.push_back(numer);
						numer++;
						kolejka.push_back(*iter1);
					}
				}
				kolejka.pop_front();
			}
			skladowe_ilosc.push_back(numer);
			components++;
		}
		adjacency_list[0].wspolrzedne.resize(components, 0);
	}
	for (vector<int>::iterator iter = adjacency_list[0].sasiedzi.begin(); iter != adjacency_list[0].sasiedzi.end(); iter++)
	{
		adjacency_list[*iter].wspolrzedne.resize(components, 0);
	}
	return components;
}

int etykietuj(int components)
{
	list<int> kolejka;
	kolejka.push_back(0);
	adjacency_list[0].odwiedzony = 2;

	while (!kolejka.empty())
	{
		int czy_etykietowac = adjacency_list[kolejka.front()].wspolrzedne.empty();
		if (czy_etykietowac)
			adjacency_list[kolejka.front()].wspolrzedne.resize(components, 0);
		int dolni_sasiedzi = 0;
		for (vector<int>::iterator iter1 = adjacency_list[kolejka.front()].sasiedzi.begin(); iter1 != adjacency_list[kolejka.front()].sasiedzi.end(); iter1++)
		{
			if (adjacency_list[*iter1].odwiedzony == 1)
			{
				adjacency_list[*iter1].odwiedzony = 2;
				kolejka.push_back(*iter1);
			}
			else if (adjacency_list[kolejka.front()].odleglosc == (adjacency_list[*iter1].odleglosc + 1) && dolni_sasiedzi<2 && czy_etykietowac)
			{
				dolni_sasiedzi++;
				for (int i = 0; i < components; i++)
				if (adjacency_list[*iter1].wspolrzedne[i]>adjacency_list[kolejka.front()].wspolrzedne[i])
					adjacency_list[kolejka.front()].wspolrzedne[i] = adjacency_list[*iter1].wspolrzedne[i];
			}
		}
		if (dolni_sasiedzi < 2 && czy_etykietowac)
			return -1;
		kolejka.pop_front();
	}
	return 0;
}

int wskaznikuj(int components)
{
	int temp_value;
	for (vector<wierzcholek>::iterator iter = adjacency_list.begin(); iter != adjacency_list.end(); iter++)
	{
		temp_value = przelicz(iter->wspolrzedne, components);
		if (tablica_wskaznikow[temp_value] == NULL)
			tablica_wskaznikow[temp_value] = &(*iter);
		else
			return -1;
	}
	return 0;
}

int hamming(int iloczyn, int components)
{
	temp.sasiedzi.clear();
	for (int i = 0; i < iloczyn; i++)
	{
		temp.wspolrzedne = na_odwrot(i, components);
		hamming_adjacency_list.push_back(temp);
		//cout << i << ":" << hamming_adjacency_list[i].wspolrzedne[0] << " " << hamming_adjacency_list[i].wspolrzedne[1] << " " << hamming_adjacency_list[i].wspolrzedne[2] << endl;
	}
	vector<int> wspolrzedne_temp(components, 0);
	for (int i = 0; i < iloczyn; i++) //dopisuje sasiadow w grafie Hamminga
	{
		for (int p = 0; p < components; p++)
		{
			wspolrzedne_temp = hamming_adjacency_list[i].wspolrzedne;
			for (int t = 0; t < skladowe_ilosc[p];t++)
			{
				if (t != hamming_adjacency_list[i].wspolrzedne[p])
				{
					wspolrzedne_temp[p] = t;
					hamming_adjacency_list[przelicz(wspolrzedne_temp, components)].sasiedzi.push_back(i);
				}
			}
		}
	}
	return 0;
}

int porownaj_grafy(int iloczyn, int components)
{
	for (int i = 0; i < iloczyn; i++)
	{
		if (hamming_adjacency_list[i].sasiedzi.size() != tablica_wskaznikow[i]->sasiedzi.size())
			return -2;
		vector<int>::iterator iter = hamming_adjacency_list[i].sasiedzi.begin();
		vector<int>::iterator iter1 = tablica_wskaznikow[i]->sasiedzi.begin();
		while ((iter != hamming_adjacency_list[i].sasiedzi.end()) && (iter1 != tablica_wskaznikow[i]->sasiedzi.end()))
		{
			if (*iter != przelicz(adjacency_list[(*iter1)].wspolrzedne, components))
			{
				//cout << i <<" " << *iter1 << " " << *iter << " --- " << przelicz(tablica_wskaznikow[*iter1]->wspolrzedne, components) << endl;
				return -1;
				
			}
			iter++;
			iter1++;
		}
	}

	return 0;
}

int main()	{
	temp.odleglosc = 0;
	temp.odwiedzony = 0;
	
	wczytaj();
	//sortuj();
	bfs();
	int components=skladowe_spojnosci();
	if (etykietuj(components) == -1)
	{
		cout << "NIE - etykietowanie nie powiodlo sie" << endl;
		return 0;
	
	}
	int iloczyn=1; //liczba wierzcho³ków dla grafu Hamminga
	for (vector<int>::iterator iter = skladowe_ilosc.begin(); iter != skladowe_ilosc.end(); iter++)
		iloczyn *= *iter;
	if (iloczyn!=adjacency_list.size())
	{
		cout << "NIE - nie zgadza sie liczba wierzcholkow" << endl;
		return 0;
	}
	tablica_wskaznikow.resize(iloczyn, NULL);
	if (wskaznikuj(components) == -1)
	{
		cout << "NIE - istnieja dwa wierzcholki o tej samej etykiecie" << endl;
		return 0;
	}

	sortuj_po_etykietach(iloczyn);
	//wypisz(components); //Graf G
	hamming(iloczyn, components);
	//wypisz2(components); //Graf H
	
	int w = porownaj_grafy(iloczyn, components);
	if (w == 0)
		cout << "TAK" << endl;
	else if (-2)
		cout << "NIE - odpowiadajace sobie wierzcholki maja rozna liczbe sasiadow" << endl;
	else if (-1)
		cout << "NIE - odpowiadajace sobie wierzcholki maja roznych sasiadow" << endl;

	return 0;
}