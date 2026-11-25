//Wird von allen  Modulen verwendet

#ifndef DATEN_H
#define DATEN_H

//Structs für die Fragen und Fragensätze

#define MAX_TEXT 256    //Länge Fragen / Antworttext
#define MAX_FRAGEN 64   //Länge Fragen / Antworttext
#define MAX_THEMA 64    //Länge eines Themennamens
#define MAX_MELDUNG 256 //Länge einer fehlermeldung
#define MAX_FRAGENSATZ 256
#define MAX_ANTWORTEN 4 //Antwortmöglichkeiten pro Frage
#define DATUM_LEN 20    //JJJJ-MM-TT HH:MM

typedef enum {
    OK =0,
    ERR_DATEI_FEHLT,     //Datei existiert nicht
    ERR_DATEI_LEER,     //Datei enthält keine Frage
    ERR_FORMAT,         //Datei hat ungültiges Format
    ERR_SPEICHER,       //malloc, realloc fehlerhaft
    ERR_SCHREIBEN,      //Datei konnte nicht geschrieben werden
    ERR_KEIN_THEMA,     //Thema nicht vorhanden
    ERR_PARAMETER       //ungültiger Funktionsaufruf
}Status;

typedef struct {
    Frage *fragen;;

    char antworten[MAX_ANTWORTEN][MAX_TEXT];

    int richtige_antwort;

} Frage;


typedef struct {
    Frage fragen[MAX_FRAGEN];

    char thema[MAX_TEXT];

    int anzahl_fragen;
     int anzahl;
    int kapazitaet;
    int uebersprungen;
    char erste_warnung[MAX_MELDUNG];

} Fragensatz;

typedef struct {
    Fragensatz saetze[MAX_FRAGENSATZ];

    int anzahl_fragensaetze;

} Fragensammlung;


typedef struct {
    char datum[DATUM_LEN];
    char thema[MAX_TEXT];

    int richtig;

    int gesamt;

} Ergebnis;


#endif
