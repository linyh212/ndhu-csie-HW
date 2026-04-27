% -------------------------
person(andy, male).
person(bob, male).
person(cecil, male).
person(dennis, male).
person(edward, male).
person(felix, male).
person(martin, male).
person(oscar, male).
person(quinn, male).

person(gigi, female).
person(helen, female).
person(iris, female).
person(jane, female).
person(kate, female).
person(liz, female).
person(nancy, female).
person(pattie, female).
person(rebecca, female).
% -------------------------
married(bob, helen).
married(helen, bob).

married(dennis, pattie).
married(pattie, dennis).

married(gigi, martin).
married(martin, gigi).
% -------------------------
parent(andy, bob).
parent(bob, cecil).
parent(cecil, dennis).
parent(dennis, edward).
parent(edward, felix).

parent(gigi, helen).
parent(helen, iris).
parent(iris, jane).
parent(jane, kate).
parent(kate, liz).

parent(martin, nancy).
parent(nancy, oscar).
parent(oscar, pattie).
parent(pattie, quinn).
parent(quinn, rebecca).
% -------------------------
parent(helen, cecil).
parent(bob, iris).

parent(pattie, edward).
parent(dennis, quinn).

parent(gigi, nancy).
parent(martin, helen).
% -------------------------
sibling(X, Y) :-
    parent(P, X),
    parent(P, Y),
    X \= Y.

brother(X, Y) :-
    sibling(X, Y),
    person(X, male),
    person(Y, male).
    
sister(X, Y) :-
    sibling(X, Y),
    person(X, female),
    person(Y, female).
    
cousin(X, Y) :-
    parent(P1, X),
    parent(P2, Y),
    sibling(P1, P2),
    X \= Y.
    
are_married(X, Y) :-
    married(X, Y);
    married(Y, X).
% -------------------------

:- initialization(main).
main :-
    write('Input format: relation(name1, name2)'), nl,
    repeat,
    read(Input),
    ( Input == exit ->
        nl, halt
    ;   ( call(Input) -> 
            write('True'), nl
        ;
            write('False'), nl    
        ),
        fail
    ).