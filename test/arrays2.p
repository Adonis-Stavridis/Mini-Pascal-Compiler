program arrays2
	var a: array[-2..2] of int;
	var b: array[0..3] of int;
	var c: array[4..8] of int;
	var d: array[-2..8, -1..3] of int;
	var x,y,z:int
	begin
		x := -2;
		y := 0;
		z := 4;
		a[x] := 99;
		b[y] := 98;
		c[z] := a[x] - b[y];
		write c[z];
		d[z,y] := 8;
		write d[z,y];
	end
