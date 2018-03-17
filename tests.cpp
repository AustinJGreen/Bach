//
// Created by austi on 12/9/2017.
//

#include "tests.h"

void tests::testMagic() {
    int64 a = 281474976710655UL;
    assert(magic_marbleCount(a) == 48);

    int64 b = 137455731464UL;
    assert(magic_marbleCount(b) == 5);

    int64 c = 0L;
    assert(magic_marbleCount(c) == 0);

    int64 d = 137455731464UL;
    assert(!magic_isWon(d));

    int64 e = 8725724278030336UL;
    assert(magic_isWon(e));

    int64 f = 0;
    magic_set(f, 48);
    magic_set(f, 49);
    magic_set(f, 50);
    magic_set(f, 51);
    magic_set(f, 52);
    assert(magic_isWon(f));

    magic_remove(f, 47);
    assert(magic_isWon(f));

    magic_qremove(f, 48);
    assert(!magic_isWon(f));

    magic_set(f, 48);
    assert(magic_isWon(f));

    magic_remove(f, 48);
    assert(!magic_isWon(f));

    int64 aa = 586472905206924040UL;
    assert(magic_getUnsafe(aa) == 160528718627592);
    assert(magic_getSafe(aa) == 844424930131968);

    int64 zz = 8303511812964352UL;
    assert(magic_getRot48(zz, YELLOW, YELLOW) == 140737488355328UL);

    int64 g = magic_getRange(f, 48, 52);
    assert(magic_marbleCount(g) == 4);

    magic_set(f, 48);
    g = magic_getRange(f, 48, 52);
    assert(magic_marbleCount(g) == 5);

    int64 v = 0;
    magic_set(v, 1);
    magic_set(v, 13);
    magic_set(v, 25);
    magic_set(v, 37);
    assert(magic_getRot48(v, GREEN, YELLOW) == 137472516098);

    int64 l = 0;
    magic_set(l, 25);
    assert(magic_getRot48(l, YELLOW, GREEN) == 2);

    int64 m = 1;
    assert(magic_getRot48(m, RED, YELLOW) == 4096);
    assert(magic_getRot48(m, GREEN, RED) == 4096);
    assert(magic_getRot48(m, BLUE, GREEN) == 4096);
    assert(magic_getRot48(m, YELLOW, BLUE) == 4096);

    assert(magic_get(magic_getRot48(m, BLUE, RED), 24));
    assert(magic_get(magic_getRot48(m, YELLOW, GREEN), 24));
    assert(magic_get(magic_getRot48(m, RED, BLUE), 24));
    assert(magic_get(magic_getRot48(m, GREEN, YELLOW), 24));

    assert(magic_get(magic_getRot48(m, YELLOW, RED), 36));
    assert(magic_get(magic_getRot48(m, RED, GREEN), 36));
    assert(magic_get(magic_getRot48(m, GREEN, BLUE), 36));
    assert(magic_get(magic_getRot48(m, BLUE, YELLOW), 36));

    assert(magic_get(magic_getRot48(m, YELLOW, YELLOW), 0));
    assert(magic_get(magic_getRot48(m, RED, RED), 0));
    assert(magic_get(magic_getRot48(m, GREEN, GREEN), 0));
    assert(magic_get(magic_getRot48(m, BLUE, BLUE), 0));

    int64 z = 4096;

    assert(magic_getRot48(z, YELLOW, RED) == 1);
    assert(magic_getRot48(z, RED, GREEN) == 1);
    assert(magic_getRot48(z, GREEN, BLUE) == 1);
    assert(magic_getRot48(z, BLUE, YELLOW) == 1);

    int64 y = 16777216UL;
    assert(magic_getRot48(y, YELLOW, GREEN) == 1);
    assert(magic_getRot48(y, GREEN, YELLOW) == 1);
    assert(magic_getRot48(y, RED, BLUE) == 1);
    assert(magic_getRot48(y, BLUE, RED) == 1);

    int64 x = 140737488355328UL;
    assert(magic_getRot48(x, YELLOW, BLUE) == 2048);
    assert(magic_getRot48(x, RED, YELLOW) == 2048);
    assert(magic_getRot48(x, GREEN, RED) == 2048);
    assert(magic_getRot48(x, BLUE, GREEN) == 2048);

    int64 w = 8725724278030336UL;
    assert(magic_getRot48(w, YELLOW, YELLOW) == 0);
    assert(magic_getRot48(w, YELLOW, RED) == 0);
    assert(magic_getRot48(w, YELLOW, GREEN) == 0);
    assert(magic_getRot48(w, YELLOW, BLUE) == 0);
    assert(magic_getRot48(w, RED, YELLOW) == 0);
    assert(magic_getRot48(w, RED, RED) == 0);
    assert(magic_getRot48(w, RED, GREEN) == 0);
    assert(magic_getRot48(w, RED, BLUE) == 0);
    assert(magic_getRot48(w, GREEN, YELLOW) == 0);x, y,
    assert(magic_getRot48(w, GREEN, RED) == 0);
    assert(magic_getRot48(w, GREEN, GREEN) == 0);
    assert(magic_getRot48(w, GREEN, BLUE) == 0);
    assert(magic_getRot48(w, BLUE, YELLOW) == 0);
    assert(magic_getRot48(w, BLUE, RED) == 0);
    assert(magic_getRot48(w, BLUE, GREEN) == 0);
    assert(magic_getRot48(w, BLUE, BLUE) == 0);

    int64 t = 8444249301319681UL;
    assert(magic_getRot48(t, YELLOW, BLUE) == 4096);

    int64 u = 567002796033UL;
    assert(magic_getRot48(magic_getRot48(u, YELLOW, RED), RED, YELLOW) == 567002796033UL);
    assert(magic_getRot48(magic_getRot48(u, YELLOW, BLUE), BLUE, YELLOW) == 567002796033UL);
    assert(magic_getRot48(magic_getRot48(u, YELLOW, GREEN), GREEN, YELLOW) == 567002796033UL);
    assert(magic_getRot48(magic_getRot48(u, YELLOW, RED), RED, YELLOW) == 567002796033UL);

    int64 r = 140737488355329UL;
    assert(magic_getRot48(r, YELLOW, BLUE) == 6144);
    assert(magic_getRot48(magic_getRot48(r, YELLOW, BLUE), BLUE, YELLOW) == 140737488355329UL);

    assert(magic_getRot48(r, YELLOW, GREEN) == 25165824);
    assert(magic_getRot48(magic_getRot48(r, YELLOW, GREEN), GREEN, YELLOW) == 140737488355329UL);;

    assert(magic_getRot48(r, YELLOW, RED) == 103079215104);
    assert(magic_getRot48(magic_getRot48(r, YELLOW, RED), RED, YELLOW) == 140737488355329UL);
}

