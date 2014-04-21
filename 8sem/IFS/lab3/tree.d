module tree;

import core.memory;

import std.array;
import std.conv;
import std.file;
import std.getopt;
import std.math;
import std.mmfile;
import std.stdio;

// debug = READWRITE;

// immutable uint maxX = 100_000u;
immutable uint maxN = 8;
immutable uint xSize = long.sizeof;
immutable uint addrSize = uint.sizeof;

class BTree
{
  this(string filePath)
  {
    if (exists(filePath)) {
      MmFile mf = new MmFile(filePath, MmFile.Mode.readWrite, initOffset, null);
      this(filePath, mf);
    } else {
      this(filePath, maxN, 5, 3);
    }
  }

  this(string filePath, MmFile mf)
  {
    this.mf = mf;

    this.buckSize = RWType!(ubyte).readAt(0);
    this.maxDepth = RWType!(ubyte).readAt(1);
    this.curDepth = RWType!(ubyte).readAt(2);
    this.leafSize = RWType!(ubyte).readAt(3);
    this.root     = RWType!(uint).readAt(4);

    uint leavesOffset = initOffset;
    foreach (uint i; 1 .. (maxDepth+1)) {
      if (i == maxDepth) {
        this.fileSize = leavesOffset +
          cast(uint) (xSize * leafSize + ubyte.sizeof) * pow(buckSize, i-1);
      } else {
        leavesOffset += pow(buckSize, i-1) *
          cast(uint) (addrSize * buckSize + xSize * (buckSize - 1));
      }
    }
    this.leavesOffset = leavesOffset;

    debug(READWRITE)
      writeln("Existent b-tree (bsz=", buckSize, ", lsz=", leafSize, ", cd=",
              curDepth, ", md=", maxDepth, ", fs=", fileSize, ").");

    delete mf;
    this.mf = new MmFile(filePath, MmFile.Mode.readWrite, fileSize, null);
  }

  this(string filePath, ubyte maxDepth, ubyte buckSize, ubyte leafSize)
  {
    this.maxDepth = maxDepth;
    this.buckSize = buckSize;
    this.leafSize = leafSize;

    uint leavesOffset = initOffset;
    foreach (uint i; 1 .. (maxDepth+1)) {
      if (i == maxDepth) {
        this.fileSize = leavesOffset +
          cast(uint) (xSize * leafSize + ubyte.sizeof) * pow(buckSize, i-1);
      } else {
        leavesOffset += pow(buckSize, i-1) *
          cast(uint) (addrSize * buckSize + xSize * (buckSize - 1));
      }
    }
    this.leavesOffset = leavesOffset;
    this.root = leavesOffset;

    debug(READWRITE)
      writeln("New b-tree (bsz=", buckSize, ", lsz=", leafSize, ", cd=",
              curDepth, ", md=", maxDepth, ", fs=", fileSize, ").");

    mf = new MmFile(filePath, MmFile.Mode.readWrite, fileSize, null);
    writeConf();
  }

  ~this()
  {
    if (mf !is null) {
      mf.flush();
      delete mf;
    }
  }

  bool find(long v)
  {
    uint addr = root;

    while (addr < leavesOffset) {
      Bucket b = getBucketAt(addr);
      ubyte i = 0;
      // addr = b.children[0];
      // while (b.children[i+1] != 0 && v <= b.boundaries[i]) i++;
      addr = buckChildForV(b, v);
    }
    Leaf l = getLeafAt(addr);

    foreach (ubyte i; 0..l.cnt)
      if (l.values[i] == v)
        return true;
    return false;
  }

