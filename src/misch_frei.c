/* misch_frei.c -- SUCHCODE Auftrag M (26.11.2026), Schritt 1: exakte Entscheidung "reichen f freie Zeilen?"
 *
 * Gegeben: eine Zeilenmenge (Matrix, oder Paar (U|W) mit Zeilen (U_i,W_i), (U_i,-W_i)) und d.
 * Punkt x in {+1,-1}^m ist ueberdeckt, wenn |<Zeile,x>| >= d, d.h. Abstand <= R oder >= m-R mit R = floor((m-d)/2).
 * Offene Punkte kommen in Paaren {x,-x} (Klassen). Gesucht: f Zeilen r_1..r_f, die alle offenen Klassen ueberdecken.
 *
 * Vollstaendigkeit (f beliebig, rekursiv): Ist S die Menge der noch offenen Klassen und c0 in S, so muss eine der
 * f Zeilen c0 ueberdecken. Ohne Einschraenkung ist es die erste, und da r und -r dieselben Punkte ueberdecken,
 * ohne Einschraenkung Abstand(r_1, c0) <= R. Also genuegt r_1 aus der Kugel B(c0,R) (alle Elemente durchgehen) und
 * Rekursion auf S ohne die von r_1 ueberdeckten Klassen mit f-1. Schnitt nur durch eine gueltige Schranke:
 * |S| <= f * maxcov, maxcov = groesste Zahl offener Klassen, die EINE Zeile ueberdeckt (exakt ueber alle Zeilen berechnet).
 * f = 1 wird zusaetzlich vollstaendig gezaehlt (alle 2^m Zeilen).
 *
 * Aufrufe:
 *   misch_frei matrix DATEI d FMAX [--zeigen]
 *   misch_frei paar   DATEI d FMAX [--streiche i] [--zeigen]     (i = 0-basierte Paarzeile, die wegfaellt)
 *   misch_frei punkte DATEI m R FMAX [--zeigen]                  (DATEI: offene Punkte als +-1-Zeilen, Tests)
 * Ausgabe: offene Punkte, maxcov (in Punkten), f=1: Zahl der Zeilen (alle 2^m), je f <= FMAX: JA/NEIN (+ Loesung).
 * Bauen: gcc -O3 -march=native -o build/misch_frei src/misch_frei.c
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef uint32_t u32;
static int m, R;
static u32 FULL, TOP;
static u32 *ball; static long nball;     /* alle x mit |x| <= R */
static u32 *O; static long nO;           /* offene Klassen (kanonisch: Bit m-1 = 0) */
static int64_t knoten;

static double jetzt(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + 1e-9 * t.tv_nsec; }
static inline u32 canon(u32 x) { return (x & TOP) ? (x ^ FULL) : x; }
static inline int deckt(u32 r, u32 c) { int p = __builtin_popcount(r ^ c); return p <= R || p >= m - R; }

static int lies_vektoren(const char *pfad, u32 *aus, int *mm, int max, int paar, int *mtrenn) {
    FILE *f = fopen(pfad, "r"); if (!f) { perror(pfad); exit(2); }
    char zeile[4096]; int n = 0;
    while (fgets(zeile, sizeof zeile, f)) {
        char *h = strchr(zeile, '#'); if (h) *h = 0;
        u32 v = 0; int j = 0, trenn = -1; char *p = zeile;
        for (;;) {
            while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
            if (!*p) break;
            if (*p == '|') { trenn = j; p++; continue; }
            char *e; long w = strtol(p, &e, 10); if (e == p) { fprintf(stderr, "%s: Lesefehler\n", pfad); exit(2); }
            if (w != 1 && w != -1) { fprintf(stderr, "%s: Eintrag nicht +-1\n", pfad); exit(2); }
            if (w == -1) v |= 1u << j;
            j++; p = e;
        }
        if (j == 0) continue;
        if (*mm == 0) *mm = j; else if (*mm != j) { fprintf(stderr, "%s: ungleiche Zeilenlaengen\n", pfad); exit(2); }
        if (paar) { if (trenn < 0) { fprintf(stderr, "%s: Trennstrich fehlt\n", pfad); exit(2); } *mtrenn = trenn; }
        if (n >= max) { fprintf(stderr, "zu viele Zeilen\n"); exit(2); }
        aus[n++] = v;
    }
    fclose(f);
    return n;
}

