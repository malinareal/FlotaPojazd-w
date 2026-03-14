#ifndef SAMOCHOD_H
#define SAMOCHOD_H

#include "Pojazd.h"

class Samochod : public Pojazd {

private:
    int liczbaDrzwi;
    int liczbaMiejsc;

public:
    Samochod(int id, string marka, string model, int rok, int przebieg, string status,
             int drzwi, int miejsca);

    void wyswietl() override;
};

#endif