#!/usr/bin/env python3
"""
pruef_kern.py -- Pruefcode (Auftrag P, Teil 1). Nur ganze Zahlen, keine Gemeinsamkeit mit src/pruef_ideen/kern_r3.c.

Prueft Zertifikate der Form
    INDEX a b c .. | LOCHZAHL | KERNGROESSE | h1 h2 ..
zu einer +-1-Matrix (die ersten 15 Spalten zaehlen), r gestrichenen Zeilen (a b c ..) und k Ersatzzeilen:

 (1) Jede Klasse h (14-Bit-Vertreter, Bit 14 = 0) ist ein Loch: fuer jede behaltene Zeile r gilt |<r,x>| <= 5.
 (2) Aufzaehlung ALLER Verteilungen der Kernklassen auf k Zeilen mit Vorzeichen: je Zeile gilt fuer zwei Klassen mit
     Vertreterabstand D <= 6 gleiches, D >= 9 verschiedenes Vorzeichen (D = 7, 8 frei). Keine Verteilung darf alle
     Bedingungen erfuellen. (Die Verteilung wird Klasse fuer Klasse aufgezaehlt: k Zeilen x 2 Vorzeichen je Klasse,
     Abbruch eines Zweigs erst bei verletzter Paarbedingung -- das ist die vollstaendige Aufzaehlung, mit Beschneidung.)
 (3) Lochzahl der Streichmenge unabhaengig neu berechnet und mit der Angabe verglichen.

Herleitung der Paarbedingung (fuer jede Ersatzzeile a in [-1,1]^15, Schwelle T = 5*sqrt(15/8) = 6,847):
 Aus a.x > T und a.y > T folgt a.(x+y) > 2T = 13,69; a.(x+y) <= 2*(15 - d(x,y)); also 15 - d >= 7, d <= 8.
 Zwei Klassenvertreter x, y (Abstand D) mit Orientierungen s, s' haben orientierten Abstand D (s = s') oder 15 - D (s != s').

Aufruf:
  pruef_kern.py MATRIX k ZERT.txt          Zertifikate pruefen, Lochzahlen vergleichen
  pruef_kern.py MATRIX k --kontrolle       Kontrollen (siehe unten)
"""
import sys
from itertools import combinations


def lies(fn):
    zeilen = []
    for line in open(fn):
        t = line.split()
        if not t:
            continue
        v = [int(x) for x in t]
        w = 0
        for j in range(15):
            if v[j] == -1:
                w |= 1 << j
            elif v[j] != 1:
                raise SystemExit("Eintrag nicht +-1")
        zeilen.append(w)
    return zeilen


def pop(x):
    return bin(x).count("1")


def ist_loch(x, behalten):
    for r in behalten:
        if abs(15 - 2 * pop(x ^ r)) > 5:
            return False
    return True


def alle_loecher(zeilen, gestrichen):
    behalten = [r for i, r in enumerate(zeilen) if i not in gestrichen]
    return [x for x in range(1 << 14) if ist_loch(x, behalten)]


def bedingung(x, y):
    """0 frei, 1 gleiches Vorzeichen, 2 verschiedenes"""
    D = pop(x ^ y)
    if D <= 6:
        return 1
    if D >= 9:
        return 2
    return 0


def zaehle_verteilungen(holes, k, erste_nur=False):
    """Zaehlt alle Zuweisungen (Zeile, Vorzeichen) je Klasse mit erfuellten Paarbedingungen.
    Wenn erste_nur: bricht bei der ersten ab. Rueckgabe (anzahl, beispiel)."""
    # Reihenfolge der Klassen ist beliebig (Aufzaehlung aller Verteilungen aendert sich nicht); wir ordnen so, dass
    # moeglichst frueh viele Paarbedingungen greifen. Die erste Klasse wird auf (Zeile 0, Vorzeichen 0) gesetzt: Jede
    # Verteilung geht durch Vertauschen der Zeilen und gemeinsames Umdrehen aller Vorzeichen einer Zeile in eine mit
    # dieser Eigenschaft ueber (die Paarbedingungen sind gegen beides unveraendert). Die Anzahl wird daher nur bis auf
    # diese Symmetrie gezaehlt; fuer die Frage "gibt es keine Verteilung" ist das gleichwertig.
    hs = list(holes)
    if len(hs) > 2:
        rest = hs[1:]
        ordnung = [hs[0]]
        while rest:
            beste = max(rest, key=lambda h: sum(1 for g in ordnung if bedingung(h, g)))
            ordnung.append(beste)
            rest.remove(beste)
        holes = ordnung
    n = len(holes)
    cond = [[bedingung(holes[i], holes[j]) if i != j else 0 for j in range(n)] for i in range(n)]
    zuw = [None] * n
    anz = 0
    bsp = None

    def rec(i):
        nonlocal anz, bsp
        if i == n:
            anz += 1
            if bsp is None:
                bsp = list(zuw)
            return erste_nur
        for p in range(k):
            for s in (0, 1):
                if i == 0 and (p, s) != (0, 0):
                    continue
                ok = True
                for j in range(i):
                    c = cond[i][j]
                    if c and zuw[j][0] == p:
                        if c == 1 and zuw[j][1] != s:
                            ok = False
                            break
                        if c == 2 and zuw[j][1] == s:
                            ok = False
                            break
                if ok:
                    zuw[i] = (p, s)
                    if rec(i + 1):
                        return True
        zuw[i] = None
        return False

    rec(0)
    return anz, bsp


