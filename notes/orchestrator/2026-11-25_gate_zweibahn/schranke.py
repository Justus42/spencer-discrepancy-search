# Gate Z, unabhaengiger Weg: Zaehlschranke. Wie viele harte Punkte kann EIN Wort hoechstens ueberdecken?
# Hart: Punkt x = (x1, x2, Fixteil), den kein festes Wort erreicht, d. h. falt(|x1|) + falt(|x2|) > R  (falt(w) = min(w, q-w)).
# Ein Wort w = (a, b, e) mit Blockgewichten s, t ueberdeckt x, wenn d(x, w) <= R oder >= m - R.
# Die Zahl ueberdeckter harter Punkte haengt nur von (s, t) ab. Reichen 2q Paare (zwei volle Bahnen) rechnerisch nicht fuer alle harten Punkte,
# dann ueberdecken zwei volle Bahnen die harten Punkte nie (ganz ohne Aufzaehlung).
from math import comb
def tab(q, s):
    # T[i][d] = Zahl der x1 mit Gewicht i und Abstand d zu einem festen Wort a vom Gewicht s
    T = [[0]*(q+1) for _ in range(q+1)]
    for u in range(s+1):
        for v in range(q-s+1):
            T[s-u+v][u+v] += comb(s, u)*comb(q-s, v)
    return T
def fall(q, f, R):
    m = 2*q+f; falt = lambda w: min(w, q-w)
    hart = [(i, j) for i in range(q+1) for j in range(q+1) if falt(i)+falt(j) > R]
    nh = sum(comb(q, i)*comb(q, j) for i, j in hart) * 2**f
    best = 0
    for s in range(q+1):
        Ts = tab(q, s)
        for t in range(q+1):
            Tt = tab(q, t); c = 0
            for i, j in hart:
                for d1 in range(q+1):
                    if not Ts[i][d1]: continue
                    for d2 in range(q+1):
                        if not Tt[j][d2]: continue
                        for d3 in range(f+1):
                            d = d1+d2+d3
                            if d <= R or d >= m-R: c += Ts[i][d1]*Tt[j][d2]*comb(f, d3)
            best = max(best, c)
    kap = 2*q*best   # 2q Paare; "best" zaehlt schon Wort und Komplement (d <= R oder >= m-R)
    print(f"({m},{R}) q={q} f={f}: harte Punkte {nh} (Klassen bei freier Wirkung {nh//(2*q)}), ein Paar ueberdeckt hoechstens {best}, zwei volle Bahnen hoechstens {kap}: {'REICHT NICHT -> ausgeschlossen' if kap < nh else 'Schranke entscheidet nicht'} (Verhaeltnis {kap/nh:.3f})")
for q, f, R in [(7,1,4),(7,3,5),(9,2,6),(11,1,7),(11,3,8),(11,4,8),(13,0,8),(13,2,9),(13,4,10)]:
    fall(q, f, R)
