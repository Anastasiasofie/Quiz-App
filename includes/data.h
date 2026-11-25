#ifndef DATEN_H
#define DATEN_H

//Structs für die Fragen und Fragensätze

#define MAX_TEXT 256   //provisorische Werte, eventuell anpassen oder dynamisch speicher allokieren
#define MAX_FRAGEN 100
#define MAX_THEMA 100
#define MAX_MELDUNG 256
#define MAX_FRAGENSATZ 100

typedef struct {
    char frage[MAX_TEXT];

    char antworten[4][MAX_TEXT];

    int richtige_antwort;

} Frage;


typedef struct {
    Frage fragen[MAX_FRAGEN];

    char thema[MAX_TEXT];

    int anzahl_fragen;

} Fragensatz;

typedef struct {
    Fragensatz saetze[MAX_FRAGENSATZ];

    int anzahl_fragensaetze;
    
} Fragensammlung;


typedef struct {
    char thema[MAX_TEXT];

    int richtig;

    int gesamt;
    
} Ergebnis;


#endif