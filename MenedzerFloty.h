// ============================================================
//  DODAJ TE DWE METODY DO KLASY MenedzerFloty (sekcja public)
//  Zwracaja nastepne wolne ID = max istniejacych + 1
// ============================================================

#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "class/Pojazdy.h"
#include "class/Klient.h"
#include "class/Wypozyczenie.h"
#include "json/BazaDanych.h"
using namespace std;

// ============================================================
//  Kontroler zarzadzajacy cala flota.
//  Trzyma dane w pamieci i synchronizuje je z plikami JSON.
// ============================================================
class MenedzerFloty {
private:
    vector<Pojazd*>      pojazdy;
    vector<Klient>       klienci;
    vector<Wypozyczenie> wypozyczenia;

    // Zwraca najwyzsze uzyte ID wypozyczenia (do generowania nowego)
    int nastepneIdWypozyczenia() const {
        int max = 0;
        for (const Wypozyczenie& w : wypozyczenia)
            if (w.idWypozyczenia > max) max = w.idWypozyczenia;
        return max + 1;
    }

public:
    MenedzerFloty() {
        BazaDanych::inicjalizuj();
        pojazdy      = BazaDanych::wczytajPojazdy();
        klienci      = BazaDanych::wczytajKlientow();
        wypozyczenia = BazaDanych::wczytajWypozyczenia();
    }

    ~MenedzerFloty() {
        for (Pojazd* p : pojazdy) delete p;
    }
    int nastepneIdPojazdu() const {
        int maxId = 0;
        for (const Pojazd* p : pojazdy)
            if (p->getId() > maxId) maxId = p->getId();
        return maxId + 1;
    }

    int nastepneIdKlienta() const {
        int maxId = 0;
        for (const Klient& k : klienci)
            if (k.idKlienta > maxId) maxId = k.idKlienta;
        return maxId + 1;
    }

    // ========== POJAZDY ==========

    // Zwraca false jesli ID juz istnieje
    bool dodajPojazd(Pojazd* p) {
        // Ochrona przed nullem
        if (p == nullptr) {
            cout << "Blad: Proba dodania nieistniejacego obiektu (null).\n";
            return false;
        }
        if (znajdzPojazd(p->getId()) != nullptr) {
            cout << "Blad: pojazd o ID " << p->getId() << " juz istnieje.\n";
            delete p;   // zapobiega wyciekowi pamieci
            return false;
        }
        pojazdy.push_back(p);
        BazaDanych::zapiszPojazd(p);
        return true;
    }

    bool usunPojazd(int id) {
        for (auto it = pojazdy.begin(); it != pojazdy.end(); ++it) {
            if ((*it)->getId() == id) {
                // Nie mozna usunac wypozyczonego pojazdu
                if ((*it)->getStatus() == "wypozyczony") {
                    cout << "Blad: pojazd jest aktualnie wypozyczony.\n";
                    return false;
                }
                delete *it;
                pojazdy.erase(it);
                BazaDanych::zapiszWszystkoDoJSON(pojazdy, klienci, wypozyczenia);
                cout << "Pojazd o ID " << id << " zostal usuniety.\n";
                return true;
            }
        }
        cout << "Nie znaleziono pojazdu o ID: " << id << "\n";
        return false;
    }

    // Zwraca tylko pojazdy ze statusem "dostepny"
    vector<Pojazd*> wyszukajDostepne() const {
        vector<Pojazd*> dostepne;
        for (Pojazd* p : pojazdy)
            if (p->jestDostepny()) dostepne.push_back(p);
        return dostepne;
    }

    const vector<Pojazd*>& getPojazdy() const { return pojazdy; }

    Pojazd* znajdzPojazd(int id) const {
        for (Pojazd* p : pojazdy)
            if (p->getId() == id) return p;
        return nullptr;
    }

    bool zaktualizujPrzebieg(int idPojazdu, int nowyPrzebieg) {
        Pojazd* p = znajdzPojazd(idPojazdu);
        if (!p) { cout << "Nie znaleziono pojazdu o ID: " << idPojazdu << "\n"; return false; }
        if (!p->zaktualizujPrzebieg(nowyPrzebieg)) return false;
        BazaDanych::zapiszWszystkoDoJSON(pojazdy, klienci, wypozyczenia);
        cout << "Przebieg zaktualizowany do " << nowyPrzebieg << " km.\n";
        return true;
    }

    // ========== KLIENCI ==========

    bool dodajKlienta(const Klient& k) {
        if (znajdzKlienta(k.idKlienta) != nullptr) {
            cout << "Blad: klient o ID " << k.idKlienta << " juz istnieje.\n";
            return false;
        }
        klienci.push_back(k);
        BazaDanych::zapiszKlienta(k);
        return true;
    }

    const vector<Klient>& getKlienci() const { return klienci; }

    Klient* znajdzKlienta(int id) {
        for (Klient& k : klienci)
            if (k.idKlienta == id) return &k;
        return nullptr;
    }

    // ========== WYPOZYCZENIA ==========

    // Rejestruje nowe wypozyczenie i zmienia status pojazdu na "wypozyczony"
    bool wypozyczPojazd(int idKlienta, int idPojazdu,
                        const string& dataOd, const string& dataDo, double koszt) {
        Pojazd* p = znajdzPojazd(idPojazdu);
        Klient* k = znajdzKlienta(idKlienta);

        if (!p) { cout << "Blad: pojazd o ID " << idPojazdu << " nie istnieje.\n"; return false; }
        if (!k) { cout << "Blad: klient o ID " << idKlienta << " nie istnieje.\n"; return false; }
        if (!p->jestDostepny()) {
            cout << "Blad: pojazd nie jest dostepny (status: " << p->getStatus() << ").\n";
            return false;
        }
        if (koszt < 0) { cout << "Blad: koszt nie moze byc ujemny.\n"; return false; }

        p->setStatus("wypozyczony");

        Wypozyczenie w(nastepneIdWypozyczenia(), idPojazdu, idKlienta, dataOd, dataDo, koszt, false);
        wypozyczenia.push_back(w);

        BazaDanych::zapiszWszystkoDoJSON(pojazdy, klienci, wypozyczenia);
        cout << "Wypozyczenie zarejestrowane (ID: " << w.idWypozyczenia << ").\n";
        return true;
    }

    // Rejestruje zwrot: oznacza wypozyczenie jako oddane, zmienia status pojazdu
    bool zwrocPojazd(int idWypozyczenia) {
        for (Wypozyczenie& w : wypozyczenia) {
            if (w.idWypozyczenia == idWypozyczenia) {
                if (w.oddany) {
                    cout << "Blad: to wypozyczenie zostalo juz zwrocone.\n";
                    return false;
                }
                w.oddany = true;

                Pojazd* p = znajdzPojazd(w.idPojazdu);
                if (p) p->setStatus("dostepny");

                BazaDanych::zapiszWszystkoDoJSON(pojazdy, klienci, wypozyczenia);
                cout << "Zwrot zarejestrowany. Pojazd ID " << w.idPojazdu << " jest teraz dostepny.\n";
                return true;
            }
        }
        cout << "Nie znaleziono wypozyczenia o ID: " << idWypozyczenia << "\n";
        return false;
    }

    const vector<Wypozyczenie>& getWypozyczenia() const { return wypozyczenia; }
};


