#include <stdio.h>
#include <stdlib.h>
#define PLAYER_O 1
#define PLAYER_X 2
#define MASK 3

static const char *PLAYER_O_WINS = "O wins";
static const char *PLAYER_X_WINS = "X wins";
static const int winning_layouts[] = {
	/* 01 00 00 00 01 00 00 00 01 */ 0x10101,
	/* 00 00 01 00 01 00 01 00 00 */ 0x01110,
	/* 01 01 01 00 00 00 00 00 00 */ 0x15000,
	/* 01 00 00 01 00 00 01 00 00 */ 0x10410,
};

void parse_file(FILE * gameFile, int *gameBoard, int *gameMask)
{
	// need 18 bits
	int board = 0;
	int mask = 0;
	char line[3] = { 0 };
	while (fscanf(gameFile, "%c | %c | %c\n", &line[0], &line[1], &line[2])
	       != EOF) {
		for (int i = 0; i < sizeof(line) / sizeof(char); i++) {
			board <<= 2;
			mask <<= 2;
			switch (line[i]) {
			case '-':
			case ' ':
				board |= 0;
				mask |= 0;
				break;
			case 'O':
				board |= PLAYER_O;
				mask |= 3;
				break;
			case 'X':
				board |= 2;
				mask |= 3;
				break;
			default:
				perror("Bad data.");
				exit(EXIT_FAILURE);
			}
		}
		*gameBoard = board;
		*gameMask = mask;
	}
	if (ferror(gameFile) != 0) {
		perror("Failed to read game file.");
		exit(EXIT_FAILURE);
	}
	fclose(gameFile);
}

unsigned char validate_winner(int winning_layout, int bitField, int bitMask)
{
	if ((bitField & winning_layout) == winning_layout) {
		return PLAYER_X;
	} else if (((bitField ^ bitMask) & winning_layout) == winning_layout) {
		return PLAYER_O;
	}
}

void print_winner(unsigned char winner)
{
	switch (winner) {
	case PLAYER_O:
		puts(PLAYER_O_WINS);
		return;
	case PLAYER_X:
		puts(PLAYER_X_WINS);
		return;
	default:
		return;
	}
}

int main(void)
{
	int bitField, bitMask;
	FILE *gameFile = fopen("game", "r");
	parse_file(gameFile, &bitField, &bitMask);

	unsigned char winner =
	    validate_winner(winning_layouts[0], bitField, bitMask) ||
	    validate_winner(winning_layouts[1], bitField, bitMask);
	if (winner) {
		print_winner(winner);
		return 0;
	}

	int winning_layout = winning_layouts[2];
	for (int j = 0; j < 3; j++) {
		winner |= validate_winner(winning_layout, bitField, bitMask);
		if (winner) {
			print_winner(winner);
			return 0;
		}
		winning_layout >>= 3;
	}

	winning_layout = winning_layouts[3];
	for (int j = 0; j < 3; j++) {
		winner |= validate_winner(winning_layout, bitField, bitMask);
		if (winner) {
			print_winner(winner);
			return 0;
		}
		winning_layout >>= 1;
	}

	return 0;
}
