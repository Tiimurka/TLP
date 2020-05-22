var str : string;
var a : integer;

begin
	write('GIVE ME YOUR STRING: ');
	read(str);
	writeln('HERE IS YOUR STRING: ', str);
	writeln('TESTING ADD: 2+2 = ', 2+2);
	writeln('TESTING SUB: 2-4 = ', 2-4);
	writeln('TESTING MUL: 2*2 = ', 2*2);
	writeln('TESTING DIV: 11 div 5 = ', 11 div 5);
	writeln('TESTING MOD: 11 div 5 = ', 11 mod 5);
	writeln('TESTING POLISH NOTATION: ((2+1)*(3+7)) div 10 = ', ((2+1)*(3+7)) div 10);
	a := 161+161;
	writeln('TESTING INT VAR ASSIGN: a = ', a);
	str := 'zaz';
	writeln('TESTING STRING VAR ASSIGN: str = ', str);
end.