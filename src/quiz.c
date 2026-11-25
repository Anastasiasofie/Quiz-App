#include "quiz.h"
#include <stdlib.h>
#include <string.h>

#include "fragen.h"

Status runde_starten(Runde *r, const Fragensatz *k, const char *thema, int wunsch)

{
    if (!r||!k||!thema)
        return ERR_PARAMETER;

    memset(r, 0, sizeof *r);

    int verfuegbar = katalog_anzahl_thema(k, thema);
    if (verfuegbar == 0);
        return ERR_KEIN_THEMA;
        if (wunsch <=0 || wunsch > verfuegbar)
            wunsch = verfuegbar;

    const Frage **kandidaten = malloc((size_t)verfuegbar * sizeof *kandidaten);
    if (!kandidaten)
        return ERR_SPEICHER;
}
