#pragma once
#include <string>
using namespace std;

class Serwis {
public:
    string dataPrzegladu;
    string opisUsterki;
    bool   czySprawny;

    Serwis(string dataPrzegladu, string opisUsterki, bool czySprawny)
        : dataPrzegladu(dataPrzegladu), opisUsterki(opisUsterki), czySprawny(czySprawny) {}
};
