//
// Created by austi on 12/9/2017.
//

#include "position.h"

namespace Zobrist
{
    int64 holes[4][53];
    int64 color[TEAM_COUNT];
    int64 doubleCount[3];
}

void position::init()
{
    prng rng(1070372);

    for (int t = 0; t < TEAM_COUNT; t++)
    {
        for (int h = 0; h < 53; h++)
        {
            Zobrist::holes[t][h] = rng.rand64();
        }

        Zobrist::color[t] = rng.rand64();
    }

    for (int d = 0; d < 3; d++)
    {
        Zobrist::doubleCount[d] = rng.rand64();
    }
}

position::position()
{
    teamsMode = false;
    disposed =  false;
    current = nullptr;
    reset();
}

position::position(int64 t[TEAM_COUNT], int toMove, int doublesCnt, int removed)
{
    teamsMode = false;
    disposed = false;
    current = new pstate();
    current->key = 0;
    current->removed = removed;
    current->toMove = toMove;
    current->doublesCnt = doublesCnt;
    current->prev = nullptr;

    for (int i = 0; i < TEAM_COUNT; i++)
    {
        teams[i] = t[i];

        int64 m = t[i];
        while (m)
        {
            int next = magic_popMsb(m);
            current->key ^= Zobrist::holes[i][next];
        }
    }

    current->key ^= Zobrist::color[toMove];
    current->key ^= Zobrist::doubleCount[doublesCnt];
}

position::position(const position& pos)
{
    teamsMode = pos.isTeamsMode();
    disposed = false;
    for (int i = 0; i < TEAM_COUNT; i++)
    {
        teams[i] = pos.teams[i];
    }

    copyState(pos.current);
}

position::~position()
{
    dispose();
}

void position::dispose()
{
    if (!disposed) {
        deleteStates();
        disposed = true;
    }
}

void position::copyState(pstate *state)
{
    assert(!disposed);
    pstate *head;
    pstate *curfrom = state;
    while (curfrom != nullptr)
    {
        pstate *curcopy = new pstate();
        curcopy->key = curfrom->key;
        curcopy->removed = curfrom->removed;
        curcopy->toMove = curfrom->toMove;
        curcopy->doublesCnt = curfrom->doublesCnt;
        assert(curcopy->isValid());

        if (curfrom == state)
        {
            current = curcopy;
            head = curcopy;
        }
        else
        {
            head->prev = curcopy;
            head = curcopy;
        }

        curfrom = curfrom->prev;
    }
}

void position::deleteStates()
{
    pstate *cur = current;
    while (cur != nullptr)
    {
        pstate* prev = cur->prev;
        assert(cur->isValid());
        delete cur;
        cur = prev;
    }

    current = nullptr;
    assert(current == nullptr);
}

bool position::isTeamsMode() const
{
    return teamsMode;
}

int position::getToMove() const
{
    return current->toMove;
}

int position::getDoublesCnt() const
{
    return current->doublesCnt;
}

bool position::isValid() const
{
    if (current == nullptr)
    {
        return false;
    }

    for (int i = 0; i < TEAM_COUNT; i++)
    {
        int64 mc = magic_marbleCount(teams[i]);
        int64 invalidbits = teams[i] >> 53;
        if (mc > MARBLE_COUNT || invalidbits)
        {
            return false;
        }
    }

    return true;
}

