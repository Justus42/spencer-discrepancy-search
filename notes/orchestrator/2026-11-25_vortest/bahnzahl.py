# Erwartete Bahnzahlen (Burnside/Moebius) fuer Z_q auf c Zyklen der Laenge q plus f Fixspalten, Paare {w,~w}
from math import comb
def mu(n):
    r=1;p=2
    while p*p<=n:
        if n%p==0:
            n//=p
            if n%p==0: return 0
            r=-r
        p+=1
    return -r if n>1 else r
def divs(n): return [d for d in range(1,n+1) if n%d==0]
def P(c,q,f,d): return (2**(c*d+f) + (2**(c*d) if (f==0 and (q//d)%2==0) else 0))//2
def bahnen(c,q,f): return {s: sum(mu(s//d)*P(c,q,f,d) for d in divs(s))//s for s in divs(q)}
def V(m,R): return sum(comb(m,k) for k in range(R+1))
assert bahnen(2,10,0)=={1:4,2:2,5:204,10:52326} and bahnen(2,7,1)=={1:4,7:2340}
for (m,R,B,c,q,f) in [(25,8,25,2,11,3),(25,8,25,2,12,1),(23,7,25,2,11,1),(21,6,25,2,10,1),(30,10,31,2,13,4),(30,10,31,2,14,2),(30,10,31,2,15,0),(28,9,31,2,13,2),(26,8,31,2,13,0),(26,8,31,2,11,4),(24,7,31,2,11,2),(22,6,31,2,11,0)]:
    b=bahnen(c,q,f)
    print(f"({m},{R}) {q}+{q}+{f}: Bahnen {b}, hoechstens {B} Paare, 2V/2^m={2*V(m,R)/2**m:.4f}, Dichte bei {B} Paaren {2*B*V(m,R)/2**m:.3f}")
