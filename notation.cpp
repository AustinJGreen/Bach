//
// Created by austi on 12/9/2017.
//

#include "notation.h"

std::string notation::teams[TEAM_COUNT] = { "yellow", "red", "green", "blue" };
char notation::team_chars[TEAM_COUNT] = { 'y', 'r', 'g', 'b' };

notation::notation()
{
}

roll notation::fromStr(std::string rollStr)
{
    int num1 = rollStr[0] - 48;
    int num2 = rollStr[2] - 48;
    return roll(num1, num2);
}

int notation::fromStr(std::string sq, int perspective)
{
    if (sq[0] == 'h')
    {
        if (sq.length() < 3)
        {
            return -1;
        }

        int value = std::stoi(sq.substr(2));
        return 47 + value;
    }
    else
    {
        //Find team char
        for (int t = 0; t < TEAM_COUNT; t++)
        {
            if (sq[0] == team_chars[t])
            {
                int yellow_value = 12 * t;
                int value = std::stoi(sq.substr(1)) - 1; //Indexed at 1

                int64 m = 0;
                magic_set(m, value);

                return magic_getMsb(magic_getRot48(m, t, perspective));
            }
        }
    }

    return -1;
}

std::string notation::toStr(int sq, int perspective)
{
    assert(perspective >= 0 && perspective < TEAM_COUNT);
    std::string str = std::string();
    if (sq >= 48)
    {
        str.append("h");
        str.push_back(team_chars[perspective]);

        int quadValue = (sq % 12) + 1;
        str.append(std::to_string(quadValue));
    }
    else
    {
        int64 m = 0;
        magic_set(m, sq);
        int64 rot = magic_getRot48(m, perspective, 0);
        int val = magic_getMsb(rot);

        int quadIndex = val / 12;
        str.push_back(team_chars[quadIndex]);

        int quadValue = (val % 12) + 1;
        str.append(std::to_string(quadValue));
    }

    return str;
}

std::string notation::toStr(int64 move)
{
    if (move == NONE)
    {
        return "NONE";
    }

    std::string str = std::string();
    str.append(toStr(move_getFirst(move)));
    if (move_isTwoStep(move))
    {
        str.append(", ");
        str.append(toStr(move_getSecond(move)));
    }

    return str;
}

std::string notation::toStr(int32 move)
{
    int team = pseudomove_getTeam(move);
    int from = pseudomove_getFrom(move);
    int to = pseudomove_getTo(move);

    std::string str = std::string();
    if (!pseudomove_isTakeout(move))
    {
        str.append(toStr(from, team));
    }

    str.append(toStr(to, team));
    return str;

}

std::string notation::toStr(int team)
{
    assert(team >= 0 && team < TEAM_COUNT);
    return teams[team];
}