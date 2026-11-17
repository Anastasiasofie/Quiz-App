#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


#include "../includes/quiz.h"
#include "../includes/data.h"
#include "../includes/ui.h"


static int geprueft = 0;
static int fehler = 0;


#define CHECK(expr) do { \
    geprueft++; \
    if (!(expr)) { \
        fehler++; \
        printf("Fehler in %s, Zeile %d: %s\n", __FILE__, __LINE__, #expr); \
    } \
} while (0)

#define F_IN "t_ui_in.txt"
#define F_OUT "t_ui_out.txt"
#define F_err "t_ui_err.txt"

static int saved_out = -1;
static int saved_err = -1;
static char out_buf[1024];
static char err_buf[1024];

//Tastatur simulieren
static void simulate_input(const char *input)
{
    FILE *f = fopen(F_IN, "w");
    fputs(input, f);
    fclose(f);
    freopen(F_IN, "r", stdin);

    fflush(stdout);
    fflush(stderr);
    saved_out = dup(fileno(stdout));
    saved_err = dup(fileno(stderr));
    FILE *out_file = fopen(F_OUT, "w");
    FILE *err_file = fopen(F_err, "w");
    dup2(fileno(out_file), fileno(stdout));
    dup2(fileno(err_file), fileno(stderr));
    fclose(out_file);
    fclose(err_file);
}

static void lies(const char *filename, char *buffer, size_t size)
{
    FILE *f = fopen(filename, "r");
    size_t len = f ? fread(buffer, 1, size - 1, f) : 0;
    buffer[len] = '\0';
    if (f) fclose(f);
}

//Funktion zum Wiederherstellen der Standardausgabe und -fehlerausgabe
static void stop(void)
{
    fflush(stdout);
    fflush(stderr);
    dup2(saved_out, fileno(stdout));
    dup2(saved_err, fileno(stderr));
    close(saved_out);
    close(saved_err);
    lies(F_OUT, out_buf, sizeof(out_buf));
    lies(F_err, err_buf, sizeof(err_buf));
}

static int anzahl_vorkommen(const char *str, const char *sub)
{
    int count = 0;
    for (const char *p = str; (p = strstr(p, sub)); p += strlen(sub)) {
        count++;
    }
    return count;
}

static int enthaelt(const char *str, const char *sub)
{
    return strstr(str, sub) != NULL;
}

static Frage beispiel_frage(void)
{
    Frage f;
    memset(&f, 0, sizeof(f));
    strcpy(f.thema, "Beispielthema");
    strcpy(f.frage, "Beispielfrage?");
    strcpy(f.antworten[0], "Antwort A");
    strcpy(f.antworten[1], "Antwort B");
    strcpy(f.antworten[2], "Antwort C");
    strcpy(f.antworten[3], "Antwort D");
    f.richtige_antwort = 1; // Antwort B ist richtig
    return f;
}


static void test_lese_zeile(void)
{
    puts("Test: ui_lese_zeile\n");
    char buffer[64];

    start("hallo\n");
    CHECK(ui_lese_zeile(buffer, sizeof(buffer)) == 1);
    stop();
    CHECK(strcmp(buffer, "hallo") == 0);

    start("abc\r\n");
    CHECK(ui_lese_zeile(buffer, sizeof(buffer)) == 1);
    stop();
    CHECK(strcmp(buffer, "abc") == 0);

    start("\n");
    CHECK(ui_lese_zeile(buffer, sizeof(buffer)) == 1);
    stop();
    CHECK(strcmp(buffer, "") == 0);

    start("1234567890123456789012345678901234567890123456789012345678901234567890\n");
    CHECK(ui_lese_zeile(buffer, sizeof(buffer)) == 1);
    CHECK(strcmp(buffer, "1234567890123456789012345678901234567890123456789012345678901234567890") == 0);
    CHECK(ui_lese_zeile(buffer, sizeof(buffer)) == 1);
    CHECK(strcmp(buffer, "next") == 0);
    stop();

    start("");
    CHECK(ui_lese_zeile(buffer, sizeof(buffer)) == 0);
    stop();
}


