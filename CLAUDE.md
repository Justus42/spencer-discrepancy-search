# Arbeitsregeln für dieses Repo

Lies zuerst `STATUS.md`. Dort stehen Ziel, Stand, Fehlversuche und das nächste Gate.

## Umgebung

Windows-PC: Ryzen 7 9800X3D (8 Kerne), 32 GB RAM, Nvidia RTX 5080 (16 GB), 4-TB-SSD.
Der Nutzer ist kein Programmierer. Erkläre jeden Schritt in einem Satz und frage, bevor du
etwas außerhalb dieses Ordners installierst oder veränderst.

## Regeln

1. Ein Ergebnis gilt erst, wenn `src/check_disc.py` und `verifiers/verify_spencer.py`
   exakt dieselbe Diskrepanz melden. Die beiden Skripte teilen keinen Code und bleiben getrennt.
2. Vor jeder Suche auf neuen Parametern: eine bekannte Matrix aus `controls/` derselben
   Größenordnung reproduzieren.
3. Suchcode und Prüfcode nie vermischen. Prüfskripte rechnen nur mit ganzen Zahlen oder Brüchen.
4. Lange Läufe nach `runs/` protokollieren.
   - Lokal auf dem PC: im Hintergrund starten, nicht laufend abfragen, erst auf Zuruf auswerten.
   - In einer Cloud-Sitzung: Der Container kann jederzeit neu starten, laufende Prozesse und
     nicht gepushte Dateien gehen dann verloren (so geschehen am 23.11.2026, zweimal). Deshalb
     in Blöcken von höchstens 30 Minuten rechnen, im Vordergrund warten, Logs laufend
     schreiben, nach jedem Block `STATUS.md` aktualisieren, committen und pushen. Am Ende ohne
     Zuruf auswerten. Während ein Block mit Zeitlimit läuft, nichts anderes rechnen.
5. Jeden Fehlversuch mit Parametern und Laufzeit in `STATUS.md` unter „Fehlversuche“ eintragen.
6. Auf einem eigenen Branch arbeiten und nur auf Anweisung nach `main` zusammenführen.
7. Nichts nach außen melden (keine Pull Requests an fremde Repos, keine Posts).

Sprache: Deutsch, kurz.
