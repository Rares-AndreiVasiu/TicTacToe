#include <stdio.h>
#include <stdbool.h>
#include <time.h> //used to set the time delay
#include <stdlib.h>
#include <limits.h>

#define PLAYER_ONE 1 // uses the cross
#define PLAYER_TWO 2 // uses the oval 

#define DBG 0
#define HUMAN 1
#define AI 0

#define clearConosole() printf("\e[1;1H\e[2J")

int board[10];

/*
 * this is the main play function
 * here we have all the main calls 
 * of helper functions
*/

enum _TicTacToe
{
	Cross = 1, 
	Oval = 2,
	Space = -1
};

void Menu()
{
	puts("<=== TIC TAC TOE === >");
	puts("Multiplayer: press 1");
	puts("Single-Player: press 2");
	puts("Exit game: press 0");
}

void initBoard()
{

#if DBG == 1
	puts("Here we initialize the board");
#endif

	for(int i = 1; i <= 9; ++ i)
	{
		board[i] = Space;
	}
}

void holdON(unsigned int seconds)
{
	unsigned int retTime = time(0) + seconds; // get the finishing time + 5s

	while(time(0) < retTime);
}

/*
	This functions handles the printing of the tictactoe board
*/
void printBoard()
{
	holdON(1); //we should wait a bit to see how it changes the movement

	clearConosole();

	puts("-------");

	for(int i = 1; i <= 9; ++ i)
	{
		if(i == 4 || i == 7)
		{
			printf("\n");
			puts("-------");
		}
		
		if(i % 3 == 1)
		{
			printf("|");
		}

		switch(board[i])
		{
			case -1:
			{
				printf(" |");
				break;
			}
			
			case 1:
			{		
				printf("X|");
				break;
			}

			case 2:
			{
				printf("O|");
				break;
			}
		}
	}
	printf("\n-------\n");
}

bool checkUserInput(int x, int y)
{
	return (x >= 1 && x <= 3) && (y >= 1 && y <= 3);
}

bool checkCorrectMove(int posInBoard)
{
	if(board[posInBoard] == Space)
	{
		return true;
	}
	return false;
}

unsigned int emptySpots()
{
	unsigned int freeSpots = 0;

	for(int i = 1; i <= 9; ++ i)
	{
		if(board[i] == Space)
		{
			freeSpots++;
		}
	}
	return freeSpots;
}

unsigned int checkWinner()
{
	unsigned int winner = 0;

	//check on horizontal for winners
	for(int i = 1; i <= 9 && !winner; i += 3)
	{
		if( (board[i] == board[i + 1]) && (board[i] == board[i + 2]) && (board[i] != Space))
		{
			winner = board[i];
		}
	}

	//check on vertical for winners
	for(int i = 1; i <= 3 && !winner; ++ i)
	{
		if( (board[i] == board[i + 3]) && (board[i] == board[i + 6]) && (board[i] != Space))
		{
			winner = board[i];
		}
	}

	//check on main diagonal for winners
	if( (board[1] == board[5]) && (board[1] == board[9]) && (board[1] != Space))
	{
		winner = board[1];
	}

	//check on second diagonal for winners
	if( (board[3] == board[5]) && (board[3] == board[7]) && (board[3] != Space))
	{
		winner = board[3];
	}

	/*
		if the number of free space is 0 and there is no winner
		we have a tie
	*/

	if(!emptySpots() && !winner)
	{
		return 11;
	}
	
	return winner;
}

void ExitGame()
{
	/*
		we should wait a bit to tell the user 
		that he is about to leave the game 
		5seconds is good enough
	*/

	puts("You are about to leave the game. Have a good day!");

	holdON(5);

	exit(EXIT_SUCCESS);
}

void playMultiplayer()
{

#if DBG == 1
	puts("POWER MULTIPLAYER");
#endif

	bool flagGameOver = false;

	/*
		good to know: rand() % (max_number + 1 - minimum_number) + minimum_number
		we randomize who starts the game
		for more fun
	*/
	int turn = rand() % 2 + 1;

	while(!flagGameOver)
	{
		printBoard();

		int flagCorrectInput = 0;
		
		int xCoordinate, yCoordinate;

		while(!flagCorrectInput)
		{
			printf("%s's turn. Type in the coordinates to move: ", (turn == PLAYER_ONE) ? "Player One" : "Player Two");

			if(scanf("%d %d", &xCoordinate, &yCoordinate) == 2)
			{
				if(checkUserInput(xCoordinate, yCoordinate))
				{
					int pos = (xCoordinate - 1) * 3 + yCoordinate;

					if(checkCorrectMove(pos))
					{
						if(turn == PLAYER_ONE)
						{
							board[pos] = Cross;
						}
						else
						{
							board[pos] = Oval;
						}
						
						flagCorrectInput = true;
					}
				}
			}
		}

		//we check at each step if the game is not already over
		unsigned gameStatus = checkWinner();

		if(gameStatus != 0)
		{
			flagGameOver = true;
			
			printBoard();

			if(gameStatus == Cross)
			{
				printf("Player One wins!\n");
			}
			else
			{
				if(gameStatus == Oval)
				{
					printf("Player Two wins!\n");
				}
				else
				{
					printf("TIE!\n");
				}
			}
		}

		if(turn == PLAYER_ONE)
		{
			turn = PLAYER_TWO;
		}
		else
		{
			turn = PLAYER_ONE;
		}
	}
}

