#include "import.h"

#include <stdio.h>
#include <stdlib.h>


FILE *open_csv(const char *dateiname){     //öffnet CSV Datei und gibt FILE Zeiger zurück, bei Fehler gibt es NULL zurück
    FILE *datei = fopen(dateiname, "r");

    if (datei == NULL) {
        return NULL;
    }  
    
    return datei;

};	


int parse_csv_line(void){

};	

int validate_question(void){

};	

int import_csv(void){

};	


