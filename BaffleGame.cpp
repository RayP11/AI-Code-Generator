
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAXCOLUMN = 10;
const int MAXROW = 10;

enum Direction { LEFT, RIGHT, UP, DOWN };
enum Level { BEGIN = 4, INTERMEDIATE = 7, ADVANCED = 10 };
enum Grid { LEFTBAFFLE, RIGHTBAFFLE, EMPTY, LEFTFOUND, RIGHTFOUND };

Grid board[MAXROW][MAXCOLUMN];

void PrintBoard(Grid board[MAXROW][MAXCOLUMN], bool foundOnly);
void PrintScore(int numLaserShot, int numGuess);
void SetBoard(Grid board[MAXROW][MAXCOLUMN], Level gameLevel);
int TrackLaser(Grid board[MAXROW][MAXCOLUMN], int laserX, int laserY, Direction dir);
bool MakeGuess(Grid board[MAXROW][MAXCOLUMN], int guessR, int guessC, Direction dir);

int main() {
    srand(time(0));
    int numLaserShot = 0, numGuess = 0;
    Level gameLevel;
    char command;
    bool gameWon = false;

    cout << "Welcome to the Baffle Game!" << endl;
    cout << "Choose game level: (B)eginner, (I)ntermediate, (A)dvanced: ";
    char levelChoice;
    cin >> levelChoice;

    switch (levelChoice) {
        case 'B': case 'b': gameLevel = BEGIN; break;
        case 'I': case 'i': gameLevel = INTERMEDIATE; break;
        case 'A': case 'a': gameLevel = ADVANCED; break;
        default: cout << "Invalid choice. Defaulting to Beginner level." << endl; gameLevel = BEGIN;
    }

    SetBoard(board, gameLevel);

    while (!gameWon) {
        cout << "Enter command (L)aser, (G)uess, (S)core, (P)rint, (C)heat, (Q)uit: ";
        cin >> command;

        switch (command) {
            case 'L': case 'l': {
                int entryPoint;
                cout << "Enter laser entry point (0-39): ";
                cin >> entryPoint;
                if (entryPoint < 0 || entryPoint > 39) {
                    cout << "*** Invalid entry point - Try again ***" << endl;
                    break;
                }
                numLaserShot++;
                int exitPoint = TrackLaser(board, entryPoint, 0, RIGHT);
                cout << "Laser shot #" << numLaserShot << " exited the box at " << exitPoint << "." << endl;
                break;
            }
            case 'G': case 'g': {
                int guessR, guessC;
                char dirChar;
                cout << "Enter guess row (0-9), column (0-9), and direction (L/R): ";
                cin >> guessR >> guessC >> dirChar;
                Direction dir = (dirChar == 'L' || dirChar == 'l') ? LEFT : RIGHT;
                if (guessR < 0 || guessR >= MAXROW || guessC < 0 || guessC >= MAXCOLUMN) {
                    cout << "*** Guess out of bounds - Try again ***" << endl;
                    break;
                }
                numGuess++;
                if (MakeGuess(board, guessR, guessC, dir)) {
                    cout << "This is guess #" << numGuess << ". Congratulations, you have now found a baffle." << endl;
                } else {
                    cout << "This is guess #" << numGuess << ". Sorry, better luck next time." << endl;
                }
                break;
            }
            case 'S': case 's':
                PrintScore(numLaserShot, numGuess);
                break;
            case 'P': case 'p':
                PrintBoard(board, true);
                break;
            case 'C': case 'c':
                PrintBoard(board, false);
                break;
            case 'Q': case 'q':
                gameWon = true;
                break;
            default:
                cout << "*** Illegal command - Try again ***" << endl;
        }
    }

    cout << "Thank you for playing the Baffle Game!" << endl;
    return 0;
}

