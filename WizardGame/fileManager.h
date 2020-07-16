#pragma once
#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <experimental/filesystem>

std::string removeWord(std::string str, std::string word)
{
    // Check if the word is present in string 
    // If found, remove it using removeAll() 
    if (str.find(word) != std::string::npos)
    {
        size_t p = -1;

        // To cover the case 
        // if the word is at the 
        // beginning of the string 
        // or anywhere in the middle 
        std::string tempWord = word + " ";
        while ((p = str.find(word)) != std::string::npos)
            str.replace(p, tempWord.length(), "");

        // To cover the edge case 
        // if the word is at the 
        // end of the string 
        tempWord = " " + word;
        while ((p = str.find(word)) != std::string::npos)
            str.replace(p, tempWord.length(), "");
    }

    // Return the resultant string 
    return str;
}

std::string convertPath(std::string input, std::vector<std::string> mods) {
    std::string temp = input;
    for (std::string folder : mods) {
        bool exist = std::experimental::filesystem::exists("./mods/"+ folder + "/" + removeWord(input, "./res"));
        if (exist) {
            temp = "./mods/" + folder + "/" + removeWord(input, "./res");
        }
        
    }
	return temp;
}