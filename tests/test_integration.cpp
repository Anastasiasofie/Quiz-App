//Hier wird getestet ob die einzelnen Module zusammenarbeiten können
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "data.h"
#include "import.h"
#include "ui.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <io.h>

// Simuliert Benutzereingaben und fängt printf-Ausgaben ab
class ConsoleTest {
private:
    FILE* eingabe;
    FILE* ausgabe;
    int alte_stdin;
    int alte_stdout;

public:
    ConsoleTest(const char* benutzereingabe = "") {
        eingabe = tmpfile();
        ausgabe = tmpfile();

        fputs(benutzereingabe, eingabe);
        rewind(eingabe);

        fflush(stdout);

        alte_stdin = dup(fileno(stdin));
        alte_stdout = dup(fileno(stdout));

        dup2(fileno(eingabe), fileno(stdin));
        dup2(fileno(ausgabe), fileno(stdout));

        clearerr(stdin);
        clearerr(stdout);
    }

    std::string output() {
        fflush(stdout);
        rewind(ausgabe);

        char puffer[4096];
        std::string text;

        size_t gelesen;
        while ((gelesen = fread(puffer, 1, sizeof(puffer), ausgabe)) > 0) {
            text.append(puffer, gelesen);
        }

        return text;
    }

    ~ConsoleTest() {
        fflush(stdout);

        dup2(alte_stdin, fileno(stdin));
        dup2(alte_stdout, fileno(stdout));

        close(alte_stdin);
        close(alte_stdout);

        fclose(eingabe);
        fclose(ausgabe);

        clearerr(stdin);
        clearerr(stdout);
    }
};


TEST_CASE("Importierte Fragen funktionieren mit der UI", "[import][ui][integration]") {

    static Fragensammlung sammlung = {};

    // 1. CSV importieren
    std::memset(&sammlung, 0, sizeof(sammlung));

    REQUIRE(import_csv("docs/fragen.csv", &sammlung) == 1);

    REQUIRE(sammlung.anzahl_fragensaetze == 2);


    // 2. Themenanzeige testen
    {
        ConsoleTest console;

        ui_themen_anzeigen(&sammlung);

        std::string text = console.output();

        CHECK(text.find("Allgemeinwissen") != std::string::npos);
        CHECK(text.find("C-Programmierung") != std::string::npos);
    }


    // 3. Thema auswählen (Eingabe: 1)
    int thema_index;

    {
        ConsoleTest console("1\n");

        thema_index = ui_themen_wahl(&sammlung);

        std::string text = console.output();

        CHECK(text.find("Allgemeinwissen") != std::string::npos);
    }

    REQUIRE(thema_index == 0);


    // 4. Erste Frage beantworten (Eingabe: B)
    Fragensatz* satz = &sammlung.saetze[thema_index];

    REQUIRE(satz->anzahl_fragen > 0);

    Frage* frage = &satz->fragen[0];

    int antwort;

    {
        ConsoleTest console("B\n");

        antwort = ui_frage_stellen(frage, 1, satz->anzahl_fragen);

        std::string text = console.output();

        CHECK(text.find("Was ist die Hauptstadt von Australien?")!= std::string::npos);

        CHECK(text.find("Canberra") != std::string::npos);
    }

    CHECK(antwort == 1);


    // 5. Rückmeldung testen
    {
        ConsoleTest console;

        int richtig = (antwort == frage->richtige_antwort);

        ui_rueckmeldung(richtig, frage);

        std::string text = console.output();

        CHECK(text.find("Richtig!") != std::string::npos);
    }
}