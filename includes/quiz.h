#ifndef QUIZ_H
#define QUIZ_H

#include "types.h"

typedef struct {
    char thema[MAX_THEMA];
    const Fragen **fragen;
    int anzahl_fragen;
    int aktuelle_frage;
    int richtige_antworten;
} Runde;
