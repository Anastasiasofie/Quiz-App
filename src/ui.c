#include "ui.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>


int ui_lese_zeile(char *puffer, size_t n){
    if (!fgets(puffer, (int)n, stdin)) {
        return 0; // Fehler beim Lesen der Zeile
    }
    if (!strchr(puffer, '\n')) {
        int c;
        while ((c=getchar()) != '\n' && c != EOF); // Rest der Zeile verwerfen
    }
    puffer[strcspn(puffer, "\r\n")] = '\0'; // Zeilenumbruch entfernen
    return 1; // Zeile erfolgreich gelesen
}

int ui_fehler(const char *text)
{
    fprintf(stderr, "\n [Fehler] %s\n", text);
    return 0;
}

void ui_info(const char *text)
{
    printf("\n[Info] %s\n", text);
}

int ui_lesen_zahl(const char *promt, int min, int max, int *wert)
{
    char puffer[64];
    for(;;) {
        printf("%s", promt);
        if (!ui_lese_zeile(puffer, sizeof(puffer))) {
            return 0;
        }

        char *ende;
        errno = 0;
        long zahl = strtol(puffer, &ende, 10);
        while (*ende && isspace((unsigned char)*ende)) {
            ende++;
        }
        if (puffer[0] == '\0' || *ende != '\0' || errno != 0 || zahl < min || zahl > max) {
            char msg[MAX_MELDUNG];
            snprintf(msg, sizeof(msg), "Ungültige Eingabe, bitte eine Zahl zwischen %d und %d eingeben.", min, max);
            ui_fehler(msg);
            continue;
        }
        *wert = (int)zahl;
        return 1;
    }
}

int ui_hauptmenue(void)
{
    int wahl;
    printf("\n================================\n");
    printf("            QUIZ-APP\n");
    printf("================================\n");
    printf("1. Quiz starten\n");
    printf("2. Themen Anzeigen\n");
    printf("3. Beenden\n");
    printf("================================\n");
    printf("\n");
    printf("Ihre Auswahl: ");
    scanf("%d", &wahl);
    return wahl;
}

void ui_themen_anzeigen(const themen[] [MAX_THEMA], const int *anzahl, int n)
{
    printf("\n================================\n");
    printf("            Verfügabre\n");
    printf("            THEMEN\n");
    printf("================================\n");
    for (int i = 0; i < n; i++) {
        printf("%2d) %-30s (%d Fragen)\n", i + 1, themen[i], anzahl[i]);
    }
}

int ui_themen_Wahl(char themen[] [MAX_THEMA], const int *anzahl, int n)
{
    int wahl;
    ui_themen_anzeigen(themen, anzahl, n);
    printf(" 0) Zurück\n");
    if (!ui_lesen_zahl("Bitte das Thema Wählen: ", 0, n, &wahl) || wahl == 0) {
         return -1;
    }
    return wahl - 1;
}

