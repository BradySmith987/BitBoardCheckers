#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

unsigned long long SetBit(unsigned long long value, int position)
{
    if (position < 0 || position >= 64) return value;
    return value | (1ULL << position);
}

unsigned long long ClearBit(unsigned long long value, int position)
{
    if (position < 0 || position >= 64) return value;
    return value & ~(1ULL << position);
}

unsigned long long ToggleBit (unsigned long long value, int position)
{
    if (position < 0 || position >= 64) return value;
    return value ^ (1ULL << position);
}

int GetBit (unsigned long long value, int position) 
{
    if (position < 0 || position >= 64) return value;
    int bit = (value >> position) & 1;
    return bit;
}

int CountBits(unsigned long long value)
{
    int bit;
    int count = 0;
    for (int i = 0; i < 32; i++)
    {
        bit = (value >> i) & 1;
        if (bit == 1)
            count++;
    }
    return count;
}

unsigned int ShiftLeft(unsigned int value, int positions)
{
    if (positions < 0 || positions >= 32) return value;
    return (value << positions);
}

unsigned int ShiftRight(unsigned int value, int positions)
{
    if (positions < 0 || positions >= 32) return value;
    return (value >> positions);
}

void PrintBinary(unsigned long long value)
{
    char temp[65];
    int pos = 0;
    int base = 2;
    char out[65];

    if (value == 0)
    {
        out[0] = '0';
        out[1] = '\0';
        printf("%s\n", out);
        return;
    }

    while (value > 0)
    {
        int remainder = value % base;
        value = value / base;

        if (remainder < 10)
            temp[pos++] = '0' + remainder;
        else
            temp[pos++] = 'A' + (remainder - 10);
    }

    for (int i = 0; i < pos; i++)
    {
        out[i] = temp[pos - i - 1];
    }
    out[pos] = '\0';
    printf("%s\n", out);
}

void PrintHex(unsigned long long value)
{
    char temp[65];
    int pos = 0;
    int base = 16;
    char out[65];

    if (value == 0)
    {
        out[0] = '0';
        out[1] = '\0';
        printf("%s\n", out);
        return;
    }

    while (value > 0)
    {
        int remainder = value % base;
        value = value / base;

        if (remainder < 10)
            temp[pos++] = '0' + remainder;
        else
            temp[pos++] = 'A' + (remainder - 10);
    }

    for (int i = 0; i < pos; i++)
    {
        out[i] = temp[pos - i - 1];
    }
    out[pos] = '\0';
    printf("%s\n", out);
}



typedef struct {
    unsigned long long player1_pieces;
    unsigned long long player1_kings;
    unsigned long long player2_pieces;
    unsigned long long player2_kings;
    unsigned long long board;
    int currentTurn;            //'1' is player 1's turn, '2' is player two's turn
}Game;

void DisplayBoard(unsigned long long p1p, unsigned long long p1k, unsigned long long p2p, unsigned long long p2k)
{
    char p1pSymbol = '1';
    char p1kSymbol = '2';
    char p2pSymbol = '3';
    char p2kSymbol = '4';
    char emptySymbol = ' ';
    printf("INDEX: \"%c\" = Player 1 Pieces | \"%c\" = Player 1 Kings | \"%c\" = Player 2 Pieces | \"%c\" = Player 2 Kings | \"%c\" = Empty Square\n",
         p1pSymbol, p1kSymbol, p2pSymbol, p2kSymbol, emptySymbol);
    for (int row = 7; row >= 0; row--) 
    {    
        printf("%d\t", row * 8);
        for (int col = 0; col < 8; col++) 
        {     
            int index = row * 8 + col;
            if ((p1p >> index) & 1)
                printf("[%c]", p1pSymbol);
            else if ((p1k >> index) & 1)
                printf("[%c]", p1kSymbol);
            else if ((p2p >> index) & 1)
                printf("[%c]", p2pSymbol);
            else if ((p2k >> index) & 1)
                printf("[%c]", p2kSymbol);
            else
                printf("[%c]", emptySymbol);
        }
        printf("\n");
    }
    printf("\n\t 0  1  2  3  4  5  6  7\n");
}

int isOccupied(Game *game, int position)
{
    return ((game -> board >> position) & 1);
}

