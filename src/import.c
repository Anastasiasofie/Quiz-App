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


int validate_question(void){

};	

int import_csv(void){

};	


