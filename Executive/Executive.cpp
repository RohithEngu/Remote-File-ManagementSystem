///////////////////////////////////////////////////////////////////////////////
// Executive.cpp - Executive cpp file for FileCatalog                        //
// ver 1.0                                                                   //
// ------------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                               //
// Platform:    Asus N51Vn , Core2 Duo, Windows 7                            //
// Application: Project#1 - Spring'2015                                      //
// Author:      Rohith Engu || Fall'2014 || roengu@syr.edu || +1 315-560-6468//
///////////////////////////////////////////////////////////////////////////////


/*
Module Operations:
===============
This is the starting point of the file catalog application,
accepts user inputs and redirects the operation to respective modules accordingly.
*/

#include <iostream>
#include <regex>
#include "FileManager.h"
#include "Executive.h"
#include "DataStore.h"
#include "DisplayManager.h"
#include <sstream>
#include <string>
using namespace std;
//----< Default Constructor >----
Executive::Executive(){
	patterns.push_back("*.*");
	isSubDir = false;
	isDup = false;
	isSearchKwd = false;
}
//---< Parameterized Constructor >----
Executive::Executive(int argc, char* argv[]){
	_argc = argc;
	_argv = argv;
	patterns.push_back("*.*");
	isSubDir = false;
	isDup = false;
	isSearchKwd = false;
}
//----< method to process command line parameters >----
void Executive::processCommands(){
	DisplayManager::title("Input Arguments Passed");
	for (int i = 1; i < _argc; i++){
		string tmp = _argv[i];
		cout << tmp << " ";
	}
	cout << endl;
	if (_argc > 2){
		checkCommands();		
	}
	else{
		cout << "Invalid Number of inputs !!!" << endl;
	}
}
//-----< check commands and add >----
void Executive::checkCommands(){
	path = _argv[1];
	regex search_regex("\\/f[A-Za-z]+$");
	for (int i = 2; i < _argc; i++){
		string tmp = _argv[i];
		if (tmp == "/s"){
			isSubDir = true;
			continue;
		}
		if (tmp == "/d"){
			isDup = true;
			continue;
		}
		if (regex_match(tmp, search_regex)){
			isSearchKwd = true;
			keyword = tmp.substr(2, tmp.size());
			continue;
		}
	}
}
//-----< Method to generate File catalog >----
void Executive::generateFileCatalog(){
	FileManager fm(path, patterns,isSubDirectorySearch());
	finalFileSet = fm.getFinalFileSet();
	DisplayManager::title("File Catalogue Based on user Inputs");
	DisplayManager::displayFileList(finalFileSet);
	if (isDupFileList()){
		DisplayManager::title("Duplicate files found");
		DisplayManager::displayDuplicateFileList(finalFileSet);
	}
}
//----< Check if it is a Sub Directory Search >----
bool Executive::isSubDirectorySearch(){
	return isSubDir;
}
//----< Check if command contains Keyword search >----
bool Executive::isKeywordSearch(){
	return isSearchKwd;
}
//----< Check if command contains Duplicate searching >----
bool Executive::isDupFileList(){
	return isDup;
}
//----< add Pattern >----
void Executive::addPattern(string& pattern){
	if (patterns.size() == 1 && patterns[0] == "*.*")
		patterns.pop_back();
	patterns.push_back(pattern);
}
//----< Return keyword >----
string Executive::getKeyword(){
	return keyword;
}
//----< Display Brief Summary >----
void Executive::briefSummary(string path){
	FileManager fm(path, patterns, isSubDirectorySearch());
	fm.briefSummary();
}
//----< Check files for Keyword >----
void Executive::searchFilesForKey(string keyword){
	FileManager f(path, patterns, isSubDirectorySearch());
	DataStore::Store fileMap = f.getSearchResults(keyword);
	DisplayManager::title("Files with Keyword : " + keyword + " ");
	DisplayManager::displayFileList(fileMap);
	//matchedfiles = f.getFilesForKey(keyword);
}
//----< searches for Files with key and patterns >----
void Executive::searchFilesForKeyAndPatterns(string keywrd,vector<string> tmp){
	FileManager f(path, patterns, isSubDirectorySearch());
	DataStore::Store fileMap = f.getCustomSearchResults(keywrd, tmp);
	DisplayManager::title("Files with Keyword : " + keywrd + " ");
	DisplayManager::displayFileList(fileMap);
}
//----< Test Stub >----
#ifdef TEST_EXECUTIVE
int main(int argc, char* argv[]){

	try{
		Executive exec(argc, argv);
		exec.processCommands();
		if (argc >= 2){
			exec.generateFileCatalog();
			if (exec.isKeywordSearch()){
				exec.searchFilesForKey(exec.getKeyword());
			}
			DisplayManager::title("Enter a Keyword to search in File Catalog :");
			string keywrd;
			cin >> keywrd;
			DisplayManager::title("Enter file pattern/s for searching (for multiple give a ' ' after each) :");
			string input;
			cin.ignore();
			getline(cin, input);
			
			istringstream buf(input);
			istream_iterator<string> beg(buf), end;
			vector<string> tokens(beg, end);
			regex fileext_regex("[A-Za-z0-9\\*]+\\.[A-Za-z\\*]+", regex_constants::ECMAScript |
				regex_constants::icase);
			for (string tmp : tokens){
				if (!regex_match(tmp, fileext_regex)){
					DisplayManager::title("Pattern " + tmp + " is invalid !!!", ' ');
					return 0;
				}
			}
			
				
			exec.searchFilesForKeyAndPatterns(keywrd,tokens);
		}		
	}
	catch (exception e){
		cout << "Exception !!" << e.what() << endl;
	}

}
#endif