def pruefe_zertifikate(zeilen, k, fn, shard=0, nshard=1):
    gut = schlecht = 0
    zl = 0
    lochzahl_abw = 0
    hist = {}
    for line in open(fn):
        if line.startswith("#") or not line.strip():
            continue
        zl += 1
        if (zl - 1) % nshard != shard:
            continue
        if zl % 100 == 0:
            print("  ... Zeile", zl, file=sys.stderr, flush=True)
        teile = [t.strip() for t in line.split("|")]
        if len(teile) != 4:
            print("Zeile nicht lesbar:", line[:80])
            schlecht += 1
            continue
        kopf = [int(t) for t in teile[0].split()]
        gestr = kopf[1:]
        nh_angabe = int(teile[1])
        kern = [int(t) for t in teile[3].split()] if teile[3].strip() and not teile[3].startswith("VERT") else None
        if kern is None:
            print("kein Zertifikat:", line.strip()[:80])
            schlecht += 1
            continue
        behalten = [r for i, r in enumerate(zeilen) if i not in gestr]
        ok = len(set(kern)) == len(kern) and all(0 <= h < (1 << 14) for h in kern)
        ok = ok and all(ist_loch(h, behalten) for h in kern)
        if ok:
            anz, _ = zaehle_verteilungen(kern, k, erste_nur=True)
            ok = (anz == 0)
        nh = len(alle_loecher(zeilen, gestr))
        if nh != nh_angabe:
            lochzahl_abw += 1
        hist[(nh, len(kern))] = hist.get((nh, len(kern)), 0) + 1
        if ok:
            gut += 1
        else:
            schlecht += 1
            print("ZERTIFIKAT FALSCH:", gestr, kern)
    print("Zertifikate bestaetigt: %d, nicht bestaetigt: %d, Lochzahl weicht von Angabe ab: %d" % (gut, schlecht, lochzahl_abw))
    for key in sorted(hist):
        print("  Loecher %d, Kerngroesse %d: %d Streichmengen" % (key[0], key[1], hist[key]))
    return schlecht == 0 and lochzahl_abw == 0


