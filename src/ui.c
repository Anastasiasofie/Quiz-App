

#include <stdlib.h>
#include <string.h>


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
