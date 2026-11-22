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
include/            src/
quiz.h        <-->  quiz.c         Spiellogik: Runde, Zufall, Auswertung
ui.h          <-->  ui.c           Oberfläche: Ein-/Ausgabe, Eingabeprüfung
                    main.c         Ablaufsteuerung (verbindet die Schichten)
```
## CSV Format :

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

## Datenstrukturen 
Frage = eine einzelne Frage

Fragensatz = mehrere Fragen zu einem Thema

Fragensammlung = mehrere Fragensätze
 
Fragensammlung -> Fragensatz -> Frage

## Unit tests Aufbau
( "Testfall", "[Funktionsname]")