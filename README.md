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
