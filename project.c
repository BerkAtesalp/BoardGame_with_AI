#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

#define SIZE 7
#define PLAYER1 'X'
#define PLAYER2 'O'

// Function to initialize the game board
void initializeBoard(char board[SIZE][SIZE], int player1_pieces, int player2_pieces) {
    int i, j;

    // Initialize the board with empty spaces
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            board[i][j] = ' ';
        }
    }

    // Randomly place Player 1 pieces
    for (i = 0; i < player1_pieces; i++) {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        while (board[row][col] != ' ') {
            row = rand() % SIZE;
            col = rand() % SIZE;
        }
        board[row][col] = PLAYER1;
    }

    // Randomly place Player 2 pieces
    for (i = 0; i < player2_pieces; i++) {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        while (board[row][col] != ' ') {
            row = rand() % SIZE;
            col = rand() % SIZE;
        }
        board[row][col] = PLAYER2;
    }
}

// Function to print the game board
void printBoard(char board[SIZE][SIZE]) {
    int i, j;

    // Print column numbers
    printf("  ");
    for (i = 1; i <= SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");

    // Print rows and pieces
    for (i = 0; i < SIZE; i++) {
        printf("%c ", 'a' + i);
        for (j = 0; j < SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

// Function to check if a move is valid
bool isValidMove(char board[SIZE][SIZE], int fromRow, int fromCol, int toRow, int toCol, char currentPlayer) {
    if (fromRow < 0 || fromRow >= SIZE || fromCol < 0 || fromCol >= SIZE ||
        toRow < 0 || toRow >= SIZE || toCol < 0 || toCol >= SIZE) {
        return false; // Out of bounds
    }

    if (board[fromRow][fromCol] != currentPlayer || board[toRow][toCol] != ' ') {
        return false; // Invalid piece or destination
    }

    // Check if the move is horizontal or vertical
    if ((abs(fromRow - toRow) == 1 && fromCol == toCol) || (fromRow == toRow && abs(fromCol - toCol) == 1)) {
        return true; // Valid move
    }

    return false; // Invalid move
}

// Function to check if a player has a valid move
bool hasValidMove(char board[SIZE][SIZE], char currentPlayer) {
    int i, j;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (board[i][j] == currentPlayer) {
                // Check if the player has a valid move in any direction
                if ((i - 1 >= 0 && board[i - 1][j] == ' ') || (i + 1 < SIZE && board[i + 1][j] == ' ') ||
                    (j - 1 >= 0 && board[i][j - 1] == ' ') || (j + 1 < SIZE && board[i][j + 1] == ' ')) {
                    return true;
                }
            }
        }
    }

    return false; // No valid move
}

// Heuristic function
int heuristic(char board[SIZE][SIZE], char player) {
    int count = 0;

    // Count the number of pieces for the given player
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == player) {
                count++;
            }
        }
    }

    return count;
}

// Function to evaluate the board state using the heuristic
int evaluateBoard(char board[SIZE][SIZE]) {
    int scorePlayer1 = heuristic(board, PLAYER1);
    int scorePlayer2 = heuristic(board, PLAYER2);

    // The heuristic score is the difference between the number of pieces
    // Player 1's pieces are positive, and Player 2's pieces are negative
    return scorePlayer1 - scorePlayer2;
}

// Function for the computer's turn using the minimax algorithm with alpha-beta pruning
int minimax(char board[SIZE][SIZE], int depth, bool isMaximizing, int alpha, int beta) {
    if (depth == 0 || !hasValidMove(board, PLAYER1) || !hasValidMove(board, PLAYER2)) {
        return evaluateBoard(board);
    }

    int bestValue;
    if (isMaximizing) {
        bestValue = INT_MIN;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == PLAYER2) {
                    for (int direction = 0; direction < 4; direction++) {
                        int newRow = i + (direction == 1) - (direction == 0);
                        int newCol = j + (direction == 3) - (direction == 2);

                        if (isValidMove(board, i, j, newRow, newCol, PLAYER2)) {
                            char original = board[i][j];
                            board[i][j] = ' ';
                            board[newRow][newCol] = PLAYER2;

                            int moveValue = minimax(board, depth - 1, false, alpha, beta);
                            bestValue = (bestValue > moveValue) ? bestValue : moveValue;

                            alpha = (alpha > bestValue) ? alpha : bestValue;

                            board[i][j] = original;
                            board[newRow][newCol] = ' ';

                            if (beta <= alpha)
                                break;
                        }
                    }
                }
            }
        }
        return bestValue;
    } else {
        bestValue = INT_MAX;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == PLAYER1) {
                    for (int direction = 0; direction < 4; direction++) {
                        int newRow = i + (direction == 1) - (direction == 0);
                        int newCol = j + (direction == 3) - (direction == 2);

                        if (isValidMove(board, i, j, newRow, newCol, PLAYER1)) {
                            char original = board[i][j];
                            board[i][j] = ' ';
                            board[newRow][newCol] = PLAYER1;

                            int moveValue = minimax(board, depth - 1, true, alpha, beta);
                            bestValue = (bestValue < moveValue) ? bestValue : moveValue;

                            beta = (beta < bestValue) ? beta : bestValue;

                            board[i][j] = original;
                            board[newRow][newCol] = ' ';

                            if (beta <= alpha)
                                break;
                        }
                    }
                }
            }
        }
        return bestValue;
    }
}

