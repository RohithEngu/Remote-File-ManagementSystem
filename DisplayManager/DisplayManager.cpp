#include "DisplayManager.h"
/**************************************************************************************************
*FileName      : DisplayManager.cpp - Package to display messages and text
*Author        : Rohith Engu, SUID : 678923180, CE Fall'14, Syracuse University
*                  roengu@syr.edu, (315) 560-6468
*Version       : 1.1
*Langage       : C++11
*Platform      : Windows 7 ultimate, Asus N51Vn -64bit(core2duo), Microsoft Visual Studio Ultimate 2013.
*Application   : Project # 3 ,Object Oriented Design CSE 687
*Reference     : Prof.Jim Fawcett's Help for Project # 3 on Sockets Communication.
**************************************************************************************************/
#ifdef TEST_DISPLAYMANAGER

using namespace std;
using namespace ApplicationHelpers;
using namespace FileSystem;
int main()
{
	Message msg; Header hdr;

	hdr.setCmd("FILE_UPLOAD");
	hdr.setTargetAddr("localhost", 8088);//IP address
	hdr.setAttrib("filename", "./samjackson.jpg");//name value pairs
	hdr.setAttrib("filetype", "binary");
	hdr.setAttrib("msg", "File transfer");
	hdr.setSrcAddr("Rohit Engu", 9999);
	hdr.setContentLength(1024);
	msg.setHdr(hdr);
	DisplayManager::displayTitle("Message", '=');
	DisplayManager::displayMsg(msg);
}

#endif