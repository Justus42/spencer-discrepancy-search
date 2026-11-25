#!/usr/bin/env python3
"""Gate R9, Punkt 1: eigene Auswertung der Roh-Logs (ohne Code der Sitzung).
Liest je Fall runs/spann_rang9/<name>.log.txt, sammelt alle BEREICH-Zeilen samt der davor stehenden
Problem-Zeile, und prüft: richtige Problemdatei, k=15, Symmetrie ja, Aufgabenzahl konstant und gleich
dem Altlog aus Auftrag S, Bereiche OHNE_LOESUNG decken 0..N lückenlos und ohne Überlappung,
Knotensumme, Abgleich mit der Fortschrittsdatei."""
import re, sys, os
R = "runs/spann_rang9"
FAELLE = ["l111_r9_y384", "l111_r9_y128", "l111_r9_y129", "ctrl16_r9_y7", "l111_r9_y385", "ctrl16_r9_y40"]
gesamt_k = gesamt_a = gesamt_s = gesamt_b = 0
ok_alle = True
for name in FAELLE:
    zeilen = open(f"{R}/{name}.log.txt").read().splitlines()
    kopf = None; laufzeit = None; bereiche = []; fehler = []
    for z in zeilen:
        if z.startswith("Problem "):
            kopf = z
        m = re.match(r"Knoten (\d+), Laufzeit ([\d.]+) s", z)
        if m: laufzeit = float(m.group(2)); kn_zeile = int(m.group(1))
        m = re.match(r"BEREICH (\d+) (\d+) von (\d+) Aufgaben insgesamt: Knoten (\d+), (\w+)", z)
        if m:
            von, bis, n, kn, erg = int(m.group(1)), int(m.group(2)), int(m.group(3)), int(m.group(4)), m.group(5)
            if f"probleme/{name}.txt" not in kopf: fehler.append("falsche Problemdatei")
            if " k=15," not in kopf: fehler.append("k != 15")
            if "Symmetrie ja (12 Erzeuger)" not in kopf: fehler.append("Symmetrie fehlt")
            if kn != kn_zeile: fehler.append("Knoten-Zeilen ungleich")
            bereiche.append((von, bis, n, kn, erg, laufzeit))
    ns = {b[2] for b in bereiche}
    voll = sorted((b for b in bereiche if b[4] == "OHNE_LOESUNG" and b[1] > b[0]))
    andere = [b for b in bereiche if b[4] != "OHNE_LOESUNG"]
    pos = 0; luecke = False; ueberl = False
    for b in voll:
        if b[0] > pos: luecke = True
        if b[0] < pos: ueberl = True
        pos = max(pos, b[1])
    N = ns.pop() if len(ns) == 1 else None
    kn = sum(b[3] for b in voll); sek = sum(b[5] for b in voll)
    # Altlog aus Auftrag S
    alt = open(f"runs/spann/teil2b_{name}_k15.log.txt").read()
    n_alt = int(re.search(r"Aufgaben: (\d+)", alt).group(1))
    # Fortschrittsdatei
    fs = [l.split() for l in open(f"{R}/{name}.fortschritt") if not l.startswith("#")]
    fs_t = sorted((int(a), int(b), int(c), e) for a, b, c, d, e in fs)
    gleich_fs = fs_t == [(b[0], b[1], b[3], b[4]) for b in voll]
    gut = (not fehler and not andere and not luecke and not ueberl and N is not None and pos == N and N == n_alt and gleich_fs)
    ok_alle &= gut
    print(f"{name}: Aufgaben {N} (Altlog S: {n_alt}), Bereiche {len(voll)}, gedeckt 0..{pos}, Lücke {luecke}, Überlappung {ueberl}, "
          f"andere Ergebnisse {len(andere)}, Knoten {kn}, Sekunden {sek:.0f}, Fortschrittsdatei gleich {gleich_fs}, Fehler {fehler} -> {'OK' if gut else 'NICHT OK'}")
    gesamt_k += kn; gesamt_a += N or 0; gesamt_s += sek; gesamt_b += len(voll)
print(f"Summe: Aufgaben {gesamt_a}, Bereiche {gesamt_b}, Knoten {gesamt_k}, Sekunden {gesamt_s:.0f} -> {'ALLE OK' if ok_alle else 'NICHT OK'}")