int canCapture(Game *game, int position, int direction)
{
    int col = position % 8;
    if (GetBit(game->player1_kings, position) == 1)
    {
        if (direction == 1)
        {
            if ((((game->player2_kings >> position + 7) & 1) || ((game->player2_pieces >> position + 7) & 1)) && col >= 2 && isOccupied(game, position + 14) == 0)
                return 1;
            else
                return 0;
        }
        else if (direction == 2)
        {
            if ((((game->player2_kings >> position + 9) & 1) || ((game->player2_pieces >> position + 9) & 1)) && col <= 5 && isOccupied(game, position + 18) == 0)
                return 1;
            else
                return 0;
        }
        else if (direction == 3)
        {
            if ((((game->player2_kings >> position - 9) & 1) || ((game->player2_pieces >> position - 9) & 1)) && col >= 2 && isOccupied(game, position - 18) == 0)
                return 1;
            else
                return 0;
        }
        else if (direction == 4)
        {
            if ((((game->player2_kings >> position - 7) & 1) || ((game->player2_pieces >> position - 7) & 1)) && col <= 5 && isOccupied(game, position - 14) == 0)
                return 1;
            else
                return 0;
        }
        else
            return 0;
    }
    else if (GetBit(game->player1_pieces, position) == 1)
    {
        if (direction == 1)
        {
            if ((((game->player2_kings >> position + 7) & 1) || ((game->player2_pieces >> position + 7) & 1)) && col >= 2 && isOccupied(game, position + 14) == 0)
                return 1;
            else
                return 0;
        }
        else if (direction == 2)
        {
            if ((((game->player2_kings >> position + 9) & 1) || ((game->player2_pieces >> position + 9) & 1)) && col <= 5 && isOccupied(game, position + 18) == 0)
                return 1;
            else
                return 0;
        }
        else if (direction == 3)
            return 0;
        else if (direction == 4)
            return 0;
        else
            return 0;
    }
    else if (GetBit(game->player2_kings, position) == 1)
    {
        if (direction == 1)
        {
            if ((((game->player1_kings >> position + 7) & 1) || ((game->player1_pieces >> position + 7) & 1)) && col >= 2 && isOccupied(game, position + 14) == 0)
                return 1;
            else
                return 0;
        }
        else if (direction == 2)
        {
            if ((((game->player1_kings >> position + 9) & 1) || ((game->player1_pieces >> position + 9) & 1)) && col <= 5 && isOccupied(game, position + 18) == 0)
                return 1;
            else
                return 0;
        }
        else if (direction == 3)
        {
            if ((((game->player1_kings >> position - 9) & 1) || ((game->player1_pieces >> position - 9) & 1)) && col >= 2 && isOccupied(game, position - 18) == 0)
                return 1;
            else
                return 0;
        }
        else if (direction == 4)
        {
            if ((((game->player1_kings >> position - 7) & 1) || ((game->player1_pieces >> position - 7) & 1)) && col <= 5 && isOccupied(game, position - 14) == 0)
                return 1;
            else
                return 0;
        }
        else
            return 0;
    }
    else if (GetBit(game->player2_pieces, position) == 1)
    {
        if (direction == 1)
            return 0;
        else if (direction == 2)
            return 0;
        else if (direction == 3)
        {
            if ((((game->player1_kings >> position - 9) & 1) || ((game->player1_pieces >> position - 9) & 1)) && col >= 2 && isOccupied(game, position - 18) == 0)
                return 1;
            else
                return 0;
        }
        else if (direction == 4)
        {
            if ((((game->player1_kings >> position - 7) & 1) || ((game->player1_pieces >> position - 7) & 1)) && col <= 5 && isOccupied(game, position - 14) == 0)
                return 1;
            else
                return 0;
        }
        else
            return 0;
    }
    else
        return 0;
}