  void add(long v)
  {
    if (root >= leavesOffset) {
      Leaf rootLeaf = getLeafAt(root);

      ubyte i = 0;
      while (i < rootLeaf.cnt && rootLeaf.values[i] < v)
        ++i;
      if (i < leafSize && rootLeaf.values[i] == v)
        return;
      rootLeaf.values.insertInPlace(i, v);
      if (rootLeaf.cnt < leafSize) {
        // Just adding to the root leaf
        // printf("Adding to the root leaf: %ld\n", v);
        // writefln("%s @ 0x%0.8x", rootLeaf.values, rootLeaf.addr);
        ++rootLeaf.cnt;
        setLeaf(rootLeaf);
      } else {
        // Splitting the root leaf
        // printf("Splitting the root leaf while adding: %ld\n", v);
        // writefln("%s @ 0x%0.8x", rootLeaf.values, rootLeaf.addr);
        Bucket rootBuck = allocateBucket();
        Leaf l2 = allocateLeaf();
        l2.values = rootLeaf.values[(leafSize+1)/2 .. $];
        l2.cnt = cast(ubyte) l2.values.length;
        rootLeaf.values = rootLeaf.values[0 .. (leafSize+1)/2];
        rootLeaf.cnt = cast(ubyte) rootLeaf.values.length;
        rootBuck.boundaries[0] = l2.values[0];
        rootBuck.children[0] = rootLeaf.addr;
        rootBuck.children[1] = l2.addr;
        this.curDepth++;
        RWType!(ubyte).writeAt(2, curDepth);
        setLeaf(l2);
        setLeaf(rootLeaf);
        setBucket(rootBuck);
        this.root = rootBuck.addr;
        RWType!(uint).writeAt(4, root);
      }
    } else {
      uint addr = root;
      ubyte d = 0;
      Bucket[] path = new Bucket[maxDepth];

      while (addr < leavesOffset) {
        Bucket b = getBucketAt(addr);
        path[d++] = b;
        ubyte i = 0;
        // writefln("b: %s, %s @ 0x%0.8x", b.children, b.boundaries, b.addr);
        while (i < buckSize-1 && b.children[i+1] != 0 && v >= b.boundaries[i])
          ++i;
        addr = b.children[i];
        // if (i==0)
        //   writeln("i: ", i);
      }

      Leaf l = getLeafAt(addr);
      {
        ubyte i = 0;
        while (i < l.cnt && l.values[i] < v)
          ++i;
        if (i < leafSize && l.values[i] == v)
          return;
        l.values.insertInPlace(i, v);
        // if (i == 0) {
        //   Bucket b = path[d-1];
        //   while (i < buckSize-1 && b.children[i+1] != 0 && v >= b.boundaries[i])
        //     ++i;
        //   if (i != 0)
        //     b.boundaries[i-1] = v;
        //   setBucket(b);
        // }
      }
      if (l.cnt < leafSize) {
        // Just adding to non-root leaf
        // printf("Adding to a leaf: %ld\n", v);
        // writefln("%s @ 0x%0.8x", l.values, l.addr);
        l.values[l.cnt++] = v;
        setLeaf(l);
      } else {
        Bucket b = path[d-1];
        // printf("Splitting a leaf while adding: %ld\n", v);
        // writefln("Cur bucket: %s, %s @ 0x%0.8x",
        //          b.children, b.boundaries, b.addr);
        if (b.children.back == 0) {
          ubyte i = 0;
          while (i < buckSize-1 && b.children[i+1] != 0 && v >= b.boundaries[i])
            ++i;
          Leaf l2 = allocateLeaf();
          l2.values = l.values[(leafSize+1)/2 .. $];
          l2.cnt = cast(ubyte) l2.values.length;
          l.values = l.values[0 .. (leafSize+1)/2];
          l.cnt = cast(ubyte) l.values.length;
          // writeln("i: ", i);
          b.boundaries.insertInPlace(i, l2.values[0]);
          b.children.insertInPlace(i+1, l2.addr);
          setLeaf(l2);
          setLeaf(l);
          setBucket(b);
        } else {
          balanceTree(path, d, v);
          add(v);
        }
      }
    }
  }

private:
  struct Bucket
  {
    uint addr = 0;
    long[] boundaries;
    uint[] children;
  }

  struct Leaf
  {
    uint addr = 0;
    ubyte cnt;
    long[] values;
  }

  Bucket getBucketAt(uint at)
  {
    assert(at >= initOffset);
    assert(at < leavesOffset);

    Bucket res;
    res.addr = at;
    res.boundaries = new long[buckSize-1];
    res.children = new uint[buckSize];

    foreach (ubyte i; 0 .. cast(ubyte)(buckSize-1)) {
      res.boundaries[i] = RWType!(long).readAt(cast(uint)(at + i*long.sizeof));
    }

    foreach (ubyte i; 0 .. buckSize) {
      res.children[i] = RWType!(uint).readAt(cast(uint) (at +
                                                         (buckSize-1)*
                                                         long.sizeof +
                                                         i*uint.sizeof));
    }
    return res;
  }


