//
// Created by austi on 12/9/2017.
//

#ifndef BACH_POSITION_H
#define BACH_POSITION_H

#include "types.h"
#include "magic.h"
#include "pseudomove.h"
#include "move.h"
#include "movelist.h"
#include "roll.h"
#include "pstate.h"
#include "prng.h"

#include <string>

class position
{
    friend class tests;

private:
    bool teamsMode;
    bool disposed;
    pstate *current;
    int64 teams[TEAM_COUNT];

    /**
     * Tests if a pseudomove is legal
     * @param m The pseudomove to test
     * @return True if the pseudomove is legal; otherwise False
     */
    bool isLegalPseudo(int32 m) const;

    /**
     * Performs a pseudomove
     * @param m The pseudomove to do
     */
    void doMove(int32 &m);

    /**
     * Undoes a pseudomove performed
     * @param m The pseudomove performed that is to be undone
     */
    void undoMove(int32 &m);

    /**
     * Generates take out moves
     * @param moves The movelist to generate the moves into
     * @param team The team to generate the moves for
     * @param value The value of the die not used to take the marble out
     */
    void genTakeOutMoves(movelist &moves, int team, int value) const;

    /**
     * Copies the state pointer into the current state
     * @param state The state pointer to copy from
     */
    void copyState(pstate *state);

    /**
     * Deletes the positions states
     */
    void deleteStates();
public:

    /**
     * Initializes the positions zobrist values
     */
    static void init();

    /**
     * Default position constructor
     */
    position();

    /**
     * Create a new position from values
     * @param teams The value of each team's position
     * @param toMove The player who's turn it is to move
     * @param doublesCnt The amount of doubles the current player has rolled in a row
     * @param removed The hole of a removed marble due to rolling doubles
     */
    position(int64 teams[TEAM_COUNT], int toMove, int doublesCnt, int removed);

    /**
     * Copy constructor
     * @param pos The position to copy the value from
     */
    position(const position& pos);

    /**
     * Position deconstructor
     */
    ~position();

    /**
     * Disposes of allocated resources
     */
    void dispose();

    /**
     * Gets if the position is currently teams if True or 4-player if False
     * @return
     */
    bool isTeamsMode() const;

    /**
     * Gets the current player who is moving
     * @return
     */
    int getToMove() const;

    /**
     * Gets the amount of doubles the current player has rolled
     * @return
     */
    int getDoublesCnt() const;

    /**
     * Tests if a position is valid
     * @return True if valid; otherwise false
     */
    bool isValid() const;

    /**
     * Converts the position to a readable string
     * @return A readable string that represents the position
     */
    std::string toString() const;

    /**
     * Sets the positions team mode
     * @param teamsMode True for teams, False for 4-player
     */
    void setTeamsMode(bool teamsMode);

    /**
     * Sets a marble for a team in that teams relative hole number
     * @param team The team to set a marble for
     * @param hole The hole number to set the marble in
     */
    void set(std::string sq, int team);

    /**
     * Sets a marble for a team in that teams relative hole number
     * @param team The team to set a marble for
     * @param hole The hole number to set the marble in
     */
    void set(int team, int hole);

    /**
     * Removes the leading unsafe marble from a team
     * @param team The team to remove the marble from
     * @return The position of the leading marble that was removed
     */
    int removeLeading(int team);

    /**
     * Resets the position to the starting position
     */
    void reset();

    /**
     * Sets the current player to the next player and increments the state
     */
    void next(roll r);

    /**
     * Performs a move to the current position
     * @param m The move to perform
     */
    void doMove(int64 &m, roll r);

    /**
     * Undoes a move performed
     * @param m The move performed that is to be undone
     */
    void undoMove(int64 &m);

    /**
     * Gets the value of a team's state on the board
     * @param team The team to get the value of
     * @return The value of a team's state on the board
     */
    int64 get(int team) const;

    /**
     * Gets the amount of active marbles that a team has
     * @param team The team to lookup
     * @return The amount of active marbles that a team has
     */
    int64 marbleCount(int team) const;

    /**
     * Gets the winner of the position if any
     * @return The winner of the position; otherwise -1
     */
    int getWinner() const;

    /**
     * Tests if a move is legal
     * @param m The move to test
     * @return True if the move is legal; otherwise False
     */
    bool isLegal(int64 &m) const;

    /**
     * Gets a list of legal moves
     * @param roll The dice roll
     * @param team The player
     * @return A list of legal moves for the given player and roll
     */
    movelist getLegalMoves(const roll& roll, int team) const;

    /**
     * Computes and returns the hash of the position
     */
    int64 getHash() const;

    /**
     * Tests if this position is equal to another position
     * @param pos The position to test equivalence with
     * @return True if equal; otherwise False
     */
    bool equals(const position &pos) const;
};

#endif