void PrintBoard(Grid board[MAXROW][MAXCOLUMN], bool foundOnly) {
    cout << "  10  11  12  13  14  15  16  17  18  19" << endl;
    for (int i = 0; i < MAXROW; i++) {
        cout << "+---+---+---+---+---+---+---+---+---+---+" << endl;
        cout << 9 - i << "|";
        for (int j = 0; j < MAXCOLUMN; j++) {
            if (foundOnly) {
                if (board[i][j] == LEFTFOUND) cout << " \\ |";
                else if (board[i][j] == RIGHTFOUND) cout << " / |";
                else cout << "   |";
            } else {
                if (board[i][j] == LEFTBAFFLE || board[i][j] == LEFTFOUND) cout << " \\ |";
                else if (board[i][j] == RIGHTBAFFLE || board[i][j] == RIGHTFOUND) cout << " / |";
                else cout << "   |";
            }
        }
        cout << 20 + i << endl;
    }
    cout << "+---+---+---+---+---+---+---+---+---+---+" << endl;
    cout << "  39  38  37  36  35  34  33  32  31  30" << endl;
}

void PrintScore(int numLaserShot, int numGuess) {
    cout << "Number of shots: " << numLaserShot << endl;
    cout << "Number of guesses: " << numGuess << endl;
    cout << "Current score: " << (numLaserShot + 2 * numGuess) << endl;
}

void SetBoard(Grid board[MAXROW][MAXCOLUMN], Level gameLevel) {
    for (int i = 0; i < MAXROW; i++) {
        for (int j = 0; j < MAXCOLUMN; j++) {
            board[i][j] = EMPTY;
        }
    }

    int bafflesSet = 0;
    while (bafflesSet < gameLevel) {
        int row = rand() % MAXROW;
        int col = rand() % MAXCOLUMN;
        if (board[row][col] == EMPTY) {
            board[row][col] = (rand() % 2 == 0) ? LEFTBAFFLE : RIGHTBAFFLE;
            bafflesSet++;
        }
    }
}

int TrackLaser(Grid board[MAXROW][MAXCOLUMN], int laserX, int laserY, Direction dir) {
    int row, col;
    if (laserX < 10) { // Left edge
        row = 9 - laserX;
        col = -1;
        dir = RIGHT;
    } else if (laserX < 20) { // Top edge
        row = -1;
        col = laserX - 10;
        dir = DOWN;
    } else if (laserX < 30) { // Right edge
        row = 29 - laserX;
        col = 10;
        dir = LEFT;
    } else { // Bottom edge
        row = 10;
        col = 39 - laserX;
        dir = UP;
    }

    while (true) {
        switch (dir) {
            case RIGHT: col++; break;
            case LEFT: col--; break;
            case UP: row--; break;
            case DOWN: row++; break;
        }

        if (row < 0) return 10 + col; // Top edge
        if (row >= MAXROW) return 39 - col; // Bottom edge
        if (col < 0) return 9 - row; // Left edge
        if (col >= MAXCOLUMN) return 20 + row; // Right edge

        if (board[row][col] == LEFTBAFFLE || board[row][col] == LEFTFOUND) {
            switch (dir) {
                case RIGHT: dir = DOWN; break;
                case LEFT: dir = UP; break;
                case UP: dir = RIGHT; break;
                case DOWN: dir = LEFT; break;
            }
        } else if (board[row][col] == RIGHTBAFFLE || board[row][col] == RIGHTFOUND) {
            switch (dir) {
                case RIGHT: dir = UP; break;
                case LEFT: dir = DOWN; break;
                case UP: dir = LEFT; break;
                case DOWN: dir = RIGHT; break;
            }
        }
    }
}

bool MakeGuess(Grid board[MAXROW][MAXCOLUMN], int guessR, int guessC, Direction dir) {
    if (board[guessR][guessC] == LEFTBAFFLE && dir == LEFT) {
        board[guessR][guessC] = LEFTFOUND;
        return true;
    } else if (board[guessR][guessC] == RIGHTBAFFLE && dir == RIGHT) {
        board[guessR][guessC] = RIGHTFOUND;
        return true;
    }
    return false;
}