int isValidMove(Game *game, int position, int direction)
{
    int col = position % 8;
    
    if (position < 0 || position > 63) return 0;
    if (direction < 1 || direction > 4) return 0;

    if (direction == 1 && col == 0) return 0; // up-left from left edge
    if (direction == 2 && col == 7) return 0; // up-right from right edge
    if (direction == 3 && col == 0) return 0; // down-left from left edge
    if (direction == 4 && col == 7) return 0; // down-right from right edge
    if (position < 8 && direction == 3) return 0; //down left at bottom
    if (position < 8 && direction == 4) return 0; // down right at bottom
    if (position > 55 && direction == 1) return 0; // up left at top
    if (position > 55 && direction == 2) return 0; // up left at top

    
    if (game->currentTurn == 1)
    {
        if (GetBit(game->player1_kings, position) == 1)
        {
            if (direction == 1)
            {
                if ((((game->board >> (position + 7)) & 1) == 0) || canCapture(game, position, direction))
                    return 1;
                else
                    return 0;
            }
            else if (direction == 2)
            {
                if ((((game->board >> (position + 9)) & 1) == 0) || canCapture(game, position, direction))
                    return 1;
                else
                    return 0;
            }
            else if (direction == 3)
            {
                if ((((game->board >> (position - 9)) & 1) == 0) || canCapture(game, position, direction))
                    return 1;
                else
                    return 0;
            }
            else if (direction == 4)
            {
                if ((((game->board >> (position - 7)) & 1) == 0) || canCapture(game, position, direction))
                    return 1;
                else
                    return 0;
            }
            else
                return 0;
        }
        else if (GetBit(game->player1_pieces, position) == 1)
        {
            if (direction == 1)
            {
                if ((((game->board >> (position + 7)) & 1) == 0) || canCapture(game, position, direction))
                    return 1;
                else
                    return 0;
            }
            else if (direction == 2)
            {
                if ((((game->board >> (position + 9)) & 1) == 0) || canCapture(game, position, direction))
                    return 1;
                else
                    return 0;
            }
            else if (direction == 3)
            {
                return 0;
            }
            else if (direction == 4)
            {
                return 0;
            }
            else
                return 0;
        }
        else
        {
            return 0;
        }
    }
    else if (game->currentTurn == 2)
    {
        if (GetBit(game->player2_kings, position) == 1)
        {
            if (direction == 1)
            {
                if ((((game->board >> (position + 7)) & 1) == 0) || canCapture(game, position, direction))
                    return 1;
                else
                    return 0;
            }
            else if (direction == 2)
            {
                if ((((game->board >> (position + 9)) & 1) == 0) || canCapture(game, position, direction))
                    return 1;
                else
                    return 0;
            }
            else if (direction == 3)
            {
                if ((((game->board >> (position - 9)) & 1) == 0) || canCapture(game, position, direction))
                    return 1;
                else
                    return 0;
            }
            else if (direction == 4)
            {
                if ((((game->board >> (position - 7)) & 1) == 0) || canCapture(game, position, direction))
                    return 1;
                else
                    return 0;
            }
            else
                return 0;
        }
        else if (GetBit(game->player2_pieces, position) == 1)
        {
            if (direction == 1)
            {
                return 0;
            }
            else if (direction == 2)
            {
                return 0;
            }
            else if (direction == 3)
            {
                if ((((game->board >> (position - 9)) & 1) == 0) || canCapture(game, position, direction))
                    return 1;
                else
                    return 0;
            }
            else if (direction == 4)
            {
                if ((((game->board >> (position - 7)) & 1) == 0) || canCapture(game, position, direction))
                    return 1;
                else
                    return 0;
            }
            else
                return 0;
        }
        else
        {
            return 0;
        }
    }
    else
        return 0;
    return 0;
}

void KingCheck(Game *game)
{
    for (int i = 56; i < 64; i++)
    {
        if (GetBit(game->player1_pieces, i) == 1)
        {
            game->player1_pieces = ClearBit(game->player1_pieces, i);
            game->player1_kings = SetBit(game->player1_kings, i);
        }
    }
    for (int j = 0; j < 8; j++)
    {
        if (GetBit(game->player2_pieces, j) == 1)
        {
            game->player2_pieces = ClearBit(game->player2_pieces, j);
            game->player2_kings = SetBit(game->player2_kings, j);
        }
    }
}


