#ifndef UI_H
#define UI_H

int ui_lese_zeile(char *puffer, size_t groesse);

int ui_lesen_zahl(const char *promt, int min, int max, int *wert);

int ui_hauptmenue(void);

int ui_themen_anzeigen(char themen[][MAX_THEMA], const int *anzahl, int n);


int ui_themen_wahl(char themen[][MAX_THEMA], const int *anzahl, int n);





#endif
