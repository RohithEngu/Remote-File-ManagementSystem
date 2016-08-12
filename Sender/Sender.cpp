/**************************************************************************************************
*FileName      : Sender.cpp - Class acting as a Sender of a Peer
*Author        : Rohith Engu, SUID : 678923180, CE Fall'14, Syracuse University
*                  roengu@syr.edu, (315) 560-6468
*Version       : 1.1
*Langage       : C++11
*Platform      : Windows 7 ultimate, Asus N51Vn -64bit(core2duo), Microsoft Visual Studio Ultimate 2013.
*Application   : Project # 3 ,Object Oriented Design CSE 687
*Reference     : Prof.Jim Fawcett's Help for Project # 3 on Sockets Communication.
**************************************************************************************************/
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include "Sender.h"
using namespace std;
using namespace ApplicationHelpers;
using namespace FileSystem;
//-----< default constructor initializing target port and address >----------
Sender::Sender() :targetAddr(""), targetPort(0){ 
	_connector = new SocketConnecter();
}
//-------< destructor >--------
Sender::~Sender(){
	delete senderthread;
}
//-------< connect to provided ip and portno >----------
bool Sender::connect(std::string ip, size_t port){
	try{
		if (!_connector->connect(ip, port))
		{
			string msg = "\n\n Sender: Couldn't Connect to " + ip + ":" + to_string(port) + "\n\n";
			Verbose::show(msg,true);
			return false;
		}
		else
		{	//check target address and assign
			if (ip != targetAddr || port != targetPort){
				string msg = "Sender Connected to " + ip + ":" + to_string(port);
				DisplayManager::displayTitle(msg, '-');
			}
			targetAddr = ip; targetPort = port;
			return true;
		}
	}
	catch (std::exception e){
		std::cout << e.what();
		return false;
	}
	return true;
}
//---< starting the sender >-------
void Sender::start(){
	//starting the dispatcher
	senderthread = new std::thread(
	[&]()
	{
		processMessages();
	});
	senderthread->join();
}
//------< method for stopping the sender >--------
void Sender::stop(){
	
}
//------< enquiong message to sender queue >--------
void Sender::postMessage(const Message& msg){
	sendQ.enQ(msg);
}
//--< method which is called by dispatcher to dequeue messages and process >-------
void Sender::processMessages(){
	try{
		while(true)
		{
			::Sleep(1000);
			Message msg = sendQ.deQ();//dequeueing from sender queue
			Header hdr = msg.getHeader();
			string tmp_addr = hdr.getTargetAddress();
			size_t tmp_port = hdr.getTargetPort();
			if (hdr.getCmd() == "STOP")
			{
				DisplayManager::displayTitle("STOP message Received", '-');
				DisplayManager::displayTitle("Terminating Sender ...", ' ');
				break;
			}
			if (!checkAndReconnect(tmp_addr, tmp_port)){//check if target address is different and connect
				continue;
			}
			if (!sendMsg(msg)){//send message to another peer
				continue;
			}
		} 
	}
	catch (exception e){
		e.what();
	}
}
//-----< method to handle sending of messages to another peer's receiver >-------
bool Sender::sendMsg(Message& msg){
	Header hdr = msg.getHeader();
	string msg_content = hdr.getAttrib("msg");
	if (hdr.getCmd() == "FILE_UPLOAD"){
		if (!uploadFile(msg)){//check if file upload is successful
			std::cout << "\n File Upload Failed !!";
			return false;
		}
	}
	else{
		string Msg = "Sending " + msg.getHeader().getCmd()+" Command" + " to " + hdr.getTargetAddress() + ":" + to_string(hdr.getTargetPort());
		DisplayManager::displayTitle(Msg, '.');
		if (!_connector->sendString(msg.getMsg())){
			DisplayManager::displayTitle(msg.getHeader().getCmd()+" Message Sending Failed",' ');
			return false;
		}
	}
	return true;
}
//------< method to upload file >------
bool Sender::uploadFile(Message& msg){
	Header hdr = msg.getHeader(); string path = hdr.getAttrib("filename");//getting the filename
	string sMsg = "Sending file " + Path::getFullFileSpec(path) + " to " + hdr.getTargetAddress() + ":" + to_string(hdr.getTargetPort());
	DisplayManager::displayTitle(sMsg,'.');	
	ifstream ifstream(path.c_str(), std::ios::in | std::ios::binary); //opening in binary mode to read
	if (ifstream.is_open())	{
		ifstream.seekg(0, ios::beg);
		bool isStartFlagSent = false;
		while (!ifstream.eof()){
			byte block[1024];
			ifstream.read((char*)block, 1024);//Read blocks of 1024 from file to be sent to the receiver
			if (!isStartFlagSent){
				hdr.setCmd("FILE_UPLOAD_BEGIN");//If start of file, to open a stream at the receiver end
				isStartFlagSent = true;
			}
			else{
				hdr.setCmd("FILE_UPLOAD");
			}
			int bytesRead = static_cast<int> (ifstream.gcount());
			hdr.setContentLength(bytesRead); msg.setHdr(hdr);
			if (_connector->bytesWaiting() == 0){
				if (!_connector->sendString(msg.getMsg(), '\0')){	//send header with content length for the body to follow
					std::cout << "\n  Header Sending Failed !!";
					break;
				}
			}
			if (!sendBody(bytesRead, block)){ //send blocks of body
				std::cout << "\n  Body Sending Failed !!";
				break;
			}
		}
		ifstream.close();
		handleEndOfFile(msg,hdr);
	}
	else
		return false;
	return true;
}
bool Sender::handleEndOfFile(Message& msg, Header& hdr){
	string path = hdr.getAttrib("filename");
	hdr.setCmd("FILE_END"); hdr.setContentLength(0);
	msg.setHdr(hdr);
	if (!_connector->sendString(msg.getMsg(), '\0')){
		return false;
	}
	string sMsg1 = "Ended sending of file :" + Path::getFullFileSpec(path);
	DisplayManager::displayTitle(sMsg1, '.');
	DisplayManager::displayTitle("Waiting For Acknowledgement ..", ' ');
	return true;
}
//------< sending Block of file data >-------
bool Sender::sendBody(int bytesRead, byte* block){
	if (bytesRead < 1024){//if block size is less than 1024 bytes
		char* blockResized = new char[bytesRead];
		for (int i = 0; i < bytesRead; i++){
			blockResized[i] = block[i];
		}
		if (_connector->bytesWaiting()>0){
			::Sleep(100);
		}
		if (!_connector->send(bytesRead, (char*)(blockResized))){
			return false;
		}
		delete[] blockResized;
	}
	else{
		if (!_connector->send(bytesRead, (char*)block)){
			return false;
		}
	}
	return true;
}
//-----< checking and reconnecting >-------
bool Sender::checkAndReconnect(string tmpAddr, size_t tmpPort){
	if (tmpAddr == targetAddr && tmpPort == targetPort){
		return true;
	}
	if (!this->connect(tmpAddr, tmpPort))
		return false;
	targetAddr = tmpAddr; targetPort = tmpPort;
	return true;
}
//----< test Stub >------
#ifdef TEST_SENDER
#include "../Receiver/Receiver.h"
int main()
{
	int ret = 0;
	try
	{
		string srcAddr = "127.0.0.1",  targetAddr = "127.0.0.1";
		size_t targetPort = 8484, srcPort1 = 8181,srcPort2 = 8383;
		Message msg, msg1; Header hdr,hdr1;		
		hdr.setCmd("FILE_UPLOAD");
		hdr.setTargetAddr(targetAddr, targetPort);
		hdr.setAttrib("filename", "./samjackson.jpg");
		hdr.setAttrib("filetype", "binary");
		hdr.setAttrib("msg", "File transfer");
		hdr.setSrcAddr(srcAddr, srcPort1);
		hdr.setContentLength(1024);
		msg.setHdr(hdr);
		SocketSystem ss;
		Sender sndr1,sndr2;
		int clientPort = 8484;
		Receiver rcvr(8484,Socket::IP4);
		rcvr.start();
		while (!sndr1.connect("localhost", 8484)){
			::Sleep(100);
		}
		sndr1.postMessage(msg);
		sndr1.start();
		//sndr1.postMessage(msg1);
		/*thread * t = new std::thread(&Sender::processMessages, sndr1);
		::Sleep(1000);
		t->join();*/
		cin.get();
	}
	catch (...)
	{
		std::cout << "\n\n  something bad happend to a sender";
		ret = 1;
	}
	return ret;
}

#endif