int minimax(int *board, int depthOfSearch, bool isMaximizing)
{
	unsigned gameStatus = checkWinner();

	/*
		we enode the following: X = 1,
								O: -1,
								tie = 0
		for the minimax algorithm
	*/

	// we check at each step if the game is not already over
	if (gameStatus != 0)
	{
		switch (gameStatus)
		{
			case Cross:
			{
				return 1;
				break;
			}

			case Oval:
			{
				return -1;
				break;
			}

			case 11:
			{
				return 0;
				break;
			}
		}
	}

	//tha maximizing player is the AI
	if(isMaximizing)
	{
		int bestScore = INT_MIN;

		for(int i = 1; i <= 9; ++ i)
		{
			if(board[i] == Space)
			{
				board[i] = Cross;

				int score = minimax(board, depthOfSearch + 1, false); //we swtich roles so make it false

				board[i] = Space;

				if(score > bestScore)
				{
					bestScore = score;
				}
			}
		}
		//finding the bestScore for the next possible moves
		return bestScore;
	}
	else
	{
		//we start the minizing player is the human

		int bestScore = INT_MAX;

		for(int i = 1; i <= 9; ++ i)
		{
			if(board[i] == Space)
			{
				board[i] = Oval;

				int score = minimax(board, depthOfSearch + 1, true);//switch to ai, its maximizing

				board[i] = Space;

				if(score < bestScore)
				{
					bestScore = score;
				}
			}
		}
		return bestScore;
	}
}

//we handle the best current move for the AI
void BestMove(unsigned int *curretPlayer)
{
	int bestScore = INT_MIN;

	int bestPosition = 0;

	//AI doing something pretty smart now

	for(int i = 1; i <= 9; ++ i)
	{
		//check for free spots in the board to let AI to choose
		if(board[i] == Space)
		{
			board[i] = Cross;

			//the next move is the minizing move
			int score = minimax(board, 0, false);//we want to find the score after calling minimax algorithm
			// it will have to recursively check the subtrees in the binary tree of possible moves
			
			board[i] = Space;

			if(score > bestScore)
			{
				bestScore = score;
				bestPosition = i;
			}
		}
	}
	board[bestPosition] = Cross;

	*curretPlayer = HUMAN;
}

//another game from selected from the main menu
void playSinglePlayer()
{
	unsigned int currentPlayer = AI;

	int flagGameOver = 0;

	do
	{
		printBoard();

		//check for whom is playing
		if(currentPlayer == HUMAN)
		{	
			//human does the turn
			bool flagCorrectInput = false;

			int xCoordinate, yCoordinate;

			while (!flagCorrectInput)
			{
				printf("\nType in the coordinates to move: ");

				if (scanf("%d %d", &xCoordinate, &yCoordinate) == 2)
				{
					if (checkUserInput(xCoordinate, yCoordinate))
					{
						int pos = (xCoordinate - 1) * 3 + yCoordinate;

						if (checkCorrectMove(pos))
						{
							board[pos] = Oval;

							flagCorrectInput = true;
						}
					}
				}
			}
			//switch roles
			currentPlayer = AI;
		}
		BestMove(&currentPlayer); //call the ai to play

		unsigned gameStatus = checkWinner();

		//we check at each step if the game is not already over
		if(gameStatus != 0)
		{
			flagGameOver = true;
			
			printBoard();

			if(gameStatus == Cross)
			{
				printf("Player One wins!\n");
			}
			else
			{
				if(gameStatus == Oval)
				{
					printf("Player Two wins!\n");
				}
				else
				{
					printf("TIE!\n");
				}
			}
		}
	}while(!flagGameOver);
}

void takeUserInputForGameType()
{
	int choice = 0;

	bool flag = false;

	//loop untill the user is able to introduce the correct input
	while(!flag)
	{
		printf("\nYour choice: ");

		if(scanf("%d", &choice) == 1)
		{
			if(choice == 0 || choice == 1 || choice == 2)
			{
				flag = true;
			}
		}
	}
	
	//now we have to redirect the type of game that the user wants to play
	switch (choice)
	{
		case 0:
		{
			ExitGame();

			break;
		}
			
		case 1:
		{
			playMultiplayer();
			
			break;
		}
			
		case 2:
		{
			playSinglePlayer();

			break;
		}
	}
}

void eventHandler()
{
	Menu();

	takeUserInputForGameType();
}

int main()
{
	initBoard();

	eventHandler();
	
	return 0;
}

/*
1 2 3
4 5 6
7 8 9

*/