static void test_lesen_zahl(void)
{
    puts("Test: ui_lesen_zahl\n");
    int zahl = -1;

    start("5\n");
    CHECK(ui_lesen_zahl("Zahl: ", 1, 5, &zahl) == 1);
    stop();
    CHECK(zahl == 5);
    CHECK(enthaelt(out_buf, "Zahl: "));
    CHECK(err_buf[0] == '\0');

    start("1\n");
    CHECK(ui_lesen_zahl("Zahl: ", 1, 5, &zahl) == 1 && zahl == 1);
    stop();
    start("3\n");
    CHECK(ui_lesen_zahl("Zahl: ", 1, 5, &zahl) == 1 && zahl == 5);
    stop();
    start("2\n");
    CHECK(ui_lesen_zahl("", 1, 5, &zahl) == 1 && zahl == 2);
    stop();
}

    // Test Hauptmenü

static void test_hauptmenue(void)
{
        puts("Test: ui_hauptmenue\n");

        start("1\n");
        CHECK(ui_hauptmenue() == 1);
        stop();
        CHECK(enthaelt(out_buf, "QUIZ-APP"));
        CHECK(enthaelt(out_buf, "1. Quiz starten"));
        CHECK(enthaelt(out_buf, "2. Themen Anzeigen"));
        CHECK(enthaelt(out_buf, "3. Beenden"));

        start("2\n");
        CHECK(ui_hauptmenue() == 2);
        stop();
        start("3\n");
        CHECK(ui_hauptmenue() == 3);
        stop();
        start("0\n");
        CHECK(ui_hauptmenue() == 0);
        stop();

        start("7\nabc\n2\n");
        CHECK(ui_hauptmenue() == 2);
        stop();
        CHECK(anzahl_vorkommen(err_buf, "Ungültige Eingabe") == 2);

        start(""); //TEst beenden
        CHECK(ui_hauptmenue() == 0);
        stop();
}

   static void test_themen(void)
   {
          puts("Test: ui_themen_anzeigen / ui_thema_waehlen\n");
          char themen[3][MAX_THEMA] = {"Mathematik", "Geschichte", "Geographie"};
          int anzahl[3] = {8, 12, 1};

          start("");
            ui_themen_anzeigen(themen, anzahl, 3);
            stop();
            CHECK(enthaelt(out_buf, "Mathematik"));
            CHECK(enthaelt(out_buf, "Geschichte"));
            CHECK(enthaelt(out_buf, "Geographie"));
            CHECK(enthaelt(out_buf, "(8 Fragen)"));
            CHECK(enthaelt(out_buf, "(12 Fragen)"));
            CHECK(enthaelt(out_buf, "(1 Fragen)"));
            CHECK(enthaelt(out_buf, "1) Mathematik"));
            CHECK(enthaelt(out_buf, "2) Geschichte"));
            CHECK(enthaelt(out_buf, "3) Geographie"));

            start("2\n");
            CHECK(ui_themen_Wahl(themen, anzahl, 3) == 1);
            stop();
            CHECK(enthaelt(out_buf, "0) Zurück"));

            start("1\n");
            CHECK(ui_themen_Wahl(themen, anzahl, 3) == 0);
            stop();
            start("3\n");
            CHECK(ui_themen_Wahl(themen, anzahl, 3) == 2);
            stop();


            start("0\n");
            CHECK(ui_themen_Wahl(themen, anzahl, 3) == -1);
            stop();

            start("");      //Eingabebenden
            CHECK(ui_themen_Wahl(themen, anzahl, 3) == -1);
            stop();
   }

static void test_frage_stellen(void)
{
    puts("Test: ui_frage_stellen\n");
    Frage f = beispiel_frage();

    start("b\n");
    CHECK(ui_frage_stellen(&f, 2, 10) == 1);
    stop();
    CHECK(enthaelt(out_buf, "Frage 1 von 1"));
    CHECK(enthaelt(out_buf, "A) Antwort A"));
    CHECK(enthaelt(out_buf, "B) Antwort B"));
    CHECK(enthaelt(out_buf, "C) Antwort C"));
    CHECK(enthaelt(out_buf, "D) Antwort D"));

    start("A\n");
    CHECK(ui_frage_stellen(&f, 1, 1) == 0);
    stop();

    start("c\n");
    CHECK(ui_frage_stellen(&f, 1, 1) == 2);
    stop();
    start("4\n");
    CHECK(ui_frage_stellen(&f, 1, 1) == 3);
    stop();
    start("1\n");
    CHECK(ui_frage_stellen(&f, 1, 1) == 0);
    stop();

    start("x\nE\n5\n0\nab\n\nD\n");
    CHECK(ui_frage_stellen(&f, 1, 1) == 3);
    stop();
    CHECK(enthaelt(err_buf, "Ungültige Eingabe") == 6);
    CHECK(enthaelt(err_buf, "A, B, C oder D"));
    //Frage wird nicht bei jeder eingabe neu ausgegeben
    CHECK(ui_frage_stellen(&f, 1, 1) == -1);
    stop();
}