  void setBucket(Bucket b)
  {
    const uint at = b.addr;
    assert(at >= initOffset);
    assert(at < leavesOffset);

    foreach (ubyte i; 0 .. cast(ubyte)(buckSize-1)) {
      RWType!(long).writeAt(cast(uint) (at + i*long.sizeof), b.boundaries[i]);
    }

    foreach (ubyte i; 0 .. buckSize) {
      RWType!(uint).writeAt(cast(uint) (at + (buckSize-1)*long.sizeof +
                                        i*uint.sizeof),
                            b.children[i]);
    }
  }

  uint firstFreeBucketAddr()
  {
    bool isFree(uint addr)
    {
      const uint offs = cast(uint) ((buckSize-1)*long.sizeof);
      return RWType!(uint).readAt(addr + offs) == 0;
    }

    uint addr = initOffset;
    const uint step = cast(uint) ((buckSize-1)*long.sizeof + buckSize*uint.sizeof);
    while (addr < leavesOffset && !isFree(addr))
      addr += step;
    if (addr < leavesOffset)
      return addr;
    else
      return 0;
  }

  Bucket allocateBucket()
  {
    Bucket b;
    b.boundaries = new long[buckSize-1];
    b.children = new uint[buckSize];
    b.addr = firstFreeBucketAddr();
    RWType!(uint).writeAt(cast(uint) (b.addr + (buckSize-1)*long.sizeof),
                          0xffffffff);
    return b;
  }

  Leaf getLeafAt(uint at)
  {
    assert(at >= leavesOffset);
    assert(at < fileSize);

    Leaf res;
    res.addr = at;
    res.values = new long[leafSize];
    res.cnt = RWType!(ubyte).readAt(at);
    assert(res.cnt <= leafSize);
    foreach (ubyte i; 0 .. res.cnt) {
      res.values[i] =
        RWType!(long).readAt(cast(uint) (at + ubyte.sizeof + i*long.sizeof));
    }
    return res;
  }

  void setLeaf(Leaf l)
  {
    const uint at = l.addr;
    assert(at >= leavesOffset);
    assert(at < fileSize);
    assert(l.cnt <= leafSize);

    RWType!(ubyte).writeAt(at, l.cnt);

    foreach (ubyte i; 0 .. l.cnt) {
      RWType!(long).writeAt(cast(uint) (at + ubyte.sizeof + i*long.sizeof),
                            l.values[i]);
    }
  }

  uint firstFreeLeafAddr()
  {
    bool isFree(uint addr)
    {
      return RWType!(ubyte).readAt(addr) == 0;
    }

    uint addr = leavesOffset;
    const uint step = cast(uint) (leafSize*long.sizeof + ubyte.sizeof);
    while (addr < fileSize && !isFree(addr))
      addr += step;
    if (addr < fileSize)
      return addr;
    else
      return 0;
  }

  Leaf allocateLeaf()
  {
    Leaf l;
    l.values = new long[leafSize+1];
    l.addr = firstFreeLeafAddr();
    RWType!(ubyte).writeAt(l.addr, 1);
    return l;
  }


  template RWType(T)
  {
    T readAt(uint at)
    {
      debug (READWRITE) {
        printf("Reading %3d bytes at %0.8x: ", T.sizeof, at);
        printf(" (%0.16lx) ", mf[at .. at+T.sizeof].ptr);
        stdout.flush();
      }
      T res = *cast(T *) (mf[at .. at+T.sizeof].ptr);
      debug (READWRITE) printf("0x%0.8x\n", res);
      return res;
    }

    void writeAt(uint at, T val)
    {
      byte[T.sizeof] buf = *cast(byte[T.sizeof] *) &val;
      debug (READWRITE)
        printf("Writing %3d bytes at %0.8x: 0x%0.8x\n", T.sizeof, at, val);
      foreach (ushort i; 0 .. T.sizeof) {
        mf[at+i] = buf[i];
      }
      // mf.flush();
    }
  }

