#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string_view>
#include <algorithm>
#include <cctype>
#include <unordered_set>

struct WordWithLine {
    std::string word;
    std::string content;
    std::string middle;
    int hasNumber;
};

std::vector<WordWithLine> extractWordsWithNumbers(const std::string& filename) {
    std::vector<WordWithLine> wordsWithNumbers;

    std::ifstream file(filename);
    if (!file) {
        std::cout << "Failed to open the file." << std::endl;
        return wordsWithNumbers;
    }

    std::string line;
    int lineNumber = 1;

    while (std::getline(file, line)) {
        std::size_t delimiterPos1 = line.find('|');
        std::size_t delimiterPos = line.find_last_of('|');
        if (delimiterPos != std::string::npos) {
            std::string firstWord = line.substr(0, delimiterPos1);
            std::string secondGroup = line.substr(delimiterPos1, delimiterPos - delimiterPos1+1);
            std::string content = line.substr(delimiterPos + 1);

            std::string wordFinal;
            bool containsNumber = false;
            for (char c : firstWord) {
                if (std::isdigit(c)) {
                    containsNumber = true;
                    break;
                }
                wordFinal.push_back(c);
            }
            if (containsNumber) {
                WordWithLine wordWithLine;
                wordWithLine.word = wordFinal;
                wordWithLine.content = content;
                wordWithLine.middle = secondGroup;
                wordWithLine.hasNumber = 1;
                wordsWithNumbers.push_back(wordWithLine);
            }
            else{
                WordWithLine wordWithLine;
                wordWithLine.word = wordFinal;
                wordWithLine.content = content;
                wordWithLine.middle = secondGroup;
                wordWithLine.hasNumber = 0;
                wordsWithNumbers.push_back(wordWithLine);
            }
        }
        lineNumber++;
    }

    file.close();

    return wordsWithNumbers;
}

std::vector<WordWithLine> mixContentOfRepeatedWords(const std::vector<WordWithLine>& wordsWithNumbers) {
    std::unordered_map<std::string, std::string> uniqueWords;
    std::vector<WordWithLine> updatedWords;

    for (const WordWithLine& word : wordsWithNumbers) {
        if (word.hasNumber == 1) {
            auto it = uniqueWords.find(word.word);
            if (it != uniqueWords.end()) {
                // Word is repeated, mix the content
                it->second += " " + word.content;
            } else {
                // Store the word and its content
                uniqueWords[word.word] = word.content;
            }
        }
    }

    for (const WordWithLine& word : wordsWithNumbers) {
        auto it = uniqueWords.find(word.word);
        if (it != uniqueWords.end()) {
            // Create a new WordWithLine with updated content
            WordWithLine updatedWord = word;
            updatedWord.content = it->second;
            updatedWords.push_back(updatedWord);
            // Erase the word from uniqueWords to avoid duplicates
            uniqueWords.erase(it);
        } else {
            // Word is not repeated, add it as-i
            int exists = 0;
            for(WordWithLine x : updatedWords){
                if(x.word == word.word){
                    exists = 1;
                }
            }
            if(exists == 0){
                updatedWords.push_back(word);
            }
        }
    }
    return updatedWords;
}

int main() {
    std::string filename = "EnglishWords.txt";  // Replace with the path to your text file
    std::ofstream exitFile("output.txt");

    std::vector<WordWithLine> wordsWithNumbers = extractWordsWithNumbers(filename);
    std::vector<WordWithLine> final = mixContentOfRepeatedWords(wordsWithNumbers);
    int x = 0;
    if (final.empty()) {
        std::cout << "No words with numbers found." << std::endl;
    } else {
        for (const WordWithLine& wordWithLine : final) {
            exitFile << wordWithLine.word << wordWithLine.middle << wordWithLine.content << std::endl;
        }
        std::cout << x << std::endl;
    }

    return 0;
}