#include <iostream>
#include <string>

using namespace std;

class Match 
{
    /*
        Class to create a tennis match between two named players, A and B. 
        A will always serve first.
    */
    public:
        Match(const string &aPlayer, const string &bPlayer)
        {
            aName_ = aPlayer;
            bName_ = bPlayer;
        }

        friend void playGame(Match match, int numberOfGames);
        void aWinsPoint();
        void bWinsPoint();
        void winPoint(const string& playerName);
        void umpire(ostream& out) const;
        void scoreboard(ostream& out) const;

    private:
        void resetPoints()
        {
            aPoints_ = bPoints_ = 0;
        }

        int aGames_ = 0;
        int bGames_ = 0;
        int aPoints_ = 0;
        int bPoints_ = 0;
        bool serve_ = true; // A serves when true, B serves when false;
        string aName_;
        string bName_;

        // Used to convert points to umpire and scoreboard formats.
        const string umpireScore[5] = {"love", "fifteen", "thirty", "forty", "advantage"};
        const string boardScore[5] = {"0", "15", "30", "40", "Ad"};
};

bool gameWon(int myPoints, int otherPoints)
{
    // self has scored after 40 pts and other has 30 or fewer pts, self wins
    if (myPoints == 4 && otherPoints <= 2)
    {
        return true;
    } 
    // self has greater than Advantage, other has 40 pts, self wins
    else if (myPoints == 5 && otherPoints == 3)
    {
        return true;
    }

    else return false;
}

void Match::winPoint(const string& playerName)
{
    if (playerName == aName_)
    {
        // If B has Advantage, B loses a point, to return to deuce
        if (bPoints_ == 4 && aPoints_ == 3) 
        {
            --bPoints_;
        } else
        {
            ++aPoints_;
        }

        // A wins the game, resetting the points and switching who serves
        if (gameWon(aPoints_, bPoints_))
        {
            ++aGames_;
            resetPoints();

            // Outputs scoreboard when game is won
            if (serve_) 
            {
                cout << "[ " << aName_ << "* " << aGames_ << " - " 
                    << bGames_ << " " << bName_ << " ]" << endl;
            } else
            {
                cout << "[ " << aName_ << " " << aGames_ << " - " 
                    << bGames_ << " *" << bName_ << " ]" << endl;
            }

            // Umpire announcement when game is won
            cout << "game, " << aName_ << "!" << endl;
            cout << endl;

            serve_ = !serve_;
        } 
    } else
    {
        // If A has Advantage, A loses a point, to return to deuce
        if (aPoints_ == 4 && bPoints_ == 3) 
        {
            --aPoints_;
        } else
        {
            ++bPoints_;
        }

        // B wins the game, resetting the points and switching who serves
        if (gameWon(bPoints_, aPoints_))
        {
            ++bGames_;
            resetPoints();

            if (serve_) 
            {
                cout << "[ " << aName_ << "* " << aGames_ << " - " 
                    << bGames_ << " " << bName_ << " ]" << endl;
            } else
            {
                cout << "[ " << aName_ << " " << aGames_ << " - " 
                    << bGames_ << " *" << bName_ << " ]" << endl;
            }

            cout << "game, " << bName_ << "!" << endl;
            cout << endl;

            serve_ = !serve_;
        }
    }
}

void Match::umpire(ostream& out) const
{
        if (aPoints_ == 4 && bPoints_ == 3) // If A has advantage
        {
            out << "advantage " << aName_ << "." << endl;
        }
        else if (aPoints_ == 3 && bPoints_ == 4) // If B has advantage
        {
            out << "advantage " << bName_ << "." << endl;
        } 
        else if (aPoints_ == 0 && bPoints_ == 0) // If love-all
        {
            out << "love-all." << endl;
        } 
        else if (aPoints_ == 3 && bPoints_ == 3) // If deuce
        {
            out << "deuce." << endl;
        }
        else
        {
            out << umpireScore[aPoints_] << "-" << umpireScore[bPoints_] << "." << endl;
        }
}

void Match::scoreboard(ostream &out) const
{
    {
        if (serve_) // If A is serving
        {
            if (aPoints_ == 4) // If A has Advantage
            {
                out << "[ " << aName_ << "*" << " Ad - " << bName_ << " ]" << endl;
            } else if (bPoints_ == 4) // If B has Advantage
            {
                out << "[ " << aName_ << "*" << " - Ad " << bName_ << " ]" << endl;
            } else
            {
                out << "[ " << aName_ << "* " << boardScore[aPoints_] << " - " 
                << boardScore[bPoints_] << " " << bName_ << " ]" << endl;
            }

        } else // If B is serving
        {
            if (aPoints_ == 4) // If A has Advantage
            {
                out << "[ " << aName_ << " " << " Ad - *" << bName_ << " ]" << endl;
            } else if (bPoints_ == 4) // If B has Advantage
            {
                out << "[ " << aName_ << " " << " - Ad *" << bName_ << " ]" << endl;
            } else
            {
                out << "[ " << aName_ << " " << boardScore[aPoints_] << " - " 
                << boardScore[bPoints_] << " *" << bName_ << " ]" << endl;     
            }
        }
    }
}

ostream& operator<<(ostream& out, const Match& match)
{
    // Overloads << operator
    match.scoreboard(out);
    match.umpire(out);
    return out;
}

void playGame(Match match, int numberOfGames)
{
    int aGame = match.aGames_;
    int bGame = match.bGames_;

    // Loops until the specified number of games have been played
    while ((match.aGames_ + match.bGames_) - (aGame + bGame) < numberOfGames)
    {
        cout << match;

        // Selects a random winner
        int random = rand() % 2;
        if (random == 1)
        {
            match.winPoint(match.aName_);
        } else match.winPoint(match.bName_);
    }
}


int main()
{
    Match match1("Adam", "Badam");

    // Plays until 3 games are won.
    playGame(match1, 3);

    return EXIT_SUCCESS;
}