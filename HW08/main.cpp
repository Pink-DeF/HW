#include <iostream>
#include <filesystem>
#include <regex>
#include <vector>
#include <string>

using namespace std;
namespace fs = filesystem;

class FileRenamer
{
private:
//Подготовка патерна дпя regex
    string get_pattern(const string& pattern)
    {
        std::string regex_pattern;
        
        for (char c : pattern) {
            if (c == '*') { regex_pattern += ".*"; }
            else if (c == '.') { regex_pattern += "."; }
            else if (c == '?' || c == '+' || c == '^' || c == '$' ||
                       c == '|' || c == '{' || c == '}' || c == '(' ||
                       c == ')' || c == '[' || c == ']' || c == '\\')
            {
                regex_pattern += '\\';
                regex_pattern += c;
            }
            else { regex_pattern += c; }
        }
        
        return regex_pattern;
    }
    
    bool rename(const fs::path& oldPath) {
        string oldName = oldPath.filename().string();
        string newName;
        
        string regexPat = get_pattern(searchPat);
        regex patRegex(regexPat);
            
        if (regex_match(oldName, patRegex))
        {
            smatch match;
            if (regex_search(oldName, match, patRegex)) { newName = regex_replace(oldName, patRegex, replacePat); }
        }
        
        if (!newName.empty() && newName != oldName) {
            fs::path newPath = oldPath.parent_path() / newName;
            
            try 
            {
                fs::rename(oldPath, newPath);
                return true;
            } 
            catch (...) {}
        }
        
        return false;
    }
    
public:
    FileRenamer(fs::path dir, string search, string replace)
        : rootDir(dir), searchPat(search), replacePat(replace) {}
    
    void rename_recursive() {
        if (!fs::exists(rootDir) || !fs::is_directory(rootDir)){ return; }
        
        try {
            for (const auto& entry : fs::recursive_directory_iterator(rootDir)) {
                if (entry.path() == rootDir){ continue; }
                
                rename(entry.path());
            }
        }
        catch (const fs::filesystem_error& e){ cerr << "Error accessing directory: " << e.what() << endl; }
    }

private:
    fs::path rootDir;
    string searchPat;
    string replacePat;
};

int main(int argc, char* argv[]) {
    if (argc < 4) {
        return 1;
    }
    
    fs::path directory;
    string search_pattern;
    string replace_pattern;

    if (!fs::exists(directory)) { return 1; }
    if (argc == 4)
    {
        directory = argv[1];
        search_pattern = argv[2];
        replace_pattern = argv[3];
    }
    else if (argc == 5 && string(argv[2]) == "-r")
    {
        directory = argv[1];
        search_pattern = argv[3];
        replace_pattern = argv[4];
    }
    else { return 1; }
    
    try
    {
        FileRenamer renamer(directory, search_pattern, replace_pattern);
        renamer.rename_recursive();
    }
    catch (...) { return 1; }
    
    return 0;
}