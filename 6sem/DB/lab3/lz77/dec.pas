program dec;
uses
   Classes, lz1;

begin
   decompress(TFileStream.Create(argv[1], fmOpenRead),
              TFileStream.Create(argv[2], fmCreate));
end.
