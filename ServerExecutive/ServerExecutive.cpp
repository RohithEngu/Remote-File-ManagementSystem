/**************************************************************************************************
*FileName      : ServerExecutive.cpp - Acts as a Peer (Just Named it Server for better Understanding)
*Author        : Rohith Engu, SUID : 678923180, CE Fall'14, Syracuse University
*                  roengu@syr.edu, (315) 560-6468
*Version       : 1.1
*Langage       : C++11
*Platform      : Windows 7 ultimate, Asus N51Vn -64bit(core2duo), Microsoft Visual Studio Ultimate 2013.
*Application   : Project # 4 ,Object Oriented Design CSE 687
*Reference     : Prof.Jim Fawcett's Help for Project # 3 on Sockets Communication.
**************************************************************************************************/
#include "ServerExecutive.h"
//-------< parametarized constructor >---------
ServerExecutive::ServerExecutive(std::size_t portno, Socket::IpVer ipv):recvr(new Receiver(portno,ipv)),sndr(new Sender()),srcAddr("localhost"),srcPort(portno),trgtAddr(""),trgtPort(0),ss(new SocketSystem()){
	DisplayManager::displayTitle("Server's Receiver Listening on: " + to_string(portno), '-');
}
//----< Default Destructor >----
ServerExecutive::~ServerExecutive(){
	delete sndr;
	delete recvr;
	delete ss;
}
//-------< callback method which is called when receiver is done receiving file >---------
void ServerExecutive::uploadAcknowledgementServer(Message& msg){
	DisplayManager::displayTitle(" Enquing Reply to Sender Queue ..", '*');
	Message m;
	Header h;
	h.setCmd(msg.getHeader().getCmd());
	h.setSrcAddr(trgtAddr, trgtPort);//IP address
	h.setTargetAddr(msg.getHeader().getSrcAddr(), msg.getHeader().getSrcPort());
	h.setAttrib("filename", file);
	m.setHdr(h);
	sndr->postMessage(std::move(m));
}
//--------< Method to start Client Executive >-------
void ServerExecutive::start(){
	dispatcherThread = new thread(&ServerExecutive::processMessages, this);
	dispatcherThread->detach();
	recvr->start();
	sndr->start();	
}
//-------< method to upload file >--------
void ServerExecutive::uploadFile(const std::string& filepath, const string& Addr, size_t Port){
	//constructing file upload message
	Header hdr; Message msgFileUpload;
	hdr.setCmd("FILE_UPLOAD");
	trgtAddr = Addr;
	trgtPort = Port;
	file = filepath;
	hdr.setTargetAddr(trgtAddr, trgtPort);//IP address
	hdr.setAttrib("filename", filepath);//name value pairs
	hdr.setSrcAddr(srcAddr, srcPort);
	hdr.setContentLength(1024);
	msgFileUpload.setHdr(hdr);
	sndr->postMessage(msgFileUpload);
}
//----< Method to Dequeue from Receiver and Send it to sender to send back to Client >---
void ServerExecutive::processMessages(){
	while (true){
		Message toSend = recvr->getMessage();
		sndr->postMessage(toSend);
	}	

}
//-------< method to stop the sender of server executive >--------
void ServerExecutive::stop(){
	::Sleep(4000);
	sndr->stop();
}
//-------< test stub >---------
#ifdef TEST_SERVER
struct Cosmetic
{
	~Cosmetic()
	{
		std::cout << "\n  press Enter to exit: ";
		std::cin.get();
		std::cout << "\n\n";
	}
};
int main(int argc, char* argv[]){
	int cnt = 0;
	Verbose v(true);
	if (argc > 1){
		size_t portToListen = atoi(argv[1]);
		ServerExecutive s(portToListen, Socket::IpVer::IP4);
		s.start();
		Cosmetic c;
		::Sleep(3000);
		s.stop();
	}
	else
		std::cout << "\n Incorrect Number of parameters !!!";
}
#endif