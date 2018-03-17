//
// Created by austi on 12/9/2017.
//

#include "eval.h"

eval::eval()
{
    for (int i = 1; i <= 6; i++)
    {
        for (int j = 1; j <= 6; j++)
        {
            all_rolls[((i - 1) * 6) + j - 1] = roll(i, j);
        }
    }

    for (int k = 1; k <= 6; k++)
    {
        doubles_rolls[k - 1] = roll(k, k);
    }

    for (int d = 0; d < 36; d++)
    {
        dis_lookup[d] = calc_lookup(d, 0, false);
        dis_lookup_doubles[d] = calc_lookup(d, 0, true);
    }
}

float eval::calc_lookup(int dis, int doubleCnt, bool doublesOnly) const
{
    if (doubleCnt == 3)
    {
        return 0;
    }

    float prob = 0;
    for (int i = 0; i < 36; i++)
    {
        if (all_rolls[i].isDoubles())
        {
            if (doubleCnt == 2)
            {
                continue;
            }

            if (all_rolls[i].total() == dis)
            {
                prob += (1 / 36.0f);
                continue;
            }
            else
            {
                prob += (1 / 36.0f) * calc_lookup(dis - all_rolls[i].total(), doubleCnt + 1, doublesOnly);
            }
        }
        else if (all_rolls[i].total() == dis && (!doublesOnly || doubleCnt > 0))
        {
            prob += (1 / 36.0f);
        }
    }

    return prob;
}

float eval::calc_prob(int64 player, int from, int to) const
{
    int dis = to - from;
    assert(dis > 0);

    int64 inpath = magic_getRange(player, from + 1, to);
    if (magic_marbleCount(inpath) > 1)
    {
        //Cannot get to target, impossible to move 3 marbles, we need doubles
        float prob = 0;

        for (int j = 0; j < 6; j++)
        {

        }

        return 0;
    }
    else if (inpath)
    {
        //If distance is greater than 6 then we need doubles
        if (dis > 6)
        {

            return 0;
        }

        //get mobility of marble in front of us
        //to see probability of getting a roll
        int64 mobility = inpath >> magic_getLsb(inpath) + 1;
        int mobility_dis = magic_getLsb(mobility);
    }


    //Pseudocode
    //If marbles in the way, we need rolls that will move marble out of the way and move from marble the distance
    //	-Check if possible before calculating
    //If no marbles in the way check for two cases
    //	-If we can split our roll with another marble
    //	-If we can split out roll with a new marble

    if (dis > 6)
    {
        //use lookup table
        return dis_lookup[dis];
    }
    else if (magic_marbleCount(player) < MARBLE_COUNT)
    {
        return (20 / 36.0f) + dis_lookup_doubles[dis];
    }
    else
    {
        //Can use
        int max_split = 0;

        int64 m = player;
        if (magic_marbleCount(m) >= 2)
        {
            while (m)
            {
                int marble = magic_popMsb(m);
                int pos = marble;
                int holes_ahead = 52 - pos;

                int64 sixahead = player >> (pos + 1) & (1 << 6) - 1;

                int split = 0;
                if (!sixahead)
                {
                    split = holes_ahead < 6 ? holes_ahead : 6;
                }
                else
                {
                    int lsb = holes_ahead < 6 ? holes_ahead : magic_getLsb(sixahead);
                    split = lsb > split ? lsb : split;
                }

                max_split = split > max_split ? split : max_split;
            }
        }

        if (dis < max_split)
        {
            return (11 / 36.0f) + dis_lookup_doubles[dis];
        }
        else
        {
            return dis_lookup_doubles[dis];
        }
    }
}

float eval::score_marble(const position& pos, int us, int h) const
{
    //Distance traveled
    int64 safe_inv = magic_getSafe(pos.get(us)) ^ 8725724278030336;
    int farthest = magic_getMsb(safe_inv);
    int goal = farthest - 1;

    float distancePer = (h + 1) / (float)(goal + 1);

    //magic mobilitymagic = pos.get(us) >> h.pos() + 1;

    //float mobility = mobilitymagic.getLsb() / (float)goal;
    return (distancePer * distancePer);
}

float eval::score_team(const position& pos, int team) const
{
    float score = 0;
    int64 teammarbles = pos.get(team);

    while (teammarbles)
    {
        int h = magic_popMsb(teammarbles);
        score += score_marble(pos, team, h);
    }

    return score;
}

score eval::score_pos(const position& pos) const
{
    score s = score();
    for (int t = 0; t < TEAM_COUNT; t++)
    {
        s.set(t, score_team(pos, t));
    }

    return s;
}

float eval::evaluate_marble(const position& pos, int team, int h) const
{
    position p = pos;
    int64 safe_inv = magic_getSafe(pos.get(team)) ^ 8725724278030336;
    int farthest = magic_getMsb(safe_inv);
    int goal = farthest - 1;

    float distance = (h + 1) / (float)(goal + 1);

    int64 u = 0;
    magic_set(u, h);

    //Calculate probability of being taken
    float takeProb = 0;
    int curPlayer = NEXT_TEAM(team);
    while (curPlayer != team)
    {
        for (int r = 0; r < 21; r++)
        {
            roll cur = ALL_NO_DUPL[r];
            float curprob = cur.isDoubles() ? (1 / 36.0f) : (2 / 36.0f);

            bool canTake = false;
            movelist curMoves = pos.getLegalMoves(cur, curPlayer);
            for (int m = 0; m < curMoves.count(); m++)
            {
                int64 curMove = curMoves[m];
                p.doMove(curMove, cur);
                int32 first = move_getFirst(curMove);
                int32 second = move_getSecond(curMove);
                p.undoMove(curMove);

                if (pseudomove_getCapturedTeam(first) == team || pseudomove_getCapturedTeam(second) == team)
                {
                    canTake = true;
                    break;
                }
            }

            if (canTake)
            {
                takeProb += curprob;
            }
        }

        curPlayer = NEXT_TEAM(curPlayer);
    }

    if (takeProb > 1)
    {
        return 0;
    }
    else
    {
        return distance * (1 - takeProb);
    }
}

float eval::evaluate_team(const position& pos, int team) const
{
    float score = 0;
    int64 teammarbles = pos.get(team);

    while (teammarbles)
    {
        int h = magic_popMsb(teammarbles);
        float ceval = evaluate_marble(pos, team, h);
        score += ceval;
    }

    return score;
}

score eval::evaluate_pos(const position& pos) const
{
    score s = score();
    for (int t = 0; t < TEAM_COUNT; t++)
    {
        s.set(t, evaluate_team(pos, t));
    }

    return s;
}