//direction 1 = up left
//direction 2 = up right
//direction 3 = down left
//direction 4 = down right
void move(Game *game, int position, int direction)
{
    if (isValidMove(game, position, direction) == 1)
    {
        if (((game->player1_kings >> position)&1) == 1)
        {
            if (direction == 1)
            {
                if (canCapture(game, position, direction) == 0)
                {
                    game->player1_kings = SetBit(game->player1_kings, position + 7);
                    game->player1_kings = ClearBit(game->player1_kings, position);
                    game->currentTurn = 2;
                }
                else
                {
                    game->player1_kings = SetBit(game->player1_kings, position + 14);
                    game->player1_kings = ClearBit(game->player1_kings, position);
                    game->player2_kings = ClearBit(game->player2_kings, position + 7);
                    game->player2_pieces = ClearBit(game->player2_pieces, position + 7);
                }
            }
            else if (direction == 2)
            {
                if (canCapture(game, position, direction) == 0)
                {
                    game->player1_kings = SetBit(game->player1_kings, position + 9);
                    game->player1_kings = ClearBit(game->player1_kings, position);
                    game->currentTurn = 2;
                }
                else
                {
                    game->player1_kings = SetBit(game->player1_kings, position + 18);
                    game->player1_kings = ClearBit(game->player1_kings, position);
                    game->player2_kings = ClearBit(game->player2_kings, position + 9);
                    game->player2_pieces = ClearBit(game->player2_pieces, position + 9);
                }
            }
            else if (direction == 3)
            {
                if (canCapture(game, position, direction) == 0)
                {
                    game->player1_kings = SetBit(game->player1_kings, position - 9);
                    game->player1_kings = ClearBit(game->player1_kings, position);
                    game->currentTurn = 2;
                }
                else
                {
                    game->player1_kings = SetBit(game->player1_kings, position - 18);
                    game->player1_kings = ClearBit(game->player1_kings, position);
                    game->player2_kings = ClearBit(game->player2_kings, position - 9);
                    game->player2_pieces = ClearBit(game->player2_pieces, position - 9);
                }
            }
            else if (direction == 4)
            {
                if (canCapture(game, position, direction) == 0)
                {
                    game->player1_kings = SetBit(game->player1_kings, position - 7);
                    game->player1_kings = ClearBit(game->player1_kings, position);
                    game->currentTurn = 2;
                }
                else
                {
                    game->player1_kings = SetBit(game->player1_kings, position - 14);
                    game->player1_kings = ClearBit(game->player1_kings, position);
                    game->player2_kings = ClearBit(game->player2_kings, position - 7);
                    game->player2_pieces = ClearBit(game->player2_pieces, position - 7);
                }
            }
        }
        if (((game->player1_pieces >> position)&1) == 1)
        {
            if (direction == 1)
            {
                if (canCapture(game, position, direction) == 0)
                {
                    game->player1_pieces = SetBit(game->player1_pieces, position + 7);
                    game->player1_pieces = ClearBit(game->player1_pieces, position);
                    game->currentTurn = 2;
                }
                else
                {
                    game->player1_pieces = SetBit(game->player1_pieces, position + 14);
                    game->player1_pieces = ClearBit(game->player1_pieces, position);
                    game->player2_kings = ClearBit(game->player2_kings, position + 7);
                    game->player2_pieces = ClearBit(game->player2_pieces, position + 7);
                }
            }
            else if (direction == 2)
            {
                if (canCapture(game, position, direction) == 0)
                {
                    game->player1_pieces = SetBit(game->player1_pieces, position + 9);
                    game->player1_pieces = ClearBit(game->player1_pieces, position);
                    game->currentTurn = 2;
                }
                else
                {
                    game->player1_pieces = SetBit(game->player1_pieces, position + 18);
                    game->player1_pieces = ClearBit(game->player1_pieces, position);
                    game->player2_kings = ClearBit(game->player2_kings, position + 9);
                    game->player2_pieces = ClearBit(game->player2_pieces, position + 9);
                }
            }
        }

        if (((game->player2_kings >> position)&1) == 1)
        {
            if (direction == 1)
            {
                if (canCapture(game, position, direction) == 0)
                {
                    game->player2_kings = SetBit(game->player2_kings, position + 7);
                    game->player2_kings = ClearBit(game->player2_kings, position);
                    game->currentTurn = 1;
                }
                else
                {
                    game->player2_kings = SetBit(game->player2_kings, position + 14);
                    game->player2_kings = ClearBit(game->player2_kings, position);
                    game->player1_kings = ClearBit(game->player1_kings, position + 7);
                    game->player1_pieces = ClearBit(game->player1_pieces, position + 7);
                }
            }
            else if (direction == 2)
            {
                if (canCapture(game, position, direction) == 0)
                {
                    game->player2_kings = SetBit(game->player2_kings, position + 9);
                    game->player2_kings = ClearBit(game->player2_kings, position);
                    game->currentTurn = 1;
                }
                else
                {
                    game->player2_kings = SetBit(game->player2_kings, position + 18);
                    game->player2_kings = ClearBit(game->player2_kings, position);
                    game->player1_kings = ClearBit(game->player1_kings, position + 9);
                    game->player1_pieces = ClearBit(game->player1_pieces, position + 9);
                }
            }
            else if (direction == 3)
            {
                if (canCapture(game, position, direction) == 0)
                {
                    game->player2_kings = SetBit(game->player2_kings, position - 9);
                    game->player2_kings = ClearBit(game->player2_kings, position);
                    game->currentTurn = 1;
                }
                else
                {
                    game->player2_kings = SetBit(game->player2_kings, position - 18);
                    game->player2_kings = ClearBit(game->player2_kings, position);
                    game->player1_kings = ClearBit(game->player1_kings, position - 9);
                    game->player1_pieces = ClearBit(game->player1_pieces, position - 9);
                }
            }
            else if (direction == 4)
            {
                if (canCapture(game, position, direction) == 0)
                {
                    game->player2_kings = SetBit(game->player2_kings, position - 7);
                    game->player2_kings = ClearBit(game->player2_kings, position);
                    game->currentTurn = 1;
                }
                else
                {
                    game->player2_kings = SetBit(game->player2_kings, position - 14);
                    game->player2_kings = ClearBit(game->player2_kings, position);
                    game->player1_kings = ClearBit(game->player1_kings, position - 7);
                    game->player1_pieces = ClearBit(game->player1_pieces, position - 7);
                }
            }
        }
        if (((game->player2_pieces >> position)&1) == 1)
        {
            if (direction == 3)
            {
                if (canCapture(game, position, direction) == 0)
                {
                    game->player2_pieces = SetBit(game->player2_pieces, position - 9);
                    game->player2_pieces = ClearBit(game->player2_pieces, position);
                    game->currentTurn = 1;
                }
                else
                {
                    game->player2_pieces = SetBit(game->player2_pieces, position - 18);
                    game->player2_pieces = ClearBit(game->player2_pieces, position);
                    game->player1_kings = ClearBit(game->player1_kings, position - 9);
                    game->player1_pieces = ClearBit(game->player1_pieces, position - 9);
                }
            }
            else if (direction == 4)
            {
                if (canCapture(game, position, direction) == 0)
                {
                    game->player2_pieces = SetBit(game->player2_pieces, position - 7);
                    game->player2_pieces = ClearBit(game->player2_pieces, position);
                    game->currentTurn = 1;
                }
                else
                {
                    game->player2_pieces = SetBit(game->player2_pieces, position - 14);
                    game->player2_pieces = ClearBit(game->player2_pieces, position);
                    game->player1_kings = ClearBit(game->player1_kings, position - 7);
                    game->player1_pieces = ClearBit(game->player1_pieces, position - 7);
                }
            }
        }
    }
    game->board = (game->player1_pieces | game->player2_pieces | game->player1_kings | game->player2_kings);
}