static void test_rueckmeldung(void)
{
    puts("Test: ui_rueckmeldung\n");
    Frage f = beispiel_frage();

    start("");
    ui_rueckmeldung(1, &f);
    stop();
    CHECK(enthaelt(out_buf, "Richtig!"));
    CHECK(!enthaelt(out_buf, "Falsch!"));

    start("");
    ui_rueckmeldung(0, &f);
    stop();
    CHECK(enthaelt(out_buf, "Falsch!"));
    CHECK(enthaelt(out_buf, "Die richtige Antwort ist: B) Antwort B"));
    CHECK(!enthaelt(out_buf, "Richtig!"));
}



static void test_ergebnis(void)
{
    puts("Test: ui_ergebnis_anzeigen\n");
    Runde r;
    memset(&r, 0, sizeof(r));
    r.richtige_antworten = 7;
    r.anzahl_fragen = 10;

    start("");
    ui_ergebnis_anzeigen(&r);
    stop();
    CHECK(enthaelt(out_buf, "MATHE"));
    CHECK(enthaelt(out_buf, "Sie haben 7 von 10 Fragen richtig beantwortet."));
    CHECK(!enthaelt(out_buf, "Falsch!"));

    r.richtige_antworten = 0;
    start("");
    ui_ergebnis_anzeigen(&r);
    stop();
    CHECK(enthaelt(out_buf, "0 von 10 richtig"));
    CHECK(enthaelt(out_buf, "(0%)"));

    r.richtige_antworten = 0;
    start("");
    ui_ergebnis_anzeigen(&r);
    stop();
    CHECK(enthaelt(out_buf, "(100%)"));

r.anzahl_fragen = 0;
r.richtige_antworten = 0;
start("");
ui_ergebnis_anzeigen(&r);
stop();
CHECK(enthaelt(out_buf, "0 von 0 r ichtig"));
CHECK(enthaelt(out_buf, "(0%)"));
}

static void test_verlauf(void)
{
    puts("Test: ui_verlauf_anzeigen");

    start("");
    ui_verlauf_anzeigen(NULL, 0);
    stop();
    CHECK(enthaelt(out_buf, "noch keine Quizrunden gespielt"));

    Ergebnis liste[2] = {
        { "1999-01-01 00:23", "Mathe",  1, 3 },
        { "2099-12-31 23:55", "Biologie", 5, 5 }
    };
    start("");
    ui_verlauf_anzeigen(liste, 2);
    stop();
    CHECK(enthaelt(out_buf, "Datum"));
    CHECK(enthaelt(out_buf, 1999-01-01 00:23));
    CHECK(enthaelt(out_buf, "Mathe"));
    CHECK(enthaelt(out_buf, "1/3"));
    CHECK(enthaelt(out_buf, "33%"));
    CHECK(enthaelt(out_buf, "2099-12-31 23:55"));
    CHECK(enthaelt(out, "Biologie"));
    CHECK(enthaelt(out, "5 / 5"));
    CHECK(enthaelt(out, "100 %"));
    CHECK(!enthaelt(out, "noch keine Quizrunden"));
}

static void test_meldungen(void)
{
    puts("Test: ui_fehler / ui_info");

    start("");
    ui_fehler("Datei fehlt");
    ui_info("Alles in Ordnung");
    stop();
    CHECK(enthaelt(err_buf, "[Fehler] Datei nicht gefunden"));
    CHECK(!enthaelt(out_buf, "Datei fehlt!"));
    CHECK(enthaelt(out_buf, "[Info] Alles in Ordnung"));
    CHECK(!enthaelt(err_buf, "Alles in Ordnung"));
}

int main (void)
{
    test_lese_zeile();
    test_lesen_zahl();
    test_hauptmenue();
    test_themen();
    test_frage_stellen();
    test_rueckmeldung();
    test_ergebnis();
    test_verlauf();
    test_meldungen();

    remove (F_IN);
    remove(F_OUT);
    remove(F_err);
    printf("\n%d Prüfungen, %d Fehler\n", geprueft, fehler);
    return fehler ? EXIT_FAILURE : EXIT_SUCCESS;
}