def kontrollen(zeilen):
    """Kontrolle 1: zwei gestrichene Zeilen, zwei Ersatzzeilen = die Originalzeilen: muss verteilbar sein.
       Zeugnis: Jede Lochklasse wird der Originalzeile mit |<r,x>| >= 7 zugeordnet, Vorzeichen = Vorzeichen des Skalarprodukts;
       alle Paarbedingungen werden direkt geprueft. Zusaetzlich findet die Aufzaehlung (auf Teilmengen) Verteilungen."""
    ok = True
    n1 = 0
    for gestr in combinations(range(len(zeilen)), 2):
        behalten = [r for i, r in enumerate(zeilen) if i not in gestr]
        holes = [x for x in range(1 << 14) if ist_loch(x, behalten)]
        orig = [zeilen[i] for i in gestr]
        zuw = {}
        for x in holes:
            gef = None
            for p, r in enumerate(orig):
                sp = 15 - 2 * pop(x ^ r)
                if abs(sp) >= 7:
                    gef = (p, 0 if sp > 0 else 1)
                    break
            zuw[x] = gef
        unabgedeckt = [x for x in holes if zuw[x] is None]
        # Paarbedingungen
        verletzt = 0
        for i, x in enumerate(holes):
            for y in holes[i + 1:]:
                c = bedingung(x, y)
                if c and zuw[x][0] == zuw[y][0]:
                    if (c == 1) != (zuw[x][1] == zuw[y][1]):
                        verletzt += 1
        if verletzt or unabgedeckt or gestr[0] == 0:
            print("Kontrolle 1: gestrichen %s: %d Loecher, ohne Originalzeile %d, verletzte Paarbedingungen %d" % (gestr, len(holes), len(unabgedeckt), verletzt))
        n1 += 1
        ok = ok and not unabgedeckt and verletzt == 0
        # Aufzaehlung muss auf Teilmengen eine Verteilung finden (jede Teilmenge der Loecher ist verteilbar)
        import random
        rnd = random.Random(7)
        for _ in range(20):
            tm = rnd.sample(holes, 12)
            anz, _ = zaehle_verteilungen(tm, 2, erste_nur=True)
            if anz == 0:
                print("  Aufzaehlung findet auf verteilbarer Teilmenge nichts!")
                ok = False
    print("Kontrolle 1: %d Streichpaare, Originalzeilen als Ersatz: alle Loecher abgedeckt, keine Paarbedingung verletzt: %s" % (n1, ok))
    # Kontrolle 2: zwei gestrichene Zeilen, eine Ersatzzeile: Zertifikat sollte existieren (Dreier unvertraeglicher Klassen genuegt)
    gestr = (0, 1)
    behalten = [r for i, r in enumerate(zeilen) if i not in gestr]
    holes = [x for x in range(1 << 14) if ist_loch(x, behalten)]
    gefunden = None
    for a, b, c in combinations(holes[:80], 3):
        anz, _ = zaehle_verteilungen([a, b, c], 1, erste_nur=True)
        if anz == 0:
            gefunden = (a, b, c)
            break
    print("Kontrolle 2: gestrichen (0,1), eine Ersatzzeile: Zertifikat", gefunden)
    ok = ok and gefunden is not None
    # Kontrolle 3: die Aufzaehlung erkennt ein bekanntes unverteilbares Beispiel und ein verteilbares
    x, y, z = 0, 0b11111, 0b1111111111  # D(x,y)=5 gleich, D(y,z)=5 gleich, D(x,z)=10 verschieden: auf einer Zeile unmoeglich
    a1, _ = zaehle_verteilungen([x, y, z], 1)
    a2, _ = zaehle_verteilungen([x, y, z], 2)
    a3, _ = zaehle_verteilungen([x, (1 << 14) - 1], 1)  # D = 14: verschieden, auf einer Zeile moeglich
    print("Kontrolle 3: Dreieck auf 1 Zeile %d (soll 0), auf 2 Zeilen %d (soll > 0), D=14 auf 1 Zeile %d (soll 1, gezaehlt bis auf Symmetrie)" % (a1, a2, a3))
    ok = ok and a1 == 0 and a2 > 0 and a3 == 1
    return ok


def negativkontrolle(zeilen, k, fn):
    """Verfaelschte Zertifikate muessen abgelehnt werden: (a) ein Element gestrichen (Kern ist minimal, danach verteilbar),
    (b) ein Element durch eine Nicht-Lochklasse ersetzt."""
    n_a = n_b = tot = 0
    for line in open(fn):
        if line.startswith("#") or not line.strip():
            continue
        teile = [t.strip() for t in line.split("|")]
        kopf = [int(t) for t in teile[0].split()]
        gestr = kopf[1:]
        kern = [int(t) for t in teile[3].split()]
        behalten = [r for i, r in enumerate(zeilen) if i not in gestr]
        tot += 1
        # (a) jedes Element einzeln streichen: mindestens eines der Teilzertifikate muss verteilbar sein (Minimalitaet)
        if any(zaehle_verteilungen(kern[:i] + kern[i + 1:], k, erste_nur=True)[0] > 0 for i in range(len(kern))):
            n_a += 1
        # (b) Nicht-Loch einsetzen
        nichtloch = next(x for x in range(1 << 14) if not ist_loch(x, behalten))
        if not all(ist_loch(h, behalten) for h in kern[:-1] + [nichtloch]):
            n_b += 1
    print("Negativkontrolle: %d Zertifikate; gestrichenes Element macht verteilbar: %d; eingesetzte Nicht-Loecher erkannt: %d" % (tot, n_a, n_b))
    return n_a == tot and n_b == tot


def main():
    zeilen = lies(sys.argv[1])
    k = int(sys.argv[2])
    if len(sys.argv) > 4 and sys.argv[4] == "--negativ":
        ok = negativkontrolle(zeilen, k, sys.argv[3])
        sys.exit(0 if ok else 1)
    if sys.argv[3] == "--kontrolle":
        ok = kontrollen(zeilen)
        print("KONTROLLEN:", "bestanden" if ok else "NICHT BESTANDEN")
        sys.exit(0 if ok else 1)
    sh = int(sys.argv[4]) if len(sys.argv) > 5 else 0
    nsh = int(sys.argv[5]) if len(sys.argv) > 5 else 1
    ok = pruefe_zertifikate(zeilen, k, sys.argv[3], sh, nsh)
    print("ERGEBNIS:", "alle bestaetigt" if ok else "ABWEICHUNG")
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
