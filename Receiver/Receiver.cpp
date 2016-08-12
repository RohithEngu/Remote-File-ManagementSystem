/**************************************************************************************************
*FileName      : Receiver.cpp - Class acting as a receiver of a Peer
*Author        : Rohith Engu, SUID : 678923180, CE Fall'14, Syracuse University
*                  roengu@syr.edu, (315) 560-6468
*Version       : 1.2
*Langage       : C++11
*Platform      : Windows 7 ultimate, Asus N51Vn -64bit(core2duo), Microsoft Visual Studio Ultimate 2013.
*Application   : Project # 3 ,Object Oriented Design CSE 687
*Reference     : Prof.Jim Fawcett's Help for Project # 3 on Sockets Communication.
**************************************************************************************************/
#include "Receiver.h"
using namespace ApplicationHelpers;
using namespace FileSystem;
//-----< default parameterized constructor for client handler >---
ClientHandler::ClientHandler(BlockingQueue<Message>* bq_) {//passing blocking queue of receiver as a reference
	bq = bq_;
}
//-----< operator which is called when receiver accepts connection >------
void ClientHandler::operator()(Socket& socket_)
{
	while (true)//goes on until stop message is received in client handler queue
	{
		Message msgReceived;
		if (socket_.bytesWaiting() == 0)
			continue;
		std::string recvd_msg = socket_.recvString('\0');//receiving strings (basically receives header)
		msgReceived.parseMsg(recvd_msg);//parsing message received in string to message object			
		std::string command = msgReceived.getHeader().getCmd();
		setUploadPath("../DownloadedIn" + to_string(msgReceived.getHeader().getTargetPort())+"/");
		setDownloadPath("../DownloadfromHere");
		createDirectories();
		std::string srcfile = msgReceived.getHeader().getAttrib("filename"); //getting the source filename
		//making a file to write appending the sender peer's port to file name
		std::string destfilePath = getUploadPath()+Path::getName(srcfile);
		//if command is file upload or file upload begin
		if (command == "FILE_UPLOAD" || command == "FILE_UPLOAD_BEGIN")
		{
			
			saveToFile(socket_, msgReceived, srcfile, destfilePath);//save to file
			continue; 
		}
		else if (checkOtherCommands(command, msgReceived)){
			continue;
		}
		else if (checkIfAcknowledgement(command, msgReceived)){
			continue;
		}
	}
}
//-----< Method to check for other commands than file upload >--------
bool ClientHandler::checkOtherCommands(std::string& command, Message& msgReceived){
	std::string srcfile = msgReceived.getHeader().getAttrib("filename"); //getting the source filename
	//making a file to write appending the sender peer's port to file name
	std::string destfilePath = getUploadPath() + Path::getName(srcfile);
	if (command == "FILE_DOWNLOAD"){
		enqueueDownloadMsg(msgReceived);
		return true;
	}
	else if (command == "FILE_END"){ //if end of file received at receiver end
		DisplayManager::displayTitle("File Saved at Location : " + Path::getFullFileSpec(destfilePath), ' ');
		DisplayManager::displayTitle("Constructing and Sending Acknowledgement Reply", '-');
		Message acknowledge;
		Header ackhdr;
		ackhdr.setCmd("Acknowledge");
		std::size_t targetPort = msgReceived.getHeader().getTargetPort();
		ackhdr.setSrcAddr(msgReceived.getHeader().getTargetAddress(), targetPort);
		ackhdr.setTargetAddr(msgReceived.getHeader().getSrcAddr(), msgReceived.getHeader().getSrcPort());
		ackhdr.setAttrib("filename", msgReceived.getHeader().getAttrib("filename"));
		ackhdr.setAttrib("Mode", msgReceived.getHeader().getAttrib("Mode"));
		acknowledge.setHdr(ackhdr);
		std::string msg = acknowledge.getMsg();
		bq->enQ(acknowledge);
		return true;
	}
	else if (command == "GET_DOWNLOAD_FILES"){
		getDownloadFiles(msgReceived);
		return true;
	}
	else if (command == "GET_FILES"){
		getSearchedFiles(msgReceived);
		return true;
	}
	else if (command == "TEXT_SEARCH"){
		getTextSearchFiles(msgReceived);		
		return true;
	}
	return false;
}
//------< Method to check if the command is an acknowledgement >------
bool ClientHandler::checkIfAcknowledgement(std::string& command, Message& msgReceived){
	if (command == "Acknowledge"){
		if (msgReceived.getHeader().getAttrib("Mode") != "download")
			bq->enQ(msgReceived);
		DisplayManager::displayTitle("Received Acknowledge of file: " + msgReceived.getHeader().getAttrib("filename") + " Uploaded", '.');
		return true;
	}
	else if (command == "GET_DOWNLOAD_FILES_ACK"){
		bq->enQ(msgReceived);
		DisplayManager::displayTitle("Received file List to Download", '.');
		return true;
	}
	else if (command == "GET_FILES_ACK"){
		bq->enQ(msgReceived);
		DisplayManager::displayTitle("Received List After File Searching", '.');
		return true;
	}
	else if (command == "TEXT_SEARCH_ACK"){
		auto now = std::chrono::high_resolution_clock::now();
		auto nanos = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
		std::stringstream sstream;
		sstream << nanos;
		std::string timeStr = sstream.str();
		Message acknowledge;
		Header ackhdr;
		ackhdr.setCmd("TEXT_SEARCH_ACK");
		std::size_t targetPort = msgReceived.getHeader().getTargetPort();
		ackhdr.setSrcAddr(msgReceived.getHeader().getSrcAddr(), msgReceived.getHeader().getSrcPort() );
		ackhdr.setTargetAddr(msgReceived.getHeader().getTargetAddress(), targetPort);
		ackhdr.setAttrib("StartTime", msgReceived.getHeader().getAttrib("StartTime"));
		ackhdr.setAttrib("EndTime", timeStr);
		acknowledge.setBody(msgReceived.getBody());
		acknowledge.setHdr(ackhdr);
		bq->enQ(acknowledge);
		DisplayManager::displayTitle("Received List After Text Searching", '.');
		return true;
	}
	return false;
}
//------<Method to enqueue File Download such that Sender would dequeue and upload to other peer >-----
void ClientHandler::enqueueDownloadMsg(Message& msg){
	string filename = msg.getHeader().getAttrib("filename");
	vector<string> patterns;
	patterns = msg.getHeader().getPatterns();
	string dwnloadPath = getDownloadPath() + "/"+msg.getHeader().getAttrib("filePath");
	FileManager f(dwnloadPath, patterns, true);
	for(string patt : patterns){
		f.getAllFiles(getDownloadPath(), patt, true);
	}
	filemap result = f.returnDataSet();
	string path = "";
	bool firstFlag = true;
	for (DataStore::iterator iter = result.begin(); iter != result.end(); ++iter){
		int i = 0;
		if (iter->first == filename){
			for (DataStore::ListOfIters::iterator it = iter->second.begin(); it != iter->second.end()&&firstFlag; ++it){
				 path = **it;
				 firstFlag = false;
			}
			Message downloadMsg;
			Header dwnHdr;
			dwnHdr.setCmd("FILE_UPLOAD");
			std::size_t targetPort = msg.getHeader().getTargetPort();
			dwnHdr.setSrcAddr(msg.getHeader().getTargetAddress(), targetPort);
			dwnHdr.setTargetAddr(msg.getHeader().getSrcAddr(), msg.getHeader().getSrcPort());
			dwnHdr.setAttrib("filename", path+msg.getHeader().getAttrib("filename"));
			dwnHdr.setAttrib("Mode", "download");
			downloadMsg.setHdr(dwnHdr);
			bq->enQ(downloadMsg);
			return;
		}
	}
}
//------< Method to retrieve Matched files >-----
void ClientHandler::getTextSearchFiles(Message& msgRecvd){
	string textToSearch = msgRecvd.getHeader().getAttrib("text");
	string path = getDownloadPath() +"/"+ msgRecvd.getHeader().getAttrib("filePath");
	FileManager f(path, msgRecvd.getHeader().getPatterns(), true);
	for (string patt : msgRecvd.getHeader().getPatterns()){
		f.getAllFiles(path, patt, true);
	}
	vector<string> results = f.getFilesForKey(textToSearch);
	string dwnldPath = getDownloadPath();
	dwnldPath = dwnldPath.substr(3);
	for (string file : results){
		std::size_t pos = file.find(dwnldPath);
		std::string relativeFileName = file.substr(pos);
		msgRecvd.appendToBody(relativeFileName + "#");
	}
	Message acknowledge;
	Header ackhdr;
	ackhdr.setAttrib("StartTime", msgRecvd.getHeader().getAttrib("StartTime"));
	ackhdr.setCmd("TEXT_SEARCH_ACK");
	std::size_t targetPort = msgRecvd.getHeader().getTargetPort();
	ackhdr.setSrcAddr(msgRecvd.getHeader().getTargetAddress(), targetPort);
	ackhdr.setTargetAddr(msgRecvd.getHeader().getSrcAddr(), msgRecvd.getHeader().getSrcPort());
	acknowledge.setHdr(ackhdr);
	acknowledge.setBody(msgRecvd.getBody());
	bq->enQ(acknowledge);
}
//------< creating directories using const upload path >---------
void ClientHandler::createDirectories(){
	if (!Directory::exists(getUploadPath())){
		Directory::create(getUploadPath());
	}
	if (!Directory::exists(getDownloadPath())){
		Directory::create(getDownloadPath());
	}
}
//---< method to retrieve File Names for downloading >-----
void ClientHandler::getDownloadFiles(Message& msg){
	string path = getDownloadPath() + "/" + msg.getHeader().getAttrib("filePath");
	FileManager f(path, msg.getHeader().getPatterns(), true);
	for (string patt : msg.getHeader().getPatterns()){
		f.getAllFiles(path, patt, true);
	}
	filemap result = f.returnDataSet();
	for (DataStore::iterator iter = result.begin(); iter != result.end(); ++iter){
		msg.appendToBody(iter->first + "#");
	}
	Message acknowledge;
	Header ackhdr;
	ackhdr.setCmd("GET_DOWNLOAD_FILES_ACK");
	std::size_t targetPort = msg.getHeader().getTargetPort();
	ackhdr.setSrcAddr(msg.getHeader().getTargetAddress(), targetPort);
	ackhdr.setTargetAddr(msg.getHeader().getSrcAddr(), msg.getHeader().getSrcPort());
	acknowledge.setHdr(ackhdr);
	acknowledge.setBody(msg.getBody());
	bq->enQ(acknowledge);
}
//-----< method to retrieve file names with their server directory subtree >-----
void ClientHandler::getSearchedFiles(Message& msgReceived){
	std::string path = getDownloadPath() + "/" + msgReceived.getHeader().getAttrib("filePath");
	FileManager f(path, msgReceived.getHeader().getPatterns(), true);
	for (string patt : msgReceived.getHeader().getPatterns()){
		f.getAllFiles(path, patt, true);
	}
	filemap result = f.returnDataSet();
	string dwnldPath = getDownloadPath();
	dwnldPath = dwnldPath.substr(3);
	for (DataStore::iterator iter = result.begin(); iter != result.end(); ++iter){		
		for (DataStore::ListOfIters::iterator it = iter->second.begin(); it != iter->second.end(); ++it){
			string tmp = **it + iter->first;
			std::size_t pos = tmp.find(dwnldPath);
			std::string relativeFileName = tmp.substr(pos);
			msgReceived.appendToBody(relativeFileName + "#");
		}
	}
	Message acknowledge;
	Header ackhdr;
	ackhdr.setCmd("GET_FILES_ACK");
	std::size_t targetPort = msgReceived.getHeader().getTargetPort();
	ackhdr.setSrcAddr(msgReceived.getHeader().getTargetAddress(), targetPort);
	ackhdr.setTargetAddr(msgReceived.getHeader().getSrcAddr(), msgReceived.getHeader().getSrcPort());
	acknowledge.setHdr(ackhdr);
	acknowledge.setBody(msgReceived.getBody());
	std::string msg = acknowledge.getMsg();
	bq->enQ(acknowledge);
}
//-----< saves the block received to new file at upload path >------
void ClientHandler::saveToFile(Socket& socket_, Message& recvdMsg, string srcFilePath, string destFilePath){
	try{
		string cmd = recvdMsg.getHeader().getCmd();
		size_t len = recvdMsg.getHeader().getContentLength();
		string fileType = recvdMsg.getHeader().getAttrib("fileType");
		char* block = new char[len];
		std::ofstream ofstream;
		if (recvdMsg.getHeader().getCmd() == "FILE_UPLOAD_BEGIN"){
			ofstream.open(destFilePath.c_str(), std::ios::out | ios::binary);
		}
		else{			
			ofstream.open(destFilePath.c_str(), std::ios::out |std::ios::app | ios::binary);//opening in appending mode
		}
		if (socket_.recv(len, block)){//receiving block with content length sent from sender
			if (ofstream.good()){
				ofstream.write((char*)block, len);//writing to file
				delete[] block;
				ofstream.close();//closing the file stream
			}
		}
		else{
			Verbose::show("\n Receive failed", true);
		}
		if (cmd == "FILE_UPLOAD_BEGIN"){
			string srcAddr = recvdMsg.getHeader().getSrcAddr();
			size_t srcPort = recvdMsg.getHeader().getSrcPort();
			string sMsg = "Receiving file :" + Path::getName(srcFilePath) + " from " + srcAddr + ":" + to_string(srcPort);
			DisplayManager::displayTitle(sMsg, '.');
		}
	}
	catch (exception e){
		cout << e.what();
	}
}
//----< defaultConstructor >-----
Receiver::Receiver():ch(&recvQ){

}
//----< parameterized constructor which initializes listener, client handler, dispatcher >---------
Receiver::Receiver(std::size_t portno, Socket::IpVer ipver) :_port(portno), ipv(ipver), _listener(new SocketListener(portno, ipver)), ch(&recvQ){
}
void Receiver::listen(std::size_t portno){
	_listener = new SocketListener(portno, Socket::IP4);
}
//-----< destructor >-----
Receiver::~Receiver(){
}
//------< start method of receiver >----
void Receiver::start(){
	_listener->start(ch);//started listener
	//dp.start();//starting dispatcher
}
//-------< Retrieving Message from Receive Queue >-----
Message Receiver::getMessage(){
	return recvQ.deQ();
}
//------< stop method of receiver >-------
void Receiver::stop(){
	ch.stop = true;
	Message msgStop;
	Header hdrStop;
	hdrStop.setCmd("STOP");
	msgStop.setHdr(hdrStop);
	recvQ.enQ(msgStop);
}

//-----< test stub >-----
#ifdef TEST_RECEIVER
int main(){
	SocketSystem ss;
	Receiver rcvr(8484, Socket::IP4);
	rcvr.start();
	Receiver rcvr1(8282, Socket::IP4);
	rcvr1.start();
	cin.get();
}
#endif