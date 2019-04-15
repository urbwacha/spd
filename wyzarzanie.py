import random as r
import re
from math import exp
import time



"""
czytanie z pliku
"""
def read():
    with open("neh.data.txt") as file:
        lines = file.readlines()
        start = re.compile(r'data\.\d{3}:')
        end = re.compile(r'\n')
        flag = False
        for line in lines:
            if re.match(start, line):
                data = []
                flag = True
                continue
            if re.fullmatch(end, line):
                flag = False
            if flag:
                tmp = list(map(int, line.split()))
                data.append(tmp)
                if data[0][0]+1 == len(data):
                    yield [[data[i][y] for i in range(1, len(data))] for y in range(data[0][1])]
        return StopIteration

"""
SORTOWANIE
append-dodawanie na koniec listy/tablicy
"""
def appendtab(wzor, *argumenty):
    tab = []
    for arg in argumenty:
        tab.append([arg[i-1] for i in wzor])
    return tab


"""
1 - INICJALIZACJA - tworzenie rozwiazania poczatkowego
range(start, stop, step) - tworzy liste o dlugosic len()
*argumenty - nieznana ilosc rozwiazan - tablica dynamiczna
"""
def inicjuj(*argumenty):
    granica = [[] for i in range(len(argumenty))]
    granica[0] = [sum(argumenty[0][:i]) for i in range(1, len(argumenty[0]) + 1)]
    for n in range(len(argumenty[0])):
        for i in range(1, len(argumenty)):
            if n == 0:
                granica[i].append(granica[i - 1][n] + argumenty[i][n])
            else:
                if granica[i - 1][n] > granica[i][n - 1]:
                    granica[i].append(granica[i - 1][n] + argumenty[i][n])
                else:
                    granica[i].append(granica[i][n - 1] + argumenty[i][n])
    return granica[len(argumenty) - 1][len(argumenty[0]) - 1]


"""
2 - GENEROWANIE RUCHU - sasiada
insert(indeks, element) - wstawianie elementu na pozycje indeks
insert = True(dla insert) /False(dla swap)
rozwiaz[1:4] to wyjscie to 2,3,4
pop(indeks) - usuwa argument z pozycji o polozeniu indeks
"""
def generuj_sasiada(rozwiaz, insert = True):
    rozwiaz_sasiada = rozwiaz[::]
    if insert:
        element = rozwiaz_sasiada.pop(r.randint(0, len(rozwiaz)-1))
        rozwiaz_sasiada.insert(r.randint(0, len(rozwiaz)-1), element)
    else:
        index = r.sample(list(range(len(rozwiaz))), 2)
        rozwiaz_sasiada[index[0]], rozwiaz_sasiada[index[1]] = rozwiaz_sasiada[index[1]], rozwiaz_sasiada[index[0]]
    return rozwiaz_sasiada

"""
3 - WYKONANIE LUB NIEWYKONANIE RUCHU
"""
def ruch_proba(cmax, cmax_bis, Temp):
    if Temp == 0:          # TEMPERATURA
        return -1
    elif cmax_bis < cmax:
        return 1
    else:
        return exp((cmax-cmax_bis)/Temp)


"""
Wykonanie ruchu wewnatrz tablicy task
"""
def ruch(*argumenty):
    task = [argumenty[i][0] for i in range(len(argumenty))]
    task = [[task[z][y] for z in range(len(task))] for y in range(len(task[0]))]
    return task


"""
4 - SCHLADZANIE
    k - aktualna iteracja
    k_max - maksymalna liczba iteracji
"""
def schladzanie(Temp, parametr = None, k = None, k_max = None):
    if parametr:
        return parametr * Temp
    return Temp * (k/k_max)


"""
5 - KRYTERIUM STOPU (zalezne od liczby literiacji, temp granicznej)
* - wiec przekazywana cala lista
"""
def rozwiaz(*argumenty):
    tasks = list(zip(*argumenty))
    task = [[v, i] for i, v in enumerate(tasks)]
    wynik = task[::]
    r.shuffle(wynik)
    Temp = 200
    iteracja = 50000
    for i in range(iteracja):
        b_wynik = generuj_sasiada(wynik, insert = True)
        p = ruch_proba(inicjuj(*ruch(*wynik)), inicjuj(*ruch(*b_wynik)), Temp)
        if p >= r.uniform(0, 1):
            wynik = b_wynik
        Temp = schladzanie(Temp, parametr = 0.99)
    wynik = [wynik[i][1] for i in range(len(wynik))]
    return list(map(lambda x: x+1, wynik))

"""
MAIN
"""
if __name__ == '__main__':
    przyklady = read()          ## czytanie z pliku
    for ktoreDane, end in enumerate(przyklady):
        if ktoreDane == 84:    ## wybor danych z pliku neh_data.txt
            ktoreDane = rozwiaz(*end)
            print("Cmax na poczatku to:", inicjuj(*end),'\n')
            print("Cmax na koniec to:", inicjuj(*appendtab(ktoreDane, *end)),'\n')
            print(ktoreDane)
