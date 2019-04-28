#include <stdio.h>
#include <stdlib.h>

void parse_file(char *fileName, int *gameBoard, int *gameMask)
{
    FILE *gameFile = fopen("game", "r");
    // need 18 bits
    int board = 0;
    int mask = 0;
    char line[3] = {0};
    while (fscanf(gameFile, "%c | %c | %c\n", &line[0], &line[1], &line[2]) != EOF)
    {
        for (int i = 0; i < sizeof(line) / sizeof(char); i++)
        {
            board <<= 0x2;
            mask <<= 0x2;
            switch (line[i])
            {
            case '-':
                board |= 0x0;
                mask |= 0x0;
                break;
            case 'O':
                board |= 0x1;
                mask |= 0x3;
                break;
            case 'X':
                board |= 0x2;
                mask |= 0x3;
                break;
            default:
                perror("Bad data.");
                exit(EXIT_FAILURE);
            }
        }
        *gameBoard = board;
        *gameMask = mask;
    }
    if (ferror(gameFile) != 0)
    {
        perror("Failed to read game file.");
        exit(EXIT_FAILURE);
    }
    fclose(gameFile);
}
int checkWin(int winningGame, int bitField, int bitMask)
{
    if ((bitField & winningGame) == winningGame)
        return 10;
    else if (((bitField ^ bitMask) & winningGame) == winningGame)
        puts("X wins");
}
int main(void)
{
    int bitField, bitMask;
    // parse tic-tac-toe board - 
    parse_file("game", &bitField, &bitMask);
    const int winningGames[] = {
        /* 01 00 00 00 01 00 00 00 01 */ 0x10101,
        /* 00 00 01 00 01 00 01 00 00 */ 0x01110,
        /* 01 01 01 00 00 00 00 00 00 */ 0x15000,
        /* 01 00 00 01 00 00 01 00 00 */ 0x10410,
    };
    checkWin(winningGames[0], bitField, bitMask);
    checkWin(winningGames[1], bitField, bitMask);
    int winningGame = winningGames[2];
    for (int j = 0; j < 3; j++)
    {
        checkWin(winningGame, bitField, bitMask);
        winningGame >>= 3;
    }
    winningGame = winningGames[3];
    for (int j = 0; j < 3; j++)
    {
        checkWin(winningGame, bitField, bitMask);
        winningGame >>= 1;
    }

    return 0;
}
