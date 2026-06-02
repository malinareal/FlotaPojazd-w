// ============================================================
//  DODAJ TE DWE METODY DO KLASY MenedzerFloty (sekcja public)
//  Zwracaja nastepne wolne ID = max istniejacych + 1
// ============================================================

#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>

#include "class/Pojazdy.h"
#include "class/Klient.h"
#include "class/Wypozyczenie.h"
#include "json/BazaDanych.h"

using namespace std;

class MenedzerFlotyError : public std::runtime_error {
public:
    explicit MenedzerFlotyError(const string& message) 
        : std::runtime_error(message) {}
};

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
        if (wypozyczenia.empty()) return 1;
        auto it = max_element(wypozyczenia.begin(), wypozyczenia.end(),
            [](const Wypozyczenie& a, const Wypozyczenie& b) {
                return a.idWypozyczenia < b.idWypozyczenia;
            });
        return it->idWypozyczenia + 1;
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

    MenedzerFloty(const MenedzerFloty&) = delete;
    MenedzerFloty& operator=(const MenedzerFloty&) = delete;

    int nastepneIdPojazdu() const {
        if (pojazdy.empty()) return 1;
        auto it = max_element(pojazdy.begin(), pojazdy.end(),
            [](const Pojazd* a, const Pojazd* b) {
                return a->getId() < b->getId();
            });
        return (*it)->getId() + 1;
    }

    int nastepneIdKlienta() const {
        if (klienci.empty()) return 1;
        auto it = max_element(klienci.begin(), klienci.end(),
            [](const Klient& a, const Klient& b) {
                return a.idKlienta < b.idKlienta;
            });
        return it->idKlienta + 1;
    }

    // ========== POJAZDY ==========

    // Zwraca false jesli ID juz istnieje
    void dodajPojazd(Pojazd* p) {
        // Ochrona przed nullem
        if (p == nullptr) {
            throw MenedzerFlotyError("Proba dodania nieistniejacego obiektu (null).");
        }
        if (znajdzPojazd(p->getId()) != nullptr) {
            delete p;   // zapobiega wyciekowi pamieci
            throw MenedzerFlotyError("Pojazd o ID " + to_string(p->getId()) + " juz istnieje.");
        }
        pojazdy.push_back(p);
        BazaDanych::zapiszPojazd(p);
    }

    void usunPojazd(int id) {
        auto it = find_if(pojazdy.begin(), pojazdy.end(), 
            [id](Pojazd* p) { return p->getId() == id; });

        if (it == pojazdy.end()) {
            throw MenedzerFlotyError("Nie znaleziono pojazdu o ID: " + to_string(id));
        }
        // Nie mozna usunac wypozyczonego pojazdu
        if ((*it)->getStatus() == "wypozyczony") {
            throw MenedzerFlotyError("Nie mozna usunac pojazdu, ktory jest aktualnie wypozyczony.");
        }

        delete *it;
        pojazdy.erase(it);
        BazaDanych::zapiszWszystkoDoJSON(pojazdy, klienci, wypozyczenia);
    }

    // Zwraca tylko pojazdy ze statusem "dostepny"
    vector<const Pojazd*> wyszukajDostepne() const {
        vector<const Pojazd*> dostepne;
        for (const Pojazd* p : pojazdy) {
            if (p->jestDostepny()) dostepne.push_back(p);
        }
        return dostepne;
    }

    const vector<Pojazd*>& getPojazdy() const { return pojazdy; }

    Pojazd* znajdzPojazd(int id) {
        auto it = find_if(pojazdy.begin(), pojazdy.end(), 
            [id](Pojazd* p) { return p->getId() == id; });
        return (it != pojazdy.end()) ? *it : nullptr;
    }

    const Pojazd* znajdzPojazd(int id) const {
        auto it = find_if(pojazdy.begin(), pojazdy.end(), 
            [id](const Pojazd* p) { return p->getId() == id; });
        return (it != pojazdy.end()) ? *it : nullptr;
    }

    void zaktualizujPrzebieg(int idPojazdu, int nowyPrzebieg) {
        Pojazd* p = znajdzPojazd(idPojazdu);
        if (!p) throw MenedzerFlotyError("Nie znaleziono pojazdu o podanym ID.");
        
        if (!p->zaktualizujPrzebieg(nowyPrzebieg)) {
            throw MenedzerFlotyError("Blad aktualizacji przebiegu.");
        }
        BazaDanych::zapiszWszystkoDoJSON(pojazdy, klienci, wypozyczenia);
    }

    // ========== KLIENCI ==========

    void dodajKlienta(const Klient& k) {
        if (znajdzKlienta(k.idKlienta) != nullptr) {
            throw MenedzerFlotyError("Klient o podanym ID juz istnieje.");
        }
        klienci.push_back(k);
        BazaDanych::zapiszKlienta(k);
    }

    const vector<Klient>& getKlienci() const { return klienci; }

    Klient* znajdzKlienta(int id) {
        auto it = find_if(klienci.begin(), klienci.end(), 
            [id](Klient& k) { return k.idKlienta == id; });
        return (it != klienci.end()) ? &(*it) : nullptr;
    }

    // ========== WYPOZYCZENIA ==========

    // Rejestruje nowe wypozyczenie i zmienia status pojazdu na "wypozyczony"
    void wypozyczPojazd(int idKlienta, int idPojazdu, const string& dataOd, const string& dataDo, double koszt) {
        Pojazd* p = znajdzPojazd(idPojazdu);
        Klient* k = znajdzKlienta(idKlienta);

        if (!p) throw MenedzerFlotyError("Pojazd o podanym ID nie istnieje.");
        if (!k) throw MenedzerFlotyError("Klient o podanym ID nie istnieje.");
        if (!p->jestDostepny()) throw MenedzerFlotyError("Pojazd nie jest dostepny (status: " + p->getStatus() + ").");
        if (koszt < 0) throw MenedzerFlotyError("Koszt nie moze byc ujemny.");

        p->setStatus("wypozyczony");

        Wypozyczenie w(nastepneIdWypozyczenia(), idPojazdu, idKlienta, dataOd, dataDo, koszt, false);
        wypozyczenia.push_back(w);

        BazaDanych::zapiszWszystkoDoJSON(pojazdy, klienci, wypozyczenia);
    }

    // Rejestruje zwrot: oznacza wypozyczenie jako oddane, zmienia status pojazdu
    void zwrocPojazd(int idWypozyczenia) {
        auto it = find_if(wypozyczenia.begin(), wypozyczenia.end(), 
            [idWypozyczenia](Wypozyczenie& w) { return w.idWypozyczenia == idWypozyczenia; });

        if (it == wypozyczenia.end()) {
            throw MenedzerFlotyError("Nie znaleziono wypozyczenia o podanym ID.");
        }
        if (it->oddany) {
            throw MenedzerFlotyError("To wypozyczenie zostalo juz zwrocone.");
        }

        it->oddany = true;
        Pojazd* p = znajdzPojazd(it->idPojazdu);
        if (p) p->setStatus("dostepny");

        BazaDanych::zapiszWszystkoDoJSON(pojazdy, klienci, wypozyczenia);
    }

    const vector<Wypozyczenie>& getWypozyczenia() const { return wypozyczenia; }
};
