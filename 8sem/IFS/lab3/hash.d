module hash;

import core.memory;

import std.array;
import std.conv;
import std.file;
import std.getopt;
import std.math;
import std.mmfile;
import std.stdio;

//debug = READWRITE

immutable uint maxX = 100_000u;
immutable uint maxN = 10_000u;
immutable uint xSize = long.sizeof;

class Hash
{
  this(string filePath)
  {
    if (exists(filePath)) {
      this.mf = new MmFile(filePath, MmFile.Mode.readWrite, initOffset, null);//, 0x100_0000);
      this(filePath, mf);
    } else {
      this(filePath, maxX, maxN, 5);
    }
  }

  this(string filePath, MmFile mf)
  {
    this.mf = mf;
    this.maxItmCount = RWType!(uint).readAt(0);
    this.hshCount = RWType!(uint).readAt(4);
    this.buckSize = RWType!(ushort).readAt(8);
    this.buckOffset = cast(uint) (initOffset + uint.sizeof * hshCount);
    this.fileSize =
      cast(uint) (buckOffset +
                  ulong.sizeof * maxItmCount +
                  (lrint(cast(real)(maxItmCount) / buckSize - 0.5) + 1) *
                  (uint.sizeof + ubyte.sizeof));

    debug(READWRITE)
      writeln("Existent hash (mx=", maxItmCount, ", mn=", hshCount, ", bs=",
              buckSize, ", bo=", buckOffset, ", fs=", fileSize, ").");

    this.mf = new MmFile(filePath, MmFile.Mode.readWrite, fileSize, null);
    delete mf;
  }

  this(string filePath, uint maxItmCount, uint hshCount, ushort buckSize)
  {
    this.maxItmCount = maxItmCount;
    this.hshCount = hshCount;
    this.buckSize = buckSize;
    this.buckOffset = cast(uint) (initOffset + uint.sizeof * hshCount);
    this.fileSize =
      cast(uint) (buckOffset +
                  ulong.sizeof * maxItmCount +
                  (lrint(cast(real)(maxItmCount) / buckSize - 0.5) + 1) *
                    (uint.sizeof + ubyte.sizeof));

    debug(READWRITE)
      writeln("New hash (mx=", maxItmCount, ", mn=", hshCount, ", bs=",
              buckSize, ", bo=", buckOffset, ", fs=", fileSize, ").");

    mf = new MmFile(filePath, MmFile.Mode.readWrite, fileSize, null);
    writeConf();
  }

  ~this()
  {
    mf.flush();
    delete mf;
  }

  bool find(long v)
  {
    uint hsh = hash(v);
    Bucket b = getFirstBucketFor(hsh);
    do {
      foreach (ushort i; 0 .. b.cnt) {
        if (b.values[i] == v)
          return true;
      }
      delete b.values;

      if (b.next != 0)
        b = getBucketAt(b.next);
    } while (b.next != 0);

    return false;
  }

  void add(long v)
  {
    uint addr = cast(uint) (hash(v) * uint.sizeof + initOffset);
    uint baddr = RWType!(uint).readAt(addr);
    if (baddr == 0) {
      Bucket b = allocateBucket();
      b.values[b.cnt++] = v;
      RWType!(uint).writeAt(addr, b.addr);
      setBucket(b);
    } else {
      Bucket b = firstIncompleteBucket(baddr);
      b.values[b.cnt++] = v;
      //printf("Bucket @0x%0.8x (%hhd) %d %d %d %d %d\n", b.addr, b.cnt, b.values[0], b.values[1], b.values[2], b.values[3], b.values[4]);
      setBucket(b);
    }
  }

private:
  struct Bucket
  {
    uint addr = 0;
    ubyte cnt = 0;
    long[] values;
    uint next = 0;
  }

  uint hash(long v)
  {
    return cast(uint) ((v * v) % hshCount);
    //return 0x100;
  }

  Bucket getFirstBucketFor(uint hsh)
  {
    uint addr = RWType!(uint).readAt(cast(uint) (initOffset + hsh * uint.sizeof));
    if (addr == 0)
      return Bucket();
    else
      return getBucketAt(addr);
  }

