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

           if (feld >= max_felder || position >= MAX_TEXT - 1) {
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

    return 1;
}


int parse_correct_answer(const char *richtige_Antwort) {     //wichtig damit man als Antwort sowohl 1-4 als auch A-D eingeben kann

    if (richtige_Antwort == NULL || strlen(richtige_Antwort) != 1) {
        return -1;
    }

    char zeichen = richtige_Antwort[0];

    if (zeichen >= 'a' && zeichen <= 'd') {
        zeichen = zeichen - 'a' + 'A';
    }

    if (zeichen >= 'A' && zeichen <= 'D') {
        return zeichen - 'A';
    }

    if (zeichen >= '1' && zeichen <= '4') {
        return zeichen - '1';
    }

    return -1;
}

Fragensatz *find_fragensatz(Fragensammlung *sammlung, const char *thema) {

    if (sammlung == NULL || thema == NULL) {
        return NULL;
    }

    if (sammlung->anzahl_fragensaetze < 0 ||
        sammlung->anzahl_fragensaetze > MAX_FRAGENSATZ) {
        return NULL;
    }

    for (int i = 0; i < sammlung->anzahl_fragensaetze; i++) {

        if (strcmp(sammlung->saetze[i].thema, thema) == 0) {
            return &sammlung->saetze[i];
        }
    }

    return NULL;
}

int import_csv(const char *dateiname, Fragensammlung *sammlung) {

if (dateiname == NULL || sammlung == NULL) {
    return 0;}

FILE *datei = open_csv(dateiname); 

if (datei == NULL) {
        fprintf(stderr, "Fehler: CSV-Datei konnte nicht geöffnet werden.\n");
        return 0;};

// Maximale Länge einer Zeile:
// 7 Felder mit jeweils maximal MAX_TEXT - 1 Zeichen,
// 6 Trennzeichen und ein Zeilenende.

    char zeile[7 * MAX_TEXT + 2];

    char felder[7][MAX_TEXT];

    int zeilennummer = 0;

    int importierte_fragen = 0;

    while (fgets(zeile, sizeof(zeile), datei) != NULL) {

    zeilennummer++;

    // Kopfzeile überspringen
    if (zeilennummer == 1) {
        continue;
    }

    // Leerzeilen ignorieren
    if (zeile[0] == '\n' || zeile[0] == '\r') {
        continue;
    }

    // Kommentarzeilen ignorieren
    if (zeile[0] == '#') {
        continue;
    }

    // Prüfen, ob die Zeile vollständig eingelesen wurde
    if (strchr(zeile, '\n') == NULL && !feof(datei)) {
        fprintf(stderr, "Fehler: Zeile %d ist zu lang.\n", zeilennummer);
        fclose(datei);
        return 0;
    }

    // CSV-Zeile aufteilen
    int anzahl_felder = parse_csv_line(zeile, felder, 7);

    if (!validate_question(felder, anzahl_felder)) {
        fprintf(stderr, "Fehler: Ungültige Frage in Zeile %d.\n", zeilennummer);
        fclose(datei);
        return 0;
    }

    // Richtige Antwort überprüfen
    int richtige_antwort = parse_correct_answer(felder[6]);

    if (richtige_antwort == -1) {
        fprintf(stderr, "Fehler: Ungültige richtige Antwort in Zeile %d.\n", zeilennummer);
        fclose(datei);
        return 0;
    }

// Passenden Fragensatz suchen
        Fragensatz *satz = find_fragensatz(sammlung,felder[0]);


 // Thema existiert noch nicht
        if (satz == NULL) {

            if (sammlung->anzahl_fragensaetze >= MAX_FRAGENSATZ) {
                fprintf(stderr, "Fehler: Maximale Anzahl an Themen erreicht.\n");
                fclose(datei);
                return 0;
            }

            satz = &sammlung->saetze[sammlung->anzahl_fragensaetze];

            memset(satz, 0, sizeof(Fragensatz));

            strcpy(satz->thema, felder[0]);

            sammlung->anzahl_fragensaetze++;
        }

        // Prüfen, ob im Fragensatz noch Platz ist
        if (satz->anzahl_fragen >= MAX_FRAGEN) {
            fprintf(stderr, "Fehler: Zu viele Fragen für das Thema %s.\n",
                   satz->thema);
            fclose(datei);
            return 0;
        }

        // Neue Frage anlegen
        Frage *frage = &satz->fragen[satz->anzahl_fragen];

        strcpy(frage->frage, felder[1]);

        for (int i = 0; i < 4; i++) {
        strcpy(frage->antworten[i], felder[i + 2]);
        }

        frage->richtige_antwort = richtige_antwort;

        satz->anzahl_fragen++;
        importierte_fragen++;
    }
    if (importierte_fragen == 1) {
    printf("1 Frage erfolgreich importiert.\n");
    } 
    else {
    printf("%d Fragen erfolgreich importiert.\n", importierte_fragen);
    }
    
    fclose(datei);

    return 1;
}

