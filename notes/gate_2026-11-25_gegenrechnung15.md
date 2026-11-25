# Gates vom 25.11.2026 früh: Auftrag C (Gegenrechnung (15,4)) und Rest von Auftrag A

Prüfer: Orchestrator-Claude. Beide Gates bestanden. Gemergt mit festgenageltem Stand: PR 26 (`07076bb`, normal),
PR 27 (`8edcb40`, Squash). `main` danach `0fe18f9`.

## Auftrag C (PR 26, Protokoll `notes/laeufe/2026-11-25_gegenrechnung15.md`)

| Nr. | Prüfung | Ergebnis |
|---|---|---|
| 1 | Zahlen der Sitzung gegen die Vorrechnung des Orchestrators vom 24.11. (`notes/orchestrator/2026-11-24_abend/`) | alle gleich: 2344 Punktklassen, 2340 volle Bahnen, 4 feste Zeilen; 112 Lösungen mit 32 Wörtern (4 Paare fester Zeilen je 28, 2 Paare 0); 0 mit 30 und 28; kleinste Restzahl 466 (je 56 Bahnpaare); Ordnung 11 und 13: 0; `ctrl_16` ohne eine Zeile: 466. Zwei unabhängige Programme |
| 2 | Neue Aussage „112 Lösungen = 2 Klassen zu je 56, eine davon `ctrl_16`“ | mit eigenem Code bestätigt (`notes/orchestrator/2026-11-25_gate_gegenrechnung15/gate_c.py`, bliss): 2 Klassen, je 56, beide mit Gruppenordnung 5376 |
| 3 | Zusatz: Hadamard-Klassen | In beiden Klassen haben je zwei Zeilen Skalarprodukt ±1; beide lassen sich zu Hadamard-Matrizen der Ordnung 16 ergänzen. Die zwei Matrizen sind nicht Hadamard-äquivalent, die eine ist äquivalent zur Transponierten der anderen, keine zu ihrer eigenen Transponierten, keine zu Sylvester. Rechnung des Orchestrators, nicht gegengerechnet |
| 4 | Änderungen an `STATUS.md`, `LANDKARTE.md`; Dateigrößen | stimmig; keine großen Dateien |

Einordnung zu Nr. 3: Unter den fünf Klassen der Ordnung 16 gibt es genau ein Paar, das durch Transponieren vertauscht wird
(Literatur aus dem Gedächtnis, unbelegt); EinsteinArena-Thread 273 nennt für den 17x17-Zeugen `had.16.3`/`had.16.4`.
Welche unserer beiden Klassen welchen Namen trägt, ist nicht geprüft.

## Rest von Auftrag A (PR 27, Protokoll `notes/laeufe/2026-11-25_einzelnull.md`)

- Umkodierung für n = 11 von der Sitzung nachgerechnet (5632 Zeilen, 0 Abweichungen, 512 voll komplementär): gleiche
  Zahlen wie beim Orchestrator.
- Profilsuche gemischte Träger (10, 8, 6): Profile (5,0), (4,0), (3,1), (2,1), (1,2), (0,2) vollständig ohne Lösung;
  (3,0), (2,0), (1,1), (1,0), (0,1) offen. **Nicht unabhängig geprüft:** ein Programm, keine Positivkontrolle, in der
  eine Lösung kleine Träger braucht. Die Frage „11 x 11 mit Einträgen 0, ±1 und Diskrepanz 6“ bleibt unentschieden.
- Geprüft am Stand `8edcb40`: keine Konfliktmarker, keine großen Dateien im Baum, STATUS und LANDKARTE stimmig.
- Der Branch `ccr-6615c26c-zvmbps` ließ sich vom Orchestrator nicht löschen (Schreibweg gesperrt). Er enthält in seiner
  Historie etwa 86 MB Fortsetzungsdateien und sollte von Justus gelöscht werden.
