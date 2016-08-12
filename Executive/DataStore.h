#ifndef DATASTORE_H
#define DATASTORE_H
/**************************************************************************************************
*FileName      : DataStore.h - Header file for Storage class of the files.
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
*using Path = std::string;
*using Paths = std::set < Path >;
*using PathIter = Paths::iterator;
*using ListOfIters = std::list < PathIter >;
*using File = std::string;
*using Store = std::map < File, ListOfIters >;
*--DataStructure which is a map with key as filename and value as iterator pointing to set of paths
*	where the file is found.
*using iterator = Store::iterator;
*void save(const std::string& fileName, const std::string& path);
*--save method for storing filename with path.
*iterator begin(){ return store.begin(); }
*iterator end(){ return store.end(); }
*Store getFileSet();
*
* Maintenance History:
* --------------------
* ver 1.0 : 25 Jan 2014
* - first release
*
*/
#include <set>
#include <list>
#include <map>
#include <string>
#include <vector>

class DataStore{

public:
	DataStore(){};
	~DataStore(){};
	using Path = std::string;
	using Paths = std::set < Path >;
	using PathIter = Paths::iterator;
	using ListOfIters = std::list < PathIter >;
	using File = std::string;
	using Store = std::map < File, ListOfIters >;
	using iterator = Store::iterator;

	void save(const std::string& fileName, const std::string& path);
	iterator begin(){ return store.begin(); }
	iterator end(){ return store.end(); }
	
	Store getFileSet();

private:
	Store store;
	Paths paths;

};

#endif