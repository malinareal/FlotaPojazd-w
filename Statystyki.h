#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "class/Wypozyczenie.h"
#include "class/Pojazdy.h"
using namespace std;

// ============================================================
//  Klasa analizy - tylko metody statyczne, nie trzyma danych
// ============================================================
class Statystyki {
public:

    static void generujraport(const vector<Wypozyczenie>& wypozyczenia) {
        cout << "\n===== RAPORT WYPOZYCZEN =====\n";
        if (wypozyczenia.empty()) {
            cout << "Brak wypozyczen w systemie.\n";
            return;
        }

        double sumaKosztow = 0.0;
        int aktywne = 0, zakonczone = 0;

        for (const Wypozyczenie& w : wypozyczenia) {
            cout << "ID: "          << w.idWypozyczenia
                 << " | Pojazd ID: "<< w.idPojazdu
                 << " | Klient ID: "<< w.idKlienta
                 << " | Od: "       << w.dataOd
                 << " | Do: "       << w.dataDo
                 << " | Koszt: "    << w.koszt << " PLN"
                 << " | Status: "   << (w.oddany ? "zwrocony" : "aktywne") << "\n";

            sumaKosztow += w.koszt;
            if (w.oddany) zakonczone++; else aktywne++;
        }

        cout << "------------------------------\n";
        cout << "Aktywne wypozyczenia: " << aktywne << "\n";
        cout << "Zakonczone:           " << zakonczone << "\n";
        cout << "Laczny przychod:      " << sumaKosztow << " PLN\n";
    }

    static void pokazNajczestszUsterki(const vector<Pojazd*>& pojazdy) {
        cout << "\n===== NAJCZESTSZE USTERKI =====\n";

        map<string, int> licznik;
        for (const Pojazd* p : pojazdy)
            for (const Serwis& s : p->getHistoriaSerwisowa())
                licznik[s.opisUsterki]++;

        if (licznik.empty()) {
            cout << "Brak zapisanych usterek.\n";
            return;
        }

        for (const auto& para : licznik)
            cout << para.first << " - " << para.second << " raz(y)\n";
    }
};
