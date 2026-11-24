"""Tabelle der 16 Familien (aus dem Auftrag des Nutzers vom 24.11.2026) und Lader für die Generatoren.
Kein Such- und kein Prüfcode. Gemeinsam benutzt von zaehle.py, sieb.py, pruefe.py.

Felder: Schlüssel -> dict(zeilen, spalten, anzahl, d, besser)
  d      = Spalten - 2R (Rekordschwelle): Gegenbeispiel = Vorzeichenvektor mit Maximum < d.
  besser = Angabe des Orchestrators (Auftrag, Punkt 4): ('hoechstens', L) = jede Matrix hat ein Gegenbeispiel mit Maximum <= L;
           ('best', B, k)        = beste Diskrepanz der Familie ist B (k Matrizen mit exakter Prüfung bei B).
"""
import importlib.util, os

HIER = os.path.dirname(os.path.abspath(__file__))

FAMILIEN = {
    "astra_1": dict(zeilen=25, spalten=23, anzahl=4356,   d=9,  besser=("best", 7, 12)),
    "astra_2": dict(zeilen=15, spalten=15, anzahl=59049,  d=7,  besser=("hoechstens", 3)),
    "astra_3": dict(zeilen=25, spalten=21, anzahl=1176,   d=9,  besser=("hoechstens", 5)),
    "astra_4": dict(zeilen=20, spalten=20, anzahl=1024,   d=8,  besser=("hoechstens", 4)),
    "astra_5": dict(zeilen=25, spalten=23, anzahl=2346,   d=9,  besser=("best", 7, 4)),
    "astra_6": dict(zeilen=20, spalten=18, anzahl=2160,   d=8,  besser=("best", 6, 1)),
    "astra_7": dict(zeilen=25, spalten=25, anzahl=1568,   d=9,  besser=("hoechstens", 5)),
    "astra_8": dict(zeilen=31, spalten=30, anzahl=1024,   d=10, besser=("hoechstens", 8)),
    "sol_1":   dict(zeilen=15, spalten=15, anzahl=153664, d=7,  besser=("best", 5, 50)),
    "sol_2":   dict(zeilen=20, spalten=20, anzahl=71280,  d=8,  besser=("hoechstens", 4)),
    "sol_3":   dict(zeilen=25, spalten=23, anzahl=8192,   d=9,  besser=("hoechstens", 5)),
    "sol_4":   dict(zeilen=25, spalten=25, anzahl=7920,   d=9,  besser=("best", 7, 8)),
    "sol_5":   dict(zeilen=25, spalten=25, anzahl=6144,   d=9,  besser=("hoechstens", 5)),
    "sol_6":   dict(zeilen=31, spalten=26, anzahl=40680,  d=10, besser=("hoechstens", 8)),
    "sol_7":   dict(zeilen=31, spalten=28, anzahl=17920,  d=10, besser=("hoechstens", 8)),
    "sol_8":   dict(zeilen=31, spalten=30, anzahl=10000,  d=10, besser=("hoechstens", 8)),
}

def lade(schluessel):
    """Importiert src/rundlauf/<schluessel>.py und gibt kandidaten (Generatorfunktion) zurück."""
    pfad = os.path.join(HIER, schluessel + ".py")
    spec = importlib.util.spec_from_file_location("gen_" + schluessel, pfad)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod.kandidaten

def stufe1(schluessel):
    """Ziel T1 (Maximum < T1) der ersten, billigen Stufe: unter dem Wert aus Auftrag Punkt 4."""
    b = FAMILIEN[schluessel]["besser"]
    return b[1] + 1 if b[0] == "hoechstens" else b[1]
