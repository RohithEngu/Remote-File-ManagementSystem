/////////////////////////////////////////////////////////////////
//  relatonships.cpp - demonstrate basic class relationships   //
//  version 2.0                                                //
//                                                             //
//  Jim Fawcett, CSE687, Spring 2003                           //
/////////////////////////////////////////////////////////////////
//                                                             //
//  This is a demonstration program so it does not have all    //
//  the features of a module, e.g.:                            //
//    - full prologue                                          //
//    - manual page                                            //
//    - maintenance page                                       //
//                                                             //
/////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Relationships.h"
using namespace std;

//----< assignment >-------------------------------------------
//
//  This is just what the compiler generated assignment does
//
//  D& D::operator =(const D &d)
//  {
//    if(this == &d) return *this;
//    (*this).B::operator=(d); 
//    pU = d.pU;    // don't delete because D did not create - see main
//    return *this;
//  }

//----< display object state >---------------------------------

void D::show()
{
  long save = std::cout.flags();
  formatOut(); 
  std::cout << "\n  " << std::setw(30) 
            << "Derived object speaking." << "My value is " 
            << value().value(); 

  // here is where D uses U

  if(pU != 0)
    std::cout << "\n  " << std::setw(30) 
              << "D Used object speaking:" << "My value is "
              << "\"" << (pU->value()).c_str()    // using U
              << "\"\n";
  std::cout << std::endl;
  std::cout.flags(save);
}

//
//----< register used object >---------------------------------

void D::setU(U *ptr) { pU = ptr; }    // here is where D gets a
                                             // pointer to a U object

//----< disclose physical layout of d object >-----------------

void D::showParts()
{
  long saveIOstate = std::cout.flags();

  size_t startD = reinterpret_cast<size_t>((D*)this);
  size_t   endD = startD + sizeof(*this);
  std::cout << "\n  location of d is from: " << startD << " to: " << endD
            << ", size of d is: " << sizeof(*this);
  
  size_t startB = reinterpret_cast<size_t>((B*)this);
  size_t   endB = startB + sizeof((B&)*this);
  std::cout << "\n  location of b is from: " << startB << " to: " << endB
            << ", size of b is: " << sizeof(*(B*)this);
            
  size_t startC = reinterpret_cast<size_t>((C*)&value());
  size_t   endC = startC + sizeof((C&)*this);
  std::cout << "\n  location of c is from: " << startC << " to: " << endC
            << ", size of c is: " << sizeof(value());
            
  size_t startU = reinterpret_cast<size_t>((U*)pU);
  size_t   endU = startU + sizeof(*pU);
  std::cout << "\n  location of u is from: " << startU << " to: " << endU
            << ", size of u is: " << sizeof(*pU);

  std::cout << "\n\n  or, if you like hexidecimal:\n" << hex;

  std::cout << "\n  location of d is from: " << startD << " to: " << endD
            << ", size of d is: " << sizeof(*this);
  
  std::cout << "\n  location of b is from: " << startB << " to: " << endB
            << ", size of b is: " << sizeof(*(B*)this);
            
  std::cout << "\n  location of c is from: " << startC << " to: " << endC
            << ", size of c is: " << sizeof(value());
            
  std::cout << "\n  location of u is from: " << startU << " to: " << endU
            << ", size of u is: " << sizeof(*pU);

  std::cout.flags(saveIOstate);
}

//
///////////////////////////////////////////
// non-standard entry point created by   //
// Visual Studio Application Wizard      //
///////////////////////////////////////////
// int _tmain(int argc, _TCHAR* argv[])  //
///////////////////////////////////////////

int main(int argc, char *argv[])
{
  cout << "\n  Demonstrating Class Relationships "
       << "\n ===================================\n";
       
  C c(1);                       // main aggregates a C
  c.show();
  
  B b(0);                       // main aggregates a B
  b.show();
  
  D d(-1);                      // main aggregates a D
  d.show();
  
  U u("a U-held string");       // main aggregates a U
  u.show();
  
  d.value() = -2;
  d.setU(&u);
  u.value() = "another U-held string";
  d.show();
  
  cout << "\n  These are d's parts:\n";
  d.showParts();
  
  cout << "\n\n";
	return 0;
}

