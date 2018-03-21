# Bach
Bach is a command line program that can play [Marbleboard](https://github.com/ajosg/MarbleBoardGame). 

# Commands
```
newgame - starts a new game
roll [x+y] - sets the current players roll, ex: roll 5+1
move [move] - plays the move for the current player
tomove - outputs the current player's turn
moves - outputs all the legal moves for the current position
go infinite - Searches until the stop command is called to find the best move.
go [time_milliseconds] - searches for the specified time to find the best move.
stop - Stops searching
quit - Exits the program
```

# Implementation Details
Written originally in C#, but C# is too slow and doesn't provide nice features like implicit int to bool operations, and inlining is just ugly in C#. Major speed ups from the magic class which takes advantage of the fact that all holes represent a single bit and that each player only has 55 holes, so each player's path can be represented by a long. Hence, for implementing the states, I used an array of 4 longs. Magic.h holds all the bit manipulations schemes. Searching is threaded using native pthreads, and the search uses monte-carlo tree search, because it out performs any attempt to design an evaluation function and perform a MaxN search. A lot of inspiration from this project's design was from the chess engine, [Stockfish](https://github.com/official-stockfish).
