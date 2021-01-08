program arrays3
	var val : array[-2..2, 0..3] of int;
	var t : array[0..3,1..2] of int;
	var t1, t2 : array[0..3] of int;
	var a,b:int
	begin
		a:= 1;
		b:= 3;

		t1[a] := 10;
		write t1[a];

		t2[b] := 15;
		write t2[b];

		t1[t2[b] - t1[a] - 3] := 99;
		write t1[t2[b]];

		t1[t2[b-a]] := 500;
		write t1[t2[b-a]];

		t[b,a] := t1[a] + t1[t2[b-a]];
		write t[b,a];
	end
