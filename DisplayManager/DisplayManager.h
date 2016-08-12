#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H
/**************************************************************************************************
*FileName      : DisplayManager.h - Package to display any output
*Author        : Rohith Engu, SUID : 678923180, CE Fall'14, Syracuse University
*                  roengu@syr.edu, (315) 560-6468
*Version       : 1.1
*Langage       : C++11
*Platform      : Windows 7 ultimate, Asus N51Vn -64bit(core2duo), Microsoft Visual Studio Ultimate 2013.
*Application   : Project # 3 ,Object Oriented Design CSE 687
*Reference     : Prof.Jim Fawcett's Help for Project # 3 on Sockets Communication.
**************************************************************************************************
* Package Operations:
* -------------------
* This Package for displayng requirements for Message Passing Communication
*
* Interface:
* ----------
* DisplayManager::displayTitle("Message", '=');
* DisplayManager::displayMsg(msg);
*
* Required Files:
* ---------------
*   - ClientExecutive.cpp
*	- AppHelpers.cpp
*	- Message.cpp
*	- FileSystem.cpp
*
* Build Process:
* --------------
*   cl /EHsc /DTEST_DISPLAYMANAGER DisplayManager.cpp ../ApplicationHelpers/AppHelpers.cpp ../ApplicationHelpers/FileSystem.cpp ../Message/Message.cpp
*
* Maintenance History:
* --------------------
* Ver 1.0 : 03 Mar 15
* - first release
*/
#include <iostream>
#include <string>
#include "../ApplicationHelpers/AppHelpers.h"
#include "../ApplicationHelpers/FileSystem.h"
#include "../Message/Message.h"

using namespace std;
using namespace ApplicationHelpers;
using namespace FileSystem;

class DisplayManager{
public:
	//-----< method to display text >--------
	static void displayTitle(const std::string& msg , char underscore = '='){
		string tempStr = "\n  " + msg + "\n " + string(msg.size() + 2, underscore);
		Verbose::show(tempStr+"\n", true);
	}
	//-----< method to display Message >-------
	static void displayMsg(Message& msg){
		string srcaddr = msg.getHeader().getSrcAddr() + ":" + to_string(msg.getHeader().getSrcPort());
		string cmd = msg.getHeader().getCmd();
		Verbose::show("\n Dequed : " + cmd + " from " + srcaddr, true);
	}
};

#endif