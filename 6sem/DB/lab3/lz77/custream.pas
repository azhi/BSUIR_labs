{*
 * Program gAErch
 * File custream.pas
 *}
unit custream;

interface

uses
  Classes;

  procedure putst(const st : TStream; const n : byte; const x : word);
  function getst(const st : TStream; const n : byte):word;
  procedure putrest(const st : TStream);
  procedure init_custream;

implementation

var
  puts, putno : byte;
  getp, getno : byte;

procedure putst(const st : TStream; const n : byte; const x : word);
begin
  inc(puts,x shl putno);
  case n+putno of
  0..7   : inc(putno, n);
  8..15  : begin
      st.WriteByte(puts);
      puts:=x shr (8-putno);
      inc(putno,n-8);
    end;
  16..23 : begin
      st.WriteByte(puts);
      puts:=x shr (8-putno);
      st.WriteByte(puts);
      puts:=x shr (16-putno);
      inc(putno,n-16);
    end;
   end;
end;

procedure putrest(const st : TStream);
begin
  if putno=0 then exit;
  st.WriteByte(puts);
  puts:=0;
  putno:=0;
end;

function getst(const st : TStream; const n : byte) : word;
var
  t, res : word;
begin
  case n-getno of
  -8..0: begin
      res:=getp mod (1 shl n);
      dec(getno,n);
      getp:=getp shr n;
    end;
  1..8: begin
      t:=st.readByte;
      res:=getp;
      inc(res,t shl getno);
      res:=res mod (1 shl n);
      getp:=t shr (n-getno);
      inc(getno,8-n);
    end;
  9..16: begin
      t:=st.ReadWord;
      res:=getp;
      inc(res,t shl getno);
      res:=res mod (1 shl n);
      getp:=t shr (n-getno);
      inc(getno,16-n);
    end;
  end;
  getst:=res;
end;

procedure init_custream;
begin
  getp:=0;
  getno:=0;
  puts:=0;
  putno:=0;
end;

end.
