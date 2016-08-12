/**************************************************************************************************
*FileName      : DataStore.cpp - implementation file for Storage class of the files.
*Author        : Rohith Engu, SUID : 678923180, CE Fall'14, Syracuse University
*                  roengu@syr.edu, (315) 560-6468
*Version       : 1.0
*Langage       : C++11
*Platform      : Windows 7 ultimate, Asus N51Vn -64bit(core2duo), Microsoft Visual Studio Ultimate 2013.
*Application   : Project # 4 ,Object Oriented Design CSE 687
*Reference     : Prof.Jim Fawcett's Code help on DataStore
***************************************************************************************************/
#include "DataStore.h"
#include <iostream>
using namespace std;

//----------< save filename along with path >----------
void DataStore::save(const string& fileName, const string& path)
{
	//if an entry is not present already
	if (store.count(fileName) == 0){

		pair<DataStore::PathIter, bool> ret = paths.insert(path);
		ListOfIters newListIter;
		newListIter.push_back(ret.first);
		store[fileName] = newListIter;

	}
	else{

		pair<DataStore::PathIter, bool> retIt = paths.insert(path);
		Store::iterator storeIter = store.find(fileName);
		storeIter->second.push_back(retIt.first);

	}

}

//---------<returning the store>-------

DataStore::Store DataStore::getFileSet(){
	return store;
}
//----< Test Stub >----
#ifdef TEST_DATASTORE

void main(){
	DataStore d;
	string filename1 = "FileName1.cpp";
	string filename2 = "FileName2.h";
	string pathname1_1="C:\\Temp";
	string pathname1_2="D:\\Temp";
	string pathname1_3="C:\\Temp";
	string pathname2_1 = "C:\\Temp";
	string pathname2_2 = "C:\\Temp";
	string pathname2_3 = "D:\\Folder1";
	d.save(filename1,pathname1_1);
	d.save(filename1,pathname1_2);
	d.save(filename1,pathname1_3);
	d.save(filename2, pathname2_1);
	d.save(filename2, pathname2_2);
	d.save(filename2, pathname2_3);
	DataStore::Store files = d.getFileSet();
	for (DataStore::iterator iter = files.begin(); iter != files.end(); ++iter){
		int i = 0;
		cout<<endl<<"FileName :" + iter->first<<endl;
		cout<<endl<<"Paths";
		for (DataStore::ListOfIters::iterator it = iter->second.begin(); it != iter->second.end(); ++it){
			cout << ++i << "." << **it << '\n';
		}
		cout << '\n';
	}
}

#endif