#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cctype>
using namespace std;

int main() {
    srand(time(0));

    bool gameEnd = false;
    bool flag = true;
    bool guessed = false;
    bool invalid = false;
    bool repeat;

    int r;
    int c = 0;
    int score = 0;
    int numGuesses = 6;
    int hintsGiven;
    int foundIndex;
    int count;
    int hintIndex;
    
    string wordFile = "words.txt";
    string scoreFile = "scores.txt";
    string input;
    string guess;
    string randomWord;
    string name;
    string line;

    vector<string> guesses;
    vector<string> words;

    string hangmen[7] = {
        " _______\n|       |\n|\n|\n|\n|____",
        " _______\n|       |\n|       O\n|\n|\n|____",
        " _______\n|       |\n|       O\n|       |\n|\n|____",  
        " _______\n|       |\n|       O\n|     __|\n|\n|____", 
        " _______\n|       |\n|       O\n|     __|__\n|\n|____",
        " _______\n|       |\n|       O\n|     __|__\n|      /\n|____",
        " _______\n|       |\n|       O\n|     __|__\n|      / \\\n|____"
    };

    ifstream in;
    in.open(wordFile);
    if(in.fail()) {
        cout << "Failed to load words." << endl;
        in.clear();
    }
    else {
        while(getline(in, line)) {
            words.push_back(line);
        }
        in.close();
    }

    do {
        cout << "Enter your name: ";
        getline(cin, name);

        if(name.length() < 1) {
            invalid = true;
        }
        else {
            for(char c: name) {
                if(!isalpha(c)) {
                    invalid = true;
                    break;
                }
            }
        }
    } while(invalid);
    
    cout << "Welcome, " << name << "!" << endl;

    do {
        cout << "View previous scores? y/n: ";
        getline(cin, input);
        if(input == "y") {
            ifstream scores;
            scores.open(scoreFile); 
            cout << endl << "Previous scores: " << endl;
            while(getline(scores, line)) {
                cout << line << endl;
                c++;
            }
            scores.close();
            if(c == 0) {
                cout << "No previous scores have been saved." << endl;
            }
            
        }
    } while(input != "y" && input != "n" || input.length() > 1);
            
    do {
        r = rand() % words.size();
        randomWord = words.at(r); 
        char blanks[randomWord.length()];
        for(int i = 0; i < randomWord.length(); i++) {
            blanks[i] = '_';
        }

        numGuesses = 6;
        hintsGiven = 0;
        gameEnd = false;
        flag = true;
        guessed = false;
        guesses.clear();

        do {
            cout << hangmen[6-numGuesses] << endl;

            if(numGuesses <= 3) {
                count = 0;
                for(int i = 0; i < randomWord.length(); i++) {
                    if(blanks[i] == '_') {
                        count++;
                    }
                }
                if(count > 1 && hintsGiven < 2) {
                    do {
                        cout << endl << "Would you like a hint? y/n: ";
                        getline(cin, input);
                        if(input == "y") {
                            hintsGiven++;
                            for(int i = 0; i < randomWord.length(); i++) {
                                if(blanks[i] == '_') {
                                    blanks[i] = randomWord[i];
                                    hintIndex = i;
                                    break;
                                }
                            }
                            for(int i = 0; i < randomWord.length(); i++) {
                                if(blanks[hintIndex] == randomWord[i]) {
                                    blanks[i] = randomWord[i];
                                }
                            }
                        }
                        else if (input == "n") {
                            numGuesses++;
                        }
                    } while(input != "y" && input != "n" || input.length() > 1);
                }
            }

            cout << endl;
            for(char c: blanks) {
                cout << c;
            }
            cout << endl << "Guesses remaining: " << numGuesses << endl;
            
            do {
                cout << "Enter a letter: ";
                getline(cin, guess);
               
                for(string s: guesses) {
                    if(guess == s) {
                        guessed = true;
                    }
                }
                if(guessed) {
                    cout << "You already guessed that letter." << endl;
                    guessed = false;
                }
                else {
                    guesses.push_back(guess);
                }

            } while(guess.length() > 1 || !isalpha(guess[0]) || guessed);

            foundIndex = -1;
            for(int i = 0; i < randomWord.length(); i++) {
                if(guess[0] == randomWord[i]) {
                    blanks[i] = randomWord[i];
                    foundIndex = 0;
                }
            }
            if(foundIndex == -1 && !guessed) {
                cout << "Try again!" << endl;
                numGuesses--;
            }

            for(char c: blanks) {
                if(c != '_') {
                    flag = false;
                }
                else {
                    flag = true;
                    break;
                }
            }
            if(!flag) {
                gameEnd = true;
                if(numGuesses == 0) {
                    cout << hangmen[6] << endl;
                }
                score++;
                cout << "You won!" << endl;
            }
            else if(numGuesses == 0) {
                gameEnd = true;
                cout << hangmen[6] << endl;
                cout << "You lost!" << endl;
            }
        } while(!gameEnd);
        cout << "The word was " << randomWord << "." << endl << "Your score is " << score << "." << endl;

        do {
            cout << endl << "Play again? y/n: ";
            getline(cin, input);
            if(input == "y") {
                repeat = true;
                cout << endl;
            }
            else if(input == "n") {
                cout << "Thank you for playing!" << endl;
                repeat = false;
            }
        } while(input != "y" && input != "n" || input.length() > 1);
    } while(repeat);

    ofstream out;
    out.open(scoreFile, fstream::app);
    out << name << ": " << score << endl;
    out.close();

    return 0;
}
