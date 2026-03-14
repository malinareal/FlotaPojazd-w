#ifndef DOSTAWCZY_H
#define DOSTAWCZY_H

#include "Pojazd.h"

class Dostawczy : public Pojazd {

private:
    double ladownosc;
    double objetosc;

public:
    Dostawczy(int id, string marka, string model, int rok, int przebieg, string status,
              double ladownosc, double objetosc);

    void wyswietl() override;
};

#endif