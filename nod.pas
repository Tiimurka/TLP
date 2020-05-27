var 
	k, l, n, check: integer;
 
begin
    writeln ('Введите два числа: ');
								
    readln (l);
	readln (k);
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
end.


 
