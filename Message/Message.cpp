/**************************************************************************************************
*FileName      : Message.cpp - Class for constructing and parsing http kind of messages
*Author        : Rohith Engu, SUID : 678923180, CE Fall'14, Syracuse University
*                  roengu@syr.edu, (315) 560-6468
*Version       : 1.1
*Langage       : C++11
*Platform      : Windows 7 ultimate, Asus N51Vn -64bit(core2duo), Microsoft Visual Studio Ultimate 2013.
*Application   : Project # 3 ,Object Oriented Design CSE 687
**************************************************************************************************/
#include "Message.h"
using namespace std;
//----< method which makes message object into string with delimiters >--------
void Message::makeMsg(){
	msg = "";
	try{
		msg = "command=" + hdr.getCmd() + "~"+ "trgtAddress=" + hdr.getTargetAddress() + "~"+ "trgtPort=" + to_string(hdr.getTargetPort()) + "~"
			+ "srcAddress=" + hdr.getSrcAddr() + "~"+ "srcPort=" + to_string(hdr.getSrcPort()) + "~"
			+ "content_length=" + to_string(hdr.getContentLength()) + "~";

		unordered_map<std::string, std::string>::iterator iter;
		unordered_map<string, string> attribs = hdr.getAllAttribs();
		for (iter = attribs.begin(); iter != attribs.end(); ++iter) {
			msg += iter->first + "=" + iter->second + "~";
		}
	}
	catch (exception e){
		cout << e.what();
	}
}
//------< method to get attribute value from string >--------
string Message::getOptionValue(string option, string msg){
	vector<string> optionsWithValues;
	std::string token;
	std::string val;
	size_t pos = 0;
	string delimiter = "~";
	while ((pos = msg.find(delimiter)) != std::string::npos) {
		token = msg.substr(0, pos);
		optionsWithValues.push_back(token);
		msg.erase(0, pos + delimiter.length());
	}
	optionsWithValues.push_back(msg);
	delimiter = "=";
	pos = 0;
	if (optionsWithValues.size() > 0){
		for (string optionVal : optionsWithValues){
			if (optionVal.find(option) != std::string::npos){
				std::size_t option_start = optionVal.find("=");
				if (option_start != std::string::npos){
					val = optionVal.substr(option_start + 1, optionVal.length());
					return val;
				}
				return "";
			}
		}
	}
	return "";
}

#ifdef TEST_MESSAGE

int main(){
	string srcAddr = "127.0.0.1", targetAddr = "127.0.0.1", targetCommunicator = "file_processor";
	size_t targetPort = 8181, srcPort = 8484;
	Message msg; Header hdr, hdrEcho;
	hdr.setCmd("send_file");
	hdr.setTargetAddr(targetAddr, targetPort);//IP address
	hdr.setAttrib("filename", "A:/College/OOD/Project#3/Project_3/MessagePassingCommunication/Sender/test.txt");//name value pairs
	hdr.setAttrib("msg", "File transfer");
	hdr.setSrcAddr(srcAddr, srcPort);
	hdr.setContentLength(1024);
	msg.setHdr(hdr);
	cout<< msg.getMsg();
	
}

#endif