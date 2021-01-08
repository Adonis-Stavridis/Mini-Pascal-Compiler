program arrays4
	var val : array[-2..2, 0..3, 1..4, 0..2] of int;
	var a,b,c,d :int
	begin
		a:=-2;
		b:= 0;
		c:= 1;
		d:= 0;
		val[a,b,c,d] := 99;
		write val[a,b,c,d];
		val[a+2,b+2,c+3,d+1] := 98;
		write val[a+2,b+2,c+3,d+1];
		val[a+3,b,c,d] := 18;
		write val[a*100,b,c*100,d];
		write val[(a+1)^2,b,c,d];
	end
