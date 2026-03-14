#ifndef POJAZD_H
#define POJAZD_H

#include <iostream>
#include <string>

using namespace std;

class Pojazd {
protected:
    int id;
    string marka;
    string model;
    int rok;
    int przebieg;
    string status;

public:
    Pojazd(int id, string marka, string model, int rok, int przebieg, string status);

    virtual void wyswietl();
};

#endif