std::string position::toString() const
{
    std::string quads[4] = { "y", "r", "g", "b" };
    std::string names[4] = { "yellow", "red", "green", "blue" };
    std::string str = std::string();
    for (int t = 0; t < TEAM_COUNT; t++)
    {
        int linepos = 0;
        str.append("<");
        str.append(names[t]);
        str.append(">");

        linepos += 2;
        linepos += (int)names[t].length();

        int nleft = 12 - (linepos % 12);
        for (int m = 0; m < nleft; m++)
        {
            str.append(" ");
        }

        linepos += nleft;

        int64 unsafe_left = magic_getRot48(teams[t], t, YELLOW);
        while (unsafe_left)
        {
            int next = magic_popMsb(unsafe_left);
            int index = next;
            int quad = (index) / 12;
            int quadIndex = (index % 12) + 1;

            str.append(quads[quad]);

            str.append(std::to_string(quadIndex));
            str.append(" ");
        }

        int64 safe = magic_getSafe(teams[t]);
        while (safe)
        {
            int next_safe = magic_popMsb(safe);
            str.append("h");
            str.append(quads[t]);

            int index = next_safe;
            int quadIndex = (index % 12) + 1;
            str.append(std::to_string(quadIndex));
            str.append(" ");
        }

        str.append("\n");
    }

    str.append("{ ");
    for (int z = 0; z < TEAM_COUNT; z++)
    {
        str.append(std::to_string(teams[z]));
        if (z != TEAM_COUNT - 1)
        {
            str.append(", ");
        }
    }

    str.append("}\r\n");

    return str;
}

void position::setTeamsMode(bool teamsMode)
{
    this->teamsMode = teamsMode;
}

void position::set(std::string sq, int team)
{
    set(team, notation::fromStr(sq, team));
}

void position::set(int team, int hole)
{
    magic_set(teams[team], hole);
}

int position::removeLeading(int team)
{
    assert(magic_isValid(teams[team]));
    int64 pmag = teams[team];
    int64 unsafe = magic_getUnsafe(pmag);
    if (!unsafe)
    {
        return -1;
    }

    int leading = magic_getMsb(unsafe);
    magic_remove(teams[team], leading);
    assert(magic_isValid(teams[team]));
    return leading;
}

void position::reset()
{
    for (int i = 0; i < TEAM_COUNT; i++)
    {
        teams[i] = 0;
    }

    deleteStates();
    current = new pstate();
    current->key = 0;
    current->removed = -1;
    current->toMove = YELLOW;
    current->doublesCnt = 0;
    current->prev = nullptr;
}

void position::next(roll r)
{
    assert(!disposed);
    pstate* next = new pstate();
    next->removed = -1;
    next->doublesCnt = current->doublesCnt;
    if (r.isDoubles() && current->doublesCnt + 1 < 3)
    {
        next->toMove = current->toMove;
        next->doublesCnt++;
    }
    else
    {
        if (current->doublesCnt + 1 >= 3)
        {
            int r = removeLeading(current->toMove);
            next->removed = r;
        }

        next->toMove = NEXT_TEAM(current->toMove);
        next->doublesCnt = 0;
    }

    assert(current->isValid());
    next->prev = current;
    current = next;
    assert(current->isValid());

    //Remove old hash
    current->key ^= Zobrist::doubleCount[current->prev->doublesCnt];
    current->key ^= Zobrist::color[current->prev->toMove];

    //Add new hash
    current->key ^= Zobrist::color[current->toMove];
    current->key ^= Zobrist::doubleCount[current->doublesCnt];
}

void position::doMove(int32 &m)
{
    assert(!disposed);
    assert(isValid());
    int us = pseudomove_getTeam(m);
    assert(us >= 0 && us < TEAM_COUNT);

    int from = pseudomove_getFrom(m);
    int to = pseudomove_getTo(m);
    assert(from >= -1 && from <= 52 && to >=0 && to <= 53);
    assert(magic_get(teams[us], to) == 0);

    if (!pseudomove_isTakeout(m))
    {
        assert(magic_get(teams[us], from) == 1);
        current->key ^= Zobrist::holes[us][from];
        magic_remove(teams[us], from);
    }

    current->key ^= Zobrist::holes[us][to];
    magic_set(teams[us], to);
    assert(magic_get(teams[us], to) == 1);
    assert(magic_isValid(teams[us]));

    //Update other magics
    for (int t = 0; t < TEAM_COUNT; t++)
    {
        if (t != us)
        {
            int64 rot = magic_getRot48(teams[t], t, us);

            //Get taken marbles
            int64 captured = rot & teams[us];

            if (captured)
            {
                //Derotate
                int64 derot = magic_getRot48(captured, us, t);
                assert(magic_marbleCount(derot) == 1);

                //Store captured
                int pos = magic_getLsb(derot);
                pseudomove_setCapturedTeam(m, t);
                current->key ^= Zobrist::holes[t][pos];

                //Remove taken marbles
                teams[t] = teams[t] ^ derot;

                //Only one marble can be captured per move.
                assert(magic_isValid(teams[t]));
                break;
            }
        }
    }

    assert(isValid());
}

