#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "import.h"
#include <string.h>

//Tests für open_csv

TEST_CASE("CSV Datei erfolgreich öffnen", "[open_csv]") {

    FILE *datei = open_csv("tests/test.csv");

    CHECK(datei != NULL);

    if (datei != NULL) {
        fclose(datei);
    }
}


TEST_CASE("Nicht existierende CSV Datei öffnen", "[open_csv]") {

    FILE *datei = open_csv("tests/datei_existiert_nicht.csv");

    CHECK(datei == NULL);
}

//tests für parse_csv_line


TEST_CASE("CSV Zeile korrekt aufteilen [parse_csv_line]") {

    char felder[4][MAX_TEXT];

    int anzahl = parse_csv_line(
        "Wie heisst du?;Max;Mueller;20",
        felder,
        4
    );

    CHECK(anzahl == 4);

    CHECK(strcmp(felder[0], "Wie heisst du?") == 0);
    CHECK(strcmp(felder[1], "Max") == 0);
    CHECK(strcmp(felder[2], "Mueller") == 0);
    CHECK(strcmp(felder[3], "20") == 0);
}


TEST_CASE("CSV Zeile mit nur einem Feld", "[parse_csv_line]") {

    char felder[4][MAX_TEXT];

    int anzahl = parse_csv_line("Hallo", felder, 4);

    CHECK(anzahl == 1);
    CHECK(strcmp(felder[0], "Hallo") == 0);
}

TEST_CASE("CSV Zeile mit leeren Feldern", "[parse_csv_line]") {

    char felder[4][MAX_TEXT];

    int anzahl = parse_csv_line("Max;;20;", felder, 4);

    CHECK(anzahl == 4);

    CHECK(strcmp(felder[0], "Max") == 0);
    CHECK(strcmp(felder[1], "") == 0);
    CHECK(strcmp(felder[2], "20") == 0);
    CHECK(strcmp(felder[3], "") == 0);
}


TEST_CASE("NULL als Zeile", "[parse_csv_line]") {

    char felder[4][MAX_TEXT];

    CHECK(parse_csv_line(NULL, felder, 4) == -1);
}


TEST_CASE("NULL als Feldmatrix", "[parse_csv_line]") {

    CHECK(parse_csv_line("Max;20", NULL, 4) == -1);
}


TEST_CASE("Maximale Feldanzahl gleich null", "[parse_csv_line]") {

    char felder[4][MAX_TEXT];

    CHECK(parse_csv_line("Max;20", felder, 0) == -1);
}


TEST_CASE("Mehr Felder als erlaubt", "[parse_csv_line]") {

    char felder[2][MAX_TEXT];

    CHECK(parse_csv_line("Max;20;Berlin", felder, 2) == -1);
}


TEST_CASE("Text ist zu lang", "[parse_csv_line]") {

    char zeile[300];

    memset(zeile, 'A', 300);

    char felder[2][MAX_TEXT];

    CHECK(parse_csv_line(zeile, felder, 2) == -1);
}


// Tests für validate_question

TEST_CASE("gültige Frage", "[validate_question]") {

    char felder[7][MAX_TEXT] = {
        "Mathematik",
        "Was ist 2 + 2?",
        "Antwort 1",
        "Antwort 2",
        "Antwort 3",
        "Antwort 4",
        "4"
    };

    CHECK(validate_question(felder, 7) == 1);
}

TEST_CASE("ungültige Eingaben", "[validate_question]") {

    SECTION("Anzahl der Felder ist kleiner als 7") {
        char felder[7][MAX_TEXT] = {
           "Thema", "Frage", "A1", "A2", "A3", "A4", "2"
        };

        CHECK(validate_question(felder, 6) == 0);
    }

    SECTION("Anzahl der Felder ist größer als 7") {
        char felder[7][MAX_TEXT] = {
            "Thema", "Frage", "A1", "A2", "A3", "A4", "2"
        };

        CHECK(validate_question(felder, 8) == 0);
    }

    SECTION("Felder-Pointer ist NULL") {
        CHECK(validate_question(NULL, 7) == 0);
    }

    SECTION("Ein Feld ist leer") {
        char felder[7][MAX_TEXT] = {
            "Thema", "Frage", "A1", "", "A3", "A4", "2"
        };

        CHECK(validate_question(felder, 7) == 0);
    }

    SECTION("Richtige Antwort ist keine Zahl") {
        char felder[7][MAX_TEXT] = {
           "Thema", "Frage", "A1", "A2", "A3", "A4", "abc"
        };

        CHECK(validate_question(felder, 7) == 0);
    }

    SECTION("Richtige Antwort enthält Zahl und Buchstaben") {
        char felder[7][MAX_TEXT] = {
            "Thema", "Frage", "A1", "A2", "A3", "A4", "2abc"
        };

        CHECK(validate_question(felder, 7) == 0);
    }

    SECTION("Richtige Antwort ist kleiner als 1") {
        char felder[7][MAX_TEXT] = {
            "Thema", "Frage", "A1", "A2", "A3", "A4", "0"
        };

        CHECK(validate_question(felder, 7) == 0);
    }

    SECTION("Richtige Antwort ist größer als 4") {
        char felder[7][MAX_TEXT] = {
            "Thema", "Frage", "A1", "A2", "A3", "A4", "5"
        };

        CHECK(validate_question(felder, 7) == 0);
    }
}

//tests für find_fragensatz

TEST_CASE("Vorhandenes Thema finden", "[find_fragensatz]") {

    static Fragensammlung sammlung = {};  
    //wurde mit static deklariert, da sonst stack overflow -> dynamische Speicherallokierung muss noch implementiert werden

    sammlung.anzahl_fragensaetze = 2;

    strcpy(sammlung.saetze[0].thema, "Mathematik");
    strcpy(sammlung.saetze[1].thema, "Informatik");

    Fragensatz* ergebnis = find_fragensatz(&sammlung, "Informatik");

    REQUIRE(ergebnis != NULL);
    CHECK(strcmp(ergebnis->thema, "Informatik") == 0);
}

TEST_CASE("find_fragensatz gibt NULL zurück, wenn Thema nicht existiert", "[find_fragensatz]") {

    static Fragensammlung sammlung = {};

    sammlung.anzahl_fragensaetze = 2;

    strcpy(sammlung.saetze[0].thema, "Mathematik");
    strcpy(sammlung.saetze[1].thema, "Informatik");

    Fragensatz* ergebnis =
        find_fragensatz(&sammlung, "Physik");

    REQUIRE(ergebnis == NULL);
}

TEST_CASE("find_fragensatz gibt NULL bei leerer Sammlung zurück", "[find_fragensatz]") {

    static Fragensammlung sammlung = {};

    sammlung.anzahl_fragensaetze = 0;

    Fragensatz* ergebnis =
        find_fragensatz(&sammlung, "Mathematik");

    REQUIRE(ergebnis == NULL);
}

TEST_CASE("find_fragensatz gibt den richtigen Fragensatz zurück", "[find_fragensatz]") {

    static Fragensammlung sammlung = {};

    sammlung.anzahl_fragensaetze = 3;

    strcpy(sammlung.saetze[0].thema, "Mathematik");
    strcpy(sammlung.saetze[1].thema, "Informatik");
    strcpy(sammlung.saetze[2].thema, "Biologie");

    Fragensatz* ergebnis =
        find_fragensatz(&sammlung, "Biologie");

    REQUIRE(ergebnis == &sammlung.saetze[2]);
    REQUIRE(strcmp(ergebnis->thema, "Biologie") == 0);
}
