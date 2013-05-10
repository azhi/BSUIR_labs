{*
 * Program gAErch
 * File lz1.pas
 *}

unit lz1;

interface
uses
    Classes;

  procedure description(var a : PChar);
  procedure ext(var a : PChar);
  procedure compress(fin:TStream; fout:TStream);
  procedure decompress(fin:TStream; fout:TStream);

implementation
uses
    custream;

type
  buffer = array [0..65535] of byte;

const
  buflength=31;
  diclength=255;


procedure description(var a : PChar);
begin
  a:='lz77 compression library';
end;

procedure ext(var a : PChar);
begin
  a:='lz1';
end;

procedure compress(fin:TStream; fout:TStream);
var
  buf   : ^buffer;
  dict  : ^buffer;
  dd,td : ^buffer;

  procedure cp(var d1 : buffer; const d2 : buffer);
  var
    i : word;
  begin
    for i:=0 to diclength do
      d1[i]:=d2[i];
  end;

  function findmatch(const w : buffer; const m : word; var pos:word) : boolean;
  var
    t,p : word;
    i   : word;
  begin
    if diclength=0 then begin
      p:=fin.Position;
      for t:=0 to fin.Size do begin
        fin.seek(t,soFromBeginning);
        for i:=0 to m do
          if w[i]<>fin.ReadByte then
            break
          else if i=m then begin
            //WriteLn('Find: found length ',top,' on position ',t);
            fin.seek(p,soFromBeginning);
            pos:=t;
            exit(true);
          end;
        fin.seek(p,soFromBeginning);
      end;
    end else
      for t:=pos to diclength do
        for i:=0 to m do
          if w[i]<>dict^[(t+i) mod (diclength+1)] then break
          else if i=m then begin pos:=t; exit(true); end;
    pos:=0;
    exit(false);
  end;

  function nextj(var j : word; const top,p : word):boolean;
  var
    k,s,i : word;
    b     : boolean;
  begin
    cp(td^,dict^);
    for i:=j+1 to diclength do begin
      if i=p mod (diclength+1) then continue;
      cp(dict^,dd^);
      s:=i;
      b:=true;
      for k:=0 to top do begin
        if b then b:=(findmatch(buf^, k, s))and(s=i)
        else break;
        dict^[(p+k)mod(diclength+1)]:=buf^[k];
      end;
      if b then begin j:=i; exit(true); end;
    end;
    cp(dict^,td^);
    exit(false);
  end;

var
  top	  : word;
  posit   : Word;
  j       : Word;
  pp      : Word;
  p       : longInt;
  lnb,lnd : byte;

begin
  lnb:=0;
  pp:=buflength;
  while pp<>0 do begin
    inc(lnb);
    pp:=pp shr 1;
  end;

  lnd:=0;
  pp:=diclength;
  while pp<>0 do begin
    inc(lnd);
    pp:=pp shr 1;
  end;
  if diclength=0 then lnb:=16;
  top:=0;
  j:=0;
  init_custream;
  getmem(buf, buflength+1);
  getmem(dict, diclength+1);
  getmem(dd, diclength+1);
  getmem(td, diclength+1);
  fillbyte(buf^,buflength+1,0);
  fillbyte(dict^, diclength+1,0);

  for p:=0 to fin.size-1 do begin
    buf^[top]:=fin.ReadByte;
    //if p = 342 then begin for pp:=0 to diclength do write(chr(dict^[pp mod (diclength+1)])); writeLn; readkey(); end;
    if not findmatch(buf^,top,posit) or (p-top=posit) or (p=fin.size-1) or
        (top=buflength) or ((posit<>j) or (posit=p mod (diclength+1))) and
        not nextj(posit,top,p-top) then
    begin
      //WriteLn('Dumping on pos ',p,' with ending ',chr(buf^[top]));
      {fout.WriteByte(top);
      fout.WriteWord(j);
      fout.WriteByte(buf^[top]);}
      putst(fout,lnb,top);
      putst(fout,lnd,j);
      putst(fout,8,buf^[top]);
      dict^[p mod (diclength+1)]:=buf^[top];
      top:=0;
      j:=0;
      posit:=0;
      cp(dd^,dict^);
    end else begin
      //WriteLn('Adding ',chr(buf^[top]),' found on pos ',posit);
      dict^[p mod (diclength+1)]:=buf^[top];
      inc(top);
      j:=posit;
    end;
  end;
  putrest(fout);
  freemem(dict,buflength+1);
  freemem(buf,buflength+1);
  freemem(dd,buflength+1);
  freemem(td,buflength+1);
  fout.Destroy;
end;

procedure decompress(fin:TStream; fout:TStream);
var
  dict : ^buffer;

  procedure cop(s : word; t : word);
  var
    i    : ^buffer;
    sz,j : word;

  begin
    if s=0 then exit;
    if diclength=0 then
    begin
      sz:=s;
      if sz > fout.position-t then
        sz:=fout.position-t;
      getmem(i,sz);
      fout.seek(t,soFromBeginning);
      fout.Read(i^,sz);
      fout.seek(0,soFromEnd);
      for j:=1 to s div sz do
        fout.Write(i^,sz);
      fout.Write(i^,s mod sz);
      freemem(i,sz);
    end else begin
      for j:=1 to s do begin
        dict^[(fout.position) mod (diclength+1)]:=dict^[(t+j-1)mod(diclength+1)];
        fout.writeByte(dict^[(t+j-1) mod (diclength+1)]);
      end;
    end;
  end;

var
  p       : longint;
  t,pp    : word;
  s       : byte;
  lnb,lnd : byte;

begin
  lnb:=0;
  pp:=buflength;
  while pp<>0 do begin
    inc(lnb);
    pp:=pp shr 1;
  end;

  lnd:=0;
  pp:=diclength;
  while pp<>0 do begin
    inc(lnd);
    pp:=pp shr 1;
  end;
  if diclength=0 then lnb:=16;

  init_custream;
  getmem(dict,diclength+1);
  fillbyte(dict^, diclength+1,0);
  for p:=0 to round((fin.size)*8/(8+lnd+lnb)-1) do begin
    {s:=fin.readByte;
    t:=fin.readWord;}
    s:=getst(fin,lnb);
    t:=getst(fin,lnd);
    cop(s,t);
    {s:=fin.readByte;}
    s:=getst(fin,8);
    dict^[fout.position mod (diclength+1)]:=s;
    fout.writeByte(s);
  end;
  freemem(dict,diclength+1);
  fin.Destroy;
end;

end.