// Function for the computer's turn with AI using minimax
void computerTurnAI(char board[SIZE][SIZE]) {
    int bestMoveValue = INT_MIN;
    int bestMoveFromRow, bestMoveFromCol, bestMoveToRow, bestMoveToCol;

    // Iterate through all possible moves for Player 2
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == PLAYER2) {
                for (int direction = 0; direction < 4; direction++) {
                    int newRow = i + (direction == 1) - (direction == 0);
                    int newCol = j + (direction == 3) - (direction == 2);

                    if (isValidMove(board, i, j, newRow, newCol, PLAYER2)) {
                        char original = board[i][j];
                        board[i][j] = ' ';
                        board[newRow][newCol] = PLAYER2;

                        // Use the minimax algorithm to find the best move with increased depth
                        int moveValue = minimax(board, 8, false, INT_MIN, INT_MAX);

                        // Update the best move if a better move is found
                        if (moveValue > bestMoveValue) {
                            bestMoveValue = moveValue;
                            bestMoveFromRow = i;
                            bestMoveFromCol = j;
                            bestMoveToRow = newRow;
                            bestMoveToCol = newCol;
                        }

                        board[i][j] = original;
                        board[newRow][newCol] = ' ';
                    }
                }
            }
        }
    }

    // Make the best move
    board[bestMoveFromRow][bestMoveFromCol] = ' ';
    board[bestMoveToRow][bestMoveToCol] = PLAYER2;

    // Print the move
    printf("Computer moves the piece at %c%d to %c%d\n", 'a' + bestMoveFromRow, bestMoveFromCol + 1, 'a' + bestMoveToRow, bestMoveToCol + 1);
}
// Function for the user's turn
void userTurn(char board[SIZE][SIZE]) {
    char fromCoord[3], toCoord[3];
    int fromRow, fromCol, toRow, toCol;

    // Get the piece to move from the user
    do {
        printf("Choose piece to move: ");
        scanf("%s", fromCoord);
        fromRow = fromCoord[0] - 'a';
        fromCol = fromCoord[1] - '0' - 1;
    } while (board[fromRow][fromCol] != PLAYER1 || !hasValidMove(board, PLAYER1));

    // Get the new position from the user
    do {
        printf("Choose the new position for %s: ", fromCoord);
        scanf("%s", toCoord);
        toRow = toCoord[0] - 'a';
        toCol = toCoord[1] - '0' - 1;
    } while (!isValidMove(board, fromRow, fromCol, toRow, toCol, PLAYER1));

    // Make the move
    board[fromRow][fromCol] = ' ';
    board[toRow][toCol] = PLAYER1;

    // Print the move
    printf("Player moves the piece at %s to %s\n", fromCoord, toCoord);
}

int main() {
    char board[SIZE][SIZE];
    int player1_pieces, player2_pieces, turnLimit;
    int currentPlayer = 1; // 1 for Player 1, 2 for Player 2

    srand(time(NULL)); // Seed for random number generation

    // Get user input for game setup
    printf("Enter the number of pieces for Player 1: ");
    scanf("%d", &player1_pieces);
    printf("Enter the number of pieces for Player 2: ");
    scanf("%d", &player2_pieces);
    printf("Enter the turn limit: ");
    scanf("%d", &turnLimit);

    // Initialize the game board
    initializeBoard(board, player1_pieces, player2_pieces);

    // Main game loop
    for (int turn = 1; turn <= turnLimit; turn++) {
        printf("\nTurn %d:\n", turn);
        printBoard(board);

        if (currentPlayer == 1) {
            userTurn(board);
        } else {
            computerTurnAI(board); // Use the AI version
        }

        // Check for a winner or tie
        if (!hasValidMove(board, PLAYER1)) {
            printf("\nPlayer 2 (Computer) wins! Player 1 has no valid moves.\n");
            break;
        } else if (!hasValidMove(board, PLAYER2)) {
            printf("\nPlayer 1 wins! Player 2 (Computer) has no valid moves.\n");
            break;
        }

        // Switch to the other player
        currentPlayer = 3 - currentPlayer; // Toggle between 1 and 2
    }

    return 0;
}