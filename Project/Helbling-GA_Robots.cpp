/*
**  GA_Robots.cpp - Final Project: Genetic Algorithm Robots
**  Cole Helbling, CISP 400
**  07 May 2018
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

class Robot;

const bool dummy = (srand(time(NULL)), true); // Seed random for constants
const int SIZE_X = 12;         // Size of map X
const int SIZE_Y = 12;         // Size of map Y
const int BATTERIES_MAX = (SIZE_X * SIZE_Y) * 0.4;
const int CHROMOSOME_SIZE = 9; // Amount of characters in a single chromosome
const int DNA_TOT_SIZE = 216;  // Total amount of characters
const int MAP_X = SIZE_X + 2;  // Size of map X + 2 (border walls)
const int MAP_Y = SIZE_Y + 2;  // Size of map Y + 2 (border walls)
const int MOVES_MAX = 12 + rand() % ((25 + 1) - 12);
const int POPULATION = 200;    // Size of population
const int STARTING_ENERGY = 7 + rand() % ((10 + 1) - 7);

// Function Prototypes
int generateMap(vector<vector<int>> &);
string getRandomDNA(int);
vector<vector<int>> convertGeneToDecimal(Robot);
// void displayMap(vector<vector<int>>, Robot); // A debug function, never actually used except for testing
void parseGenes(vector<Robot>, vector<vector<int>>);
void Reproduce(vector<Robot> &);

class Robot
{
    string DNA;
    float fitness;
    int id;
    int energy = STARTING_ENERGY;
    int energy_gathered = 0;
    int moves = 0;
    bool move = false;
    int currX = -1;
    int currY = -1;
    static int currID;
    static int generation;
    string facing = "";
    vector<string> genes;
public:
    Robot() : DNA(""), fitness(0.0f), id(currID++)
    {
    }

    int getID()
    {
        return id;
    }

    void incrementGeneration()
    {
        generation++;
    }

    int getGeneration()
    {
        return generation;
    }

    void harvestBattery(vector<vector<int>> & grid, int x, int y)
    {
        if (grid[x][y] == 11)
        {
            energy += 5;
            energy_gathered += 5;
            grid[x][y] = 12;
        }
    }

    void setPosition(vector<vector<int>> & grid, int x, int y)
    {
        harvestBattery(grid, x, y); // checks if battery is at new position so we don't have to do it elsewhere

        if (currX == x && currY == y)
            return;
        else
        {
            currX = x;
            currY = y;
        }
    }

    /*int getX()
    {
        return currX;
    }

    int getY()
    {
        return currY;
    }*/

    void setDNA(string d)
    {
        DNA = d;
        int offset = 0;

        for (int i = 0; i < (DNA_TOT_SIZE / CHROMOSOME_SIZE); i++)
        {
            string temp = "";

            for (int j = 0; j < CHROMOSOME_SIZE; j++)
                temp += DNA[j + offset];

            offset += CHROMOSOME_SIZE;
            genes.push_back(temp);
        }
    }

    string getDNA()
    {
        return DNA;
    }

    vector<string> getGenes()
    {
        return genes;
    }

    void setFitness(float fit)
    {
        fitness = (float) fit;
    }

    float getFitness()
    {
        if (fitness == 0.0f)
            fitness = (float) moves / (energy + 1);

        return fitness;
    }

    void decrementEnergy()
    {
        energy--;
    }

    int getEnergy()
    {
        if (energy > STARTING_ENERGY)
            return energy - STARTING_ENERGY;
        else
            return 0;
    }

    int getEnergyGathered()
    {
        return energy_gathered;
    }

    void incrementMoves()
    {
        moves++;
    }

    int getMoves()
    {
        return moves;
    }

    int observeSurroundings(vector<vector<int>> grid, string dir = "", int looking = 0)
    {
        int N, S, E, W = {0};
        int NE, NW, SE, SW = {0};

        if (!((currX >= MAP_X - 1 || currX - 1 < 0) || (currY >= MAP_Y - 1 || currY - 1 < 0)))
        {
            N = grid[currX][currY - 1];
            S = grid[currX][currY + 1];
            E = grid[currX - 1][currY];
            W = grid[currX + 1][currY];

            NE = grid[currX + 1][currY - 1];
            NW = grid[currX - 1][currY - 1];
            SE = grid[currX + 1][currY + 1];
            SW = grid[currX - 1][currY + 1];
        }
        else
        {
            N = 10;
            S = 10;
            E = 10;
            W = 10;

            NE = 10;
            NW = 10;
            SE = 10;
            SW = 10;
        }

        if (dir == "N")
        {
            if (N == looking)
                return 1;
        }
        else if (dir == "S")
        {
            if (S == looking)
                return 1;
        }
        else if (dir == "E")
        {
            if (E == looking)
                return 1;
        }
        else if (dir == "W")
        {
            if (W == looking)
                return 1;
        }
        else if (dir == "NE")
        {
            if (NE == looking)
                return 1;
        }
        else if (dir == "NW")
        {
            if (NW == looking)
                return 1;
        }
        else if (dir == "SE")
        {
            if (SE == looking)
                return 1;
        }
        else if (dir == "SW")
        {
            if (SW == looking)
                return 1;
        }

        return 0;
    }

    void setDirection(string dir)
    {
        facing = dir;
    }

    string getDirection()
    {
        return facing;
    }

    void Rotate(string rot)
    {
        // The robot has sensors facing all directions
        // Rotation is unnecessary for a robot with omnidirectional travel

        return;
    }

    void setMove(int mov)
    {
        move = mov;
    }

    int getMove()
    {
        return move;
    }

    void Move(vector<vector<int>> grid)
    {
        if (getEnergy() > 0)
        {
            if (!((currX >= MAP_X - 1 || currX - 1 < 0) || (currY >= MAP_Y - 1 || currY - 1 < 0)))
            {
                if (facing == "N")
                    setPosition(grid, currX, currY - 1);
                else if (facing == "S")
                    setPosition(grid, currX, currY + 1);
                else if (facing == "E")
                    setPosition(grid, currX - 1, currY);
                else if (facing == "W")
                    setPosition(grid, currX + 1, currY);
                else if (facing == "SW")
                    setPosition(grid, currX - 1, currY + 1);
                else if (facing == "SE")
                    setPosition(grid, currX + 1, currY + 1);
                else if (facing == "NW")
                    setPosition(grid, currX - 1, currY - 1);
                else if (facing == "NE")
                    setPosition(grid, currX + 1, currY - 1);
            }
        }
        else
            setPosition(grid, currX, currY);

        setMove(0);
    }

    string getSensorState(vector<vector<int>> grid)
    {
        string direction;
        string rotation = "00";
        string check_battery = to_string(observeSurroundings(grid, facing, 11));
        string check_wall = to_string(observeSurroundings(grid, facing, 10));

        // Ugly, but since `switch` doesn't work with strings and I'm too lazy to convert these directions to ints like I did in parseGenes()
        if (facing == "N")
            direction = "1000";
        if (facing == "S")
            direction = "0100";
        if (facing == "E")
            direction = "0010";
        if (facing == "W")
            direction = "0001";
        if (facing == "SW")
            direction = "0101";
        if (facing == "SE")
            direction = "0110";
        if (facing == "NW")
            direction = "1001";
        if (facing == "NE")
            direction = "1010";
        else
            direction = "0000";

        return direction + to_string(getMove()) + rotation + check_battery + check_wall;
    }

    int Simulate(vector<vector<int>> grid)
    {
        if (moves >= MOVES_MAX || energy <= 0)
            return -1; // Simulation is over

        if (getMove())
            Move(grid);

        incrementMoves();   // Sitting there is considered a move for convenience's sake
        decrementEnergy();  // Sitting there also takes energy

        return 0;
    }
};
int Robot::currID = 0;
int Robot::generation = 1;

