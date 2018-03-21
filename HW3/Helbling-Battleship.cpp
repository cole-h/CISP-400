/*
**  Battleship.cpp - Homework 3: Battleship
**  Cole Helbling, CISP 400
**  19 Feb 2018
*/

#include <cassert>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Ship;

// Function Prototypes
int hitOrMiss(int[][10], int, int, int &, int &, vector<Ship> &, int &, int &);
int placeShip(int[][10], int, int, vector<Ship> &);
int validSpace(int[][10], int, int, int);
void ProgramGreeting();
void randLocation(int &, int &, int);
void showMap(int[][10], bool);

enum SHIP_SIZE {CARRIER = 5, BATTLESHIP = 4, CRUISER = 3, SUBMARINE = 3, DESTROYER = 2};
int game = 1;
string name = "";

class Ship
{
private:
	int health;
	int id;
	int length;
	string name;
	vector<int> column;
	vector<int> row;
public:
	Ship(int len, vector<int> r, vector<int> col, string n, int i)
	{
		this->health = len;
		this->length = len;
		this->name = n;
		this->column = col;
		this->row = r;
		this->id = i;
	}

	void Hit()
	{
		this->health--;
	}

	int Health()
	{
		return this->health;
	}

	void setHealth(int hp)
	{
		this->health = hp;
	}

	int Status()
	{
		if (this->health <= 0)
			return 1; // ship is sunk
		else
			return 0;
	}

	int Column()
	{
		return this->column[0];
	}
};

