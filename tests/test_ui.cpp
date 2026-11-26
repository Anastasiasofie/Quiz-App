#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ui.h"
#include "data.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <stdexcept>
#include <unistd.h>

// Leitet stdin, stdout und stderr für einen einzelnen Test um.
// Die ursprünglichen Streams werden beim Verlassen des Objekts wiederhergestellt.
class ConsoleCapture {
public:
    explicit ConsoleCapture(const char *input) {
        saved_stdin_  = dup(fileno(stdin));
        saved_stdout_ = dup(fileno(stdout));
        saved_stderr_ = dup(fileno(stderr));

        if (saved_stdin_ == -1 || saved_stdout_ == -1 || saved_stderr_ == -1) {
            throw std::runtime_error("Standardstreams konnten nicht gesichert werden");
        }

        input_  = tmpfile();
        output_ = tmpfile();
        error_  = tmpfile();
        if (!input_ || !output_ || !error_) {
            cleanup();
            throw std::runtime_error("Temporäre Dateien konnten nicht erstellt werden");
        }

        if (input) std::fputs(input, input_);
        std::rewind(input_);

        if (dup2(fileno(input_), fileno(stdin)) == -1 ||
            dup2(fileno(output_), fileno(stdout)) == -1 ||
            dup2(fileno(error_), fileno(stderr)) == -1) {
            cleanup();
            throw std::runtime_error("Standardstreams konnten nicht umgeleitet werden");
        }

        clearerr(stdin);
        clearerr(stdout);
        clearerr(stderr);
    }

    ConsoleCapture(const ConsoleCapture &) = delete;
    ConsoleCapture &operator=(const ConsoleCapture &) = delete;

    ~ConsoleCapture() { cleanup(); }

    std::string output() {
        std::fflush(stdout);
        return read_file(output_);
    }

    std::string error() {
        std::fflush(stderr);
        return read_file(error_);
    }

private:
    int saved_stdin_ = -1;
    int saved_stdout_ = -1;
    int saved_stderr_ = -1;
    FILE *input_ = nullptr;
    FILE *output_ = nullptr;
    FILE *error_ = nullptr;

    static std::string read_file(FILE *file) {
        if (!file) return {};
        std::fflush(file);
        std::rewind(file);
    std::string result;
        char buffer[256];
        while (std::size_t count = std::fread(buffer, 1, sizeof(buffer), file)) {
            result.append(buffer, count);
        }
        return result;
    }

    void cleanup() {
        std::fflush(stdout);
        std::fflush(stderr);

        if (saved_stdin_ != -1) {
            dup2(saved_stdin_, fileno(stdin));
            close(saved_stdin_);
            saved_stdin_ = -1;
        }
        if (saved_stdout_ != -1) {
            dup2(saved_stdout_, fileno(stdout));
            close(saved_stdout_);
            saved_stdout_ = -1;
        }
        if (saved_stderr_ != -1) {
            dup2(saved_stderr_, fileno(stderr));
            close(saved_stderr_);
            saved_stderr_ = -1;
        }

        clearerr(stdin);
        clearerr(stdout);
        clearerr(stderr);

        if (input_)  { std::fclose(input_);  input_ = nullptr; }
        if (output_) { std::fclose(output_); output_ = nullptr; }
        if (error_)  { std::fclose(error_);  error_ = nullptr; }
    }
};

static Frage beispiel_frage() {
    static Frage frage = {};
    std::strcpy(frage.frage, "Was ist die Hauptstadt von Deutschland?");
    std::strcpy(frage.antworten[0], "Berlin");
    std::strcpy(frage.antworten[1], "Hamburg");
    std::strcpy(frage.antworten[2], "Muenchen");
    std::strcpy(frage.antworten[3], "Koeln");
    frage.richtige_antwort = 0;
    return frage;
}

static Fragensammlung * beispiel_sammlung() {
    static Fragensammlung sammlung = {};
    sammlung.anzahl_fragensaetze = 2;
    std::strcpy(sammlung.saetze[0].thema, "Mathematik");
    sammlung.saetze[0].anzahl_fragen = 5;
    std::strcpy(sammlung.saetze[1].thema, "Geschichte");
    sammlung.saetze[1].anzahl_fragen = 3;
    return &sammlung;
}