  Bucket getBucketAt(uint at)
  {
    assert(at >= buckOffset);

    Bucket res;
    res.addr = at;
    res.values = new long[buckSize];

    res.cnt = RWType!(ubyte).readAt(at);
    foreach (ushort i; 0 .. buckSize) {
      res.values[i] = RWType!(long).readAt(cast(uint) (at + ubyte.sizeof + i*long.sizeof));
    }
    res.next = RWType!(uint).readAt(cast(uint) (at + ubyte.sizeof + buckSize*long.sizeof));
    return res;
  }

  void setBucket(Bucket b)
  {
    const uint at = b.addr;
    assert(at >= buckOffset);
    assert(at < fileSize);

    RWType!(ubyte).writeAt(at, b.cnt);
    foreach (ushort i; 0 .. buckSize) {
      RWType!(long).writeAt(
        cast(uint) (at + ubyte.sizeof + i*long.sizeof), b.values[i]);
    }
    RWType!(uint).writeAt(
      cast(uint) (at + ubyte.sizeof + buckSize*long.sizeof), b.next);
  }

  uint firstFreeBucketAddr()
  {
    bool isFree(uint addr)
    {
      const uint offs = ubyte.sizeof + buckSize*long.sizeof;
      return RWType!(ubyte).readAt(addr) == 0 &&
        RWType!(uint).readAt(addr + offs) == 0;
    }

    uint addr = buckOffset;
    const uint step = ubyte.sizeof + buckSize*long.sizeof + uint.sizeof;
    while (addr < fileSize && !isFree(addr))
      addr += step;
    if (addr < fileSize)
      return addr;
    else
      return 0;
  }

  Bucket allocateBucket()
  {
    Bucket b;
    b.values = new long[buckSize];
    b.addr = firstFreeBucketAddr();
    RWType!(ubyte).writeAt(b.addr, 1);
    return b;
  }

  Bucket firstIncompleteBucket(uint addr)
  {
    const uint offs = ubyte.sizeof + buckSize*long.sizeof;
    assert(addr != 0);
    uint next = addr;
    ubyte sz;

    do {
      addr = next;
      sz = RWType!(ubyte).readAt(addr);
      next = RWType!(uint).readAt(addr + offs);
    } while (sz == buckSize && next != 0);

    if (sz == buckSize) {
      Bucket b = allocateBucket();
      RWType!(uint).writeAt(addr + offs, b.addr);
      return b;
    } else {
      return getBucketAt(addr);
    }
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
      //mf.flush();
    }
  }

  void writeConf()
  {
    assert(mf !is null);
    RWType!(uint).writeAt(0, maxItmCount);
    RWType!(uint).writeAt(4, hshCount);
    RWType!(ushort).writeAt(8, buckSize);
  }

  static immutable uint initOffset = 0x40u;

  MmFile mf;
  immutable uint maxItmCount;
  immutable uint hshCount;
  immutable ushort buckSize;
  immutable uint buckOffset;
  immutable uint fileSize;
}

void main(string[] args)
{
  string filePath = "./file.dat",
    batchFilePath = null;
  getopt(args,
         "file|f", &filePath,
         "batch|b", &batchFilePath);
  Hash h = new Hash(filePath);
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
        h.find(to!(long)(arg));
        break;
      case "add":
        //writeln("Inserting " ~ arg);
        h.add(to!(long)(arg));
        break;
      default:
        throw new Exception("unknown command: " ~ action);
      }
    }
  }
  delete h;
}

unittest
{
  string filename = "./test.dat";
  if (exists(filename))
    remove(filename);

  Hash h = new Hash(filename);
  assert(h.find(1) is false);
  h.add(1);
  assert(h.find(1) is true);
  assert(h.find(2) is false);
  assert(h.find(1001) is false);
  delete h;
  h = new Hash(filename);
  assert(h.find(1) is true);
  h.add(1);
  h.add(1);
  assert(h.find(1) is true);
  assert(h.find(1001) is false);
  h.add(2);
  assert(h.find(1) is true);
  assert(h.find(2) is true);
  assert(h.find(3) is false);
  delete h;
}
