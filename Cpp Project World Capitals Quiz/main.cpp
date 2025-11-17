#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include <functional>

using namespace std;

void loadCapitals(map<string, string>& capitals) {
    ifstream file("WorldCapitals.txt");
    if (!file.is_open()) {
        cerr << "Error: Cannot open WorldCapitals.txt" << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        size_t pos = line.find(':');
        if (pos != string::npos) {
            string country = line.substr(0, pos);
            string capital = line.substr(pos + 1);
            capitals[country] = capital;
        }
    }
    file.close();
}

void saveHighScore(int score) {
    // Read existing scores
    vector<int> scores;
    {
        ifstream in("HighScores.txt");
        if (in.is_open()) {
            int s;
            while (in >> s) scores.push_back(s);
        }
        // If file doesn't exist, scores remains empty (will be created)
    }

    // Add current score
    scores.push_back(score);

    // Sort descending
    sort(scores.begin(), scores.end(), greater<int>());

    // Keep only top 5
    if (scores.size() > 5) {
        scores.resize(5);
    }

    // Write back with sequence numbers
    ofstream out("HighScores.txt", ios::trunc);
    for (int i = 0; i < scores.size(); i++) {
        out << (i + 1) << ". " << scores[i] << '\n';
    }
}

void quiz(const map<string, string>& capitals) {
    vector<string> countries;
    for (const auto& pair : capitals) {
        countries.push_back(pair.first);
    }
    
    // Select 10 random countries
    random_device rd;
    mt19937 g(rd());
    shuffle(countries.begin(), countries.end(), g);
    if (countries.size() > 10) {
        countries.resize(10);
    }
    
    int score = 0;
    int maxScore = countries.size() * 3;
    
    for (const string& country : countries) {
        int points = 0;
        bool correct = false;
        
        for (int attempt = 1; attempt <= 3; attempt++) {
            cout << "What is the capital of " << country << "? (Attempt " << attempt << "/3): ";
            string answer;
            getline(cin, answer);
            
            if (answer == capitals.at(country)) {
                if (attempt == 1) {
                    points = 3;
                } else if (attempt == 2) {
                    points = 2;
                } else {
                    points = 1;
                }
                cout << "Correct! You got " << points << " points." << endl;
                correct = true;
                break;
            } else {
                if (attempt < 3) {
                    cout << "Wrong! Try again." << endl;
                } else {
                    cout << "Wrong! The correct answer is " << capitals.at(country) << "." << endl;
                    points = 0;
                }
            }
        }
        score += points;
    }
    
    cout << "\nYour total score is " << score << " out of " << maxScore << "." << endl;
    saveHighScore(score);
    cout << "Score saved to HighScores.txt" << endl;
}

int main() {
    map<string, string> capitals;
    loadCapitals(capitals);
    quiz(capitals);
    return 0;
}
