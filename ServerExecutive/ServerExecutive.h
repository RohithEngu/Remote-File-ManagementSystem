#ifndef SERVEREXECUTIVE_H
#define SERVEREXECUTIVE_H
/**************************************************************************************************
*FileName      : ServerExecutive.h - Acts as a Peer (Just Named it Server for better Understanding)
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
* This Package acts as a Peer in the communication channel, For better understanding I named it Server Executive
* It has ability to upload file to another Peer.
* Waits for Acknowledgement from another Peer when File is uploaded at another Peer.
*
* Interface:
* ----------
* ServerExecutive s(portToListen, Socket::IpVer::IP4);
* s.uploadFile(filenameToUpload, targetAddr, targetPort);
* s.start();
* s.stop();
*
* Required Files:
* ---------------
*   - ClientExecutive.cpp
*	- Sockets.cpp
*	- AppHelpers.cpp
*	- Receiver.cpp
*	- Sender.cpp
*	- Message.cpp
*	- FileSystem.cpp
*
* Build Process:
* --------------
*   cl /EHsc /DTEST_SERVER ServerExecutive.cpp ../Sockets/Sockets.cpp ../ApplicationHelpers/AppHelpers.cpp
	../Receiver/Receiver.cpp ../Sender/Sender.cpp ../Message/Message.cpp ../ApplicationHelpers/FileSystem.cpp
*
* Maintenance History:
* --------------------
* ver 1.1 : 01 Mar 15
* - modified File Upload to handle targetAddress and targetPort
* Ver 1.0 : 31 Apr 15
* - first release
*/
#include <iostream>
#include <string>
#include <thread>
#include "../Message/Message.h"
#include "../Sockets/Sockets.h"
#include "../Receiver/Receiver.h"
#include "../Sender/Sender.h"
#include "../BlockingQueue/BlockingQueue.h"

using namespace std;

class ServerExecutive
{
public:
	ServerExecutive(std::size_t portno, Socket::IpVer ipv);
	~ServerExecutive();
	void start();
	void stop();
	void uploadAcknowledgementServer(Message& msg);
	void uploadFile(const std::string& filepath,const string& trgtAddr,size_t trgtPort);

private:
	SocketSystem* ss;
	void processMessages();
	Receiver* recvr;
	Sender* sndr;
	size_t srcPort;
	string file;
	std::string srcAddr;
	size_t trgtPort;
	std::string trgtAddr;
	thread* dispatcherThread;
};

#endif