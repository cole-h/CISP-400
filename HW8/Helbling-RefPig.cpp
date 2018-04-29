/*
**  RefPig.cpp - Homework 8: Refactored Pig
**  Cole Helbling, CISP 400
**  28 April 2018
*/

#include <algorithm>
#include <cassert>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Function Prototypes
void ProgramGreeting();

// Specification 1 - Create an abstract class called `Player`
class Player
{
public:
	int total = 0;
	int round_pts = 0;
	string type = "";

	virtual void Roll() = 0;

	int getTotal()
	{
		return total;
	}

	int getRoundPts()
	{
		return round_pts;
	}

	string getType()
	{
		return type;
	}

	int checkWon(bool & gameOver)
	{
		if (total > 100 || round_pts + total > 100)
		{
			total += round_pts;
			cout << "\n\nGame over!\n";
			gameOver = true;
			return 1;
		}
		return 0;
	}
};

class Computer : public Player
{
	int mode = 0; // 0 for hardcoded, 1 for random
	static int losses;
	static int wins;
public:
	Computer(int m) : Player() { type = "Computer"; mode = m; }

	void Roll()
	{
		// Specification 4 - Computer rolls, 1-3 hold, 4-6 roll again (hardcoded behavior)
		while (!mode)
		{
			int roll = rand() % 6 + 1;

			switch (roll)
			{
				case 1:
					cout << type << " rolled a " << roll << " and lost a turn\n\n";
					total += round_pts;
					round_pts = 0;
					return;
				case 2:
				case 3:
					cout << type << " rolled a " << roll << " and decided to hold\n\n";
					round_pts += roll;
					total += round_pts;
					round_pts = 0;
					return;
				case 4:
				case 5:
				case 6:
					cout << type << " rolled a " << roll << "\n\n";
					round_pts += roll;
					continue;
				default:
					cout << "Something bad happened.\n";
					// Feature 1 - Make sure random roll doesn't get messed with
					assert(roll > 0 && roll < 7);
					exit(1);
			}
		}
		// Feature 5 - Randomize bot behavior
		while (mode)
		{
			int roll = rand() % 6 + 1;

			switch (roll)
			{
				case 1:
					cout << type << " rolled a " << roll << '\n';
					total += round_pts;
					round_pts = 0;
					return;
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				{
					cout << type << " rolled a " << roll;
					int choice = rand() % 1;
					round_pts += roll;
					if (choice)
					{
						cout << " and decided to hold\n";
						total += round_pts;
						round_pts = 0;
						return;
					}
					else
					{
						cout << '\n';
						continue;
					}
				}
				default:
					cout << "Something bad happened.\n";
					// Feature 2 - Make sure second random roll doesn't get messed with
					assert(roll > 0 && roll < 7);
					exit(1);
			}
		}

		return;
	}

	void setMode(int m)
	{
		mode = m;
	}

	void addWin()
	{
		wins++;
	}

	int getWins()
	{
		return wins;
	}

	void addLoss()
	{
		losses++;
	}

	int getLosses()
	{
		return losses;
	}

	template<class T> // Fixes redefinition error
	friend ostream & operator<<(ostream & out, T & player)
	{
		out << left << setw(10) << player.getType() << right << setw(20) << player.getTotal() << ' ' << player.getRoundPts() << ' ' << wins << ' ' << losses << ' ' << '\n';
		return out;
	}
};

class Human : public Player
{
	static int losses;
	static int wins;
public:
	Human() : Player() { type = "Human"; }

	void Roll()
	{
		while (true)
		{
			cout << *this;
			int choice;
			int roll = rand() % 6 + 1;

			cout << "\nRoll (0) or hold (1)? ";
			cin >> choice;

			if (!choice)
			{
					switch (roll)
					{
						case 1:
							total += round_pts;
							round_pts = 0;
							cout << "You rolled a one and lost your turn.\n\n";
							return;
						default:
							round_pts += roll;
							cout << "You rolled a " << roll << " and gained that many points.\n\n";
							continue;
					}
			}
			if (choice)
			{
				total += round_pts;
				round_pts = 0;
				return;
			}
		}
	}

	void addWin()
	{
		wins++;
	}

	int getWins()
	{
		return wins;
	}

	void addLoss()
	{
		losses++;
	}

	int getLosses()
	{
		return losses;
	}

	// Specification 3 - Use operator overloading where appropriate
	friend ostream & operator<<(ostream & out, Player & player)
	{
		out << left << setw(10) << player.getType() << right << setw(20) << player.getTotal() << ' ' << player.getRoundPts() << ' ' << wins << ' ' << losses << ' ' << '\n';
		return out;
	}
};

// Specification 2 - Create a class called Dice
class Dice
{
	int roll = 0;
public:

	friend ostream & operator<<(ostream & out, const Dice & dice)
	{

		return out;
	}
};

// Feature 4 - Keep track of wins and losses with a static variable
int Computer::wins = 0;
int Computer::losses = 0;
int Human::wins = 0;
int Human::losses = 0;

int main()
{
	srand(time(NULL));

	bool gameOver = false;
	bool wrong = true;
	int botmode;
	// Specification 5 - Have the program create a log file
	ofstream log;
	log.open("moves.dat", ios::app);

	ProgramGreeting();

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

	Computer AI(botmode);
	Human Person;

	while (!gameOver)
	{
		Person.Roll();
		AI.Roll();

		cout << AI;

		if (Person.checkWon(gameOver))
		{
			cout << "You win!\n";
			Person.addWin();
			AI.addLoss();
		}
		if (AI.checkWon(gameOver))
		{
			cout << "Computer wins!\n";
			AI.addWin();
			Person.addLoss();
		}

		log << Person;
		log << AI;

		log.flush();
	}

	log << '\n';
	log.close();

	// Feature 3 - Ask the user if they would like to play again
	char decision;
	cout << "\n\nWould you like to play again (y or n)? ";
	cin >> decision;
	switch (decision)
	{
		case 'y':
			main();
		default:
			cout << "You chose not to play again.\n";
			return 0;
	}

	return 0;
}

void ProgramGreeting()
{
	cout << "Welcome! This is the last homework assignment, \"RefPig\" -- a game where you will alternate rolling die with an AI.\n\nRolling a 1 will lose your turn and net you no points; roll anything higher and you add those points to your current turn total and continue to roll until you hit a 1. At that point, the AI will then proceed to play out its turn.\n\n";
}