int main()
{
    Game gameInstance;
    Game *game = &gameInstance;
    game->player1_pieces = 11163050ULL;
    game->player1_kings = 0ULL;
    game->player2_pieces = 6172839697753047040ULL;
    game->player2_kings = 0ULL;
    game->board = (game->player1_pieces | game->player2_pieces | game->player1_kings | game->player2_kings);
    game->currentTurn = 1;
    int cont = 1;
    
    while (cont == 1)
    {
        int position = -1;
        int direction = 0;
        KingCheck(game);
        DisplayBoard(game->player1_pieces, game->player1_kings, game->player2_pieces, game->player2_kings);
        printf("Player %d's turn\n", game->currentTurn);
        printf("Enter position of piece to move (Add numbers to the left and below the row and column): ");
        scanf("%d", &position);
        printf("Enter direction for piece to move (1 = up left | 2 = up right | 3 = down left | 4 = down right): ");
        scanf("%d", &direction);

        if (isValidMove(game, position, direction) == 1)
        {
            move(game, position, direction);
        }
        else
        {
            printf("\nINVALID MOVE TRY AGAIN\n");
        }
        
        if (game->player1_kings == 0 && game->player1_pieces == 0)
        {    
            printf("Player 2 Wins!");
            cont = 0;
        }
        if (game->player2_kings == 0 && game->player2_pieces == 0)
        {    
            printf("Player 1 Wins!");
            cont = 0;
        }
    }
    return 0;
}