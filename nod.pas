var 
	k, l, n: integer;
	
var
	c:real;
 
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


 
