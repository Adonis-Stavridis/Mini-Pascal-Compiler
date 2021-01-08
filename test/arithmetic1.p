program arithmetic1
	var i,j,max :int;

	begin
		i := 1;
		j := 4;
		max := i + j;
		max := max * 2;
		max := max / 3;
		max := max - (-3);
		max := 2 ^ 4;
		i := 3 ^ 2;

		max := -1;
		write max;
		max := -max;
		write max;
	end
