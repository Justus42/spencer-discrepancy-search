#!/usr/bin/env python3
"""Vortest Ideensammlung 25.11.2026 (Orchestrator): Zerlegung der Kern-<1,a>-Familie in zwei Haelften.
Ein Code mit Kern <1,a> (a = Einsen genau im rechten Block, Typ (m1,m2)) und K Nebenklassen ist ein Paar (U,W):
U ist K x m1, W ist K x m2, Woerter sind (U_i, W_i), (U_i, -W_i) und Komplemente (4K Woerter). Mit u = U y, v = W z gilt
    max ueber die 2K Zeilen von |<Zeile,(y,z)>| = max_i (|u_i| + |v_i|).
Ueberdeckung mit d  <=>  fuer alle y, z: max_i (|u_i| + |v_i|) >= d.
Haelt man U fest, ist die Bedingung an W klein: nur 2^(m2-1) Punkte z und die maximalen Anforderungsvektoren r(y) = d - |u(y)|.
Dieses Skript: (1) exakte Zaehlung offener (y,z)-Klassen, (2) CP-SAT-Orakel 'finde W zu festem U' (SUCHCODE),
(3) jede gefundene Loesung wird mit (1) nachgezaehlt. Nur ganze Zahlen.
Aufruf: python3 zerlegung.py MODUS [Sekunden]"""
import sys, itertools, time, numpy as np
from ortools.sat.python import cp_model

def paley1(q):
    sq={(x*x)%q for x in range(1,q)}; chi=lambda a: 0 if a%q==0 else (1 if a%q in sq else -1)
    n=q+1; S=np.zeros((n,n),dtype=np.int64); S[0,1:]=1; S[1:,0]=-1
    for i in range(q):
        for j in range(q): S[i+1,j+1]=chi(i-j)
    H=S+np.eye(n,dtype=np.int64); assert (H@H.T==n*np.eye(n,dtype=np.int64)).all(); return H

def punkte(m):   # alle x in {+-1}^m mit x_0 = +1
    X=np.array(list(itertools.product((1,-1),repeat=m-1)),dtype=np.int64)
    return np.hstack([np.ones((len(X),1),dtype=np.int64),X])

def offen(U,W,d):   # Zahl der (y,z)-Klassen (y_0 = z_0 = +1) mit max_i(|u_i|+|v_i|) < d
    au=np.abs(punkte(U.shape[1])@U.T); av=np.abs(punkte(W.shape[1])@W.T); tot=0
    for a in au: tot+=int(((a[None,:]+av).max(axis=1)<d).sum())
    return tot

def anforderungen(U,d):   # maximale Vektoren r = d - |u(y)| (nur Eintraege > 1 zaehlen, sonst ist y immer erfuellt)
    au=np.abs(punkte(U.shape[1])@U.T); par=U.shape[1]%2
    R=set()
    for a in au:
        r=d-a
        if (r<= (1 if (d-par)%2==1 else 0)).any(): continue    # eine Zeile verlangt hoechstens das Minimum von |v|: immer erfuellt
        R.add(tuple(int(t) for t in r))
    R=list(R); keep=[]
    for r in R:
        if not any(s!=r and all(si>=ri for si,ri in zip(s,r)) for s in R): keep.append(r)
    return keep, len(R)

def finde_W(U,m2,d,sek,fix=None,seed=1):
    K=U.shape[0]; req,nall=anforderungen(U,d); Z=punkte(m2)
    mdl=cp_model.CpModel(); w=[[mdl.NewBoolVar(f"w{i}_{j}") for j in range(m2)] for i in range(K)]   # 1 = Eintrag -1
    for j in range(m2): mdl.Add(w[0][j]==0)          # Spaltenvorzeichen
    for i in range(K): mdl.Add(w[i][0]==0)           # Zeilenvorzeichen
    if fix is not None:
        for (i,j,val) in fix: mdl.Add(w[i][j]==val)
    thr=sorted({t for r in req for t in r})
    lit={}
    for zi,z in enumerate(Z):
        zs=int(z.sum())
        for i in range(K):
            expr=zs-2*sum(int(z[j])*w[i][j] for j in range(m2))
            for t in thr:
                if t>m2: continue
                p=mdl.NewBoolVar(""); n=mdl.NewBoolVar("")
                mdl.Add(expr>=t).OnlyEnforceIf(p); mdl.Add(expr<=-t).OnlyEnforceIf(n); lit[(zi,i,t)]=(p,n)
        for r in req:
            cl=[]
            for i in range(K):
                if (zi,i,r[i]) in lit: cl.extend(lit[(zi,i,r[i])])
            mdl.AddBoolOr(cl)
    s=cp_model.CpSolver(); s.parameters.max_time_in_seconds=sek; s.parameters.num_search_workers=2; s.parameters.random_seed=seed
    t0=time.time(); st=s.Solve(mdl); dt=time.time()-t0
    name={cp_model.OPTIMAL:"LOESUNG",cp_model.FEASIBLE:"LOESUNG",cp_model.INFEASIBLE:"UNMOEGLICH",cp_model.UNKNOWN:"ZEITGRENZE"}.get(st,str(st))
    Wn=None
    if name=="LOESUNG": Wn=np.array([[1-2*s.Value(w[i][j]) for j in range(m2)] for i in range(K)],dtype=np.int64)
    return name,Wn,dt,len(req),nall