//Unit tests for ui_lese_zeile 

TEST_CASE("Test of die Ui eine Zeile einlesen kann und beim Dateiende null liefert", "[ui_lese_zeile]") {
    SECTION("liest eine normale Zeile") {
        ConsoleCapture console("Hallo Welt\n");
        char puffer[100] = {};
        CHECK(ui_lese_zeile(puffer, sizeof(puffer)) == 1);
        CHECK(std::strcmp(puffer, "Hallo Welt") == 0);
    }

    SECTION("liefert bei Dateiende null") {
        ConsoleCapture console("");
        char puffer[100] = {};
        CHECK(ui_lese_zeile(puffer, sizeof(puffer)) == 0);
    }
}

//Unit tests for ui_lesen_zahl

TEST_CASE("Test ob die Ui eine Zahl einlesen kann", "[ui_lesen_zahl]") {
    SECTION("akzeptiert eine gültige Zahl") {
        ConsoleCapture console("2\n");
        int wert = 0;
        CHECK(ui_lesen_zahl("Zahl: ", 1, 3, &wert) == 1);
        CHECK(wert == 2);
    }

    SECTION("fragt bei zu kleiner Zahl erneut") {
        ConsoleCapture console("0\n2\n");
        int wert = 0;
        CHECK(ui_lesen_zahl("Zahl: ", 1, 3, &wert) == 1);
        CHECK(wert == 2);
    }

    SECTION("fragt bei zu großer Zahl erneut") {
        ConsoleCapture console("9\n3\n");
        int wert = 0;
        CHECK(ui_lesen_zahl("Zahl: ", 1, 3, &wert) == 1);
        CHECK(wert == 3);
    }

    SECTION("fragt bei Texteingabe erneut") {
        ConsoleCapture console("abc\n1\n");
        int wert = 0;
        CHECK(ui_lesen_zahl("Zahl: ", 1, 3, &wert) == 1);
        CHECK(wert == 1);
    }

    SECTION("liefert bei Dateiende null") {
        ConsoleCapture console("");
        int wert = 0;
        CHECK(ui_lesen_zahl("Zahl: ", 1, 3, &wert) == 0);
    }
}

//Unit tests für ui_hauptmenue

TEST_CASE("Testet das Hauptmenü", "[ui_hauptmenue]") {
    SECTION("wählt Menüpunkt 1") {
        ConsoleCapture console("1\n");
        CHECK(ui_hauptmenue() == 1);
    }
    SECTION("wählt Menüpunkt 2") {
        ConsoleCapture console("2\n");
        CHECK(ui_hauptmenue() == 2);
    }
    SECTION("wählt Menüpunkt 3") {
        ConsoleCapture console("3\n");
        CHECK(ui_hauptmenue() == 3);
    }
    SECTION("fragt eine ungültige Auswahl erneut ab") {
        ConsoleCapture console("8\n2\n");
        CHECK(ui_hauptmenue() == 2);
    }
    SECTION("liefert bei Dateiende minus eins") {
        ConsoleCapture console("");
        CHECK(ui_hauptmenue() == -1);
    }
}

//Test für ui_themen_anzeigen

TEST_CASE("Testet das Anzeigen der Themen", "[ui_themen_anzeigen]") {
    SECTION("zeigt alle vorhandenen Themen") {
        Fragensammlung *sammlung = beispiel_sammlung();
        ConsoleCapture console("");
        ui_themen_anzeigen(sammlung);
        const std::string ausgabe = console.output();
        CHECK(ausgabe.find("Mathematik") != std::string::npos);
        CHECK(ausgabe.find("Geschichte") != std::string::npos);
    }
    SECTION("behandelt einen Null-Zeiger") {
        ConsoleCapture console("");
        ui_themen_anzeigen(nullptr);
        CHECK(console.error().find("Keine Fragensammlung") != std::string::npos);
    }
}

//Test für ui_themen_wahl

