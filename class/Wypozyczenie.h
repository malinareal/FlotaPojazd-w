#pragma once
#include <string>
using namespace std;

class Wypozyczenie {
public:
    int    idWypozyczenia;
    int    idPojazdu;
    int    idKlienta;
    string dataOd;
    string dataDo;
    double koszt;
    bool   oddany;   // false = aktywne wypozyczenie, true = zwrocony

    Wypozyczenie(int idWypozyczenia, int idPojazdu, int idKlienta,
                 string dataOd, string dataDo, double koszt, bool oddany = false)
        : idWypozyczenia(idWypozyczenia), idPojazdu(idPojazdu), idKlienta(idKlienta),
          dataOd(dataOd), dataDo(dataDo), koszt(koszt), oddany(oddany) {}
};
