using System;

using Sort;

public class MainConsole
{
  public static void Main(string[] args)
  {
    if ( "--alph" == args[0] )
    {
      FileSorter.SortByAlphabet(args[1]);
    }
    else if ( "--length" == args[0] )
    {
      FileSorter.SortByLength(args[1]);
    }
    else
      Console.WriteLine("Wrong usage.\n./MainConsole.exe [--alph/length] filename");
  }

}
