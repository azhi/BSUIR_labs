program enc;
uses
   Classes, lz1;

begin
   compress(TFileStream.Create(argv[1], fmOpenRead),
            TFileStream.Create(argv[2], fmCreate));
end.
