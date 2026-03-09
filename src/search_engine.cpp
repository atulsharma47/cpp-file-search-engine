#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;

unordered_map<string, vector<string>> indexDB;


// lowercase helper
string toLower(string str)
{
    transform(str.begin(), str.end(), str.begin(),
              [](unsigned char c){ return tolower(c); });
    return str;
}


// split words
vector<string> tokenize(const string& line)
{
    vector<string> words;
    string word;

    for(char c : line)
    {
        if(isalnum(c))
        {
            word += tolower(c);
        }
        else
        {
            if(!word.empty())
            {
                words.push_back(word);
                word.clear();
            }
        }
    }

    if(!word.empty())
        words.push_back(word);

    return words;
}


// build index
void buildIndex(const string& folder)
{
    cout << "Building index...\n";

    for(const auto& entry : fs::recursive_directory_iterator(folder))
    {
        if(entry.is_regular_file())
        {
            string path = entry.path().string();

            ifstream file(path);
            if(!file.is_open())
                continue;

            string line;
            int lineNumber = 1;

            while(getline(file, line))
            {
                vector<string> words = tokenize(line);

                for(const string& w : words)
                {
                    indexDB[w].push_back(path + ":" + to_string(lineNumber));
                }

                lineNumber++;
            }
        }
    }

    // save index
    ofstream out("index.db");

    for(auto& pair : indexDB)
    {
        out << pair.first;

        for(auto& loc : pair.second)
            out << "|" << loc;

        out << endl;
    }

    cout << "Index saved to index.db\n";
}


// load index
void loadIndex()
{
    ifstream in("index.db");

    if(!in.is_open())
    {
        cout << "Index file not found. Run index command first.\n";
        exit(1);
    }

    string line;

    while(getline(in, line))
    {
        size_t pos = line.find('|');

        if(pos == string::npos)
            continue;

        string word = line.substr(0, pos);

        vector<string> locations;

        size_t start = pos + 1;

        while(true)
        {
            size_t next = line.find('|', start);

            if(next == string::npos)
            {
                locations.push_back(line.substr(start));
                break;
            }

            locations.push_back(line.substr(start, next - start));
            start = next + 1;
        }

        indexDB[word] = locations;
    }
}


// search
void searchWord(const string& word)
{
    string key = toLower(word);

    if(indexDB.find(key) == indexDB.end())
    {
        cout << "No results found.\n";
        return;
    }

    for(const string& result : indexDB[key])
        cout << result << endl;
}


int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        cout << "Usage:\n";
        cout << "search_engine.exe index <folder>\n";
        cout << "search_engine.exe search <word>\n";
        return 1;
    }

    string command = argv[1];

    if(command == "index")
    {
        buildIndex(argv[2]);
    }
    else if(command == "search")
    {
        loadIndex();
        searchWord(argv[2]);
    }

    return 0;
}