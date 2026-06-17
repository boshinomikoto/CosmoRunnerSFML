#pragma once
#include <fstream>
#include <string>

class Records
{
public:
    Records() { load(); }

    void tryUpdate(int score)
    {
        if (score > bestScore) 
        { 
            bestScore = score; 
            save(); 
        }
    }

    int getBest() { return bestScore; }

private:
    void save()
    {
        std::ofstream file("Records/text.txt");
        if (file.is_open()) file << bestScore;
    }
    void load()
    {
        std::ifstream file("Records/text.txt");
        if (file.is_open()) file >> bestScore;
    }
    int bestScore = 0;
};