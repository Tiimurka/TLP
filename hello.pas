var a,b,i : integer;

var t : real;

var r : real;



function test1 (a1, c1 : real):integer;
begin
	a := 2.28;
	c := 3.22;
	r :=14.88;
	writeln(a+c+r+4);
end;

function test2 (a2, c2 : real):integer;
begin
	writeln('More tests for the tests god!');
end;

begin
a := ((322+1)*(2*2)) mod (2+3);
b:=0;
if (a = 0) then
 begin
	if(b=0) then
		b := 1;
	writeln('kek');
	
end
else if (a < 1) then
begin
	writeln('lol');
	b := 3;
end
else
begin
	writeln('zaz');
end;

repeat
	a:=a+1;
until (a < 1000);

while (b < 1000) do begin 
	writeln('azazaz');
	b := b+200;
end;

for i:=(50+50) downto 0 do begin
	writeln(i, ' and ', b);
	b := b -1;
end;

writeln ('Hello World!');
end.