/* Rekursion: S (Laenge n) offene Klassen, f Zeilen frei. Loesung in sol[0..]. */
static long maxcov;
static int loese(const u32 *S, long n, int f, u32 *sol, u32 *puffer) {
    knoten++;
    if (n == 0) { for (int q = 0; q < f; q++) sol[q] = 0; return 1; }
    if (f == 0) return 0;
    if (n > (long)f * maxcov) return 0;
    u32 c0 = S[0];
    u32 *S2 = puffer; u32 *rest = puffer + n;
    for (long t = 0; t < nball; t++) {
        u32 r = c0 ^ ball[t];
        if (f == 1) {
            long q = 1; while (q < n && deckt(r, S[q])) q++;
            if (q == n) { sol[0] = r; return 1; }
            continue;
        }
        long k = 0;
        for (long q = 1; q < n; q++) if (!deckt(r, S[q])) S2[k++] = S[q];
        if (k > (long)(f - 1) * maxcov) continue;
        if (loese(S2, k, f - 1, sol + 1, rest)) { sol[0] = r; return 1; }
    }
    return 0;
}

static void zeige(u32 r) { for (int j = 0; j < m; j++) printf(" %2d", (r >> j & 1) ? -1 : 1); }

int main(int argc, char **argv) {
    if (argc < 5) { fprintf(stderr, "Aufruf: siehe Kopf von src/misch_frei.c\n"); return 2; }
    const char *modus = argv[1], *pfad = argv[2];
    int fmax, zeigen = 0, streiche = -1, d = -1;
    for (int a = 1; a < argc; a++) { if (!strcmp(argv[a], "--zeigen")) zeigen = 1; if (!strcmp(argv[a], "--streiche") && a + 1 < argc) streiche = atoi(argv[a + 1]); }
    double t0 = jetzt();
    static u32 Z[4096];
    int nz = 0;
    m = 0;
    if (!strcmp(modus, "punkte")) {
        m = atoi(argv[3]); R = atoi(argv[4]); fmax = atoi(argv[5]);
        int mm = 0; static u32 P[1 << 16]; int np = lies_vektoren(pfad, P, &mm, 1 << 16, 0, NULL);
        if (mm != m) { fprintf(stderr, "Laenge passt nicht\n"); return 2; }
        FULL = (m == 32) ? 0xffffffffu : ((1u << m) - 1); TOP = 1u << (m - 1);
        char *gesehen = calloc(TOP, 1); O = malloc(sizeof(u32) * (np + 1)); nO = 0;
        for (int i = 0; i < np; i++) { u32 c = canon(P[i]); if (!gesehen[c]) { gesehen[c] = 1; O[nO++] = c; } }
        free(gesehen);
        printf("# punkte %s: m=%d R=%d, %ld offene Klassen (%ld Punkte)\n", pfad, m, R, nO, 2 * nO);
    } else {
        d = atoi(argv[3]); fmax = atoi(argv[4]);
        if (!strcmp(modus, "matrix")) nz = lies_vektoren(pfad, Z, &m, 4096, 0, NULL);
        else if (!strcmp(modus, "paar")) {
            static u32 P[2048]; int mt = 0;
            int k = lies_vektoren(pfad, P, &m, 2048, 1, &mt);
            u32 MW = ((m == 32) ? 0xffffffffu : ((1u << m) - 1)) & ~((1u << mt) - 1);   /* Spalten mt..m-1 = W */
            for (int i = 0; i < k; i++) { if (i == streiche) continue; Z[nz++] = P[i]; Z[nz++] = P[i] ^ MW; }
            printf("# paar %s: K=%d (Typ (%d,%d))%s, %d Zeilen\n", pfad, k - (streiche >= 0), mt, m - mt, streiche >= 0 ? " nach Streichen" : "", nz);
            if (streiche >= 0) printf("# gestrichen: Paarzeile %d\n", streiche);
        } else { fprintf(stderr, "unbekannter Modus\n"); return 2; }
        if (m < 4 || m > 30) { fprintf(stderr, "m ausserhalb\n"); return 2; }
        R = (m - d) / 2; if (m - d < 0) R = -1;
        FULL = (1u << m) - 1; TOP = 1u << (m - 1);
        O = malloc(sizeof(u32) * TOP); nO = 0;
        for (u32 c = 0; c < TOP; c++) { int ok = 0; for (int i = 0; i < nz && !ok; i++) ok = deckt(Z[i], c); if (!ok) O[nO++] = c; }
        printf("# m=%d d=%d R=%d, %d Zeilen; offene Punkte %ld (Klassen %ld)\n", m, d, R, nz, 2 * nO, nO);
    }
    nball = 0; for (u32 x = 0; x <= FULL; x++) if (__builtin_popcount(x) <= R) nball++;
    ball = malloc(sizeof(u32) * nball); { long q = 0; for (u32 x = 0; x <= FULL; x++) if (__builtin_popcount(x) <= R) ball[q++] = x; }
    /* maxcov und f=1 vollstaendig: Zaehler je Zeilenklasse ueber die Kugeln um die offenen Klassen */
    uint32_t *cnt = calloc(TOP, sizeof(uint32_t));
    for (long i = 0; i < nO; i++) for (long t = 0; t < nball; t++) cnt[canon(O[i] ^ ball[t])]++;
    maxcov = 0; long n1 = 0; u32 bester = 0;
    for (u32 r = 0; r < TOP; r++) { if ((long)cnt[r] > maxcov) { maxcov = cnt[r]; bester = r; } if ((long)cnt[r] == nO) n1++; }
    printf("maxcov: eine Zeile ueberdeckt hoechstens %ld offene Punkte (%ld Klassen) von %ld; Beispiel:", 2 * maxcov, maxcov, 2 * nO); zeige(bester); printf("\n");
    printf("f=1: Zeilen r (alle 2^%d), die alle offenen Punkte ueberdecken: %ld (bis auf Vorzeichen %ld)\n", m, 2 * n1, n1);
    if (nO == 0) { printf("keine offenen Punkte\n"); return 0; }
    u32 sol[64]; u32 *puffer = malloc(sizeof(u32) * (nO + 1) * (fmax + 2));
    for (int f = 1; f <= fmax; f++) {
        knoten = 0; double t1 = jetzt();
        int ja = loese(O, nO, f, sol, puffer);
        printf("f=%d: %s (Knoten %lld, %.2f s)\n", f, ja ? "JA" : "NEIN (vollstaendig)", (long long)knoten, jetzt() - t1);
        if (ja && zeigen) for (int q = 0; q < f; q++) { printf("  frei:"); zeige(sol[q]); printf("\n"); }
        if (ja) {   /* Selbstkontrolle der Loesung */
            long offen = 0; for (long i = 0; i < nO; i++) { int ok = 0; for (int q = 0; q < f; q++) ok |= deckt(sol[q], O[i]); if (!ok) offen++; }
            if (offen) { printf("FEHLER: Loesung laesst %ld Klassen offen\n", offen); return 3; }
            break;
        }
        if (f == 1 && n1 > 0) { printf("FEHLER: f=1 Zaehlung %ld, Rekursion NEIN\n", n1); return 3; }
    }
    printf("# Laufzeit %.2f s\n", jetzt() - t0);
    return 0;
}
