#ifndef CLIENTUI_H
#define CLIENTUI_H
/**************************************************************************************************
*FileName      : ClientUI.h - Class for UI module of Remote Code Management
*Author        : Rohith Engu, SUID : 678923180, CE Fall'14, Syracuse University
*                  roengu@syr.edu, (315) 560-6468
*Version       : 1.0
*Langage       : C++11
*Platform      : Windows 7 ultimate, Asus N51Vn -64bit(core2duo), Microsoft Visual Studio Ultimate 2013.
*Application   : Project # 4 ,Object Oriented Design CSE 687
**************************************************************************************************
* Package Operations:
* -------------------
* This package is used for creating UI modules for meeting the requirements of Project #4.
* Has different views as Tabs which help in demonstrating meeting of requirements.
*
* Interface:
* ----------
* Application^ app = gcnew Application();
*	app->Run(gcnew Client());
*
* Maintenance History:
* --------------------
* Ver 1.0 : 27 Apr 15
* - first release
*/
using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;


#include "../MockChannel/MockChannel.h"
#include <iostream>
#include <chrono>


namespace CppCliWindows
{
	ref class Client : Window
	{
		ISendr* sendr;
		IRecvr* recvr;
		IMockChannel* commChannel;
		//Communication View objects
		Grid^ hCommViewGrid = gcnew Grid();
		Label^ portToListenLabel = gcnew Label();
		TextBox^ portToListen = gcnew TextBox();
		Label^ ipAddrLabel = gcnew Label();
		TextBox^ ipAddrTextBox = gcnew TextBox();
		Label^ portNoLabel = gcnew Label();
		TextBox^ portNoTextBox = gcnew TextBox();
		Button^ listenButton = gcnew Button();
		Button^ connectButton = gcnew Button();
		StackPanel^ commStackPanel1 = gcnew StackPanel();
		StackPanel^ commStackPanel2 = gcnew StackPanel();
		//Text Search View Objects
		Grid^ hTextSearchViewGrid = gcnew Grid();
		Label^ textWildCardLabel = gcnew Label();
		Label^ textCategoryLbl = gcnew Label();
		TextBox^ textCategoryBox = gcnew TextBox();
		TextBox^ textWildCardBox = gcnew TextBox();
		Button^ textAddBtn = gcnew Button();
		Button^ textClearBtn = gcnew Button();
		Label^ txtSelectedLbl = gcnew Label();
		ListBox^ txtSelectedListBox = gcnew ListBox();
		Label^ textEndPointLabel = gcnew Label();
		TextBox^ textEndPointBox = gcnew TextBox();
		Button^ textEndPointAddBtn = gcnew Button();
		Button^ textEndPointClearBtn = gcnew Button();
		Label^ txtEndPointSelectedLbl = gcnew Label();
		ListBox^ txtEndPointSelectedListBox = gcnew ListBox();
		Grid^ optionsGrid = gcnew Grid();
		Label^ txtRegex = gcnew Label();
		TextBox^ txtRegexBox = gcnew TextBox();
		Button^ txtSearchBtn = gcnew Button();
		StackPanel^ txtStackPanel1 = gcnew StackPanel();
		StackPanel^ txtStackPanel3 = gcnew StackPanel();
		StackPanel^ txtStackPanel2 = gcnew StackPanel();
		StackPanel^ txtFinalStackPanel = gcnew StackPanel();
		Label^ txtOutputLbl = gcnew Label();
		ListBox^ txtOutputListBox = gcnew ListBox();
		//File Search View Objects
		Grid^ hFileSearchViewGrid = gcnew Grid();
		Label^ filePathLbl = gcnew Label();
		TextBox^ filePathBox = gcnew TextBox();
		Label^ filePatternLbl = gcnew Label();
		TextBox^ filePatternBox = gcnew TextBox();
		Button^ fileAddBtn = gcnew Button();
		Button^ fileClearBtn = gcnew Button();
		Label^ fileSelected = gcnew Label();
		ListBox^ fileListBox = gcnew ListBox();
		Button^ fileSearchBtn = gcnew Button();
		StackPanel^ fileStackPanel1 = gcnew StackPanel();
		StackPanel^ fileStackPanel2 = gcnew StackPanel();
		StackPanel^ fileFinalStackPanel = gcnew StackPanel();
		Label^ fileOutputLbl = gcnew Label();
		ListBox^ fileOutputListBox = gcnew ListBox();

		//Parent Dock Panel on which grids lie
		DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
		Grid^ hGrid = gcnew Grid();
		TabControl^ hTabControl = gcnew TabControl();
		TabItem^ hSendMessageTab = gcnew TabItem();
		TabItem^ hFileUploadListTab = gcnew TabItem();
		TabItem^ hConnectTab = gcnew TabItem();
		TabItem^ hTextSearchTab = gcnew TabItem();
		TabItem^ hFileSearchTab = gcnew TabItem();
		TabItem^ hFileDownloadTab = gcnew TabItem();
		StatusBar^ hStatusBar = gcnew StatusBar();
		StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
		TextBlock^ hStatus = gcnew TextBlock();
		TextBlock^ hTextBlock1 = gcnew TextBlock();
		//File Upload View Objects
		Grid^ hFileListGrid = gcnew Grid();
		Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
		ListBox^ hListBox = gcnew ListBox();
		Button^ hFolderBrowseButton = gcnew Button();
		Button^ hUploadFilesButton = gcnew Button();
		StackPanel^ uploadPanel = gcnew StackPanel();
		Grid^ hGrid2 = gcnew Grid();
		//File Download View Objects
		Grid^ hFileDownloadListGrid = gcnew Grid();
		Label^ hDownloadPathLbl = gcnew Label();
		TextBox^ hDownloadPathBox = gcnew TextBox();
		ListBox^ hDownloadListBox = gcnew ListBox();
		Button^ hGetDownloadFilesButton = gcnew Button();
		Button^ hDownloadFilesButton = gcnew Button();
		Label^ hCategoryLabel = gcnew Label();
		TextBox^ hCategoryTextBox = gcnew TextBox();
		Button^ hCategoryAddButton = gcnew Button();
		Button^ hCategoryClearButton = gcnew Button();
		StackPanel^ hCategoryPanel = gcnew StackPanel();
		ListBox^ hCategoryListBox = gcnew ListBox();
		StackPanel^ dwnload1 = gcnew StackPanel();
		Label^ hDwnldOutputLabel = gcnew Label();
		StackPanel^ hDownloadPanel = gcnew StackPanel();
		
		//receive thread which dequeues messages from receive queue
		Thread^ recvThread;
	public:
		Client();
		~Client();
		void addEventListeners();
		void setUpView();
		void addText(String^ msg);
		void showAcknowledgement(String^ args);
		void showDownloadableFiles(String^ args);
		void showSearchedFiles(String^ args);
		void showTextSearchResults(String^ args);
		void getMessage();
		void browseForFolder(Object^ sender, RoutedEventArgs^ args);
		void OnLoaded(Object^ sender, RoutedEventArgs^ args);
		void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
		void listen(Object^ sender, RoutedEventArgs^ args);
		void connect(Object^ sender, RoutedEventArgs^ args);
		void searchText(Object^ sender, RoutedEventArgs^ args);
		void searchFiles(Object^ sender, RoutedEventArgs^ args);
		void addWildCard(Object^ sender, RoutedEventArgs^ args);
		void addEndPoint(Object^ sender, RoutedEventArgs^ args);
		void clearWildCards(Object^ sender, RoutedEventArgs^ args);
		void clearEndPoints(Object^ sender, RoutedEventArgs^ args);
		void addPattern(Object^ sender, RoutedEventArgs^ args);
		void clearPatterns(Object^ sender, RoutedEventArgs^ args);
		void uploadFiles(Object^ sender, RoutedEventArgs^ args);
		void getDownloadFiles(Object^ sender, RoutedEventArgs^ args);
		void downloadFiles(Object^ sender, RoutedEventArgs^ args);
		void addCategory(Object^ sender, RoutedEventArgs^ args);
		void clearCategories(Object^ sender, RoutedEventArgs^ args);
		
	private:
		std::string toStdString(String^ pStr);
		String^ toSystemString(std::string& str);
		Boolean connected = false;//if connected set to true
		Boolean listening = false;//if listening set to true
		void setUpStatusBar();
		void setUpTabControl();
		void setUpFileUploadListView();
		void setUpFileDownloadView();
		void setUpDownloadViewRowsAndColumns();
		void setUpFileDownloadButtons();
		void setUpConnectionView();
		void setupConnButtons();
		void setUpTextSearchView();
		void setUpTextViewRowsAndColumns();
		void setUpTextCategoryView();
		void setUpMultipleEndPointsButtons();
		void setUpTextSearchButtons();
		void setUpFileSearchView();
		void setUpFileSearchButtons();
		void setUpFileSearchOutputGrid();
		void OnSelectionChanged(System::Object ^sender, System::Windows::Controls::SelectionChangedEventArgs ^e);
};
}

#endif