int main()
{
    vector<float> avg_fitness;
    float total_fitness;
    vector<vector<int>> map;
    int robots_alive;
    int sim;
    ofstream log("data.csv");
    vector<Robot> robots(POPULATION);
    Robot temp;

    for (int i = 0; i < POPULATION; i++)
    {
        robots[i].setDNA(getRandomDNA(DNA_TOT_SIZE)); // DNA_TOT_SIZE random bits as a string ("(DNA_TOT_SIZE / CHROMOSOME_SIZE) genes of CHROMOSOME_SIZE characters")
        robots[i].setFitness(0.0f);
    }

    do
    {
        sim = 0;
        robots_alive = robots.size();
        total_fitness = 0.0f;

        map.clear();
        map.resize(MAP_X, vector<int>(MAP_Y, 12));

        // Set border walls
        for (int i = 0; i < MAP_X; i++)
            for (int j = 0; j < MAP_Y; j++)
                if ((i == 0 || j == 0) || (i == MAP_X - 1 || j == MAP_Y - 1))
                    map[i][j] = 10;

        generateMap(map);
        parseGenes(robots, map);

        for (size_t i = 0, max = robots.size(); i != max; i++)
        {
            robots[i].setPosition(map, (1 + rand() % ((12 + 1) - 1)), (1 + rand() % ((12 + 1) - 1)));

            while (sim != -1)
                sim = robots[i].Simulate(map); // interact with environment

            total_fitness += robots[i].getFitness();
            robots_alive--;
        }

        avg_fitness.push_back(total_fitness / (float) robots.size());

        cout << "avg fitness for gen " << robots[0].getGeneration() << ": " << setprecision(10) << avg_fitness[robots[0].getGeneration() - 1] << endl;

        log << avg_fitness[robots[0].getGeneration() - 1] << ',';

        // CITIATION: https://stackoverflow.com/a/29676310 - How to easily sort a vector based on a certain member using sort and a lambda function
        sort(robots.begin(),
             robots.end(),
             [](Robot& left, Robot& right)
             {
                return left.getEnergyGathered() > right.getEnergyGathered();
             });
        robots.resize(robots.size() / 2);
        Reproduce(robots);
    } while (temp.getGeneration() <= 16); // It takes about 16 generations for the robots to reach their most efficient state, though there are always outliers where they under-perform or over-perform

    log.close();

    return 0;
}

