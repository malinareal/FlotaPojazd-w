#pragma once
#include <string>
using namespace std;

class Klient {
public:
    int    idKlienta;
    string imie;
    string nazwisko;
    string nrPrawaJazdy;

    Klient(int idKlienta, string imie, string nazwisko, string nrPrawaJazdy)
        : idKlienta(idKlienta), imie(imie), nazwisko(nazwisko), nrPrawaJazdy(nrPrawaJazdy) {}

    string getImieNazwisko() const { return imie + " " + nazwisko; }
};
