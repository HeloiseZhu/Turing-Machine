; This program checks if the input string is in the form of a^ib^ja^ib^j(i, j>0).
; Input: a string of a's and b's, e.g. 'aabaab'
; Process:
; 1) Copy the first a^ib^j in the input to 2nd tape, and rewrite them with blanks on 1st tape
; 2) Compare two strings on 1st tape and 2nd tape. If same, accept. Otherwise, reject.

; the finite set of states
#Q = {0,cp1,cp2,cmp,mh,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,reject6,reject7,halt_reject}

; the finite set of input symbols
#S = {a,b}

; the complete set of tape symbols
#G = {a,b,_,t,r,u,e,f,a,l,s}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 2

; the transition functions

; State 0: start state
0 a_ a_ ** cp1
0 b_ b_ ** reject6 ; 'b...'
0 __ __ ** reject  ; empty input

; State cp1: copy the first a^i string to the 2nd tape
cp1 a_ _a rr cp1
cp1 b_ b_ ** cp2
cp1 __ __ *l reject6 ; 'a...a'

; State cp2: copy the first b^j string to the 2nd tape
cp2 a_ a_ *l mh
cp2 b_ _b rr cp2
cp2 __ __ *l reject6 ; 'a...ab...b'

; State mh: move 2nt head to the left
mh ab ab *l mh
mh aa aa *l mh
mh a_ a_ *r cmp

; State cmp: compare two strings
cmp _a _a ** reject7 ; not match
cmp _b _b ** reject7 ; not match
cmp a_ a_ ** reject7 ; not match
cmp b_ b_ ** reject7 ; not match
cmp aa __ rr cmp
cmp bb __ rr cmp
cmp ab ab ** reject7 ; not match
cmp ba ba ** reject7 ; not match
cmp __ __ ** accept

; State accept*: write 'true' on 1st tape
accept __ t_ r* accept2
accept2 __ r_ r* accept3
accept3 __ u_ r* accept4
accept4 __ e_ ** halt_accept

; State reject*: write 'false' on 1st tape
reject __ f_ r* reject2
reject2 __ a_ r* reject3
reject3 __ l_ r* reject4
reject4 __ s_ r* reject5
reject5 __ e_ ** halt_reject

; State reject6: clear tapes
reject6 a_ __ r* reject6
reject6 b_ __ r* reject6
reject6 _a __ *l reject6
reject6 _b __ *l reject6
reject6 __ __ ** reject

; State reject7: clear tapes
reject7 a_ __ r* reject7
reject7 b_ __ r* reject7
reject7 _a __ *r reject7
reject7 _b __ *r reject7
reject7 aa __ rr reject7
reject7 ab __ rr reject7
reject7 ba __ rr reject7
reject7 bb __ rr reject7
reject7 __ __ ** reject