def finde_W2(U,m2,d,sek,seed=1,erste_fest=True):
    """Wie finde_W, aber jede Zeile von W waehlt EIN Wort aus allen 2^(m2-1) Woertern (bis auf Vorzeichen): rein boolesches Modell."""
    K=U.shape[0]; req,nall=anforderungen(U,d); Z=punkte(m2); C=punkte(m2); A=np.abs(Z@C.T)      # A[z][c] = |<c,z>|
    mdl=cp_model.CpModel(); x=[[mdl.NewBoolVar("") for c in range(len(C))] for i in range(K)]
    for i in range(K): mdl.AddExactlyOne(x[i])
    if erste_fest: mdl.Add(x[0][0]==1)             # Spaltenvorzeichen: erste Zeile = Einsvektor
    thr=sorted({t for r in req for t in r}); g={}
    for zi in range(len(Z)):
        for t in thr:
            good=[c for c in range(len(C)) if A[zi][c]>=t]
            for i in range(K):
                b=mdl.NewBoolVar(""); g[(zi,i,t)]=b
                if good: mdl.AddBoolOr([x[i][c] for c in good]+[b.Not()])
                else: mdl.Add(b==0)
        for r in req: mdl.AddBoolOr([g[(zi,i,r[i])] for i in range(K)])
    s=cp_model.CpSolver(); s.parameters.max_time_in_seconds=sek; s.parameters.num_search_workers=2; s.parameters.random_seed=seed
    t0=time.time(); st=s.Solve(mdl); dt=time.time()-t0
    name={cp_model.OPTIMAL:"LOESUNG",cp_model.FEASIBLE:"LOESUNG",cp_model.INFEASIBLE:"UNMOEGLICH",cp_model.UNKNOWN:"ZEITGRENZE"}.get(st,str(st))
    Wn=None
    if name=="LOESUNG": Wn=np.array([C[[c for c in range(len(C)) if s.Value(x[i][c])][0]] for i in range(K)],dtype=np.int64)
    return name,Wn,dt,len(req),nall

if __name__=="__main__":
    modus=sys.argv[1]; sek=float(sys.argv[2]) if len(sys.argv)>2 else 120
    H=paley1(11); U12=H[:,1:]; W12=H[:,3:]
    print("48er nachgezaehlt: offene Klassen bei d=8:",offen(U12,W12,8),"; bei d=10:",offen(U12,W12,10)>0 and "viele" )
    if modus=="kontrolle":
        st,Wn,dt,nr,na=finde_W2(U12,9,8,sek); print(f"K=12, U = H ohne Spalte 0, Typ (11,9), d=8: {st} in {dt:.1f} s; Anforderungsvektoren {nr} maximal von {na}")
        if Wn is not None: print("  nachgezaehlt offen:",offen(U12,Wn,8)); print(Wn)
    if modus=="k11":
        for weg in ([11],[0],[5]):
            U=np.delete(U12,weg,axis=0); st,Wn,dt,nr,na=finde_W2(U,9,8,sek)
            print(f"K=11, U ohne Zeile {weg}, Typ (11,9), d=8: {st} in {dt:.1f} s; Anforderungsvektoren {nr} von {na}; Start (W ebenso gekuerzt) offen: {offen(U,np.delete(W12,weg,axis=0),8)}",flush=True)
            if Wn is not None: print("  nachgezaehlt offen:",offen(U,Wn,8)); print(Wn)
    if modus=="k10":
        for weg in ([10,11],[0,1],[3,7]):
            U=np.delete(U12,weg,axis=0); st,Wn,dt,nr,na=finde_W2(U,9,8,sek)
            print(f"K=10, U ohne Zeilen {weg}, Typ (11,9), d=8: {st} in {dt:.1f} s; Anforderungsvektoren {nr} von {na}",flush=True)
            if Wn is not None: print("  nachgezaehlt offen:",offen(U,Wn,8)); print(Wn)
