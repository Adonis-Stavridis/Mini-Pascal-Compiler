program arrays1
	var val : array[-2..2, 0..3] of int;
	var a,b,c,d,cnt,sum :int;
	begin
		a := -2;
		b := 2;
		d := 3;
		cnt := 0;
		while (a < b) do
		begin
			c := 0;
			while (c < d) do
			begin
				val[a,c] := cnt;
				write cnt;
				cnt := cnt + 1;
				c := c + 1;
			end;
			a := a + 1;
		end;

		write "------";

		a := -2;
		while (a < b) do
		begin
			c := 0;
			while (c < d) do
			begin
				write val[a,c];
				c := c + 1;
			end;
			a := a + 1;
		end;

		write "------";

		sum := 0;
		a := -2;
		while (a < b) do
		begin
			c := 0;
			while (c < d) do
			begin
				sum := sum + val[a,c];
				c := c + 1;
			end;
			a := a + 1;
		end;

		write "sum:";
		write sum;
	end
