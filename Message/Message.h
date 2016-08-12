#ifndef MESSAGE_H
#define MESSAGE_H
/**************************************************************************************************
*FileName      : Message.h - Class for constructing and parsing http kind of messages
*Author        : Rohith Engu, SUID : 678923180, CE Fall'14, Syracuse University
*                  roengu@syr.edu, (315) 560-6468
*Version       : 1.1
*Langage       : C++11
*Platform      : Windows 7 ultimate, Asus N51Vn -64bit(core2duo), Microsoft Visual Studio Ultimate 2013.
*Application   : Project # 4 ,Object Oriented Design CSE 687
**************************************************************************************************
* Package Operations:
* -------------------
* This package is used for parsing and constructing messages, has header and body
*
* Interface:
* ----------
* Message msg; Header hdr, hdrEcho;
* hdr.setCmd("send_file");
* hdr.setTargetAddr(targetAddr, targetPort);//IP address
* hdr.setAttrib("filename", "A:/College/OOD/Project#3/Project_3/MessagePassingCommunication/Sender/test.txt");
* hdr.setSrcAddr(srcAddr, srcPort);
* hdr.setContentLength(1024);
* msg.setHdr(hdr);
*
* Required Files:
* ---------------
*	- Message.cpp
*
* Build Process:
* --------------
*   cl /EHsc /DTEST_MESSAGE Message.cpp
*
* Maintenance History:
* --------------------
* Ver 1.1 : 27 Apr 15
* - modified getMsg() to handle body content
* Ver 1.0 : 27 Apr 15
* - first release
*/
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;
//-----< Header Class >-----
class Header{
private:
	string cmd;//command
	string trgtAddr;//target Address
	size_t trgtPort;//target Port
	size_t content_length; //Content length used for file blocks
	unordered_map<string, string> attribs; //map with name value attribute pair
	vector<string> patterns;
	string srcAddress;//source Address
	size_t srcPort;//source Port
public:
	using attr = unordered_map < string, string >;
	//------< default constructor >-------
	Header() : content_length(0){}
	//------< getter Setter methods >--------
	//------< get command >--------
	inline string Header::getCmd(){
		return cmd;
	}
	//------< get targetIP >--------
	inline string Header::getTargetAddress(){
		return trgtAddr;
	}
	//------< get targetPort >--------
	inline size_t Header::getTargetPort(){
		return trgtPort;
	}
	//------< get All Attributes >--------
	inline attr Header::getAllAttribs(){
		return attribs;
	}
	//------< get Attribute value >--------
	inline string Header::getAttrib(string name){
		string value = "";
		attr::const_iterator it = attribs.find(name);
		if (it != attribs.end()){
			value = it->second;
		}
		return value;
	}
	//------< get source Addr >--------
	inline string Header::getSrcAddr(){
		return srcAddress;
	}
	//------< get source Port >--------
	inline size_t Header::getSrcPort(){
		return srcPort;
	}
	//------< get content length of block >--------
	inline int Header::getContentLength(){
		return content_length;
	}
	//------< set Command >--------
	inline void Header::setCmd(string _cmd){
		cmd = _cmd;
	}
	//------< set Target Address with IP and port >--------
	inline void Header::setTargetAddr(string _addr, size_t _port){
		trgtAddr = _addr;
		trgtPort = _port;
	}
	//------< Set attribute >----------
	inline void Header::setAttrib(string name, string value){
		attribs[name] = value;
	}
	//------< Set Content Length >----------
	inline void Header::setContentLength(size_t len){
		content_length = len;
	}
	//------< set source Address with IP and port >--------
	inline void Header::setSrcAddr(string addr, size_t port){
		srcAddress = addr;
		srcPort = port;
	}
	//----< setting and getting patterns for file download >-----
	inline void setPatterns(vector<string>& patt){
		patterns = patt;
	}
	inline vector<string> getPatterns(){
		return patterns;
	}
};

class Message{
private:
	Header hdr; //header object
	std::string body; //body ( not in Use for this project )
	std::string msg; //optional message
	void makeMsg();
	std::string getOptionValue(string _hdr, string msg);
public:
	inline void appendToBody(string msg){
		body += msg;
	}
	inline void setBody(string msg){
		body = msg;
	}
	inline string getBody(){
		return body;
	}
	//------< method to parse string received into message class object >-------
	inline void parseMsg(std::string msg){
		std::string cmd = getOptionValue("command", msg);
		hdr.setCmd(cmd);
		std::string trgtaddr = getOptionValue("trgtAddress", msg);
		std::string trgtport = getOptionValue("trgtPort", msg);
		hdr.setTargetAddr(trgtaddr, atoi(trgtport.c_str()));
		std::string srcaddr = getOptionValue("srcAddress", msg);
		std::string srcport = getOptionValue("srcPort", msg);
		hdr.setSrcAddr(srcaddr, atoi(srcport.c_str()));
		std::string contentlen = getOptionValue("content_length", msg);
		hdr.setContentLength(atoi(contentlen.c_str()));
		string filename = getOptionValue("filename", msg);
		hdr.setAttrib("filename", filename);
		string filepath = getOptionValue("filePath", msg);
		hdr.setAttrib("filePath", filepath);
		string text = getOptionValue("text", msg);
		hdr.setAttrib("text", text);
		string mode = getOptionValue("Mode", msg);
		hdr.setAttrib("Mode", mode);
		body = getOptionValue("body", msg);
		string pattern = getOptionValue("Patterns", msg);
		hdr.setPatterns(SplitString(pattern, "$"));
		string startTime = getOptionValue("StartTime", msg);
		hdr.setAttrib("StartTime", startTime);
		string endTime = getOptionValue("EndTime", msg);
		hdr.setAttrib("EndTime", endTime);
		string diffTime = getOptionValue("PerformanceTime", msg);
		hdr.setAttrib("PerformanceTime", diffTime);
	}
	inline vector<string> parseBodyFileList(std::string msg){
		vector<string> result = SplitString(body, "#");
		return result;
	}
	inline vector<string> SplitString(string str, const string &delim) {
		vector<string> result;
		size_t found;
		while ((found = str.find(delim)) != string::npos) {
			result.push_back(str.substr(0, found));
			str = str.substr(found + delim.size());
		}
		return result;
	}
	//------< get message as string >----------
	inline string getMsg(){
		makeMsg();//makes message object into string
		string patternString = "Patterns=";
		if (hdr.getPatterns().size() > 0){
			for (string pat : hdr.getPatterns()){
				patternString += pat + "$";
			}
		}
		else{
			patternString += "*.*";
		}
		return msg+"body="+body+"~"+patternString;
	}
	//------< get header >----------
	inline Header getHeader(){
		return hdr;
	}
	//------< set header >----------
	inline void setHdr(Header _hdr){
		hdr = _hdr;
	}
};



#endif