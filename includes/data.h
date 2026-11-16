#ifndef DATEN_H
#define DATEN_H

//Structs für die Fragen und Fragensätze

#define MAX_TEXT 256   //provisorische Werte, eventuell anpassen oder dynamisch speicher allokieren
#define MAX_FRAGEN 100
#define MAX_THEMA 100
#define MAX_MELDUNG 256

typedef struct {
    char frage[MAX_TEXT];

    char antworten[4][MAX_TEXT];

    int richtige_antwort;

    char thema[MAX_TEXT];

} Frage;


typedef struct {
    Frage fragen[MAX_FRAGEN];

    int anzahl_fragen;

    char thema[MAX_TEXT];

} Fragensatz;

typedef struct {
    char thema[MAX_TEXT];

    int richtig;

    int gesamt;

} Ergebnis;

#endif