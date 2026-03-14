#ifndef MOTOCYKL_H
#define MOTOCYKL_H

#include "Pojazd.h"

class Motocykl : public Pojazd {

private:
    int pojemnoscSilnika;

public:
    Motocykl(int id, string marka, string model, int rok, int przebieg, string status,
             int pojemnosc);

    void wyswietl() override;
};

#endif