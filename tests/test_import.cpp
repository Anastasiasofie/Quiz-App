#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "import.h"
#include <string.h>

//Tests für open_csv

TEST_CASE("CSV Datei erfolgreich öffnen") {

    FILE *datei = open_csv("tests/test.csv");

    CHECK(datei != NULL);

    if (datei != NULL) {
        fclose(datei);
    }
}


TEST_CASE("Nicht existierende CSV Datei öffnen") {

    FILE *datei = open_csv("tests/datei_existiert_nicht.csv");

    CHECK(datei == NULL);
}

//tests für parse_csv_line

TEST_CASE("CSV Zeile korrekt aufteilen") {

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



TEST_CASE("CSV Zeile mit nur einem Feld") {

    char felder[4][MAX_TEXT];

    int anzahl = parse_csv_line("Hallo", felder, 4);

    CHECK(anzahl == 1);
    CHECK(strcmp(felder[0], "Hallo") == 0);
}

TEST_CASE("CSV Zeile mit leeren Feldern") {

    char felder[4][MAX_TEXT];

    int anzahl = parse_csv_line("Max;;20;", felder, 4);

    CHECK(anzahl == 4);

    CHECK(strcmp(felder[0], "Max") == 0);
    CHECK(strcmp(felder[1], "") == 0);
    CHECK(strcmp(felder[2], "20") == 0);
    CHECK(strcmp(felder[3], "") == 0);
}


TEST_CASE("NULL als Zeile") {

    char felder[4][MAX_TEXT];

    CHECK(parse_csv_line(NULL, felder, 4) == -1);
}


TEST_CASE("NULL als Feldmatrix") {

    CHECK(parse_csv_line("Max;20", NULL, 4) == -1);
}


TEST_CASE("Maximale Feldanzahl gleich null") {

    char felder[4][MAX_TEXT];

    CHECK(parse_csv_line("Max;20", felder, 0) == -1);
}


TEST_CASE("Mehr Felder als erlaubt") {

    char felder[2][MAX_TEXT];

    CHECK(parse_csv_line("Max;20;Berlin", felder, 2) == -1);
}


TEST_CASE("Text ist zu lang") {

    char zeile[300];

    memset(zeile, 'A', 300);

    char felder[2][MAX_TEXT];

    CHECK(parse_csv_line(zeile, felder, 2) == -1);
}


// Tests für validate_question

TEST_CASE("validate_question - gültige Frage") {

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

TEST_CASE("validate_question - ungültige Eingaben") {

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