string getRandomDNA(int length)
{
    string DNA;

    for (int i = 0; i < length; i++)
    {
        float random = (float) rand() / RAND_MAX;

        if (random > 0.5f)
            DNA += "1";
        else
            DNA += "0";
    }

    return DNA;
}

// OBJECT_TYPE { WALL = 10, BATTERY = 11, NONE = 12, IGNORE = 13 };
int generateMap(vector<vector<int>> & grid)
{
    int batteries = 0;
    int random_x = 0;
    int random_y = 0;

    while (batteries < BATTERIES_MAX)
    {
        random_x = rand() % MAP_X;
        random_y = rand() % MAP_Y;

        if (grid[random_x][random_y] == 11 || grid[random_x][random_y] == 10)
            continue;

        grid[random_x][random_y] = 11;
        batteries++;
    }

    return 0;
}

void Reproduce(vector<Robot> & robots)
{
    int first = 0, second = 1;
    string child1;
    string child2;
    vector<Robot> children(robots.size());

    Robot temp;
    temp.incrementGeneration();

    if (children.size() % 2 != 0)
        robots.pop_back();

    while (first < ((int) robots.size()) && second <= ((int) robots.size()))
    {
        string first_robot = robots[first].getDNA();
        string second_robot = robots[second].getDNA();

        child1 = first_robot.substr(0, first_robot.length() / 2) + second_robot.substr(second_robot.length() / 2);
        child2 = second_robot.substr(0, second_robot.length() / 2) + first_robot.substr(first_robot.length() / 2);

        int random = rand() % 100;

        if (random < 5)
        {
            random = rand() % DNA_TOT_SIZE;

            switch (rand() % 2)
            {
                case 0:
                    child1[random] ^= 1; // Mutation; flips whatever bit is present
                    break;
                case 1:
                    child2[random] ^= 1; // Mutation; flips whatever bit is present
                    break;
            }
        }

        children[first].setDNA(child1);
        children[second].setDNA(child2);

        first += 2;
        second += 2;
    }
    robots.insert(robots.end(), children.begin(), children.end());
    // vector<Robot>(children).swap(robots);
}

