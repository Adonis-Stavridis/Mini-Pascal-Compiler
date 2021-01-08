program ifelse
	var a,b,c:bool;
	begin
		a := false;
		b := false;
		c := false;
		if (a = b) then
		begin
			write 1;
			if (a = c) then
				write 2
			else
				write 3;
		end
		else
			write 4;
		write 5;
	end
