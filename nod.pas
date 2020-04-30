var 
	k, l, n: integer;

var arr:array[-11..10] of integer;

var
	c:real;
	
function test1 (a1, c1 : real):integer;
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
 
	repeat
            if (k > l) then begin
				k := k mod l;
			end
            else begin
                l := l mod k;
			end;
        until (k = 0) or (l = 0);
	n:=k+l;
    writeln ('НОД = ', n);

//readln
end.


 
