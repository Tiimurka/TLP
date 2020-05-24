var str : string;
var a : integer;
var arr:array[0..4] of integer;

begin
	write('GIVE ME YOUR NUM: ');
	read(a);
	writeln('HERE IS YOUR NUM: ', a);
	writeln('TESTING ADD: 2+2 = ', 2+2);
	writeln('TESTING SUB: 2-4 = ', 2-4);
	writeln('TESTING MUL: 2*2 = ', 2*2);
	writeln('TESTING DIV: 11 div 5 = ', 11 div 5);
	writeln('TESTING MOD: 11 div 5 = ', 11 mod 5);
	writeln('TESTING POLISH NOTATION: ((2+1)*(3+7)) div 10 = ', ((2+1)*(3+7)) div 10);
	a := 0;
	writeln('TESTING INT VAR ASSIGN: a = ', a);
	//str := 'z';
	//writeln('TESTING STRING VAR ASSIGN: str = ', str);
	arr[0] := 1;
	arr[1] := 2;
	arr[2] := 3;
	arr[3] := 4;
	arr[4] := 5;
	writeln('TESTING ARRAY: ', arr[0], arr[1], arr[2], arr[3], arr[4]);
	writeln('TESTING ARRAY ARITHMETICS: ', arr[0] + arr[1] + arr[2] + arr[3] + arr[4]+a);
	a:= a+4;
	writeln('a = ', a);
	
	write('TESTING IF/ELSE CONSTRUCTION: ');
	if (a > 100) then
	 begin
		writeln('VAR A IS BIGGER THAN 100');
	end
	else if (a = 100) then
	begin
		writeln('VAR A IS EQUAL 100');
	end
	else
	begin
		writeln('VAR A LESS THAN 100');
		if (arr[4] > 100) then
		 begin
			writeln('VAR ARR[4] IS BIGGER THAN 100');
		end
		else if (arr[4] = 100) then
		begin
			writeln('VAR ARR[4] IS EQUAL 100');
		end
		else
		begin
			writeln('VAR ARR[4] LESS THAN 100');
		end;
	end;
	//writeln ('Hello World!');
end.