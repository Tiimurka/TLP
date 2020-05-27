var arr:array[0..9] of integer;
var min, i :integer;
	
//Поиск мин элемента в массиве
	
begin
    writeln('введите 10 элементов массива');
    min:=10000;
	i := 0;
    while(i < 10) do begin
        readln(arr[i]);
        if (min > arr[i]) then
		begin
			min:=arr[i];
		end;
		i:= i+1;
    end;
    writeln( 'Мин. элемент массива = ', min );
end.