int main()
{
	bool playing = true;
	char colui = 'z';
	int chits = 0;
	int cmisses = 0;
	int cpuGrid[10][10] = {0};
	int csunk = 0;
	int hit = -1;
	int hits = 0;
	int misses =  0;
	int playerGrid[10][10] = {0};
	int sunk = 0;
	int turn = 0;
	int valid = 0;
	ofstream log;
	vector<Ship> cpuShips;
	vector<Ship> playerShips;

	ProgramGreeting();

	// Feature 3 - Use RNG for CPU's shots
	srand(time(NULL));

	// CPU Ships
	placeShip(cpuGrid, CARRIER, 1, cpuShips);
	placeShip(cpuGrid, BATTLESHIP, 2, cpuShips);
	placeShip(cpuGrid, CRUISER, 3, cpuShips);
	placeShip(cpuGrid, SUBMARINE, 4, cpuShips);
	placeShip(cpuGrid, DESTROYER, 5, cpuShips);

	// Player Ships
	placeShip(playerGrid, CARRIER, 1, playerShips);
	placeShip(playerGrid, BATTLESHIP, 2, playerShips);
	placeShip(playerGrid, CRUISER, 3, playerShips);
	placeShip(playerGrid, SUBMARINE, 4, playerShips);
	placeShip(playerGrid, DESTROYER, 5, playerShips);

	// Feature 1 - Choose file name
	if (game == 1)
	{
		cout << "What would you like to name the log? ";
		getline(cin, name);
		name = name + ".txt";
		log.open(name);
		log << "Game #" << game << endl;
		cout << endl;
	}
	else
	{
		log.open(name, ios::out | ios::app);
		log << "\n\nGame #" << game << endl;
		cout << endl;
	}

	cout << "Game #" << game << endl;

	while (playing)
	{
		int col = -1;
		int row = -1;
		hit = -1;
		valid = 0;

		// Specification 2 - Display a map of your shots every turn before you shoot
		cout << "\nPlayer Grid:\n";
		showMap(playerGrid, true);

		// Specification 3 - Display the computer shots map after the computer's turn
		cout << "\nCPU Grid:\n";
		showMap(cpuGrid, true);

		// Specification 1 - Filter the input for valid input
		cout << "Column? ";
		cin >> colui;
		while (!valid)
		{
			switch (tolower(colui))
			{
				case 'a': col = 0; valid = 1; break;
				case 'b': col = 1; valid = 1; break;
				case 'c': col = 2; valid = 1; break;
				case 'd': col = 3; valid = 1; break;
				case 'e': col = 4; valid = 1; break;
				case 'f': col = 5; valid = 1; break;
				case 'g': col = 6; valid = 1; break;
				case 'h': col = 7; valid = 1; break;
				case 'i': col = 8; valid = 1; break;
				case 'j': col = 9; valid = 1; break;
				default:
					cout << "Invalid column. Column? ";
					cin >> colui;
					valid = 0;
					continue;
			}
		}
		cout << "Row? ";
		cin >> row;
		while (row < 0 || row > 9)
		{
			cout << "Invalid row. Row? ";
			cin >> row;
		}
		// Feature 7 - Assert that row is between 0 and 9 inclusive to account for weird behavior
		assert(row >= 0 && row <= 9);
		cout << endl;
		turn++;

		hitOrMiss(cpuGrid, col, row, misses, hits, playerShips, sunk, hit);

		// Specification 5 - Build a log file containing the moves each player made for every turn and result
		if (hit == 1)
		{
			switch (col)
			{
				case 0: log << "[A][" << row << "] -- Result: Hit\n"; break;
				case 1: log << "[B][" << row << "] -- Result: Hit\n"; break;
				case 2: log << "[C][" << row << "] -- Result: Hit\n"; break;
				case 3: log << "[D][" << row << "] -- Result: Hit\n"; break;
				case 4: log << "[E][" << row << "] -- Result: Hit\n"; break;
				case 5: log << "[F][" << row << "] -- Result: Hit\n"; break;
				case 6: log << "[G][" << row << "] -- Result: Hit\n"; break;
				case 7: log << "[H][" << row << "] -- Result: Hit\n"; break;
				case 8: log << "[I][" << row << "] -- Result: Hit\n"; break;
				case 9: log << "[J][" << row << "] -- Result: Hit\n"; break;
			}
		}
		else if (hit == 6)
		{
			switch (col)
			{
				case 0: log << "[A][" << row << "] -- Result: Miss\n"; break;
				case 1: log << "[B][" << row << "] -- Result: Miss\n"; break;
				case 2: log << "[C][" << row << "] -- Result: Miss\n"; break;
				case 3: log << "[D][" << row << "] -- Result: Miss\n"; break;
				case 4: log << "[E][" << row << "] -- Result: Miss\n"; break;
				case 5: log << "[F][" << row << "] -- Result: Miss\n"; break;
				case 6: log << "[G][" << row << "] -- Result: Miss\n"; break;
				case 7: log << "[H][" << row << "] -- Result: Miss\n"; break;
				case 8: log << "[I][" << row << "] -- Result: Miss\n"; break;
				case 9: log << "[J][" << row << "] -- Result: Miss\n"; break;
			}
		}

		// Specification 4 - Include logic in the AI to prevent the CPU from firing at a square it has already fired at
		do
		{
			col = rand() % 10;
			row = rand() % 10;
			hitOrMiss(playerGrid, col, row, cmisses, chits, cpuShips, csunk, hit);
		} while (hit == -1);

		log.flush();

		// if (sunk > 0 || csunk > 0)
			cout << "You have sunk " << sunk << " ships.\nThe CPU has sunk " << csunk << " ships.\n";

		if (sunk == 5 || csunk == 5)
			playing = false;
	}

	log.close();

	// Feature 4 - Won in x turns and stats
	if (sunk == 5)
		cout << "Congratulations! You won in " << turn << " turns!\n\nStats:\nHits\tMisses\tSunk\n" << hits << "\t" << misses << "\t" << sunk << endl;
	else if (csunk == 5)
		cout << "You lost in " << turn << " turns.\n\nStats:\nHits\tMisses\tSunk\n" << hits << "\t" << misses << "\t" << sunk << endl;

	// Feature 2 - Ask if the player wants to play again
	colui = ' ';
	cout << "Do you want to play another game (y/n)? ";
	cin >> colui;
	if (tolower(colui) == 'y')
	{
		game++;
		main();
	}
	else
		return 0;
}


void ProgramGreeting()
{
	cout << "This is the 3rd homework assignment, \"Battleship\", where you will play against a computer in a standard game of battleship.\n\n";
}


void showMap(int grid[][10], bool player)
{
	cout << "   A B C D E F G H I J\n\n";
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (player)
			{

				if (j == 0)
					cout << i << "  ";
				if (grid[i][j] == 1)
					cout << "X";
				else if (grid[i][j] == 6)
					cout << "O";
				else
				{
					if (grid[i][j] != 0)
						cout << grid[i][j];
					else
						cout << " ";
				}
			}
			else if (!player)
			{
				if (j == 0)
					cout << i << "  ";
				if (grid[i][j] == 1)
					cout << "X";
				else if (grid[i][j] == 6)
					cout << "O";
				else
					cout << " ";
			}

			if (j != 9)
				cout << " ";
		}
		cout << endl;
	}
	cout << endl;
}


