# Quiz-App (mit C in der  Konsole)

Konsolenanwendung zum Abfragen und Üben von Wissen. Die Fragen werden aus
CSV-Dateien gelesen, sodass die App ohne Programmänderung für beliebige
Themen (Allgemeinwissen, Lehrveranstaltung, ...) genutzt werden kann.

## 1. Anforderungen

| Nr | Anforderung | Umsetzung |
|----|-------------|-----------|
| A1 | Fragensätze samt Thema aus CSV laden | `fragen.c` (`katalog_laden`) |
| A2 | Verfügbare Themen anzeigen und auswählen | `ui.c` (`ui_themen_anzeigen`, `ui_thema_waehlen`) |
| A3 | Single-Choice, genau eine richtige Antwort | Datentyp `Frage` (`richtig` = ein Index 0-3) |
| A4 | Quizrunde mit zufälligen Fragen, Ergebnis anzeigen | `quiz.c`, `ui_ergebnis_anzeigen` |
| A5 | Ergebnisse dauerhaft speichern, Übersicht anzeigen | `ergebnisse.c`, Menüpunkt 3 |
| A6 | Ungültige Eingaben, fehlende/fehlerhafte Dateien behandeln | Eingabeprüfung in `ui.c`, Statuscodes + Meldungen in `fragen.c`/`ergebnisse.c` |
| A7 | Trennung Datenhaltung / Spiellogik / Oberfläche | siehe Architektur |

## 2. Architektur

```
include/
    quiz.h          Schnittstelle der Spiellogik
    ui.h            Schnittstelle der Benutzeroberfläche
    import.h        Schnittstelle zum Einlesen der Fragen
    data.h          Schnittstelle zur Datenverwaltung

src/
    quiz.c          Spiellogik: Quizrunden, zufällige Fragenauswahl und Auswertung
    ui.c            Benutzeroberfläche: Ein- und Ausgabe sowie Prüfung der Benutzereingaben
    import.c        Einlesen und Validieren der Fragen aus der CSV-Datei
    data.c          Datenverwaltung: Speichern und Laden der Quiz-Ergebnisse
    main.c          Ablaufsteuerung: Startet das Programm und verbindet die einzelnen Module
```

## 3. CSV Format :

thema;frage;antwort_a;antwort_b;antwort_c;antwort_d;richtige_antwort

| Index | CSV-Feld          | Struct-Zuordnung             |
|-------|-------------------|------------------------------|
| 0     | thema             | `Fragensatz.thema`           |
| 1     | frage             | `Frage.frage`                 |
| 2     | antwort_a         | `Frage.antworten[0]`          |
| 3     | antwort_b         | `Frage.antworten[1]`          |
| 4     | antwort_c         | `Frage.antworten[2]`          |
| 5     | antwort_d         | `Frage.antworten[3]`          |
| 6     | richtige_antwort  | `Frage.richtige_antwort`      |

#### Weitere Regeln für die CSV :

* Jede Frage muss genau 7 Felder enthalten.
* Alle Felder müssen ausgefüllt sein.
* Die richtige Antwort wird mit A, B, C, D oder 1, 2, 3, 4 angegeben.
* Das Thema bestimmt, zu welchem Fragensatz eine Frage gehört.
* Kommentare können mit # beginnen.
* Die erste Zeile enthält die Spaltenüberschriften und wird beim Import übersprungen.
* Die Antworten dürfen kein Semikolon enthalten, da dieses als Trennzeichen verwendet wird.

## 4.Datenstrukturen
Frage = eine einzelne Frage

Fragensatz = mehrere Fragen zu einem Thema

Fragensammlung = mehrere Fragensätze
 
Fragensammlung -> Fragensatz -> Frage

## 5.Unit tests Aufbau

( "Testfall", "[Funktionsname]")