#ifndef EXECUTIVE_H
#define EXECUTIVE_H
/**************************************************************************************************
*FileName      : Exectuive.h - Header file for Executing file Catalog.
*Author        : Rohith Engu, SUID : 678923180, CE Fall'14, Syracuse University
*                  roengu@syr.edu, (315) 560-6468
*Version       : 1.0
*Langage       : C++11
*Platform      : Windows 7 ultimate, Asus N51Vn -64bit(core2duo), Microsoft Visual Studio Ultimate 2013.
*Application   : Project # 4 ,Object Oriented Design CSE 687
*Reference     : Prof.Jim Fawcett's Code help on DataStore
***************************************************************************************************/
/*
* Module Operations:
* ------------------
* This Module provides an interface for saving all the files in a data structure.
* It has a main class DataStore, which has all the methods for saving.
*
* Interface:
* ----------
*	Executive exec(argc, argv);
*	exec.processCommands();
*	exec.generateFileCatalog();
*
* Maintenance History:
* --------------------
* ver 1.0 : 25 Jan 2014
* - first release
*
*/
#include <string>
#include <vector>
#include "DataStore.h"

using namespace std;

class Executive{
public:
	Executive();
	Executive(int argc, char* argv[]);
	using filemap = DataStore::Store;
	void processCommands();
	void checkCommands();
	void generateFileCatalog();
	bool isSubDirectorySearch();
	bool isKeywordSearch();
	bool isDupFileList();
	void searchFilesForKey(string keywrd);
	void searchFilesForKeyAndPatterns(string keywrd, vector<string> pattrns);
	void addPattern(string& pattern);
	string getKeyword();
	void briefSummary(string path);
private:
	int _argc;
	char** _argv;
	string path;
	vector<string> patterns;
	bool isSubDir;
	bool isDup;
	bool isSearchKwd;
	filemap finalFileSet;
	filemap matchedfiles;
	string keyword;
	
};

#endif