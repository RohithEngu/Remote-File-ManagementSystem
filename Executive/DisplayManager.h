#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H
/**************************************************************************************************
*FileName      : DisplayManager.h - Header file for Display handling of File Catalog.
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
* This module helps in dealing with display of Data store files
*
* Maintenance History:
* --------------------
* ver 1.0 : 25 Jan 2014
* - first release
*
*/
#include "DataStore.h"
#include <iostream>
using namespace std;

class DisplayManager{

public:
	DisplayManager(){};
	~DisplayManager(){};
	using fileset = DataStore::Store;
	//----< method to display File List >------
	static void displayFileList(fileset& files){
		
		for (DataStore::iterator iter = files.begin(); iter != files.end(); ++iter){
			int i = 0;
			title("FileName :" + iter->first + "");
			title("Paths");
			for (DataStore::ListOfIters::iterator it = iter->second.begin(); it != iter->second.end(); ++it){
				cout <<++i<<"."<< **it << '\n';
			}
			cout << '\n';
		}
	}
	//--< Method to display duplicate file list >----
	static void displayDuplicateFileList(fileset& files){

		for (DataStore::iterator iter = files.begin(); iter != files.end(); ++iter){
			if (iter->second.size() > 1){
				int i = 0;
				title("FileName :" + iter->first + "");
				//cout << iter->first << '\n';
				title("Paths");
				for (DataStore::ListOfIters::iterator it = iter->second.begin(); it != iter->second.end(); ++it){
					cout << ++i << "." << **it << '\n';
				}
				cout << '\n';
			}
		}
	}
	//----< method to display all files >----
	static void displayFiles(vector<string>& files){
		int i = 0;
		for (string tmp : files){
			cout << ++i<<"."<< tmp << endl;
		}
		cout << endl;
	}
	//-----< method to display title >----
	static void title(const std::string& title, char underscore = '=')
	{
		string tempStr = "\n  " + title + "\n " + string(title.size() + 2, underscore);
		cout << tempStr;
		cout << "\n";
	}
};

#endif