void parseGenes(vector<Robot> robots, vector<vector<int>> grid)
{
    bool found_match = false;

    for (size_t i = 0, max = robots.size(); i != max; i++)
    {
        vector<vector<int>> gene = convertGeneToDecimal(robots[i]);
        vector<string> bin_genes = robots[i].getGenes();
        string sensor = robots[i].getSensorState(grid);

        for (size_t j = 0, len = gene.size(); j != len; j++)
        {
            if (bin_genes[j].substr(0,7) == sensor.substr(0,7) && !found_match) // only act on the first action that matches sensor state
            {
                found_match = true;
                switch (gene[j][0])
                {
                    case 0:
                    case 3:
                    case 12:
                    case 15: // no change in direction
                        break;
                    case 1:
                    case 13: // face W
                        robots[i].setDirection("W");
                        break;
                    case 2:
                    case 14: // face E
                        robots[i].setDirection("E");
                        break;
                    case 4:
                    case 7: // face S
                        robots[i].setDirection("S");
                        break;
                    case 8:
                    case 11: // face N
                        robots[i].setDirection("N");
                        break;
                    case 5: // face SW
                        robots[i].setDirection("SW");
                        break;
                    case 6: // face SE
                        robots[i].setDirection("SE");
                        break;
                    case 9: // face NW
                        robots[i].setDirection("NW");
                        break;
                    case 10: // face NE
                        robots[i].setDirection("NE");
                        break;
                }

                switch (gene[j][1])
                {
                    case 0:
                        robots[i].setMove(0);
                        break;
                    case 1:
                        robots[i].setMove(1);
                        break;
                }

                switch (gene[j][2])
                {
                    case 0:
                    case 3:
                        break;
                    case 1:
                        robots[i].Rotate("L");
                        break;
                    case 2:
                        robots[i].Rotate("R");
                        break;
                }
            }
        }
    }
}

/*void displayMap(vector<vector<int>> grid, Robot robot)
{
    cout << endl;
    for (int i = 0; i < MAP_X; i++)
    {
        for (int j = 0; j < MAP_Y; j++)
        {
            if (i == robot.getX() && j == robot.getY())
                cout << 99 << ' ';
            else
                cout << grid[i][j] << ' ';
        }
        cout << endl;
    }
}*/

vector<vector<int>> convertGeneToDecimal(Robot robot)
{
    vector<vector<int>> decimal; // break up into 5 parts -- direction, rotation, battery, wall, move (4,2,1,1,1)
    vector<string> genes = robot.getGenes();

    for (size_t i = 0, max = genes.size(); i != max; i++)
    {
        vector<int> dec(5);
        vector<int> splitter(5);
        // Decide which bits of the DNA count as which option. If anything here changes, parseGenes() also needs to be changed
        splitter[0] = stoi(genes[i].substr(0,4));
        splitter[1] = stoi(genes[i].substr(4,1));
        splitter[2] = stoi(genes[i].substr(5,2));
        splitter[3] = stoi(genes[i].substr(7,1));
        splitter[4] = stoi(genes[i].substr(8,1));

        for (size_t j = 0, len = splitter.size(); j != len; j++)
        {
            int number = splitter[j];
            int remainder, base = 1;

            while (number > 0)
            {
                remainder = number % 10;
                dec[j] += remainder * base;
                base *= 2;
                number /= 10;
            }
        }

        decimal.push_back(dec);
    }

    return decimal;
}
