var str : string;
var a, i : integer;
var arr:array[0..4] of integer;

function test0():integer;
begin
	writeln ('THIS IS FUNCTION WITH 0 ARGS!');
end;

function test1(v:integer):integer;
begin
	writeln ('THIS IS FUNCTION WITH 1 ARG: ', v);
end;

function test2(var1, var2:integer):integer;
begin
	write ('THIS IS FUNCTION WITH 2 ARGS: ');
	writeln (var1, ' AND ', var2);
end;

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
	
	for i := (5+5) downto arr[4] do begin
		writeln('i = ', i);
	end;
	write('TESTING REPEAT/UNTIL CONSTRUCTION');
	write(' VAR A NOW ', a);
	repeat
		a:=a+1;
	until (a < 50);
	writeln(' BUT AFTER REPUN IT BECAME ', a);
	
	write('TESTING WHILE CONSTRUCTION');
	write(' VAR A NOW ', a);
	while(a > 10) do begin
		a:=a-1;
	end;
	writeln(' BUT AFTER WHILE IT BECAME ', a);
	
	writeln('TESTING LOOP WITH ARRAY');
	i := 0;
	while(i < 5) do begin
		writeln('arr[', i, '] = ', arr[i]);
		i := i+1;
	end;
	
	writeln('TESTING FUNCTIONS');
	test0();
	test1(2);
	test2(i, arr[1]);
	
	//writeln ('Hello World!');
end.