  void balanceTree(Bucket []path, ubyte d, long v)
  {
    // writefln("balance(%s, %d, %d)", path, d, v);
    immutable mid = buckSize/2;
    short i = d;
    do { --i; } while (i >= 0 && path[i].children.back != 0);
    if (i != -1) {
      // writeln("Splitting buckets:");
      foreach (short j; i .. d-1) {
        // writeln(j);
        // Balance subtree
        Bucket pb = path[j];
        Bucket b =  path[j+1];
        Bucket b2 = allocateBucket();
        long mBound = b.boundaries[mid-1];

        b2.children = b.children[mid .. $];
        b2.boundaries = b.boundaries[mid .. $];
        b2.boundaries.length = buckSize - 1;
        b2.children.length = buckSize;
        // writeln(b2.children);
        b.children = b.children[0 .. mid];
        b.boundaries = b.boundaries[0 .. mid-1];
        b.boundaries.length = buckSize - 1;
        b.children.length = buckSize;
        // writeln(b.children);

        ushort ind = 0;
        while (pb.children[ind] != b.addr)
          ++ind;

        pb.children.insertInPlace(ind+1, b2.addr);
        pb.boundaries.insertInPlace(ind, mBound);

        setBucket(b);
        setBucket(b2);
        setBucket(pb);
        path[j] = pb;
        path[j+1] = getBucketAt(buckChildForV(pb, v));
      }
    } else {
      ++this.curDepth;
      assert(curDepth <= maxDepth);
      RWType!(ubyte).writeAt(2, curDepth);

      // Split root
      // writeln("Splitting root ...");

      Bucket newRoot = allocateBucket();
      Bucket oldRoot = getBucketAt(root);
      Bucket b2 = allocateBucket();

      newRoot.children[0] = oldRoot.addr;
      newRoot.children[1] = b2.addr;
      newRoot.boundaries[0] = oldRoot.boundaries[mid-1];

      b2.children = oldRoot.children[mid .. $];
      b2.boundaries = oldRoot.boundaries[mid .. $];
      b2.boundaries.length = buckSize - 1;
      b2.children.length = buckSize;
      oldRoot.children = oldRoot.children[0 .. mid];
      oldRoot.boundaries = oldRoot.boundaries[0 .. mid-1];
      oldRoot.boundaries.length = buckSize - 1;
      oldRoot.children.length = buckSize;

      setBucket(newRoot);
      setBucket(oldRoot);
      setBucket(b2);
      this.root = newRoot.addr;
      RWType!(uint).writeAt(4, root);
    }
  }

  uint buckChildForV(Bucket b, long v)
  {
    ubyte i = 0;
    while (i < buckSize-1 && b.children[i+1] != 0 && v >= b.boundaries[i])
      ++i;
    return b.children[i];
  }

  void writeConf()
  {
    assert(mf !is null);

    RWType!(ubyte).writeAt(0, buckSize);
    RWType!(ubyte).writeAt(1, maxDepth);
    RWType!(ubyte).writeAt(2, curDepth);
    RWType!(ubyte).writeAt(3, leafSize);
    RWType!(uint).writeAt(4, root);

  }

  static immutable uint initOffset = 0x40u;

  MmFile mf;
  immutable ubyte buckSize;
  immutable ubyte leafSize;
  immutable ubyte maxDepth;
  immutable uint leavesOffset;
  /* immutable */ uint fileSize;
  uint root;
  ubyte curDepth;
}

void main(string[] args)
{
  string filePath = "./file.dat",
    batchFilePath = null;
  getopt(args,
         "file|f", &filePath,
         "batch|b", &batchFilePath);
  BTree t = new BTree(filePath);
  if (batchFilePath) {
    auto f = File(batchFilePath);
    scope(exit) f.close();
    foreach (str; f.byLine) {
      char[][] z = split(str);
      string action = z[0].dup;
      string arg = z[1].dup;
      switch (action) {
      case "find":
        //writeln("Searching for " ~ arg);
        t.find(to!(long)(arg));
        break;
      case "add":
        //writeln("Inserting " ~ arg);
        t.add(to!(long)(arg));
        break;
      default:
        throw new Exception("unknown command: " ~ action);
      }
    }
  }
  delete t;
}

unittest
{
  string filename = "./test.dat";
  if (exists(filename))
    remove(filename);

  BTree t = new BTree(filename);
  assert(t.find(1) is false);
  t.add(1);
  assert(t.find(1) is true);
  assert(t.find(2) is false);
  assert(t.find(1001) is false);
  delete t;
  t = new BTree(filename);
  assert(t.find(1) is true);
  t.add(1);
  t.add(1);
  assert(t.find(1) is true);
  assert(t.find(1001) is false);
  t.add(2);
  assert(t.find(1) is true);
  assert(t.find(2) is true);
  assert(t.find(3) is false);
  delete t;
}
