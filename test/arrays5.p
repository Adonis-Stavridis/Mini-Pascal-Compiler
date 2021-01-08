program arrays5
	var tab: array[0..3] of bool;
	var cnt:int;
	begin
		cnt := 0;
		while cnt <= 3 do
		begin
			if (cnt = 0 or cnt = 2) then
				tab[cnt] := true
			else
				tab[cnt] := false;
			cnt := cnt + 1;
		end;

		write "---";

		cnt := 0;
		while cnt <= 3 do
		begin
			write tab[cnt];
			cnt := cnt + 1;
		end
	end
