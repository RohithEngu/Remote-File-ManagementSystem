#ifndef SENDER_H
#define SENDER_H
/**************************************************************************************************
*FileName      : Sender.h - Class acting as a Sender of a Peer
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
* This package is used for sending messages to another peer and has a dispatcher which deques the message from sender queue
* and sends to another peer depending on target port and target address.
*
* Interface:
* ----------
* Sender sndr1;
* int clientPort = 8484;
* Receiver rcvr(8484,Socket::IP4);
* rcvr.start();
* while (!sndr1.connect("localhost", 8484)){
*	::Sleep(100);
* }
*  sndr1.postMessage(msg);//enqueue it to sender queue
*
* Required Files:
* ---------------
*	- Sender.cpp
*	- BlockingQueue.cpp
*	- Message.cpp
*	- FileSystem.cpp
*	- DisplayManager.cpp
*	- Sockets.cpp
*	- AppHelpers.cpp
*	- Receiver.cpp
*
* Build Process:
* --------------
*   cl /EHsc /DTEST_SENDER Sender.cpp ../BlockingQueue/BlockingQueue.cpp ../Message/Message.cpp ../ApplicationHelpers/FileSystem.cpp ../DisplayManager/DisplayManager.cpp ../ApplicationHelpers/AppHelpers.cpp ../Sockets/Sockets.cpp ../Receiver/Receiver.cpp
*
* Maintenance History:
* --------------------
* Ver 1.1 : 07 Apr 15
* - Added method for reconnecting incase of different target Address and target port
* Ver 1.0 : 27 Apr 15
* - first release
*/
#include <iostream>
#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"
#include "../Sockets/Sockets.h"
#include "../Receiver/FileSystem.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../ApplicationHelpers/FileSystem.h"
#include "../DisplayManager/DisplayManager.h"
#include "../MockChannel/MockChannel.h"

using namespace std;

class Sender : public ISendr
{
	using byte = unsigned char;
public:
	Sender();
	void postMessage(const Message& msg);
	void processMessages();
	void start();
	void stop();
	BlockingQueue<Message>& queue(){ return sendQ; }
	bool connect(std::string ip, size_t port);
	bool handleEndOfFile(Message& msg, Header& hdr);
	virtual ~Sender();
private:
	BlockingQueue<Message> sendQ;
	SocketConnecter* _connector;
	thread *senderthread;
	bool sendMsg(Message& msg);
	bool uploadFile(Message& msg);
	bool returnDownloadFilesList(Message& msg);
	bool sendBody(int bytesRead, byte* block);
	bool checkAndReconnect(string tmpAddr, size_t tmpPort);
	string targetAddr;
	size_t targetPort;
	string srcAddr;
	size_t srcPort;


};

#endif