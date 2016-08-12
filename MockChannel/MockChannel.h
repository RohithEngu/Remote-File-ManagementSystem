#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
/////////////////////////////////////////////////////////////////////////////
// MockChannel.h - Demo for CSE687 Project #4, Spring 2015                 //
// - build as DLL to show how C++\CLI client can use native code channel   //
// - MockChannel reads from sendQ and writes to recvQ                      //
// Verison - 1.1                                                                        //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015               //
/////////////////////////////////////////////////////////////////////////////
/* Maintenance History :
*--------------------
* Ver 1.1 : 27 Apr 15
* - added virtual methods to handle requirements
* Ver 1.0 : 15 Apr 15
* -first release
*/
#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
#include "../Message/Message.h"
#include <chrono>

struct ISendr
{
  virtual void postMessage(const Message& msg) = 0;
  virtual bool connect(std::string ip, size_t port) = 0;
};

struct IRecvr
{
  virtual Message getMessage() = 0;
  virtual void listen(std::size_t portno) = 0;
};

struct IMockChannel
{
public:
  virtual void start() = 0;
  virtual void stop() = 0;
  virtual void setSrcPort(std::size_t port) = 0;
  virtual void setTargetAddr(std::string addr, std::size_t port) = 0;
  virtual void uploadFile(const std::string& filepath) = 0;
  virtual void getDownloadFiles(vector<string>& patt,string path) = 0;
  virtual void downloadFile(const std::string& filename) = 0;
  virtual void getFiles(vector<string>& patterns, string& path) = 0;
  virtual void getTextSearchResults(vector<string>& patterns, string& text, size_t portno, std::string path) = 0;
};

extern "C" {
  struct ObjectFactory
  {
    DLL_DECL ISendr* createSendr();
    DLL_DECL IRecvr* createRecvr();
    DLL_DECL IMockChannel* createMockChannel(ISendr* pISendr, IRecvr* pIRecvr);
  };
}

#endif


