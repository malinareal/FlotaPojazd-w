#pragma once
#include <string>

using namespace std;

class Pojazd
{
protected:
    int id;
    string marka;
    string model;
    int rok;
    int przebieg;
    string status;

public:
    Pojazd(int id, string marka, string model, int rok, int przebieg, string status)
        : id(id), marka(marka), model(model), rok(rok), przebieg(przebieg), status(status) {}

    int getId() const { return id; }
    string getMarka() const { return marka; }
    string getModel() const { return model; }
    int getRok() const { return rok; }
    int getPrzebieg() const { return przebieg; }
    string getStatus() const { return status; }
};

class Samochod : public Pojazd
{
private:
    int drzwi;
    string silnik;

public:
    Samochod(int id, string marka, string model, int rok, int przebieg, string status, int drzwi, string silnik)
        : Pojazd(id, marka, model, rok, przebieg, status), drzwi(drzwi), silnik(silnik) {}

    int getDrzwi() const { return drzwi; }
    string getSilnik() const { return silnik; }
};

class Motocykl : public Pojazd
{
private:
    int pojemnosc;

public:
    Motocykl(int id, string marka, string model, int rok, int przebieg, string status, int pojemnosc)
        : Pojazd(id, marka, model, rok, przebieg, status), pojemnosc(pojemnosc) {}

    int getPojemnosc() const { return pojemnosc; }
};

class Dostawczy : public Pojazd
{
private:
    int ladownosc;
    string silnik;

public:
    Dostawczy(int id, string marka, string model, int rok, int przebieg, string status, int ladownosc, string silnik)
        : Pojazd(id, marka, model, rok, przebieg, status), ladownosc(ladownosc), silnik(silnik) {}

    int getLadownosc() const { return ladownosc; }
    string getSilnik() const { return silnik; }
};
