var a,b,i, y, u : integer;

var str1, str2 : string;

var r : real;

var arr:array[-11..10] of integer;

function test1 (a1, c1 : real):integer;
begin
	y := 228;
	u := 322;
	r :=14.88;
	writeln(a1+c1+r+y+u+4);
end;

function test2 ():integer;
begin
	writeln('More tests for the tests god!');
end;

begin
arr[7] := ((322+1)*(3*2)) mod (2+3);
arr[1] := 7*10--arr[7];
a:=arr[9]--7;
b:=5*-a;
//str1 := 10/a;
//str1:='test1';
//str2:='test2';
//str1:=str1+str2;
writeln(a);
//r:=-a;
test1(r, a);
if (a = 0) then
 begin
	if(b=0) then 
		b := 1;
	writeln('kek');
	
end
else if (a < 1) then
begin
	writeln(a+2);
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