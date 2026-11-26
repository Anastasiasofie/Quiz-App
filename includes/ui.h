#ifndef UI_H
#define UI_H

#include <stddef.h>
#include "data.h"

#ifdef __cplusplus
extern "C" {
#endif

int ui_lese_zeile(char *puffer, size_t groesse);

int ui_fehler(const char *text);

void ui_info(const char *meldung);

int ui_lesen_zahl(const char *promt, int min, int max, int *wert);

int ui_hauptmenue(void);

void ui_themen_anzeigen(const Fragensammlung *sammlung);

int ui_themen_wahl(const Fragensammlung *sammlung);

int ui_frage_stellen(const Frage *f, int nr, int gesamt);

void ui_rueckmeldung(int richtig, const Frage *f);

void ui_ergebnis(int richtig, int gesamt);

void ui_verlauf_anzeigen(const Ergebnis *ergebnisse, int anzahl);

#ifdef __cplusplus
}
#endif

#endif
