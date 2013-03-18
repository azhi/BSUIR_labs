using System;
using System.Collections;

namespace Sort
{
  public class FileSorter
  {
    static public void SortByAlphabet(string path)
    {
      string[] lines = System.IO.File.ReadAllLines(path);
      Array.Sort(lines);
      System.IO.File.WriteAllLines(path, lines);
    }

    static public void SortByLength(string path)
    {
      string[] lines = System.IO.File.ReadAllLines(path);
      var lengthComparer = new StringComparerByLength();
      Array.Sort(lines, lengthComparer);
      System.IO.File.WriteAllLines(path, lines);
    }

    private class StringComparerByLength : IComparer
    {
      int IComparer.Compare(object s1, object s2)
      {
        return ((string) s1).Length.CompareTo(((string) s2).Length);
      }
    }
  }
}