TEST_CASE("Testet die Themenauswahl", "[ui_themen_wahl]") {
    Fragensammlung *sammlung = beispiel_sammlung();
    SECTION("wählt das erste Thema") {
        ConsoleCapture console("1\n");
        CHECK(ui_themen_wahl(sammlung) == 0);
    }
    SECTION("wählt das zweite Thema") {
        ConsoleCapture console("2\n");
        CHECK(ui_themen_wahl(sammlung) == 1);
    }
    SECTION("Null beendet die Themenauswahl") {
        ConsoleCapture console("0\n");
        CHECK(ui_themen_wahl(sammlung) == -1);
    }
    SECTION("fragt eine ungültige Auswahl erneut ab") {
        ConsoleCapture console("5\n1\n");
        CHECK(ui_themen_wahl(sammlung) == 0);
    }
}

//Test für ui_frage_stellen

TEST_CASE("Testet das Stellen von Fragen", "[ui_frage_stellen]") {
    Frage frage = beispiel_frage();
    SECTION("akzeptiert Antwort A und liefert 0") {  //da man innerhalb des structs von 0-3 Nummeriert
        ConsoleCapture console("A\n");
        CHECK(ui_frage_stellen(&frage, 1, 5) == 0);
    }
    SECTION("akzeptiert Antwort B und liefert 1") {
        ConsoleCapture console("B\n");
        CHECK(ui_frage_stellen(&frage, 1, 5) == 1);
    }
    SECTION("akzeptiert Antwort D und liefert 3") {
        ConsoleCapture console("D\n");
        CHECK(ui_frage_stellen(&frage, 1, 5) == 3);
    }
    SECTION("akzeptiert eine numerische Antwort") {
        ConsoleCapture console("2\n");
        CHECK(ui_frage_stellen(&frage, 1, 5) == 1);
    }
    SECTION("fragt bei ungültiger Eingabe erneut") {
        ConsoleCapture console("X\nC\n");
        CHECK(ui_frage_stellen(&frage, 1, 5) == 2);
    }
    SECTION("zeigt Frage und vier Antwortmöglichkeiten") {
        ConsoleCapture console("A\n");
        ui_frage_stellen(&frage, 1, 5);
        const std::string ausgabe = console.output();
        CHECK(ausgabe.find("Was ist die Hauptstadt") != std::string::npos);
        CHECK(ausgabe.find("A) Berlin") != std::string::npos);
        CHECK(ausgabe.find("B) Hamburg") != std::string::npos);
        CHECK(ausgabe.find("C) Muenchen") != std::string::npos);
        CHECK(ausgabe.find("D) Koeln") != std::string::npos);
    }
}

//Test für ui_rueckmeldung

TEST_CASE("Testet die Rückmeldung", "[ui_rueckmeldung]") {
    Frage frage = beispiel_frage();
    SECTION("gibt Rückmeldung bei richtiger Antwort") {
        ConsoleCapture console("");
        ui_rueckmeldung(1, &frage);
        CHECK(console.output().find("Richtig") != std::string::npos);
    }
    SECTION("zeigt bei falscher Antwort die korrekte Antwort") {
        ConsoleCapture console("");
        ui_rueckmeldung(0, &frage);
        const std::string ausgabe = console.output();
        CHECK(ausgabe.find("A)") != std::string::npos);
        CHECK(ausgabe.find("Berlin") != std::string::npos);
    }
}

//test für ui_ergebnis

TEST_CASE("Testet das Anzeigen der Ergebnisse", "[ui_ergebnis]") {
    ConsoleCapture console("");
    ui_ergebnis(3, 5);
    const std::string ausgabe = console.output();
    CHECK(ausgabe.find("3") != std::string::npos);
    CHECK(ausgabe.find("5") != std::string::npos);
}

//test für ui_verlauf_anzeigen

TEST_CASE("Testet das Anzeigen des Verlaufs", "[ui_verlauf_anzeigen]") {
    Ergebnis ergebnisse[2] = {};
    std::strcpy(ergebnisse[0].thema, "Mathematik");
    ergebnisse[0].richtig = 4;
    ergebnisse[0].gesamt = 5;
    std::strcpy(ergebnisse[1].thema, "Geschichte");
    ergebnisse[1].richtig = 2;
    ergebnisse[1].gesamt = 3;

    ConsoleCapture console("");
    ui_verlauf_anzeigen(ergebnisse, 2);
    const std::string ausgabe = console.output();
    CHECK(ausgabe.find("Mathematik") != std::string::npos);
    CHECK(ausgabe.find("Geschichte") != std::string::npos);
}