void tests::testNotation() {
    assert(notation::fromStr("y1", YELLOW) == 0);
    assert(notation::fromStr("b12", YELLOW) == 47);
    assert(notation::fromStr("r1", RED) == 0);
    assert(notation::fromStr("y12", RED) == 47);
    assert(notation::fromStr("g1", GREEN) == 0);
    assert(notation::fromStr("r12", GREEN) == 47);
    assert(notation::fromStr("b1", BLUE) == 0);
    assert(notation::fromStr("g12", BLUE) == 47);
    assert(notation::fromStr("hy1", YELLOW) == 48);
    assert(notation::fromStr("hy5", YELLOW) == 52);
    assert(notation::fromStr("hr1", RED) == 48);
    assert(notation::fromStr("hr5", RED) == 52);
    assert(notation::fromStr("hg1", GREEN) == 48);
    assert(notation::fromStr("hg5", GREEN) == 52);
    assert(notation::fromStr("hb1", BLUE) == 48);
    assert(notation::fromStr("hb5", BLUE) == 52);
}

void tests::testMoveGeneration() {
    position c = position();
    c.set(YELLOW, 47);
    c.set(YELLOW, 46);

    movelist cmoves2 = c.getLegalMoves(roll(2, 5), YELLOW);
    assert(cmoves2.count() == 1);
    
    position a = position();
    a.set(YELLOW, 0);

    movelist amoves2 = a.getLegalMoves(roll(6, 1), YELLOW);
    assert(amoves2.count() == 3);

    position b = position();
    b.set(YELLOW, 51);

    movelist bmoves2 = b.getLegalMoves(roll(1, 1), YELLOW);
    assert(bmoves2.count() == 2);



    position d = position();
    d.set("b11", YELLOW);
    d.set("b12", YELLOW);

    movelist dmoves2 = d.getLegalMoves(roll(2, 5), YELLOW);
    assert(dmoves2.count() == 1);

    position e = position();
    e.set("hy2", YELLOW);
    e.set("hy3", YELLOW);
    e.set("hy4", YELLOW);
    e.set("hy5", YELLOW);
    e.set("g7", YELLOW);

    e.set("hg3", GREEN);
    e.set("hg4", GREEN);
    e.set("hg5", GREEN);
    e.set("r9", GREEN);

    movelist emovesg2 = e.getLegalMoves(roll(1, 6), GREEN);
    assert(emovesg2.count() == 3);

    movelist emovesy2 = e.getLegalMoves(roll(1, 6), YELLOW);
    assert(emovesy2.count() == 1);

    position f = position();
    f.set("g2", GREEN);
    f.set("b2", GREEN);
    f.set("y2", GREEN);
    f.set("r2", GREEN);

    movelist fmoves = f.getLegalMoves(roll(6, 1), GREEN);
    assert(fmoves.count() == 24);

    position g = position();
    g.set("y3", YELLOW);
    g.set("y5", YELLOW);
    g.set("y8", YELLOW);

    movelist gmoves2 = g.getLegalMoves(roll(2, 3), YELLOW);
    assert(gmoves2.count() == 4);

    position h = position();
    h.set("hy5", YELLOW);
    h.set("hy4", YELLOW);
    h.set("hy3", YELLOW);
    h.set("hy1", YELLOW);
    h.set("b12", YELLOW);

    movelist hmoves2 = h.getLegalMoves(roll(1, 1), YELLOW);
    assert(hmoves2.count() == 1);
    assert(h.isValid());

    position i = position();
    i.set("y2", YELLOW);
    i.set("y5", YELLOW);

    movelist imoves2 = i.getLegalMoves(roll(4, 5), YELLOW);
    assert(imoves2.count() == 3);

    position j = position();
    j.set("b2", BLUE);
    j.set("b5", BLUE);

    movelist jmoves2 = j.getLegalMoves(roll(3, 3), BLUE);
    assert(jmoves2.count() == 2);
}

