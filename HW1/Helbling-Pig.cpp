/*
**  pig.cpp - Homework 1: Pig
**  Cole Helbling, CISP 400
**  21 Jan 2018
**
**  Bonus 0: (this is sarcastic) if you enter a non-binary input (not 0 or 1) at any point,
**           the game plays itself for you!
*/

#include <iostream>  // required for cin and cout
#include <ctime>     // required to get random seed based on time

using namespace std;

// Function Prototype to have the AI play
void artificalPlayer(int, int, int &, int &);
// Function Prototype to welcome user to game
void ProgramGreeting();
// Function Prototype to roll the die and decide whether or not to add to totals
int roller(int &, bool);

bool again = false;
int losses = 0;     // not the cleanest solution...
int wins = 0;       // but it works
int bottotal = 0;
int plrtotal = 0;


// main :: Runs everything to make the game work
int main()
{
	// variable declaration
	int botgrandpts = 0;
	int botmode = 0;
	int botturnpts = 0;
	int grandpts = 0;
	int hold = 0;
	int roll = 0;
	int turnpts = 0;
	char decision;
	bool playerturn = true;
	bool wrong = true;

	ProgramGreeting();

	// Specification 4: Seed the random number based on system clock
	srand(time(NULL));

	// Feature 1: Choose between easy AI (Spec 1) or hard AI (random choices)
	while (wrong)
	{
		cout << "Would you like to play with the basic AI (0), or a more sophisticated AI (1)?\nMode (0 or 1): ";
		cin >> botmode;
		switch (botmode)
		{
			case 0: cout << "\nNormal mode it is.\n\n"; wrong = false; break;
			case 1: cout << "\nHard mode activated! This is gonna be wild!\n\n"; wrong = false; break;
			default: cout << "Not a valid choice. Try again.\n\n"; continue;
		}
	}

	// Specification 2: Run the program in a main event loop
	while (grandpts < 100 && botgrandpts < 100)
	{
		botturnpts = 0;

		// Feature 5: If player decides to play again, display cumulative points across all games for both AI and player
		if (again)
			cout << "Cumulative points for player: " << plrtotal << "\t\tCumulative points for AI: " << bottotal << endl;

		// Feature 3: Scoreboard of both player and bot's point totals
		cout << "Player's total points: " << grandpts << "\t\t\tPlayer's turn points: " << turnpts << "\nAI's total points: " << botgrandpts << endl;
		cout << "\nRoll (0) or hold (1)? ";
		cin >> hold;

		if (hold == 0 && playerturn)
		{
			turnpts += roller(roll, true);
			if (roll != 1)
				plrtotal += roll;
			if (roll == 1)
				playerturn = false;
		}

		if (hold == 1 || !playerturn)
		{
			grandpts += turnpts;
			turnpts = 0;

			// Specification 3: Use at least one function call
			artificalPlayer(botmode, roll, botturnpts, botgrandpts);
			bottotal += botturnpts;
			cout << "AI scored " << botturnpts << " for a total of " << botgrandpts << ".\n\n";
			playerturn = true;
		}
	}

	if (grandpts >= 100)
	{
		cout << "\n\nPlayer wins with " << grandpts << endl;
		wins++;
	}
	else if (botgrandpts >= 100)
	{
		cout << "\n\nAI wins with " << botgrandpts << endl;
		losses++;
	}
	else
		cout << "Nobody wins." << endl;

	// Feature 4: If so inclined, the user may play again. Keep track of wins and losses.
	cout << "\n\nWould you like to play again (y or n)? ";
	cin >> decision;
	switch (decision)
	{
		case 'y':
			cout << "Wins: " << wins << "\t\tLosses: " << losses << "\n\n\n";
			again = true;
			main();
		default:
			cout << "You chose not to play again.\n";
			return 0;
	}
}


// ProgramGreeting :: Greets the user and explains the game
void ProgramGreeting()
{
	cout << "Hello! This is the first homework assignment, \"Pig\" -- a game where you will alternate rolling die with an AI.\n\nRolling a 1 will lose your turn and net you no points; roll anything higher and you add those points to your current turn total and continue to roll until you hit a 1. At that point, the AI will then proceed to play out its turn.\n\n";
}


// artificalPlayer :: Deals with how the AI plays the game
void artificalPlayer(int mode, int die, int & botturnpts, int & botgrandpts)
{
	// Feature 2: Bot logic (random behavior)
	if (mode)
	{
		while (true)
		{
			roller(die, false);

			switch (die)
			{
				case 1:
					botgrandpts += botturnpts;
					return;
				default:
					int choice = rand() % 1000 + 1;
					botturnpts += die;
					if (choice >= 500)
					{
						botgrandpts += botturnpts;
						return;
					}
					else
						continue;
			}
		}
	}

	// Specification 1: Each decision the computer rolls, 1-3 hold, 4-6 roll again (hardcoded behavior)
	if (!mode)
	{
		while (true)
		{
			roller(die, false);

			if (die == 1)
			{
				botgrandpts += botturnpts;
				return;
			}
			else if (die <= 3 && die > 1)
			{
				botturnpts += die;
				botgrandpts += botturnpts;
				return;
			}
			else if (die <= 6 && die >= 4)
			{
				botturnpts += die;
				continue;
			}
		}
	}
}


// roller :: Rolls the die using a random number with a seed of time(NULL)
int roller(int & result, bool human)
{
	result = rand() % 6 + 1;

	switch(result)
	{
		case 1:
			if (human)
				cout << "You rolled a 1 and lost your turn\n\n";

			return 0;
		default:
			if (human)
				cout << "You rolled a " << result << " and gained that many points.\n\n";

			return result;
	}
}
