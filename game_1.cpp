#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <string>
#include <iomanip>

class Player //player class to store their information
{
public:
    std::string name;  //name
    int guesses;       //guesses

    Player() : name(""), guesses(0) {}

    Player(std::string nm, int ng) : name(nm), guesses(ng) {}
};

class LeaderBoard //leaderboard class for storing leaderboard information
{
private:
    static const int NUM_LEADERS = 5; //max number of leaders i.e. 5 here
    Player leaders[NUM_LEADERS];

public:
    LeaderBoard()
    {
        for (int i = 0; i < NUM_LEADERS; i++)
        {
            leaders[i].name = "";
            leaders[i].guesses = 0;
        }
    }

    void InsertPlayer(Player player) //inserting player details into the leaderboard as per their guesses
    {
        for (int i = 0; i < NUM_LEADERS; i++)
        {
            if (player.guesses < leaders[i].guesses || leaders[i].guesses == 0)
            {
                for (int j = NUM_LEADERS - 1; j > i; j--)
                {
                    leaders[j].name = leaders[j - 1].name;
                    leaders[j].guesses = leaders[j - 1].guesses;
                }
                leaders[i].name = player.name;
                leaders[i].guesses = player.guesses;
                break;
            }
        }
    }

    void DisplayLeaderboard() //for displaying the current leaderboard
    {
        std::cout << "\nHere are the current leaders: \n";
        for (int i = 0; i < NUM_LEADERS; i++)
        {
            if (leaders[i].guesses != 0)
                std::cout << leaders[i].name << " made " << leaders[i].guesses << " guesses\n";
        }
        std::cout << "\n";
    }

    void ReadLeaders(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cout << "Error opening file\n";
            return;
        }

        int i = 0;
        while (!file.eof() && i < NUM_LEADERS)
        {
            std::string line;
            std::getline(file, line);
            if (line.empty())
                break;

            //using temporary variables, otherwise displaying names was an issue
            char name[100]; //assuming names not longer than 100 characters
            int guesses;

            if (sscanf(line.c_str(), "%s made %d guesses", name, &guesses) == 2)
            {
                //storing info in the leaderboard array
                leaders[i].name = name;
                leaders[i].guesses = guesses;
                i++;
            }
        }
        file.close();
    }

    void WriteLeaders(const std::string &filename) //writing leaderboard info into a file
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            std::cout << "Error opening file\n";
            return;
        }

        for (int i = 0; i < NUM_LEADERS; i++)
        {
            if (leaders[i].guesses != 0)
                file << leaders[i].name << " made " << leaders[i].guesses << " guesses\n";
        }
        file.close();
    }
};

//function to play the guessing game
void PlayGuessingGame(int& guess_num, int random_num, int& num_guesses, LeaderBoard& leaderboard)
{
    std::cout << "Please enter your name to start:   ";
    std::string name;
    std::cin >> name;

    std::cout << std::fixed << std::setprecision(3) << sqrt(random_num) << " is the square root of what number? ";
    std::cin >> guess_num;
    num_guesses = 1;
    while (guess_num != random_num)
    {
        num_guesses++;
        if (guess_num > random_num)
        {
            std::cout << "too high, guess again: ";
        }
        else
        {
            std::cout << "too low, guess again: ";
        }
        std::cin >> guess_num;
    }
    std::cout << "you got it, " << name << "!\n";

    Player currentPlayer(name, num_guesses);
    leaderboard.InsertPlayer(currentPlayer);

    leaderboard.DisplayLeaderboard(); // display leaderboard
}

int main()
{
    LeaderBoard leaderboard;
    leaderboard.ReadLeaders("leaders.txt"); //read the data from the txt file

    std::cout << "Welcome! Press q to quit or any key to continue: ";
    char c;
    std::cin >> c;

    while (c != 'q')
    {
        srand(time(NULL));
        int random_num = rand() % 91 + 10;
        int guess_num;
        int num_guesses;

        PlayGuessingGame(guess_num, random_num, num_guesses, leaderboard);

        std::cout << "Press q to quit or any key to continue: ";
        std::cin >> c;
    }

    leaderboard.WriteLeaders("leaders.txt");
    std::cout << "Bye Bye !";

    return 0;
}