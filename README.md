Turn-Based Strategy Game with Minimax AI

Overview

This project is a simple turn-based strategy game played on a 7x7 board. The game features two players: Player 1, which is controlled by the user, and Player 2, which is controlled by a computer AI using the minimax algorithm with alpha-beta pruning. The objective of the game is to strategically move pieces on the board and prevent the opponent from making valid moves.

Features

User vs. Computer Gameplay: The user competes against a computer AI.
Minimax Algorithm: The computer uses the minimax algorithm with alpha-beta pruning for optimal decision-making.
Heuristic Evaluation: The game includes a heuristic function to evaluate the board state.
Random Initialization: Pieces are placed randomly on the board at the start of the game.
Input Validation: User inputs are validated for correct moves and positions.

Game Rules

The game is played on a 7x7 board.
Each player has a set number of pieces, placed randomly on the board at the start of the game.
Players take turns to move one of their pieces to an adjacent empty space (horizontally or vertically).
A player wins if the opponent has no valid moves left.
The game ends in a tie if the turn limit is reached without a winner.
