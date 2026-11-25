#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "import.h"
#include <string.h>
#include <fstream>
#include <cstdio>
#include <cstdlib>


// Unit Tests für open_csv

TEST_CASE("CSV Datei erfolgreich öffnen", "[open_csv]") {

    FILE *datei = open_csv("docs/fragen.csv");

    CHECK(datei != NULL);

    if (datei != NULL) {
        fclose(datei);
    }
}


TEST_CASE("Nicht existierende CSV Datei öffnen", "[open_csv]") {

    FILE *datei = open_csv("docs/datei_existiert_nicht.csv");

    CHECK(datei == NULL);
}

// Unit Tests für parse_csv_line


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


// Unit Tests für validate_question

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
}

//Unit Tests für parse_correct_answer

TEST_CASE("parse_correct_answer wandelt Antworten korrekt um", "[parse_correct_answer]") {

    SECTION("Großbuchstaben A bis D") {
        CHECK(parse_correct_answer("A") == 0);
        CHECK(parse_correct_answer("B") == 1);
        CHECK(parse_correct_answer("C") == 2);
        CHECK(parse_correct_answer("D") == 3);
    }

    SECTION("Kleinbuchstaben a bis d") {
        CHECK(parse_correct_answer("a") == 0);
        CHECK(parse_correct_answer("b") == 1);
        CHECK(parse_correct_answer("c") == 2);
        CHECK(parse_correct_answer("d") == 3);
    }

    SECTION("Zahlen 1 bis 4") {
        CHECK(parse_correct_answer("1") == 0);
        CHECK(parse_correct_answer("2") == 1);
        CHECK(parse_correct_answer("3") == 2);
        CHECK(parse_correct_answer("4") == 3);
    }

    SECTION("Ungültige Eingaben") {
        CHECK(parse_correct_answer("E") == -1);
        CHECK(parse_correct_answer("5") == -1);
        CHECK(parse_correct_answer("") == -1);
        CHECK(parse_correct_answer("AB") == -1);
        CHECK(parse_correct_answer(NULL) == -1);
    }
}

//Unit tests für find_fragensatz

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

//Integrationstest für import_csv

// Test 1: Erfolgreicher Import mit mehreren Themen
TEST_CASE("CSV-Datei wird korrekt importiert", "[import_csv]") {

    const char *dateiname = "docs/fragen.csv";

    Fragensammlung *sammlung = (Fragensammlung *)calloc(1, sizeof(Fragensammlung));

    REQUIRE(sammlung != NULL);

    REQUIRE(import_csv(dateiname, sammlung) == 1);

    // Zwei Themen müssen vorhanden sein
    REQUIRE(sammlung->anzahl_fragensaetze == 2);

    // Erstes Thema überprüfen
    Fragensatz *allgemeinwissen = find_fragensatz(sammlung, "Allgemeinwissen");

    REQUIRE(allgemeinwissen != NULL);
    REQUIRE(allgemeinwissen->anzahl_fragen == 8);

    REQUIRE(strcmp(allgemeinwissen->fragen[0].antworten[1],"Canberra") == 0);

    REQUIRE(allgemeinwissen->fragen[0].richtige_antwort == 1);

    // Zweites Thema überprüfen
    Fragensatz *programmieren = find_fragensatz(sammlung, "C-Programmierung");

    REQUIRE(programmieren != NULL);
    REQUIRE(programmieren->anzahl_fragen == 8);

    REQUIRE(programmieren->fragen[0].richtige_antwort == 2);

    free(sammlung);
}

// Test 2: Fehlende Datei erkennen
TEST_CASE("Fehlende CSV-Datei wird erkannt", "[import_csv]") {

    Fragensammlung *sammlung =
        (Fragensammlung *)calloc(1, sizeof(Fragensammlung));

    REQUIRE(sammlung != NULL);

    REQUIRE(import_csv("datei_existiert_nicht.csv", sammlung) == 0);

    free(sammlung);
}


// Test 3: Fehlerhafte CSV-Datei erkennen
TEST_CASE("Fehlerhafte CSV-Zeile wird erkannt", "[import_csv]") {

    const char *dateiname = "test_fehler.csv";

    {
        std::ofstream datei(dateiname);

        datei << "thema;frage;antwort_a;antwort_b;antwort_c;antwort_d;richtig\n";

        // Es fehlt ein Feld
        datei << "Allgemeinwissen;Hauptstadt?;Bonn;Berlin;Hamburg;Muenchen\n";
    }

    Fragensammlung *sammlung =
        (Fragensammlung *)calloc(1, sizeof(Fragensammlung));

    REQUIRE(sammlung != NULL);

    REQUIRE(import_csv(dateiname, sammlung) == 0);

    free(sammlung);
    std::remove(dateiname);
}


// Test 4: Ungültige richtige Antwort erkennen
TEST_CASE("Ungültige richtige Antwort wird erkannt", "[import_csv]") {

    const char *dateiname = "test_antwort.csv";

    {
        std::ofstream datei(dateiname);

        datei << "thema;frage;antwort_a;antwort_b;antwort_c;antwort_d;richtig\n";
        datei << "Allgemeinwissen;Hauptstadt?;Bonn;Berlin;Hamburg;Muenchen;E\n";
    }

    Fragensammlung *sammlung =
        (Fragensammlung *)calloc(1, sizeof(Fragensammlung));

    REQUIRE(sammlung != NULL);

    REQUIRE(import_csv(dateiname, sammlung) == 0);

    free(sammlung);
    std::remove(dateiname);
}

// Test 5: Kopfzeile, Kommentare, Leerzeilen und Windows-Zeilenenden
TEST_CASE("CSV ignoriert Kopfzeile Kommentare und Leerzeilen", "[import_csv]") {

    const char *dateiname = "test_format.csv";

    {
        std::ofstream datei(dateiname, std::ios::binary);

        datei << "thema;frage;antwort_a;antwort_b;antwort_c;antwort_d;richtig\r\n";
        datei << "\r\n";
        datei << "# Kommentarzeile\r\n";
        datei << "Allgemeinwissen;Hauptstadt?;Bonn;Berlin;Hamburg;Muenchen;B\r\n";
    }

    Fragensammlung *sammlung =
        (Fragensammlung *)calloc(1, sizeof(Fragensammlung));

    REQUIRE(sammlung != NULL);

    REQUIRE(import_csv(dateiname, sammlung) == 1);

    REQUIRE(sammlung->anzahl_fragensaetze == 1);
    REQUIRE(sammlung->saetze[0].anzahl_fragen == 1);

    REQUIRE(sammlung->saetze[0].fragen[0].richtige_antwort == 1);

    free(sammlung);
    std::remove(dateiname);
}