void tests::testPosition() {

    position a = position();

    position b = a;

    int64 takeoutMove = move_make(pseudomove_make(0, -1, 5));
    b.doMove(takeoutMove, roll(6, 5));
    assert(!b.equals(a));

    b.undoMove(takeoutMove);
    assert(b.equals(a));

    int64 illegalTest = NONE;
    assert(!a.isLegal(illegalTest));
}

void tests::testMoves() {
    int32 a1 = pseudomove_make(YELLOW, -1, 5);
    assert(pseudomove_getFrom(a1) == -1);
    assert(pseudomove_getTo(a1) == 5);
    assert(pseudomove_isTakeout(a1));
    assert(pseudomove_getTeam(a1) == YELLOW);
    assert(pseudomove_getCapturedTeam(a1) == -1);

    int32 a2 = pseudomove_make(YELLOW, 5, 6);
    assert(pseudomove_getFrom(a2) == 5);
    assert(pseudomove_getTo(a2) == 6);
    assert(!pseudomove_isTakeout(a2));
    assert(pseudomove_getTeam(a2) == YELLOW);
    assert(pseudomove_getCapturedTeam(a2) == -1);

    int64 a = move_make(a2, a1);
    assert(move_getFirst(a) == a2);
    assert(move_getSecond(a) == a1);
    assert(move_isTwoStep(a));

    int32 b1 = pseudomove_make(BLUE, 51, 53);
    assert(pseudomove_getFrom(b1) == 51);
    assert(pseudomove_getTo(b1) == 53);
    assert(!pseudomove_isTakeout(b1));
    assert(pseudomove_getTeam(b1) == BLUE);
    assert(pseudomove_getCapturedTeam(b1) == -1);

    int64 b = move_make(b1);
    assert(move_getFirst(b) == b1);
    assert(move_getSecond(b) == 0);
    assert(!move_isTwoStep(b));

    pseudomove_setCapturedTeam(b1, GREEN);
    b = move_make(b1);
    assert(pseudomove_getCapturedTeam(move_getFirst(b)) == GREEN);

    int32 c1 = pseudomove_make(RED, 47, 51);
    pseudomove_setCapturedTeam(c1, YELLOW);
    assert(pseudomove_getCapturedTeam(c1) == YELLOW);
    pseudomove_removeCapture(c1);
    assert(pseudomove_getCapturedTeam(c1) == -1);
    pseudomove_setCapturedTeam(c1, BLUE);
    assert(pseudomove_getCapturedTeam(c1) == BLUE);
}