void position::doMove(int64 &m, roll r)
{
    assert(m != NONE);
    assert(move_isValid(m));

    int32 first = move_getFirst(m);
    doMove(first);

    if (move_isTwoStep(m))
    {
        int32 second = move_getSecond(m);
        doMove(second);

        m = move_make(first, second);
    }
    else
    {
        m = move_make(first);
    }

    next(r);
}

void position::undoMove(int64 &m)
{
    assert(!disposed);
    if (move_isTwoStep(m))
    {
        int32 second = move_getSecond(m);
        undoMove(second);
    }

    int32 first = move_getFirst(m);
    undoMove(first);

    //Restore any removed leading marbles due to doubles roll
    if (current->removed != -1)
    {
        int prevTeam = PREV_TEAM(current->toMove);
        magic_set(teams[prevTeam], current->removed);
        assert(magic_isValid(teams[prevTeam]));
    }

    //Copy previous before deleting current
    pstate* prev = current->prev;
    delete current;

    //Restore previous state
    current = prev;
}

void position::undoMove(int32& move)
{
    int us = pseudomove_getTeam(move);
    int from = pseudomove_getFrom(move);
    int to = pseudomove_getTo(move);

    assert(magic_get(teams[us], to) == 1);
    magic_qremove(teams[us], to);
    assert(magic_isValid(teams[us]));
    assert(magic_get(teams[us], to) == 0);

    if (!pseudomove_isTakeout(move))
    {
        magic_set(teams[us], from);
    }

    //Restore other team's marble
    int t = pseudomove_getCapturedTeam(move);
    if (t != -1)
    {
        int64 rot = ((int64)1 << to);
        int64 derot = magic_getRot48(rot, us, t);

        teams[t] = teams[t] ^ derot;

        //Remove capture
        pseudomove_removeCapture(move);
    }

    assert(magic_isValid(teams[us]));
}

int64 position::get(int team) const
{
    assert(team >= 0 && team < TEAM_COUNT);
    return teams[team];
}

int64 position::marbleCount(int team) const
{
    assert(team >= 0 && team < TEAM_COUNT);
    return magic_marbleCount(teams[team]);
}

int position::getWinner() const
{
    for (int t = 0; t < TEAM_COUNT; t++)
    {
        if (magic_isWon(teams[t]))
        {
            return t;
        }
    }

    return -1;
}

bool position::isLegalPseudo(int32 m) const
{
    if (!pseudomove_isValid(m))
    {
        return false;
    }

    int f = pseudomove_getFrom(m);
    int t = pseudomove_getTo(m);
    int us = pseudomove_getTeam(m);

    //Useless condition?
    if (!pseudomove_isTakeout(m) && magic_get(teams[us], f) != 1)
    {
        return false;
    }

    int64 range = magic_getRange(teams[us], f + 1, t);
    return magic_marbleCount(range) == 0;
}

bool position::isLegal(int64 &m) const
{
    if (!move_isTwoStep(m))
    {
        return isLegalPseudo(move_getFirst(m));
    }
    else
    {
        int32 first = move_getFirst(m);
        int32 second = move_getSecond(m);
        if (!pseudomove_isValid(first)|| !pseudomove_isValid(second))
        {
            return false;
        }

        int us = pseudomove_getTeam(first);
        int firstTo = pseudomove_getTo(first);
        int firstFrom = pseudomove_getFrom(first);
        int secondTo = pseudomove_getTo(second);
        int secondFrom = pseudomove_getFrom(second);

        int64 firstpath = magic_getRange(teams[us], firstFrom + 1, firstTo);
        int64 secondpath = magic_getRange(teams[us], secondFrom + 1, secondTo);
        if (!firstpath && !secondpath)
        {
            return true;
        }
        else if (firstpath)
        {
            return false;
        }

        int64 cnt = magic_marbleCount(secondpath);
        int hit = secondFrom + magic_getLsb(secondpath) + 1;
        return cnt == 1 && firstFrom == hit && secondTo < firstTo;
    }
}

