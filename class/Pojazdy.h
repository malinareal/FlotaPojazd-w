#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Serwis.h"
using namespace std;

// ============================================================
//  Klasa abstrakcyjna - nie mozna tworzyc obiektow Pojazd
//  bezposrednio. Uzyj: Samochod, Motocykl, Dostawczy.
// ============================================================
class Pojazd {
protected:
    int    id;
    string marka;
    string model;
    int    rok;
    int    przebieg;
    string status;   // "dostepny" | "wypozyczony" | "serwis"
    vector<Serwis> historiaSerwisowa;

public:
    Pojazd(int id, string marka, string model, int rok, int przebieg, string status)
        : id(id), marka(marka), model(model), rok(rok), przebieg(przebieg), status(status) {}

    virtual ~Pojazd() {}

    // ---------- gettery ----------
    int    getId()       const { return id; }
    string getMarka()    const { return marka; }
    string getModel()    const { return model; }
    int    getRok()      const { return rok; }
    int    getPrzebieg() const { return przebieg; }
    string getStatus()   const { return status; }
    const vector<Serwis>& getHistoriaSerwisowa() const { return historiaSerwisowa; }

    // ---------- settery ----------
    void setStatus(const string& nowyStatus) { status = nowyStatus; }

    // Walidacja: nowy przebieg musi byc wiekszy od aktualnego
    bool zaktualizujPrzebieg(int nowyPrzebieg) {
        if (nowyPrzebieg <= przebieg) {
            cout << "Blad: nowy przebieg (" << nowyPrzebieg
                 << ") musi byc wiekszy od aktualnego (" << przebieg << ").\n";
            return false;
        }
        przebieg = nowyPrzebieg;
        return true;
    }

    void dodajSerwis(const Serwis& s) { historiaSerwisowa.push_back(s); }

    bool jestDostepny() const { return status == "dostepny"; }

    // Czysto wirtualna - kazda podklasa musi podac swoj typ
    virtual string getTyp() const = 0;
};

// ============================================================
class Samochod : public Pojazd {
private:
    int    drzwi;
    string silnik;
public:
    Samochod(int id, string marka, string model, int rok, int przebieg,
             string status, int drzwi, string silnik)
        : Pojazd(id, marka, model, rok, przebieg, status),
          drzwi(drzwi), silnik(silnik) {}

    int    getDrzwi() const { return drzwi; }
    string getSilnik() const { return silnik; }
    string getTyp()   const override { return "samochod"; }
};

// ============================================================
class Motocykl : public Pojazd {
private:
    int pojemnosc;
public:
    Motocykl(int id, string marka, string model, int rok, int przebieg,
             string status, int pojemnosc)
        : Pojazd(id, marka, model, rok, przebieg, status), pojemnosc(pojemnosc) {}

    int    getPojemnosc() const { return pojemnosc; }
    string getTyp()       const override { return "motocykl"; }
};

// ============================================================
class Dostawczy : public Pojazd {
private:
    int    ladownosc;
    string silnik;
public:
    Dostawczy(int id, string marka, string model, int rok, int przebieg,
              string status, int ladownosc, string silnik)
        : Pojazd(id, marka, model, rok, przebieg, status),
          ladownosc(ladownosc), silnik(silnik) {}

    int    getLadownosc() const { return ladownosc; }
    string getSilnik()    const { return silnik; }
    string getTyp()       const override { return "dostawczy"; }
};
