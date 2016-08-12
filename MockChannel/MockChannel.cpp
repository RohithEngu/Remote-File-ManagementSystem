/////////////////////////////////////////////////////////////////////////////
// MockChannel.cpp - Demo for CSE687 Project #4, Spring 2015               //
// - build as DLL to show how C++\CLI client can use native code channel   //
// - MockChannel reads from sendQ and writes to recvQ                      //
//                                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015               //
/////////////////////////////////////////////////////////////////////////////

#define IN_DLL
#include "MockChannel.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"
#include <string>
#include <thread>
#include <iostream>
#include "../Receiver/Receiver.h"
#include "../Sender/Sender.h"
using BQueue = BlockingQueue < Message >;
/////////////////////////////////////////////////////////////////////////////
// MockChannel class
// - reads messages from Sendr and writes messages to Recvr
//
class MockChannel : public IMockChannel
{
public:
  MockChannel(ISendr* pSendr, IRecvr* pRecvr);
  void start();
  void stop();
  void setSrcPort(std::size_t port){ srcPort = port; }
  void setTargetAddr(std::string addr, std::size_t port){ targetAddr = addr; targetPort = port; }
  void uploadFile(const std::string& filepath);
  void getDownloadFiles(vector<string>& patt, std::string path);
  void downloadFile(const std::string& filename);
  void getFiles(vector<string>& patterns, string& path);
  void getTextSearchResults(vector<string>& patterns, string& text, size_t portno, std::string path);
private:
  std::thread thread_;
  SocketSystem* ss;
  ISendr* pISendr_;
  IRecvr* pIRecvr_;
  std::size_t srcPort;
  std::string targetAddr;
  std::size_t targetPort;
  bool stop_ = false;
};

//----< pass pointers to Sender and Receiver >-------------------------------

MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr) {
	ss = new SocketSystem();
}

//----< creates thread to read from sendQ and echo back to the recvQ >-------

void MockChannel::start()
{
  std::cout << "\n  MockChannel starting up";

  thread_ = std::thread(
	  [this] {
	  Sender* pSendr = dynamic_cast<Sender*>(pISendr_);
	  Receiver* pRecvr = dynamic_cast<Receiver*>(pIRecvr_);
	  if (pSendr == nullptr || pRecvr == nullptr)
	  {
		  std::cout << "\n  failed to start Mock Channel\n\n";
		  return;
	  }
	  pRecvr->start();
	  pSendr->start();
  });

}
//----< method to upload files by adding message to sender queue >------
void MockChannel::uploadFile(const std::string& filepath){
	Header hdr; Message msgFileUpload;
	hdr.setCmd("FILE_UPLOAD");
	hdr.setTargetAddr(targetAddr, targetPort);//IP address
	hdr.setAttrib("filename", filepath);//name value pairs
	hdr.setAttrib("msg", "File transfer");
	hdr.setSrcAddr("localhost", srcPort);
	hdr.setContentLength(1024);
	msgFileUpload.setHdr(hdr);
	pISendr_->postMessage(msgFileUpload);
}
//----< method to send command for getting downloadable files >------
void MockChannel::getDownloadFiles(vector<string>& patterns,std::string path){
	Header hdr; Message msgGetDownloadFiles;
	hdr.setCmd("GET_DOWNLOAD_FILES");
	hdr.setPatterns(patterns);
	hdr.setAttrib("filePath", path);
	hdr.setTargetAddr(targetAddr, targetPort);//IP address
	hdr.setSrcAddr("localhost", srcPort);
	msgGetDownloadFiles.setHdr(hdr);
	pISendr_->postMessage(msgGetDownloadFiles);
}
//----< method for searching files >-----
void MockChannel::getFiles(vector<string>& patterns, string& path){
	Header hdr; Message msgGetDownloadFiles;
	hdr.setCmd("GET_FILES");
	hdr.setPatterns(patterns);
	hdr.setTargetAddr(targetAddr, targetPort);//IP address
	hdr.setSrcAddr("localhost", srcPort);
	hdr.setAttrib("filePath", path);
	msgGetDownloadFiles.setHdr(hdr);
	pISendr_->postMessage(msgGetDownloadFiles);
}
//----< method to enqueue Download File Message >-----
void MockChannel::downloadFile(const std::string& filename){
	Header hdr; Message msgFileDownload;
	vector<string> patt;
	patt.push_back("*.*");
	hdr.setCmd("FILE_DOWNLOAD");
	hdr.setPatterns(patt);
	hdr.setTargetAddr(targetAddr, targetPort);//IP address
	hdr.setAttrib("filename", filename);//name value pairs
	hdr.setSrcAddr("localhost", srcPort);
	hdr.setContentLength(1024);
	msgFileDownload.setHdr(hdr);
	pISendr_->postMessage(msgFileDownload);
}
//---< method to send message for Getting Text Search Results >----
void MockChannel::getTextSearchResults(vector<string>& patterns, string& text,size_t portno,std::string path){
	Header hdr; Message msgTextSearch;
	hdr.setCmd("TEXT_SEARCH");
	hdr.setPatterns(patterns);
	hdr.setAttrib("text",text);
	hdr.setAttrib("filePath", path);
	hdr.setTargetAddr(targetAddr, portno);
	hdr.setSrcAddr("localhost", srcPort);
	auto now = std::chrono::high_resolution_clock::now();
	auto nanos = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
	std::stringstream ss;
	ss << nanos;
	std::string startTime = ss.str();
	hdr.setAttrib("StartTime", startTime);
	msgTextSearch.setHdr(hdr);
	pISendr_->postMessage(msgTextSearch);
}

//----< signal server thread to stop >---------------------------------------
void MockChannel::stop() { 
	stop_ = true; 
}

//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sender; }

IRecvr* ObjectFactory::createRecvr() { return new Receiver; }

IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr) 
{ 
  return new MockChannel(pISendr, pIRecvr); 
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
  ObjectFactory objFact;
  ISendr* pSendr = objFact.createSendr();
  IRecvr* pRecvr = objFact.createRecvr();
  IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
  pMockChannel->start();
  pSendr->postMessage("Hello World");
  pSendr->postMessage("CSE687 - Object Oriented Design");
  Message msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pSendr->postMessage("stopping");
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pMockChannel->stop();
  pSendr->postMessage("quit");
  std::cin.get();
}
#endif
