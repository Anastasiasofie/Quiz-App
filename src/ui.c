
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
