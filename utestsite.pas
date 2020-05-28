var 
	k, l, n, check: integer;

var arr:array[-11..10] of integer;

var
	c:real;
	
function test1 (a1, c1 : integer):integer;
begin
	k := 228;
	l := 322;
	n :=  14;
	writeln(a1+c1+k+l+n+4);
end;
	
 
{keklol}
begin
{lolkek}
    c := %11;
	writeln ('c = ', c);
    writeln ('Введите два числа: ');
								
    readln (k, l);
	check := 2;
	repeat
            if (k > l) then begin
				k := k mod l;
			end
            else 
			begin
                l := l mod k;
			end;
			writeln(l, ' ', k);
			if(k = 0) then
				check:=0;
				
			if(l = 0) then
				check:=0;
				
        until (check = 0);
	n:=k+l;
    writeln ('НОД = ', n);
	test1(30, 50);
//readln
end.


 
