///////////////////////////////////////////////////////////////
// demoIndexers.cs - Indexers work this way
//
// Jim Fawcett, CSE775 - Distributed Objects, Spring 2003
///////////////////////////////////////////////////////////////

using System;

namespace demoIndexers
{
	class demoIndexers
	{
    private bool _ardvvark  = false;
    private bool _falcon    = true;
    private bool _armadillo = false;
    private bool _kangaroo  = false;

    public bool this[string animal]
    {
      get
      {
        switch(animal)
        {
          case "ardvvark"  : return _ardvvark;
          case "falcon"    : return _falcon;
          case "armadillo" : return _armadillo;
          case "kangaroo"  : return _kangaroo;
          default :
            throw new ArgumentException("animal");
        }
      }
      set
      {
        switch(animal)
        {
          case "ardvvark"  : _ardvvark  = value; return;
          case "falcon"    : _falcon    = value; return;
          case "armadillo" : _armadillo = value; return;
          case "kangaroo"  : _kangaroo  = value; return;
          default :
            throw new ArgumentException("animal");
        }
      }
    }
    [STAThread]
		static void Main(string[] args)
		{
      demoIndexers creature = new demoIndexers();
      Console.Write("\n  creature[\"falcon\"] is {0}",creature["falcon"]);
      creature["falcon"] = false;
      Console.Write("\n  creature[\"falcon\"] is {0}\n\n",creature["falcon"]);
    }
	}
}
