#pragma once
#include <string>
#include <vector>
#include <experimental/filesystem>
extern std::vector<std::string> mods;
static int getCharCount(std::string file) {
    ifstream  fin(file);
    char ch;
    int i, c = 0, sp = 0;
    while (fin)
    {
        fin.get(ch);
        i = ch;
        if ((i > 63 && i < 91) || (i > 96 && i < 123))
            c++;
        else
            if (ch == ' ')
                sp++;
    }
    return c;
}
static std::string removeWord(std::string str, std::string word)
{
    if (str.find(word) != std::string::npos)
    {
        size_t p = -1;
        std::string tempWord = word + " ";
        while ((p = str.find(word)) != std::string::npos)
            str.replace(p, tempWord.length(), "");
        tempWord = " " + word;
        while ((p = str.find(word)) != std::string::npos)
            str.replace(p, tempWord.length(), "");
    }
    return str;
}
static std::string convertPath(std::string input) {
    std::string temp = input;
    
    for (std::string folder : mods) {
        bool exist = std::experimental::filesystem::exists("./mods/" + folder + "/" + removeWord(input, "./res"));
        if (exist) {
            temp = "./mods/" + folder + "/" + removeWord(input, "./res");
        }
    }
    std::cout << "character count for " << temp << " is: " << getCharCount(temp) << "\n";
    return temp;
}