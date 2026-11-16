#include "data.h"

#include <stdio.h>
#include <stdlib.h>

FILE *open_csv(const char *dateiname){
    FILE *datei = fopen(dateiname, "r");

    if (datei == NULL) {
        return NULL;
    }
    fclose(dateiname);  
        
    return datei;

};	

int parse_csv_line(void){

};	

int validate_question(void){

};	
int import_csv(void){

};	

int saveQuestionSet(void){

};

int loadQuestionSet(void){
    
};
