#!/usr/bin/env python3
"""Auftrag R9: prueft eine .fortschritt-Datei: Bereiche 0..N lueckenlos, ohne Ueberlappung, alle OHNE_LOESUNG.
Aufruf: pruefe_rang9.py runs/spann_rang9/NAME.fortschritt   (Ausgabe: Zahl der Aufgaben, Summe Knoten, Sekunden, Urteil)
Zeilen mit ZEITLIMIT zaehlen nicht als gerechnet, Zeilen mit LOESUNG machen den Fall unentschieden/geloest. Nur ganze Zahlen."""
import sys
n = None; ok = []; ls = False; zl = 0
for z in open(sys.argv[1]):
    t = z.split()
    if not t: continue
    if t[0] == '#':
        if t[1] == 'AUFGABEN': n = int(t[2])
        continue
    v, b, kn, sek, er = int(t[0]), int(t[1]), int(t[2]), int(t[3]), t[4]
    if er == 'OHNE_LOESUNG': ok.append((v, b, kn, sek))
    elif er == 'LOESUNG': ls = True
    elif er == 'ZEITLIMIT': zl += 1
ok.sort()
pos = 0; kn = 0; sek = 0; luecken = []; ueber = []
for v, b, k, s in ok:
    if v > pos: luecken.append((pos, v))
    if v < pos: ueber.append((v, pos))
    pos = max(pos, b); kn += k; sek += s
if n is not None and pos < n: luecken.append((pos, n))
fertig = n is not None and not luecken and not ueber and not ls
print(f"{sys.argv[1]}: Aufgaben {n}, bis {pos} gerechnet, Bereiche {len(ok)}, Knoten {kn}, Sekunden {sek}, "
      f"Zeitlimit-Wiederholungen {zl}, Luecken {luecken}, Ueberlappungen {ueber}, Loesung {ls}")
print("ENTSCHIEDEN: keine Loesung (lueckenlos, vollstaendig)" if fertig else "NICHT ENTSCHIEDEN")
