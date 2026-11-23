# spencer-discrepancy-search

Suche nach einer besseren unteren Schranke für die Spencer-Diskrepanzkonstante
(„six standard deviations suffice“, Konstante C_10c in
[teorth/optimizationproblems](https://github.com/teorth/optimizationproblems)),
über selbstkomplementäre binäre Überdeckungscodes.

Bisher enthält dieses Repo **kein neues Resultat**, nur Prüfskripte, Kontrollmatrizen
bekannter Rekorde und Suchcode. Der aktuelle Stand steht in [STATUS.md](STATUS.md).

## Prüfen

```
python src/check_disc.py controls/ctrl_8.txt
python verifiers/verify_spencer.py --selftest controls/ctrl_8.txt
```

## KI-Einsatz

Code und Texte wurden mit KI-Modellen erstellt (Claude, Gemini, GPT). Jede Matrix in
`controls/` ist von zwei unabhängig geschriebenen Prüfskripten exakt nachgerechnet.
