#include "import.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


FILE *open_csv(const char *dateiname){     //öffnet CSV Datei und gibt FILE Zeiger zurück, bei Fehler gibt es NULL zurück
    FILE *datei = fopen(dateiname, "r");

    if (datei == NULL) {
        return NULL;
    }  
    
    return datei;

};	


int parse_csv_line(const char *zeile, char felder[][MAX_TEXT], int max_felder){  
    /*felder ist eine Matrix, jede Zeile ist ein char array,Frage, antwort_a und so weiter wird da drin gespeichert*/

    if (zeile == NULL || felder == NULL || max_felder <= 0) {
        return -1;
    }

    int feld = 0;
    int position = 0;

    for (int i = 0; ; i++) {

        char zeichen = zeile[i];

        if (zeichen == ';' || zeichen == '\0' || zeichen == '\n' || zeichen == '\r') {  //definierte Trennzeichen

            if (feld >= max_felder) {  //Prüfen ob die maximale Anzahl an Feldern überschritten ist
                return -1;
            }

            felder[feld][position] = '\0';

            feld++;
            position = 0;

            if (zeichen == '\0' || zeichen == '\n' || zeichen == '\r') {
                break;
            }

        } else {

            if (position >= MAX_TEXT - 1) {
                return -1;
            }

            felder[feld][position] = zeichen;
            position++;
        }
    }

    return feld;  //Anzahl der gefunden Felder wird zurückgegeben
};


int validate_question(char felder[][MAX_TEXT], int anzahl_felder) {

    // Prüfen, ob genau 7 Felder vorhanden sind
    if (felder == NULL || anzahl_felder != 7) {
        return 0;
    }

    // Prüfen, ob alle Felder ausgefüllt sind
    for (int i = 0; i < anzahl_felder; i++) {

        if (strlen(felder[i]) == 0) {
            return 0;
        }
    }

    // Prüfen, ob die richtige Antwort eine gültige Zahl ist
    char *ende;
    long richtige_antwort = strtol(felder[6], &ende, 10);  //strtol() wandelt Inhalt von felder[6] in eine Zahl um und gibt Pointer auf letztes Zeichen

    if (ende == felder[6] || *ende != '\0') {   //ende muss auf das Ende des Strings zeigen, sonst ist es keine gültige Zahl
        return 0;
    }

    // Prüfen, ob die Antwort zwischen 1 und 4 liegt
    if (richtige_antwort < 1 || richtige_antwort > 4) {
        return 0;
    }
 // Frage ist gültig
    return 1;
}


int import_csv(void){
return 0;
};	


