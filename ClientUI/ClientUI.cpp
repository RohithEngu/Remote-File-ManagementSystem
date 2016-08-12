#include "ClientUI.h"
#pragma warning(disable: 4244)
/**************************************************************************************************
*FileName      : ClientUI.cpp - implementation Class for UI module of Remote Code Management
*Author        : Rohith Engu, SUID : 678923180, CE Fall'14, Syracuse University
*                  roengu@syr.edu, (315) 560-6468
*Version       : 1.0
*Langage       : C++11
*Platform      : Windows 7 ultimate, Asus N51Vn -64bit(core2duo), Microsoft Visual Studio Ultimate 2013.
*Application   : Project # 4 ,Object Oriented Design CSE 687
**************************************************************************************************/
using namespace CppCliWindows;
//----< default Constructor >-----
Client::Client(){
	//Creating sender and receiver objects through Object Factory from MockChannel library >------
	ObjectFactory* pObjFact = new ObjectFactory;
	sendr = pObjFact->createSendr();
	recvr = pObjFact->createRecvr();
	commChannel = pObjFact->createMockChannel(sendr, recvr);
	delete pObjFact;

	recvThread = gcnew Thread(gcnew ThreadStart(this, &Client::getMessage));
	recvThread->Start();

	this->Loaded += gcnew System::Windows::RoutedEventHandler(this, &Client::OnLoaded);
	this->Closing += gcnew CancelEventHandler(this, &Client::Unloading);
	addEventListeners();
	//Setting Window Properties
	this->Title = "Client";
	this->Width = 800;
	this->Height = 660;
	ipAddrTextBox->Text = "localhost";
	// attach dock panel to Window
	this->Content = hDockPanel;
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);
	hStatus->FontWeight = FontWeights::Bold;
	//setting up view
	setUpView();	
}
//-----< Method to add Event Handlers to Buttons >----
void Client::addEventListeners(){
	hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &Client::browseForFolder);
	listenButton->Click += gcnew RoutedEventHandler(this, &Client::listen);
	connectButton->Click += gcnew RoutedEventHandler(this, &Client::connect);
	txtSearchBtn->Click += gcnew RoutedEventHandler(this, &Client::searchText);
	textAddBtn->Click += gcnew RoutedEventHandler(this, &Client::addWildCard);
	textEndPointAddBtn->Click += gcnew  RoutedEventHandler(this, &Client::addEndPoint);
	textEndPointClearBtn->Click += gcnew  RoutedEventHandler(this, &Client::clearEndPoints);
	textClearBtn->Click += gcnew RoutedEventHandler(this, &Client::clearWildCards);
	fileSearchBtn->Click += gcnew RoutedEventHandler(this, &Client::searchFiles);
	fileAddBtn->Click += gcnew RoutedEventHandler(this, &Client::addPattern);
	fileClearBtn->Click += gcnew RoutedEventHandler(this, &Client::clearPatterns);
	hUploadFilesButton->Click += gcnew RoutedEventHandler(this, &Client::uploadFiles);
	hGetDownloadFilesButton->Click += gcnew RoutedEventHandler(this, &Client::getDownloadFiles);
	hDownloadFilesButton->Click += gcnew RoutedEventHandler(this, &Client::downloadFiles);
	hCategoryAddButton->Click += gcnew RoutedEventHandler(this, &Client::addCategory);
	hCategoryClearButton->Click += gcnew RoutedEventHandler(this, &Client::clearCategories);
}
//-----< intialize all the views >----
void Client::setUpView(){
	setUpTabControl();
	setUpStatusBar();
	setUpConnectionView();
	setUpTextSearchView();
	setUpFileSearchView();
	setUpFileUploadListView();
	setUpFileDownloadView();
}
//----< destructor >----
Client::~Client(){
	delete commChannel;
	delete recvr;
	delete sendr;
}
//----< setting up status Bar >----
void Client::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	hStatus->Text = "Welcome to Remote Code Management User Interface !!!";
	hStatusBarItem->Content = hStatus;
	hStatusBar->Padding = Thickness(10, 2, 10, 2);
}
//-----< setting up the tabs and their contents >----
void Client::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	hFileUploadListTab->Header = "Files Upload";
	hFileDownloadTab->Header = "Files Download";
	hConnectTab->Header = "Connection";
	hTextSearchTab->Header = "Text Search";
	hFileSearchTab->Header = "File Search";
	hTabControl->Items->Add(hConnectTab);
	hTabControl->Items->Add(hTextSearchTab);
	hTabControl->Items->Add(hFileSearchTab);
	hTabControl->Items->Add(hFileUploadListTab);
	hTabControl->Items->Add(hFileDownloadTab);
	hTabControl->SelectionChanged += gcnew System::Windows::Controls::SelectionChangedEventHandler(this, &CppCliWindows::Client::OnSelectionChanged);
}
//----< method to convert System String to C++ understandable string >--
std::string Client::toStdString(String^ pStr)
{
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}
//----< method to convert C++ String to System understandable string >--
String^ Client::toSystemString(std::string& str)
{
	StringBuilder^ pStr = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
}
//----< method to Setup Connection View >--
void Client::setUpConnectionView()
{
	Console::Write("\n  setting up Conection view");
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(100);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(100);
	hCommViewGrid->RowDefinitions->Add(hRow1Def);//adding rows
	hCommViewGrid->RowDefinitions->Add(hRow2Def);
	hCommViewGrid->Margin = Thickness(80);
	portToListenLabel->Content = "Port to Listen :";
	portToListenLabel->Height = 27;
	portToListen->ToolTip = "Enter Port No to Listen to";
	portToListen->Width = 65;
	portToListen->Height = 25;
	listenButton->Content = "Listen";
	listenButton->FontWeight = FontWeights::Bold;
	listenButton->BorderThickness = Thickness(2);
	listenButton->BorderBrush = Brushes::Black;
	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 120;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = listenButton;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 10;
	commStackPanel1->Children->Add(portToListenLabel);//adding connection view elements to stack panel
	commStackPanel1->Children->Add(hSpacer);
	commStackPanel1->Children->Add(portToListen);
	commStackPanel1->Children->Add(hSpacer1);
	commStackPanel1->Children->Add(hBorder1);
	commStackPanel1->Orientation = Orientation::Horizontal;
	commStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	commStackPanel1->Margin = Thickness(20);
	hCommViewGrid->SetRow(commStackPanel1, 0);
	hCommViewGrid->Children->Add(commStackPanel1);
	setupConnButtons();
}
//----< method to Setup Connection Buttons >--
void Client::setupConnButtons(){
	ipAddrLabel->Content = "Ip Address :";
	ipAddrLabel->Height = 27;
	ipAddrTextBox->Height = 27;
	ipAddrTextBox->Width = 95;
	portNoLabel->Content = "Port No :";
	portNoLabel->Height = 27;
	portNoTextBox->ToolTip = "Server Port No's (8282,8484)";
	portNoTextBox->Height = 25;
	portNoTextBox->Width = 50;
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = connectButton;
	connectButton->Content = "Connect";
	connectButton->FontWeight = FontWeights::Bold;
	connectButton->BorderThickness = Thickness(2);
	connectButton->BorderBrush = Brushes::Black;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 10;
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer3->Width = 10;
	TextBlock^ hSpacer4 = gcnew TextBlock();
	hSpacer4->Width = 10;
	TextBlock^ hSpacer5 = gcnew TextBlock();
	hSpacer5->Width = 10;
	commStackPanel2->Children->Add(ipAddrLabel);//adding elements to stack panel
	commStackPanel2->Children->Add(hSpacer2);
	commStackPanel2->Children->Add(ipAddrTextBox);
	commStackPanel2->Children->Add(hSpacer3);
	commStackPanel2->Children->Add(portNoLabel);
	commStackPanel2->Children->Add(hSpacer4);
	commStackPanel2->Children->Add(portNoTextBox);
	commStackPanel2->Children->Add(hSpacer5);
	commStackPanel2->Children->Add(hBorder2);
	commStackPanel2->Orientation = Orientation::Horizontal;
	commStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	commStackPanel2->Margin = Thickness(20);
	hCommViewGrid->SetRow(commStackPanel2, 1);
	hCommViewGrid->Children->Add(commStackPanel2);
	hConnectTab->Content = hCommViewGrid;
}
//----< init method to setup Text Search View >--
void Client::setUpTextSearchView(){
	Console::Write("\n  setting up Text Search view");
	setUpTextViewRowsAndColumns();//setting up rows and columns
	setUpTextCategoryView();	
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 10;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 10;
	textAddBtn->Content = "Add";
	Border^ hBorder1 = gcnew Border();
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = textAddBtn;
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer3->Width = 10;
	textClearBtn->Content = "Clear";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = textClearBtn;
	TextBlock^ hSpacer4 = gcnew TextBlock();
	hSpacer4->Width = 10;
	TextBlock^ hSpacer5 = gcnew TextBlock();
	hSpacer5->Width = 10;
	txtSelectedListBox->Height = 100;
	txtSelectedListBox->Width = 70;
	txtSelectedListBox->SelectionMode = SelectionMode::Multiple;
	txtStackPanel1->Children->Add(textWildCardLabel);
	txtStackPanel1->Children->Add(hSpacer1);
	txtStackPanel1->Children->Add(textWildCardBox);
	txtStackPanel1->Children->Add(hSpacer2);
	txtStackPanel1->Children->Add(hBorder1);
	txtStackPanel1->Children->Add(hSpacer3);
	txtStackPanel1->Children->Add(hBorder2);
	txtStackPanel1->Children->Add(hSpacer4);
	txtStackPanel1->Children->Add(hSpacer5);
	txtStackPanel1->Children->Add(txtSelectedListBox);
	txtStackPanel1->Orientation = Orientation::Horizontal;
	txtStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	optionsGrid->SetColumn(txtStackPanel1, 0);
	optionsGrid->Children->Add(txtStackPanel1);
	hTextSearchViewGrid->SetRow(optionsGrid, 1);
	hTextSearchViewGrid->Children->Add(optionsGrid);
	setUpMultipleEndPointsButtons();
	setUpTextSearchButtons();
}
//----< method to setup rows and columns of Text Search View >--
void Client::setUpTextViewRowsAndColumns(){
	optionsGrid->Margin = Thickness(20);
	ColumnDefinition^ col1 = gcnew ColumnDefinition();
	ColumnDefinition^ col2 = gcnew ColumnDefinition();
	optionsGrid->ColumnDefinitions->Add(col1);
	optionsGrid->ColumnDefinitions->Add(col2);

	RowDefinition^ row0 = gcnew RowDefinition();
	row0->Height = GridLength(30);
	RowDefinition^ row1 = gcnew RowDefinition();
	row1->Height = GridLength(150);
	RowDefinition^ row2 = gcnew RowDefinition();
	row2->Height = GridLength(37);
	RowDefinition^ row3 = gcnew RowDefinition();
	row3->Height = GridLength(30);
	RowDefinition^ row4 = gcnew RowDefinition();
	hTextSearchViewGrid->RowDefinitions->Add(row0);
	hTextSearchViewGrid->RowDefinitions->Add(row1);
	hTextSearchViewGrid->RowDefinitions->Add(row2);
	hTextSearchViewGrid->RowDefinitions->Add(row3);
	hTextSearchViewGrid->RowDefinitions->Add(row4);
}
//----< method to setup Text Search Category View >--
void Client::setUpTextCategoryView(){
	textCategoryLbl->Content = "Enter Category : ";
	textCategoryLbl->Height = 27;
	textCategoryBox->Height = 27;
	textCategoryBox->Width = 100;
	textCategoryBox->ToolTip = "Ex: c++,csharp,java,other";
	StackPanel^ textCategoryStackPanel = gcnew StackPanel();
	textCategoryStackPanel->Children->Add(textCategoryLbl);
	textCategoryStackPanel->Children->Add(textCategoryBox);
	textCategoryStackPanel->Orientation = Orientation::Horizontal;
	textCategoryStackPanel->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hTextSearchViewGrid->SetRow(textCategoryStackPanel, 0);
	hTextSearchViewGrid->Children->Add(textCategoryStackPanel);
	textWildCardLabel->Content = "Enter Wild Card :";
	textWildCardLabel->Height = 27;
	textWildCardBox->Height = 27;
	textWildCardBox->Width = 65;
	textWildCardBox->ToolTip = "Enter a pattern (Ex: *.cpp)";
}
//---<setting up Multiple end points in text search >----
void Client::setUpMultipleEndPointsButtons(){
	textEndPointLabel->Content = "Enter Port Number :";
	textEndPointLabel->Height = 27;
	textEndPointBox->Height = 27;
	textEndPointBox->Width = 65;
	textEndPointBox->ToolTip = "Enter a port no";
	txtEndPointSelectedLbl->Content = "Select End Points :";
	txtEndPointSelectedLbl->Height = 27;
	txtEndPointSelectedListBox->Height = 100;
	txtEndPointSelectedListBox->Width = 70;
	txtEndPointSelectedListBox->SelectionMode = SelectionMode::Multiple;
	textEndPointAddBtn->Content = "Add";
	Border^ hBorder1 = gcnew Border();
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = textEndPointAddBtn;
	TextBlock^ hSpacer6 = gcnew TextBlock();
	hSpacer6->Width = 10;
	textEndPointClearBtn->Content = "Clear";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = textEndPointClearBtn;
	txtStackPanel2->Children->Add(txtEndPointSelectedLbl);
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 10;
	txtStackPanel2->Children->Add(hSpacer1);
	txtStackPanel2->Children->Add(textEndPointBox);
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 10;
	txtStackPanel2->Children->Add(hSpacer2);
	txtStackPanel2->Children->Add(hBorder1);
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer3->Width = 10;
	txtStackPanel2->Children->Add(hSpacer3);
	txtStackPanel2->Children->Add(hBorder2);
	TextBlock^ hSpacer4 = gcnew TextBlock();
	hSpacer4->Width = 10;
	txtStackPanel2->Children->Add(hSpacer4);
	txtStackPanel2->Children->Add(txtEndPointSelectedListBox);
	txtStackPanel2->Orientation = Orientation::Horizontal;
	txtStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	optionsGrid->SetColumn(txtStackPanel2, 1);
	optionsGrid->Children->Add(txtStackPanel2);
}
//----< method to setup buttons >--
void Client::setUpTextSearchButtons(){
	txtRegex->Content = "Text/Regular Expression to Search :";
	txtRegex->Height = 27;
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 10;
	txtRegexBox->Height = 27;
	txtRegexBox->Width = 200;
	txtRegexBox->ToolTip = "Can Also Accept Regular Expression";
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 30;
	txtSearchBtn->Content = "Search Text";
	Border^ hBorder1 = gcnew Border();
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = txtSearchBtn;
	txtStackPanel3->Children->Add(txtRegex);
	txtStackPanel3->Children->Add(hSpacer1);
	txtStackPanel3->Children->Add(txtRegexBox);
	txtStackPanel3->Children->Add(hSpacer2);
	txtStackPanel3->Children->Add(hBorder1);
	txtStackPanel3->Orientation = Orientation::Horizontal;
	txtStackPanel3->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hTextSearchViewGrid->SetRow(txtStackPanel3, 2);
	hTextSearchViewGrid->Children->Add(txtStackPanel3);
	txtOutputLbl->Content = "Output Showing Files found and their respective Machines :";
	txtOutputLbl->Height = 27;
	hTextSearchViewGrid->SetRow(txtOutputLbl, 3);
	hTextSearchViewGrid->Children->Add(txtOutputLbl);
	ScrollViewer^ scrollTxt = gcnew ScrollViewer();
	scrollTxt->Content = txtOutputListBox;
	Border^ hBorder3 = gcnew Border();
	hBorder3->BorderThickness = Thickness(2);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = scrollTxt;
	hTextSearchViewGrid->SetRow(hBorder3, 4);
	hTextSearchViewGrid->Children->Add(hBorder3);
	hTextSearchTab->Content = hTextSearchViewGrid;
}
//----< method to File Search View >--
void Client::setUpFileSearchView(){
	Console::Write("\n  setting up File Search view");
	RowDefinition^ row1 = gcnew RowDefinition();
	row1->Height = GridLength(37);
	RowDefinition^ row2 = gcnew RowDefinition();
	row2->Height = GridLength(120);
	RowDefinition^ row3 = gcnew RowDefinition();
	row3->Height = GridLength(37);
	RowDefinition^ row4 = gcnew RowDefinition();
	row4->Height = GridLength(37);
	RowDefinition^ row5 = gcnew RowDefinition();
	hFileSearchViewGrid->Margin = Thickness(1);
	hFileSearchViewGrid->RowDefinitions->Add(row1);
	hFileSearchViewGrid->RowDefinitions->Add(row2);
	hFileSearchViewGrid->RowDefinitions->Add(row3);
	hFileSearchViewGrid->RowDefinitions->Add(row4);
	hFileSearchViewGrid->RowDefinitions->Add(row5);
	filePathLbl->Content = "Enter Relative File Path :";
	filePathLbl->Height = 27;
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 10;
	filePathBox->Height = 27;
	filePathBox->Width = 300;
	filePathBox->ToolTip = "Ex: c++,csharp,java,other";
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 10;
	filePatternLbl->Content = "Pattern:";
	filePatternLbl->Height = 27;
	filePatternBox->Height = 30;
	filePatternBox->Width = 60;
	fileListBox->Height = 100;
	fileListBox->Width = 70;
	fileListBox->SelectionMode = SelectionMode::Multiple;
	fileStackPanel1->Children->Add(filePathLbl);
	fileStackPanel1->Children->Add(hSpacer1);
	fileStackPanel1->Children->Add(filePathBox);
	fileStackPanel1->Orientation = Orientation::Horizontal;
	fileStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hFileSearchViewGrid->SetRow(fileStackPanel1, 0);
	hFileSearchViewGrid->Children->Add(fileStackPanel1);
	setUpFileSearchButtons();
}
//----< method to setup buttons >--
void Client::setUpFileSearchButtons(){
	fileAddBtn->Content = "Add";
	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 60;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = fileAddBtn;
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer3->Width = 10;
	fileClearBtn->Content = "Clear";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 60;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = fileClearBtn;
	TextBlock^ hSpacer4 = gcnew TextBlock();
	hSpacer4->Width = 10;
	TextBlock^ hSpacer5 = gcnew TextBlock();
	hSpacer5->Width = 10;
	TextBlock^ hSpacer6 = gcnew TextBlock();
	hSpacer6->Width = 10;
	TextBlock^ hSpacer7 = gcnew TextBlock();
	hSpacer7->Width = 10;
	fileSearchBtn->Content = "Search Files";
	fileStackPanel2->Children->Add(filePatternLbl);
	fileStackPanel2->Children->Add(hSpacer3);
	fileStackPanel2->Children->Add(filePatternBox);
	fileStackPanel2->Children->Add(hSpacer4);
	fileStackPanel2->Children->Add(hBorder1);
	fileStackPanel2->Children->Add(hSpacer5);
	fileStackPanel2->Children->Add(hBorder2);
	fileStackPanel2->Children->Add(hSpacer6);
	fileStackPanel2->Children->Add(fileListBox);
	fileStackPanel2->Orientation = Orientation::Horizontal;
	fileStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hFileSearchViewGrid->SetRow(fileStackPanel2, 1);
	hFileSearchViewGrid->Children->Add(fileStackPanel2);
	setUpFileSearchOutputGrid();	
}
//----< method to setup output grid >--
void Client::setUpFileSearchOutputGrid(){
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = fileSearchBtn;
	StackPanel^ fileSearchPanel = gcnew StackPanel();
	fileSearchPanel->Orientation = Orientation::Vertical;
	fileSearchPanel->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	fileSearchPanel->Children->Add(hBorder3);
	hFileSearchViewGrid->SetRow(fileSearchPanel, 2);
	hFileSearchViewGrid->Children->Add(fileSearchPanel);
	fileOutputLbl->Content = "   Output Showing Files found and their respective Servers :";
	fileOutputLbl->Height = 27;
	hFileSearchViewGrid->SetRow(fileOutputLbl, 3);
	hFileSearchViewGrid->Children->Add(fileOutputLbl);

	ScrollViewer^ scrollFiles = gcnew ScrollViewer();
	scrollFiles->Content = fileOutputListBox;
	Border^ hBorder4 = gcnew Border();
	hBorder4->BorderThickness = Thickness(2);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = scrollFiles;
	hFileSearchViewGrid->SetRow(hBorder4, 4);
	hFileSearchViewGrid->Children->Add(hBorder4);

	hFileSearchTab->Content = hFileSearchViewGrid;
}
//----< method to setup File Upload View >--
void Client::setUpFileUploadListView()
{
	Console::Write("\n  setting up File Upload view");
	hFileListGrid->Margin = Thickness(20);
	hFileUploadListTab->Content = hFileListGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hFileListGrid->RowDefinitions->Add(hRow1Def);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hFileListGrid->RowDefinitions->Add(hRow2Def);
	hListBox->SelectionMode = SelectionMode::Multiple;
	ScrollViewer^ scrollFiles = gcnew ScrollViewer();
	scrollFiles->Content = hListBox;
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = scrollFiles;
	hFileListGrid->SetRow(hBorder1, 0);
	hFileListGrid->Children->Add(hBorder1);
	hFolderBrowseButton->Content = "Select Directory";
	Border^ hBorder2 = gcnew Border();
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->Child = hFolderBrowseButton;
	hUploadFilesButton->Content = "Upload Files";
	Border^ hBorder3 = gcnew Border();
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->Child = hUploadFilesButton;
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 30;
	uploadPanel->Children->Add(hBorder2);
	uploadPanel->Children->Add(hSpacer1);
	uploadPanel->Children->Add(hBorder3);
	uploadPanel->Orientation = Orientation::Horizontal;
	uploadPanel->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hFileListGrid->SetRow(uploadPanel, 1);
	hFileListGrid->Children->Add(uploadPanel);
	hFolderBrowserDialog->ShowNewFolderButton = false;
	String^ path = System::IO::Directory::GetCurrentDirectory();
	hFolderBrowserDialog->SelectedPath = path + "\\UploadFromHere\\";
}
//----< method to setup File Download View >--
void Client::setUpFileDownloadView(){
	Console::Write("\n  setting up File Download view");
	setUpDownloadViewRowsAndColumns();
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 10;
	hCategoryTextBox->Height = 30;
	hCategoryTextBox->Width = 60;
	hCategoryTextBox->ToolTip = "Ex: *.cpp,*.h,*.cs";
	hCategoryAddButton->Content = "Add";
	hCategoryClearButton->Content = "Clear";
	Border^ border1 = gcnew Border();
	border1->BorderThickness = Thickness(1);
	border1->BorderBrush = Brushes::Black;
	border1->Height = 30;
	border1->Child = hCategoryAddButton;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 10;
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer3->Width = 10;
	TextBlock^ hSpacer4 = gcnew TextBlock();
	hSpacer4->Width = 10;
	Border^ border2 = gcnew Border();
	border2->BorderThickness = Thickness(1);
	border2->BorderBrush = Brushes::Black;
	border2->Height = 30;
	border2->Child = hCategoryClearButton;
	hCategoryListBox->Height = 100;
	hCategoryListBox->Width = 70;
	hCategoryListBox->SelectionMode = SelectionMode::Multiple;
	hCategoryPanel->Children->Add(hCategoryLabel);
	hCategoryPanel->Children->Add(hSpacer1);
	hCategoryPanel->Children->Add(hCategoryTextBox);
	hCategoryPanel->Children->Add(hSpacer2);
	hCategoryPanel->Children->Add(border1);
	hCategoryPanel->Children->Add(hSpacer3);
	hCategoryPanel->Children->Add(border2);
	hCategoryPanel->Children->Add(hSpacer4);
	hCategoryPanel->Children->Add(hCategoryListBox);
	hCategoryPanel->Orientation = Orientation::Horizontal;
	hCategoryPanel->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hFileDownloadListGrid->SetRow(hCategoryPanel, 1);
	hFileDownloadListGrid->Children->Add(hCategoryPanel);
	setUpFileDownloadButtons();
}
//----< method to setup Download View Rows And Columns >--
void Client::setUpDownloadViewRowsAndColumns(){
	RowDefinition^ row1 = gcnew RowDefinition();
	row1->Height = GridLength(37);
	RowDefinition^ row2 = gcnew RowDefinition();
	row2->Height = GridLength(120);
	RowDefinition^ row3 = gcnew RowDefinition();
	row3->Height = GridLength(37);
	RowDefinition^ row4 = gcnew RowDefinition();
	row4->Height = GridLength(37);
	RowDefinition^ row5 = gcnew RowDefinition();
	hFileDownloadListGrid->RowDefinitions->Add(row1);
	hFileDownloadListGrid->RowDefinitions->Add(row2);
	hFileDownloadListGrid->RowDefinitions->Add(row3);
	hFileDownloadListGrid->RowDefinitions->Add(row4);
	hFileDownloadListGrid->RowDefinitions->Add(row5);
	hDownloadPathLbl->Content = "Enter Path to search for (optional) :";
	hDownloadPathLbl->Height = 27;
	hDownloadPathBox->Height = 27;
	hDownloadPathBox->Width = 150;
	hDownloadPathBox->ToolTip = "Ex: c++,csharp,java,other";
	StackPanel^ pathPanel = gcnew StackPanel();
	pathPanel->Children->Add(hDownloadPathLbl);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	pathPanel->Children->Add(hSpacer);
	pathPanel->Children->Add(hDownloadPathBox);
	pathPanel->Orientation = Orientation::Horizontal;
	pathPanel->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hFileDownloadListGrid->SetRow(pathPanel, 0);
	hFileDownloadListGrid->Children->Add(pathPanel);
	hCategoryLabel->Content = "Enter Pattern :";
	hCategoryLabel->Height = 27;
}
//----< method to setup download buttons >--
void Client::setUpFileDownloadButtons(){
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 30;
	hGetDownloadFilesButton->Content = "Get files";
	Border^ border1 = gcnew Border();
	border1->Width = 120;
	border1->Height = 30;
	border1->BorderThickness = Thickness(1);
	border1->BorderBrush = Brushes::Black;
	border1->Child = hGetDownloadFilesButton;
	
	hDownloadFilesButton->Content = "Download";
	Border^ border3 = gcnew Border();
	border3->Width = 120;
	border3->Height = 30;
	border3->BorderThickness = Thickness(1);
	border3->BorderBrush = Brushes::Black;
	border3->Child = hDownloadFilesButton;
	dwnload1->Children->Add(border1);
	dwnload1->Children->Add(hSpacer1);
	dwnload1->Children->Add(border3);
	dwnload1->Orientation = Orientation::Horizontal;
	dwnload1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hFileDownloadListGrid->SetRow(dwnload1, 2);
	hFileDownloadListGrid->Children->Add(dwnload1);

	hDwnldOutputLabel->Content = "Files found :";
	hDwnldOutputLabel->Height = 27;
	hFileDownloadListGrid->SetRow(hDwnldOutputLabel, 3);
	hFileDownloadListGrid->Children->Add(hDwnldOutputLabel);
	hDownloadListBox->SelectionMode = SelectionMode::Multiple;
	ScrollViewer^ scrollFiles = gcnew ScrollViewer();
	scrollFiles->Content = hDownloadListBox;
	Border^ hBorder2 = gcnew Border();
	hBorder2->BorderThickness = Thickness(2);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = scrollFiles;
	hFileDownloadListGrid->SetRow(hBorder2, 4);
	hFileDownloadListGrid->Children->Add(hBorder2);	
	hFileDownloadTab->Content = hFileDownloadListGrid;
}
//----< method on which receive thread works on >--
void Client::getMessage()
{
	while (true)
	{
		std::cout << "\n  receive thread calling getMessage()";
		Message msg = recvr->getMessage();
		Header hdr = msg.getHeader();
		String^ sMsg = toSystemString(msg.getMsg());
		array<String^>^ args = gcnew array<String^>(1);
		args[0] = sMsg;
		if (hdr.getAttrib("Mode") == "download"){//if the message is a download message send acknowledgement back to server
			String^ port = toSystemString(to_string(msg.getHeader().getSrcPort()));
			sendr->postMessage(msg);
			System::Windows::MessageBox::Show("File Downloaded Successfully And Saved at DownloadedIn"+port, "Acknowledgement Message");
			continue;
		}
		else if (hdr.getCmd() == "Acknowledge"){//if the message is a file upload acknowledgement
			Action<String^>^ act = gcnew Action<String^>(this, &Client::showAcknowledgement);
			Dispatcher->Invoke(act, args);
		}
		else if(hdr.getCmd()== "GET_DOWNLOAD_FILES_ACK"){//Acknowledgement message of file names retrieved for download
			Action<String^>^ act = gcnew Action<String^>(this, &Client::showDownloadableFiles);
			Dispatcher->Invoke(act, args);  
		}
		else if (hdr.getCmd() == "GET_FILES_ACK"){//Acknowledgement message for search of files
			Action<String^>^ act = gcnew Action<String^>(this, &Client::showSearchedFiles);
			Dispatcher->Invoke(act, args);
		}
		else if (hdr.getCmd() == "TEXT_SEARCH_ACK"){//Text Search Acknowledgement
			size_t reqTime = System::Convert::ToInt64(toSystemString(msg.getHeader().getAttrib("StartTime")));
			size_t resTime = System::Convert::ToInt64(toSystemString(msg.getHeader().getAttrib("EndTime")));
			size_t res = resTime - reqTime;//calculating performance
			String^ performanceTime = toSystemString("Performance Time (Request + Processing + Response) :"+to_string(res)+" milliseconds");
			Action<String^>^ act = gcnew Action<String^>(this, &Client::showTextSearchResults);
			Dispatcher->Invoke(act, args);
			Action<String^>^ act1 = gcnew Action<String^>(this, &Client::addText);
			Dispatcher->Invoke(act1, performanceTime);
		}
	}
}
//----< adding Text to the Status >----
void Client::addText(String^ msg)
{
	hStatus->Text = msg + "\n\n";
}
//----< Show acknowledgement of file uploaded successfully on the server >----
void Client::showAcknowledgement(String^ msg){
	Message m;
	Header h;
	m.parseMsg(toStdString(msg));
	h = m.getHeader();
	System::Windows::MessageBox::Show(toSystemString(h.getAttrib("filename") + " Uploaded Successfully !"), "Acknowledgement Message");
	hStatus->Text = toSystemString(h.getAttrib("filename")) + " Uploaded Successfully !";
}
//----< method to display downloadable files >----
void Client::showDownloadableFiles(String^ msg){
	Message m;
	Header h;
	m.parseMsg(toStdString(msg));
	vector<std::string> files = m.parseBodyFileList(toStdString(msg));
	h = m.getHeader();
	if (files.size() > 0){
		hStatus->Text = "File Names Retrieved from Server...";
		for (string file : files){
			hDownloadListBox->Items->Add(toSystemString(file));
		}
	}
	else{
		System::Windows::MessageBox::Show("No Files Found !", "Message");
	}
	
}
//----< method to display found files after searching >----
void Client::showSearchedFiles(String^ msg){
	Message m;
	Header h;
	m.parseMsg(toStdString(msg));
	vector<std::string> files = m.parseBodyFileList(toStdString(msg));
	h = m.getHeader();
	if (files.size() > 0){
		hStatus->Text = "File Names Retrieved from Server...";
		for (string file : files){
			fileOutputListBox->Items->Add(toSystemString(file) + "\t From : " + toSystemString(m.getHeader().getSrcAddr()) + ":" + m.getHeader().getSrcPort());
		}
	}
	else{
		System::Windows::MessageBox::Show("No Files Found !", "Message");
	}	
}
//----< method to display text search results >----
void Client::showTextSearchResults(String^ args){
	Message m;
	Header h;
	m.parseMsg(toStdString(args));
	vector<std::string> files = m.parseBodyFileList(toStdString(args));
	h = m.getHeader();
	if (files.size() > 0){
		for (string file : files){
			txtOutputListBox->Items->Add(toSystemString(file) + "\t\t From : " + toSystemString(m.getHeader().getSrcAddr()) + ":" + m.getHeader().getSrcPort());
		}
	}
	else{
		System::Windows::MessageBox::Show("No Files Found !", "Message");
	}
}
//----< method to open and display files for upload >----
void Client::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	hListBox->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hFolderBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hFolderBrowserDialog->SelectedPath;
		std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
		array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
		for (int i = 0; i < files->Length; ++i)
			hListBox->Items->Add(files[i]);
	}
}
void Client::uploadFiles(Object^ sender, RoutedEventArgs^ args){
	if (connected){
		if (hListBox->SelectedItems->Count > 0){
			for each (String^ item in hListBox->SelectedItems)
			{
				commChannel->uploadFile(toStdString(item));
			}
		}
	}	
	else{
		std::cout << "\n Not Connected to Server !!";
		System::Windows::MessageBox::Show("Not Connected to Server !!");
	}
}
//----< adding Categroy for text search >----
void Client::addCategory(Object^ sender, RoutedEventArgs^ args){
	if (hCategoryTextBox->Text != ""){
		hCategoryListBox->Items->Add(hCategoryTextBox->Text);
		hCategoryTextBox->Text = "";
	}
	else{
		hStatus->Text = "Enter value to add !!";
	}
}
//----< Clearing selected categories >----
void Client::clearCategories(Object^ sender, RoutedEventArgs^ args){
	if (hCategoryListBox->SelectedItems->Count > 0){
		for (int i = hCategoryListBox->SelectedItems->Count - 1; i >= 0; i--)
		{
			hCategoryListBox->Items->RemoveAt(hCategoryListBox->SelectedIndex);
		}
	}
	else{
		hStatus->Text = "\nSelect Items to Clear";
	}
}
//----< on windows load >----
void Client::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Window loaded");
}
//----< on windows unload >----
void Client::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
	Console::Write("\n  Window closing");
}
//----< called when listen button is clicked >----
void Client::listen(Object^ sender, RoutedEventArgs^ args){

	size_t portno = System::Convert::ToUInt64(portToListen->Text);
	recvr->listen(portno);
	listenButton->Background = Brushes::LawnGreen;
	hStatus->Text = "Listening....";
	listening = true;
	commChannel->setSrcPort(portno);
}
//----< called on click of connect button, establishes connection >----
void Client::connect(Object^ sender, RoutedEventArgs^ args){
	if (listening){
		std::string targetAddr = toStdString(ipAddrTextBox->Text);
		size_t portno = System::Convert::ToUInt64(portNoTextBox->Text);
		commChannel->setTargetAddr(targetAddr, portno);
		if (!connected){
			if (sendr->connect(targetAddr, portno)){
				connected = true;
				commChannel->setTargetAddr(targetAddr, portno);
				connectButton->Background = Brushes::LawnGreen;
				hStatus->Text = "Connected !!!";
			}
			connected = true;
			commChannel->start();
		}
	}
	else{
		System::Windows::MessageBox::Show("Please Start Listening and Then Connect !!", "ERROR");
	}
	
}
//----< called on click of get files >----
void Client::getDownloadFiles(Object^ sender, RoutedEventArgs^ args) {
	hDownloadListBox->Items->Clear();
	if (connected){
		vector<string> patt;
		for each (String^ item in hCategoryListBox->Items)
		{
			patt.push_back(toStdString(item));
		}
		if (patt.size() == 0){
			patt.push_back("*.*");
		}
		commChannel->getDownloadFiles(patt,toStdString(hDownloadPathBox->Text));
	}
	else{
		std::cout << "\n Not Connected to Server !!";
		System::Windows::MessageBox::Show("Something is incorrect (either Connection or selction of Categories) !!","ERROR");
	}
}
//----< called for downloading files >----
void Client::downloadFiles(Object^ sender, RoutedEventArgs^ args){
	if (connected){
		if (hDownloadListBox->SelectedItems->Count > 0){
			for each (String^ item in hDownloadListBox->SelectedItems)
			{
				commChannel->downloadFile(toStdString(item));
			}
		}
	}
	else{
		std::cout << "\n Not Connected to Server !!";
		System::Windows::MessageBox::Show("Not Connected to Server !!");
	}
}
//----< called for searching text >----
void Client::searchText(Object^ sender, RoutedEventArgs^ args){
	txtOutputListBox->Items->Clear();
	if (connected && txtEndPointSelectedListBox->HasItems && txtRegexBox->Text!=""){
		txtOutputListBox->Items->Clear();
		vector<string> patt;
		for each (String^ item in txtSelectedListBox->Items)
		{
			patt.push_back(toStdString(item));
		}
		if (patt.size() == 0){
			patt.push_back("*.*");
		}
		for each (String^ endpnt in txtEndPointSelectedListBox->Items){
			commChannel->getTextSearchResults(patt, toStdString(txtRegexBox->Text), System::Convert::ToUInt64(endpnt),toStdString(textCategoryBox->Text));
		}		
	}
	else{
		std::cout << "\n Not Connected to Server !!";
		System::Windows::MessageBox::Show("Something is incorrect (Check Connection or Add an End Point or Enter text/Regex to search) !!", "ERROR");
	}
}
//----< called for seaching files >----
void Client::searchFiles(Object^ sender, RoutedEventArgs^ args){
	fileOutputListBox->Items->Clear();
	if (connected){
		vector<string> patt;
		string path = toStdString(filePathBox->Text);
		for each (String^ item in fileListBox->Items)
		{
			patt.push_back(toStdString(item));
		}
		if (filePathBox->Text == "..")
		{
			System::Windows::MessageBox::Show("Enter Valid Relative Path", "ERROR");
		}
		else
		{
			if (patt.size() == 0){
				patt.push_back("*.*");
			}
			commChannel->getFiles(patt, path);
		}
			
	}
	else{
		std::cout << "\n Not Connected to Server !!";
		System::Windows::MessageBox::Show("Please Check File Path or Check Connection!!", "ERROR");
	}
}
//----< adding Wildcard/pattern >----
void Client::addWildCard(Object^ sender, RoutedEventArgs^ args){
	if (textWildCardBox->Text != ""){
		txtSelectedListBox->Items->Add(textWildCardBox->Text);
		textWildCardBox->Text = "";
	}
	else{
		System::Windows::MessageBox::Show("Enter Value to ADD !!", "ERROR");
	}
}
//----< Clearing selected wildcards >----
void Client::clearWildCards(Object^ sender, RoutedEventArgs^ args){
	if (txtSelectedListBox->SelectedItems->Count > 0){
		for (int i = txtSelectedListBox->SelectedItems->Count - 1; i >= 0; i--)
		{
			txtSelectedListBox->Items->RemoveAt(txtSelectedListBox->SelectedIndex);
		}
	}
	else{
		System::Windows::MessageBox::Show("Select Item/s to Clear !!", "ERROR");
	}
}
//----< adding End point>----
void Client::addEndPoint(Object^ sender, RoutedEventArgs^ args){
	if (textEndPointBox->Text != ""){
		txtEndPointSelectedListBox->Items->Add(textEndPointBox->Text);
		textEndPointBox->Text = "";
	}
	else{
		System::Windows::MessageBox::Show("Enter Value to ADD !!", "ERROR");
	}
}
//----< clearing end point >----
void Client::clearEndPoints(Object^ sender, RoutedEventArgs^ args){
	if (txtEndPointSelectedListBox->SelectedItems->Count > 0){
		for (int i = txtEndPointSelectedListBox->SelectedItems->Count - 1; i >= 0; i--)
		{
			txtEndPointSelectedListBox->Items->RemoveAt(txtEndPointSelectedListBox->SelectedIndex);
		}
	}
	else{
		System::Windows::MessageBox::Show("Select Item/s to Clear !!", "ERROR");
	}
}
//----< adding file pattern to search for >----
void Client::addPattern(Object^ sender, RoutedEventArgs^ args){
	if (filePatternBox->Text != ""){
		fileListBox->Items->Add(filePatternBox->Text);
		filePatternBox->Text = "";
	}
	else{
		System::Windows::MessageBox::Show("Enter Value to ADD !!", "ERROR");
	}
}
//----< clearing patterns >----
void Client::clearPatterns(Object^ sender, RoutedEventArgs^ args){
	if (fileListBox->SelectedItems->Count > 0){
		for (int i = fileListBox->SelectedItems->Count - 1; i >= 0; i--)
		{
			fileListBox->Items->RemoveAt(fileListBox->SelectedIndex);
		}
	}
	else{
		System::Windows::MessageBox::Show("Select Item/s to Clear !!", "ERROR");
	}
}
//----< mthod called when tab selection has been changed >-----
void CppCliWindows::Client::OnSelectionChanged(System::Object ^sender, System::Windows::Controls::SelectionChangedEventArgs ^e)
{
	hStatus->Text = "";
}
//----< test stub>----
#ifdef TEST_CLIENT
[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
	Console::WriteLine(L"\n Starting WPFCppCliDemo");

	Application^ app = gcnew Application();
	app->Run(gcnew Client());
	Console::WriteLine(L"\n\n");
}
#endif


