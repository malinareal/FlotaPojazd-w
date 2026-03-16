
#ifndef FLOTAPOJAZD_W_HISTORIA_H
#define FLOTAPOJAZD_W_HISTORIA_H
#pragma once
#include <string>
using namespace std;

struct Historia {
    int pojazd_id;
    string klient;
    string data_wypozyczenia;
    string data_zwrotu;
    bool oddany;
};
#endif //FLOTAPOJAZD_W_HISTORIA_H