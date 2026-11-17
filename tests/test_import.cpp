#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "import.h"
#include <string.h>

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
