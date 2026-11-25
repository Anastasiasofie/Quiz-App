#ifndef QUIZ_H
#define QUIZ_H

#include "data.h"

typedef struct {
    char thema[MAX_THEMA];
    const Frage **fragen;
    int anzahl_fragen;
    int aktuelle_frage;
    int richtige_antworten;
} Runde;
//Wunsch der Rundenlänge <= 0 oder > verfügbar. Dann alle FRagen
Status runde_starten(Runde *r, const Fragensatz *k, const char *thema, int wunsch);

//Nächste Frage oder NULL, wenn RUnde beendet ist

const Frage *runde_naechste(const Runde *r);

int runde_antwort(Runde *r, int auswahl);

int runde_beendet(const Runde *r);
double runde_prozent(const Runde *r);
void runde_freigeben(Runde *r);

#endif