void position::genTakeOutMoves(movelist &moves, int team, int value) const
{
    int32 takeoutmove = pseudomove_make(team, -1, 0);
    int64 fulltakeout = move_make(takeoutmove, pseudomove_make(team, 0, value));
    if (isLegal(fulltakeout))
    {
        moves.add(fulltakeout);
    }

    int64 marbles = teams[team];
    while (marbles)
    {
        int curmarble = magic_popMsb(marbles);
        int32 othermove = pseudomove_make(team, curmarble, curmarble + value);

        int64 combined1 = move_make(takeoutmove, othermove);
        int64 combined2 = move_make(othermove, takeoutmove);
        if (isLegal(combined1))
        {
            moves.add(combined1);
        }
        else if (isLegal(combined2))
        {
            moves.add(combined2);
        }
    }
}

movelist position::getLegalMoves(const roll& roll, int team) const
{
    movelist legal = movelist();
    if (roll.isDoubles() && current->doublesCnt + 1 >= 3)
    {
        return legal; //No legal moves if roll doubles three times
    }

    //Generating a move
    //Can you take out?
    //	Yes-Use the value of that die to take out, then use the rest to move any of the other marbles
    //Permutations of using values from each die to move marbles

    //foreach die value
    //	If is 1 or 6 takeout, then permutate through all marbles

    int die1 = roll.getDie1();
    int die2 = roll.getDie2();

    //If can takeout
    if (magic_marbleCount(teams[team]) < MARBLE_COUNT)
    {
        //Step 1. Generate take out move combinations for die1
        if (die1 == 1 || die1 == 6)
        {
            genTakeOutMoves(legal, team, die2);
        }

        //Step 2. Generate take out move combinations for die2
        if (die2 != die1 && (die2 == 1 || die2 == 6))
        {
            genTakeOutMoves(legal, team, die1);
        }
    }

    //Step 3. Generate permutations for other marbles
    int64 marbles = teams[team];
    int64 split[MARBLE_COUNT] = { 0 };

    int i = 0;
    while (marbles)
    {
        int cur = magic_popMsb(marbles);

        int64 curallforward = move_make(pseudomove_make(team, cur, cur + die1 + die2));
        if (isLegal(curallforward))
        {
            legal.add(curallforward);
        }

        int32 cur1 = pseudomove_make(team, cur, cur + die1);
        int32 cur2 = pseudomove_make(team, cur, cur + die2);

        //Loop through all other marbles and split moves
        int j = 0;
        int64 othermarbles = teams[team];
        while (othermarbles)
        {
            int other = magic_popMsb(othermarbles);

            //Cant split with same marble
            //also don't want to gen duplicate moves for marbles we've already split with
            if (other == cur || magic_get(split[i], j))
            {
                j++;
                continue;
            }

            //Record that we've generated split moves for the j and i marbles
            magic_set(split[j], i);

            int64 splitother1 = move_make(cur1, pseudomove_make(team, other, other + die2));
            if (isLegal(splitother1))
            {
                legal.add(splitother1);
            }

            int64 splitother2 = move_make(cur2, pseudomove_make(team, other, other + die1));
            if (die2 != die1 && isLegal(splitother2))
            {
                legal.add(splitother2);
            }

            j++;
        }

        i++;
    }

    return legal;
}

int64 position::getHash() const
{
    return current->key;
}

bool position::equals(const position &pos) const
{
    for (int t = 0;t < TEAM_COUNT; t++)
    {
        if (pos.teams[t] != teams[t])
        {
            return false;
        }
    }

    return getToMove() == pos.getToMove() && getDoublesCnt() == pos.getDoublesCnt();
}