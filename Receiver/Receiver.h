#ifndef RECEIVER_H
#define RECEIVER_H
/**************************************************************************************************
*FileName      : Receiver.h - Class acting as a receiver of a Peer
*Author        : Rohith Engu, SUID : 678923180, CE Fall'14, Syracuse University
*                  roengu@syr.edu, (315) 560-6468
*Version       : 1.3
*Langage       : C++11
*Platform      : Windows 7 ultimate, Asus N51Vn -64bit(core2duo), Microsoft Visual Studio Ultimate 2013.
*Application   : Project # 3 ,Object Oriented Design CSE 687
*Reference     : Prof.Jim Fawcett's Help for Project # 3 on Sockets Communication.
**************************************************************************************************
* Package Operations:
* -------------------
* This package is used for receiving messages and interpreting them to upload file and building a response.
*
* Interface:
* ----------
* Receiver rcvr(8484, Socket::IP4); //port to listen and ipaddress type
*	rcvr.start();
*
* Required Files:
* ---------------
*	- Receiver.cpp
*	- BlockingQueue.cpp
*	- Message.cpp
*	- FileSystem.cpp
*	- DisplayManager.cpp
*	- Sockets.cpp
*	- AppHelpers.cpp
*
* Build Process:
* --------------
*   cl /EHsc /DTEST_RECEIVER Receiver.cpp ../BlockingQueue/BlockingQueue.cpp ../Message/Message.cpp 
FileSystem.cpp ../DisplayManager/DisplayManager.cpp ../ApplicationHelpers/AppHelpers.cpp ../Sockets/Sockets.cpp ../Executive/FileManager.cpp
*
* Maintenance History:
* --------------------
* Ver 1.3 : 28 Apr 15
* - Modified Client Handler to handle new Requirements for Project #4
* Ver 1.2 : 09 Apr 15
* - Modified dispatcher and added function pointers
* Ver 1.1 : 07 Apr 15
* - Added Dispatcher to handle sending of responses
* Ver 1.0 : 27 Apr 15
* - first release
*/
#include "../Sockets/Sockets.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"
#include "FileSystem.h"
#include "../DisplayManager/DisplayManager.h"
#include "../MockChannel/MockChannel.h"
#include "../Executive/FileManager.h"
#include <map>
#include <functional>

using namespace std;

class ClientHandler
{
	using filemap = FileManager::filemap;
public:
	bool stop = false;
	void operator()(Socket& s);
	ClientHandler(BlockingQueue<Message>* bQ);
	bool checkOtherCommands(std::string& cmd, Message& msg);
	bool checkIfAcknowledgement(std::string& cmd, Message& msg);
	string getUploadPath(){ return uploadPath; }
	void setUploadPath(string path){ uploadPath = path; }
	string getDownloadPath(){ return downloadPath; }
	void setDownloadPath(string path){ downloadPath = path; }
private:
	void createDirectories();
	void getDownloadFiles(Message& recvdMsg);
	void getSearchedFiles(Message& msgReceived);
	void saveToFile(Socket& s, Message& recvdMsg, string srcFilePath, string destFilePath);
	void enqueueDownloadMsg(Message& recvdMsg);
	void getTextSearchFiles(Message& msgRecvd);
	std::string uploadPath;
	std::string downloadPath;
	BlockingQueue<Message>* bq;
};

class Receiver : public IRecvr
{
	
public:
	Receiver();
	Receiver(std::size_t portno, Socket::IpVer ipv);
	void listen(std::size_t portno);
	Message getMessage();
	void start();
	void stop();
	BlockingQueue<Message>& queue(){ return recvQ; }
	virtual ~Receiver();
private:
	BlockingQueue<Message> recvQ;
	SocketListener* _listener;
	std::size_t _port;
	Socket::IpVer ipv;
	ClientHandler ch;
	
};

#endif