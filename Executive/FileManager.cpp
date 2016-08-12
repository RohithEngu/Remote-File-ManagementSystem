#include "FileManager.h"
#include "FileSystem.h"
#include "DisplayManager.h"
#include <string>
#include <regex>
#include <iostream>
///////////////////////////////////////////////////////////////////////////////
// FileManager.cpp - FileManager header file for handling file operations      //
// ver 1.0                                                                   //
// ------------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                               //
// Platform:    Asus N51Vn , Core2 Duo, Windows 7                            //
// Application: Project#1 - Spring'2015                                      //
// Author:      Rohith Engu || Fall'2014 || roengu@syr.edu || +1 315-560-6468//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace FileSystem;

//-----< Default Parameterized Constructor >----
FileManager::FileManager(const string& path, const vector<string>& patterns, bool subDir) :_path(path), _patterns(patterns), _subDir(subDir){}
//---< returns Directories >---
FileManager::fileset FileManager::getDirectories(bool isSubDir){
	Directory::setCurrentDirectory(_path);
	if (isSubDir){
		return Directory::getDirectories(_path);
	}
	else{
		vector<string> retDir;
		retDir.push_back(Directory::getCurrentDirectory());
		return retDir;
	}
}
//---< method to retrieve final file catalog >----
FileManager::filemap FileManager::getFinalFileSet(){
	vector<string> dirs = getDirectories(isSubDirectorySearch());
	vector<string> finalFileList, tmp;
	for (string tmpdir : dirs){
		if (tmpdir != ".."){
			for (string tmppatt : _patterns){
				tmp = getFiles(tmpdir, tmppatt);
				if (tmp.size() > 0){
					for (string filename : tmp){
						string fullFileName = Path::fileSpec(Path::getFullFileSpec(tmpdir), filename);
						ds.save(Path::getName(fullFileName), Path::getPath(fullFileName));
					}
				}
			}
		}
	}
	return ds.getFileSet();
}
//----< method to retrieve all files searching sub directories >----
void FileManager::getAllFiles(const string& path, const string& pattern, bool subDir){
	std::vector<std::string> files = FileSystem::Directory::getFiles(path, pattern); //get files with patt in the path
	for (auto filename : files)
	{
		std::string filePath = FileSystem::Path::fileSpec(path, filename);
		std::string fullPathwithFile = FileSystem::Path::getFullFileSpec(filePath);
		std::string fullPath = FileSystem::Path::getPath(fullPathwithFile);
		ds.save(filename, fullPath);
	}
	if (subDir)
	{
		std::vector<std::string> dirs = FileSystem::Directory::getDirectories(path);
		for (auto d : dirs)
			if (!(d == "." || d == ".."))
				getAllFiles(FileSystem::Path::fileSpec(path, d), pattern, subDir);
	}
}
//---< Method to return the Data Store >----
FileManager::filemap FileManager::returnDataSet(){
	return ds.getFileSet();
}
//----< Method to return all files for a given key >---
vector<string> FileManager::getFilesForKey(const string& keyword){
	DataStore::Store fileCatalog = ds.getFileSet();
	vector<string> results;
	vector<string> fileSpecs;
	for (DataStore::iterator iter = fileCatalog.begin(); iter != fileCatalog.end(); ++iter){
		for (DataStore::ListOfIters::iterator it = iter->second.begin(); it != iter->second.end(); ++it){
			string tmp = **it + iter->first;
			fileSpecs.push_back(tmp);

		}
	}
	for (string file : fileSpecs){
		File f(file);
		f.open(File::in);
		bool found_flag = false;
		while (f.isGood() && !found_flag){
			string buff = f.getLine(true);
			size_t found = buff.find(keyword, std::regex_constants::ECMAScript | std::regex_constants::icase);
			regex regex_keywrd(keyword);
			std::sregex_iterator next(buff.begin(), buff.end(), regex_keywrd);
			std::sregex_iterator end;
			while (next != end){
				std::smatch match = *next;
				if (match.size() > 0 || found != std::string::npos){
					results.push_back(file);
					found_flag = true;
					break;
				}
				next++;
			}
		}		
		f.close();
	}
	return results;
}
//----< return Search Results >---
FileManager::filemap FileManager::getSearchResults(const string& keyword){
	vector<string> filePaths = getFilesForKey(keyword);
	for (string tmp : filePaths)
		ds1.save(Path::getName(tmp), Path::getPath(tmp));
	return ds1.getFileSet();
}
//----< return Search Results >---
FileManager::filemap FileManager::getCustomSearchResults(string keyword, vector<string> tmp){
	vector<string> filePaths = getFilesForKey(keyword);
	if (find(tmp.begin(), tmp.end(), "*.*") != tmp.end()){
		for (string file : filePaths){
			d.save(Path::getName(file), Path::getPath(file));
		}
	}
	else{
		for (string file : filePaths){
			string ext = Path::getExt(file);
			if ((find(tmp.begin(), tmp.end(), ext) != tmp.end()) || (find(tmp.begin(), tmp.end(), "*." + ext) != tmp.end()) ||
				(find(tmp.begin(), tmp.end(), "." + ext) != tmp.end())) {
				d.save(Path::getName(file), Path::getPath(file));
			}
		}
	}
	return d.getFileSet();
}
//----< Check if it is a sub directory search >---
bool FileManager::isSubDirectorySearch(){
	return _subDir;
}
//----< method to return Files >---
FileManager::fileset FileManager::getFiles(const string& path, const string& pattern){
	return Directory::getFiles(path, pattern);
}
//----< Display Brief Summary >----
void FileManager::briefSummary(){
	DisplayManager::title("Brief Summary");
	cout << "Considering " << Directory::getCurrentDirectory() << " as default Path";
	DisplayManager::title(" No of Directories");
	vector<string> dirs = getDirectories(true);
	vector<string> filenames,tmp;
	cout<<dirs.size()-1<<endl;
	DisplayManager::title("No of Files");
	int count = 0;
	for (string tmpdir : dirs){
		if (tmpdir != ".."){
			for (string tmppatt : _patterns){
				tmp = getFiles(tmpdir, tmppatt);
				count += tmp.size();
				if (tmp.size() > 0){
					for (string filename : tmp){
						string fullFileName = Path::fileSpec(Path::getFullFileSpec(tmpdir), filename);
						filenames.push_back(fullFileName);
					}
				}
			}
		}
	}
	cout << count << endl;
	DisplayManager::title("Files found");
	DisplayManager::displayFiles(filenames);
}
//---< Test Stub >---
#ifdef TEST_FILEMANAGER
void main(){
	vector<string> patt;
	patt.push_back("*.*");
	FileManager f(".",patt,true);
	FileManager::filemap result =  f.returnDataSet();
}
#endif