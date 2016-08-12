#ifndef FILEMANAGER_H
#define FILEMANAGER_H
///////////////////////////////////////////////////////////////////////////////
// FileManager.h - FileManager header file for handling file operations      //
// ver 1.0                                                                   //
// ------------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                               //
// Platform:    Asus N51Vn , Core2 Duo, Windows 7                            //
// Application: Project#1 - Spring'2015                                      //
// Author:      Rohith Engu || Fall'2014 || roengu@syr.edu || +1 315-560-6468//
///////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ------------------
* This Module provides an interface for getting files based on different options
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

class FileManager{
public:
	using fileset = vector < string >;
	using filemap = DataStore::Store;
	FileManager(const string& path, const vector<string>& patterns, bool subDir);
	fileset getDirectories(bool isSubDir);
	filemap getFinalFileSet();
	fileset getFiles(const string& path, const string& pattern);
	void getDuplicateFileList();
	bool isSubDirectorySearch();
	void addPattern(string pattern);
	string setKeyword();
	void getAllFiles(const string& path, const string& pattern, bool subDir);
	filemap returnDataSet();
	vector<string> getFilesForKey(const string& keyword);
	filemap getSearchResults(const string& keyword);
	filemap getCustomSearchResults(string keyword,vector<string> tmp);
	void briefSummary();
private:
	DataStore ds;
	DataStore d;
	DataStore ds1;
	string _path;
	vector<string> fileList;
	vector<string> fileFullNameList;
	vector<string> _patterns;
	bool _subDir;
	fileset findFiles(const std::string& path, const std::string& pattern);
};

#endif