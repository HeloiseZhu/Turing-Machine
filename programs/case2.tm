; This program checks if the input string is in the form of 1^mx1^n=1^mn.
; Input: a string of 1's, 'x' and '=', e.g. '11x11=1111'
; Process:
; 1) Copy 1's before 'x' in the input to 2nd tape, and rewrite with blanks. Let number of 1's on 2nd tape be m.
; 2) Copy 1's between 'x' and '=' in the input to 3rd tape, and rewrite with blanks. Let number of 1's on 3rd tape be n.
; 3) Copy 1's after '=' in the input to 4th tape, and rewrite with blanks. Let number of 1's on 4th tape be t.
; 4) For every '1' on 2nd tape(from right to left), traverse n 1's on 3rd tape(from rightmost(cmp1), or from leftmost(cmp2)),
;    and eliminate n 1's on 4th tape. This step will repeat until head of 2nd or 4th tape reads blank.;
; 5) If t=m x n, last step will repeat m times. 
;    Moreover, when 4th tape is empty, 
;    head of 2nd tape exactly reaches the first blank on the left,
;    and head of 3rd tape exactly reaches the first blank on the left(or right), accept. Otherwise, reject.


; the finite set of states
#Q = {s0,s1,s2,cp1,cp2,cp3,cmp1,cmp2,ckl,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,reject6,reject7,halt_reject}

; the finite set of input symbols
#S = {1,x,=}

; the complete set of tape symbols
#G = {1,x,=,_,t,r,u,e,f,a,l,s}

; the start state
#q0 = s0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 4

; the transition functions

; State s0: start state
s0 1___ 1___ **** cp1
s0 x___ x___ **** reject6 ; start with 'x' or '='
s0 =___ =___ **** reject6 ; start with 'x' or '='
s0 ____ ____ **** reject  ; empty input

; State cp1: copy the 1^m string to the 2nd tape
cp1 1___ _1__ rr** cp1
cp1 x___ ____ r*** s1
cp1 =___ =___ **** reject6      ; '1...1=...'
cp1 ____ ____ **** reject       ; '1...1'

; State s1: make sure there is 1 between 'x' and '='
s1 1___ 1___ **** cp2
s1 x___ x___ **** reject6       ; '1...1x...x...'
s1 =___ =___ **** reject6       ; '1...1x...=...'
s1 ____ ____ **** reject        ; '1...1x'

; State cp2: copy the 1^n string to the 3rd tape
cp2 1___ __1_ r*r* cp2
cp2 x___ x___ **** reject6      ; '1...1x1...1x...'
cp2 =___ ____ r*** s2
cp2 ____ ____ **** reject       ; '1...1x1...1'

; State s2: make sure there is 1 after '='
s2 1___ 1___ **** cp3
s2 x___ x___ **** reject6       ; '1...1x1...1=...x...'
s2 =___ =___ **** reject6       ; '1...1x1...1=...=...'
s2 ____ ____ **** reject        ; '1...1x1...1='

; State cp3: copy the 1^{mn} string to the 4th tape
cp3 1___ ___1 r**r cp3
cp3 x___ x___ **** reject6      ; '1...1x1...1=...x...'
cp3 =___ =___ **** reject6      ; '1...1x1...1=...=...'
cp3 ____ ____ *lll cmp1

; State cmp1: compare three strings
; direction: 2nd-still, 3rd-left, 4th-left
cmp1 __11 __11 **r* reject      ; m x n < t
cmp1 _1__ _1__ *l** ckl
cmp1 _1_1 _1_1 *lr* cmp2
cmp1 _11_ _11_ **** reject      ; m x n > t
cmp1 _111 _11_ **ll cmp1

; State cmp2: compare three strings
; direction: 2nd-still, 3rd-right, 4th-left
cmp2 __11 __11 **l* reject      ; m x n < t
cmp2 _1__ _1__ *l** ckl
cmp2 _1_1 _1_1 *ll* cmp1
cmp2 _11_ _11_ **** reject      ; m x n > t
cmp2 _111 _11_ **rl cmp2

; State ckl: check if there are more 1's on the 2nd tape
ckl _1__ _1__ **** reject       ; m x n > t
ckl ____ ____ **** accept

; State accept*: write 'true' on 1st tape
accept ____ t___ r*** accept2
accept2 ____ r___ r*** accept3
accept3 ____ u___ r*** accept4
accept4 ____ e___ **** halt_accept

; State reject*: write 'false' on 1st tape
reject ____ f___ r*** reject2
reject2 ____ a___ r*** reject3
reject3 ____ l___ r*** reject4
reject4 ____ s___ r*** reject5
reject5 ____ e___ **** halt_reject

reject _1__ f1__ r*** reject2
reject2 _1__ a1__ r*** reject3
reject3 _1__ l1__ r*** reject4
reject4 _1__ s1__ r*** reject5
reject5 _1__ e1__ **** halt_reject

reject ___1 f1__ r*** reject2
reject2 ___1 a1__ r*** reject3
reject3 ___1 l1__ r*** reject4
reject4 ___1 s1__ r*** reject5
reject5 ___1 e1__ **** halt_reject

reject _11_ f1__ r*** reject2
reject2 _11_ a1__ r*** reject3
reject3 _11_ l1__ r*** reject4
reject4 _11_ s1__ r*** reject5
reject5 _11_ e1__ **** halt_reject

; State reject6: clear 1st tape
reject6 1___ ____ r*** reject6
reject6 x___ ____ r*** reject6
reject6 =___ ____ r*** reject6
reject6 ____ ____ **** reject