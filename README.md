# spencer-discrepancy-search

Suche nach einer besseren unteren Schranke für die Spencer-Diskrepanzkonstante
(„six standard deviations suffice“, Konstante C_10c in
[teorth/optimizationproblems](https://github.com/teorth/optimizationproblems)),
über selbstkomplementäre binäre Überdeckungscodes.

Stand 26.11.2026: Dieses Repo enthält **keinen neuen Rekord** für die Spencer-Konstante
(bester bekannter Wert 5/√8, nicht von hier). Als Zwischenergebnisse auf dem Weg dorthin gibt es
obere Schranken für sechs Zellen binärer Überdeckungscodes, kleiner als die Post-Kéri-Tabelle
([florath/covering-codes-lean](https://github.com/florath/covering-codes-lean), Commit `bbed9a6`):

- K(20,6) ≤ 48 (Konstruktion ohne Suche, in Lean bewiesen), daraus K(22,7), K(24,8), K(26,9)
  und K(28,10) ≤ 48 (Verdreifachen einer normalen Koordinate); Tabelle 64 bzw. 56.
- K(18,5) ≤ 50 (aus einer Suche, Überdeckung in Lean bewiesen); Tabelle 64.

Diese Werte sind von Dritten nicht geprüft und bisher nirgends gemeldet. Die Literaturprüfung ist
unvollständig, für die vier Folgezellen gibt es nur den Tabellenabgleich. Für einen Rekord reicht
keiner dieser Codes. Stand, Belege und offene Punkte stehen in [STATUS.md](STATUS.md), die
Lean-Beweise in [lean/README.md](lean/README.md).

## Prüfen

```
python src/check_disc.py controls/ctrl_8.txt
python verifiers/verify_spencer.py --selftest controls/ctrl_8.txt
```

## KI-Einsatz

Code und Texte wurden mit KI-Modellen erstellt (Claude, Gemini, GPT). Jede Matrix in
`controls/` ist von zwei unabhängig geschriebenen Prüfskripten exakt nachgerechnet.