int placeShip(int grid[][10], int size, int name, vector<Ship> & ships)
{
	int row = 0;
	int col = 0;
	bool failed = true;

	randLocation(col, row, size);

	while (failed)
	{
		if (validSpace(grid, col, row, size))
		{
			randLocation(col, row, size);
			continue;
		}

		for (int i = 0; i < size; i++)
		{
			grid[row][col] = size;
			row++;
		}

		switch (name)
		{
			case 1:
			{
				vector<int> newRow(5);
				for (int i = 0; i < (int) newRow.size(); ++i)
				{
					newRow.at(i) = row;
					row++;
				}
				vector<int> newCol(5);
				for (int i = 0; i < (int) newCol.size(); ++i)
					newCol.at(i) = col;
				Ship carrier(5, newRow, newCol, "Carrier", name);
				ships.push_back(carrier);
			}
			break;
			case 2:
			{
				vector<int> newRow(4);
				for (int i = 0; i < (int) newRow.size(); ++i)
				{
					newRow.at(i) = row;
					row++;
				}
				vector<int> newCol(4);
				for (int i = 0; i < (int) newCol.size(); ++i)
					newCol.at(i) = col;
				Ship battleship(4, newRow, newCol, "Battleship", name);
				ships.push_back(battleship);
			}
			break;
			case 3:
			{
				vector<int> newRow(3);
				for (int i = 0; i < (int) newRow.size(); ++i)
				{
					newRow.at(i) = row;
					row++;
				}
				vector<int> newCol(3);
				for (int i = 0; i < (int) newCol.size(); ++i)
					newCol.at(i) = col;
				Ship cruiser(3, newRow, newCol, "Cruiser", name);
				ships.push_back(cruiser);
			}
			break;
			case 4:
			{
				vector<int> newRow(3);
				for (int i = 0; i < (int) newRow.size(); ++i)
				{
					newRow.at(i) = row;
					row++;
				}
				vector<int> newCol(3);
				for (int i = 0; i < (int) newCol.size(); ++i)
					newCol.at(i) = col;
				Ship submarine(3, newRow, newCol, "Submarine", name);
				ships.push_back(submarine);
			}
			break;
			case 5:
			{
				vector<int> newRow(2);
				for (int i = 0; i < (int) newRow.size(); ++i)
				{
					newRow.at(i) = row;
					row++;
				}
				vector<int> newCol(2);
				for (int i = 0; i < (int) newCol.size(); ++i)
					newCol.at(i) = col;
				Ship destroyer(2, newRow, newCol, "Destroyer", name);
				ships.push_back(destroyer);
			}
			break;
		}

		failed = false;
	}
	return 0;
}

// Feature 5 - Randomly generate the locations of the ships
void randLocation(int & col, int & row, int size)
{
	switch (size)
	{
		case DESTROYER:
			col = rand() % 10;
			row = rand() % 8;
			break;
		case SUBMARINE: // also CRUISER
			col = rand() % 10;
			row = rand() % 7;
			break;
		case BATTLESHIP:
			col = rand() % 10;
			row = rand() % 6;
			break;
		case CARRIER:
			col = rand() % 10;
			row = rand() % 5;
			break;
	}
}

// Feature 6 - Make sure valid ship placement
int validSpace(int grid[][10], int col, int row, int size)
{
	for (int i = row; i < (row + size); i++)
	{
		if(grid[i][col] != 0)
			return 1;
	}
	return 0;
}


int hitOrMiss(int grid[][10], int col, int row, int & misses, int & hits, vector<Ship> & ships, int & sunk, int & hit)
{
	switch (grid[row][col])
	{
		case 0:
			grid[row][col] = 6;
			misses++;
			hit = 6;
			return hit;
		case 1:
			hit = -1;
			return hit;
		case 2:
			ships[4].Hit();
			if (ships[4].Status())
				sunk++;
			grid[row][col] = 1;
			hits++;

			hit = 1;
			return hit;
		case 3:
			if (col == ships[3].Column())
			{
				ships[3].Hit();
				if (ships[3].Health() <= 0)
				{
					ships[3].setHealth(3);
					sunk++;
				}
			}
			else
			{
				ships[2].Hit();
				if (ships[2].Health() <= 0)
				{
					ships[2].setHealth(3);
					sunk++;
				}
			}

			grid[row][col] = 1;
			hits++;

			hit = 1;
			return hit;
		case 4:
			ships[1].Hit();
			if (ships[1].Status())
				sunk++;
			grid[row][col] = 1;
			hits++;

			hit = 1;
			return hit;
		case 5:
			ships[0].Hit();
			if (ships[0].Status())
				sunk++;
			grid[row][col] = 1;
			hits++;

			hit = 1;
			return hit;
	}
	return 2;
}