void tests::testHashing() {
    position a = position();

    //Move order 1
    int64 move_1a = move_make(pseudomove_make(YELLOW, -1, 0), pseudomove_make(YELLOW, 0, 2));
    int64 move_1b = move_make(pseudomove_make(YELLOW, 2, 5));

    a.doMove(move_1a, roll(1, 2));
    a.next(roll(5, 4)); //RED
    a.next(roll(4, 3)); //GREEN
    a.next(roll(3, 2)); //BLUE
    a.doMove(move_1b, roll(1, 2));

    //Move order 2
    int64 move_2a = move_make(pseudomove_make(YELLOW, -1, 0), pseudomove_make(YELLOW, 0, 5));

    position b = position();
    b.doMove(move_2a, roll(6, 4));
    assert(a.getHash() == b.getHash());

    position c = position();

    int64 move_3a = move_make(pseudomove_make(YELLOW, -1, 0), pseudomove_make(YELLOW, 0, 1));
    int64 move_3b = move_make(pseudomove_make(YELLOW, 1, 6));

    c.doMove(move_3a, roll(1, 1));
    c.doMove(move_3b, roll(2, 3));

    position d = position();
    int64 move_4a = move_make(pseudomove_make(YELLOW, -1, 0), pseudomove_make(YELLOW, 0, 6));
    d.doMove(move_4a, roll(6, 6));

    assert(c.getHash() != d.getHash()); //Doubles difference
}

void tests::testEval() {
    eval e = eval();

    position p = position();
    p.set("y6", YELLOW);
    p.set("hy1", YELLOW);

    e.calc_prob(p.get(YELLOW), 5, 10);

    position c = position();
    c.set("hy2", YELLOW);
    c.set("hy3", YELLOW);
    c.set("hy4", YELLOW);
    c.set("hy5", YELLOW);
    c.set("hg3", GREEN);
    c.set("hg4", GREEN);
    c.set("hg5", GREEN);
    c.set("b9", YELLOW);
    c.set("b8", GREEN);
    c.set("y10", GREEN);
    c.set("y11", RED);

    score wtf = e.score_pos(c);
    score wtf2 = e.evaluate_pos(c);

    movelist mtest = c.getLegalMoves(roll(1, 1), GREEN);
    for (int i = 0; i < mtest.count(); i++)
    {
        int64 curmove = mtest[i];
        c.doMove(curmove, roll(1, 1));

        score a = e.score_pos(c);
        float aVec = a.getVector(GREEN);

        score b = e.evaluate_pos(c);
        float bVec = b.getVector(GREEN);

        c.undoMove(curmove);
    }
}

void tests::testSearch() {

}

void tests::testMemory() {
    position a = position();

    //Create random position full of random states
    for (int p = 0; p < 10; p++) //10 ply
    {
        roll randomRoll = roll(random::next(1, 7), random::next(1, 7));
        movelist ml = a.getLegalMoves(randomRoll, a.getToMove());
        if (ml.count() > 0) {
            int64 m = ml[random::next(0, ml.count())];
            a.doMove(m, randomRoll);
        }
        a.next(randomRoll);
    }

    position aCopy = position(a);
    position bCopy = aCopy;
    bCopy.dispose();
    aCopy.dispose();



    return;
}
