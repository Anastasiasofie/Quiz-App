#include "ui.h"
#include "data.h"

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
    printf("2. Themen anzeigen\n");
    printf("3. Beenden\n");
    printf("================================\n");

    if (!ui_lesen_zahl("Ihre Auswahl: ", 1, 3, &wahl)) {
        return -1;
    }

    return wahl;
}

void ui_themen_anzeigen(const Fragensammlung *sammlung)
{
        if (sammlung == NULL) {  // Überprüfen, ob die Fragensammlung NULL ist
        ui_fehler("Keine Fragensammlung vorhanden.");
        return;
    }

    printf("\n================================\n");
    printf("            Verfügbare\n");
    printf("            THEMEN\n");
    printf("================================\n");
     for (int i = 0; i < sammlung->anzahl_fragensaetze; i++) {
        printf("%2d) %-30s (%d Fragen)\n",
            i + 1,
            sammlung->saetze[i].thema,
            sammlung->saetze[i].anzahl_fragen
        );
    }
}

int ui_themen_wahl(const Fragensammlung *sammlung)
{

if (sammlung == NULL) {
    ui_fehler("Keine Fragensammlung vorhanden.");
    return -1;
}

    int wahl;
    int n = sammlung->anzahl_fragensaetze;

    ui_themen_anzeigen(sammlung);

    printf(" 0) Zurück\n");

    if (!ui_lesen_zahl("Bitte das Thema wählen: ", 0, n, &wahl)
        || wahl == 0) {
        return -1;
    }

    return wahl - 1;
}

int ui_frage_stellen(const Frage *f, int nr, int gesamt)
{
if (f == NULL) {
    ui_fehler("Keine Frage vorhanden.");
    return -1;
}

    char puffer[64];
    printf("\nFrage %d von %d:\n", nr, gesamt);
    printf("%s\n", f->frage);
    for (int i = 0; i < 4; i++)
    {
        printf(" %c) %s\n", 'A' + i, f->antworten[i]);
    }

    for (;;) {
        printf("Ihre Antwort (A-D): ");
        fflush(stdout);
        if (!ui_lese_zeile(puffer, sizeof(puffer))) {
            return -1;
        }
        if (strlen(puffer) == 1) {
            char antwort = (char)toupper((unsigned char)puffer[0]);
            if (antwort >= 'A' && antwort <= 'D') {
                return antwort - 'A';
            }
            if(antwort >= '1' && antwort <= '4') {
                return antwort - '1';
            }
            ui_fehler("Ungültige Eingabe, bitte A, B, C oder D eingeben.");
        }
    }
}

void ui_rueckmeldung(int richtig, const Frage *f)
{
    if (richtig) {
        printf("\nRichtig!\n");
    } else {
        printf("\nFalsch! Die richtige Antwort wäre: %c) %s\n", 'A' + f->richtige_antwort, f->antworten[f->richtige_antwort]);
    }
}

void ui_ergebnis(int richtig, int gesamt)
{
    printf("\n================================\n");
    printf("            ERGEBNIS\n");
    printf("================================\n");
    printf("Sie haben %d von %d Fragen richtig beantwortet.\n", richtig, gesamt);
}

void ui_verlauf_anzeigen(const Ergebnis *ergebnisse, int anzahl)
{
    printf("\n================================\n");
    printf("            VERLAUF\n");
    printf("================================\n");
    for (int i = 0; i < anzahl; i++) {
        printf("%2d) Thema: %-30s | Richtig: %2d | Gesamt: %2d\n", i + 1, ergebnisse[i].thema, ergebnisse[i].richtig, ergebnisse[i].gesamt);
    }
}
