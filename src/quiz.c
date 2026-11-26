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

    int n=0;
    for (int i=0 ; i < k->anzahl_fragen; i++)
        if (strcmp(k->thema, thema)==0)
        kandidaten[n++] = &k->fragen[i];

    for (int i = 0; i < wunsch; i++) {
        int j = i + rand() % (n-i);
        const Frage *tmp = kandidaten[i];
        kandidaten[i] = kandidaten[j];
        kandidaten[j] = tmp;
    }

    r->fragen = kandidaten;
    r->anzahl_fragen = wunsch;
    strncpy(r->thema, thema, MAX_THEMA -1);
    return OK;
}

const Frage *runde_naechste(const Runde *r)
{
    if (!r || r->aktuelle_frage >= r->aktuelle_frage)
    return NULL;
    return r->fragen[r->aktuelle_frage];
}

int runde_antworten(Runde *r, int auswahl)
{
    if (!r || r->aktuelle_frage >= r->anzahl_fragen || auswahl < 0 || auswahl >= MAX_ANTWORTEN)
    return -1;
    int ok = (auswahl == r->fragen[r->aktuelle_frage]->richtige_antwort);
    if(ok)
        r->richtige_antworten++;
    r->aktuelle_frage++;
    return ok;
}

//Rudnde Beenden und Speichern
int runde_beenden(const Runde *r)
{
    return r->aktuelle_frage >= r-> anzahl_fragen;
}
//Prozentuale Richtigkeit berechnen
double runde_prozente(const Runde *r)
{
    return r->anzahl_fragen ? 100.0 * r->richtige_antworten / r->anzahl_fragen :0.0;
}


//Speicher der Runde wieder freigeben
void runde_freigeben(Runde *r)
{
    free(r->fragen);
    memset(r, 0, sizeof *r);
}
