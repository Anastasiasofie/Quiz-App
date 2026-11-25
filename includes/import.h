#include <stdio.h>
#include "data.h"

#ifndef IMPORT_H
#define IMPORT_H

#define MAX_TEXT 256


#ifdef __cplusplus     // Damit kann man die Funktionen auch in C++ verwenden, da C++ die Namensveränderung (Name Mangling) verwendet. Mit extern "C" wird das verhindert.
extern "C" {
#endif

FILE *open_csv(const char *dateiname);

int parse_csv_line(const char *zeile, char felder[][MAX_TEXT], int max_felder);

int validate_question(char felder[][MAX_TEXT], int anzahl_felder);

Fragensatz *find_fragensatz(Fragensammlung *sammlung, const char *thema);

int import_csv(const char *dateiname, Fragensammlung *sammlung);

#ifdef __cplusplus
}
#endif

#endif

