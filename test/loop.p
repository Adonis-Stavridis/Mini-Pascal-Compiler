program loop
	var i:int;
	begin
		i := 0;
		while i < 4 do
		begin
			write i;
			if (i = 2) then write "pair";
			i := i + 1;
		end
	end
