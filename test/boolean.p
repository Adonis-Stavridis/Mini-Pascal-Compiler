program boolean
	var i,j,max :bool;
	var a,b,d :int;
	var c:bool;
	begin
		i := true; write i;
		j := false; write j;
		max := i and j; write max;
		max := i or j; write max;
		max := i xor j; write max;
		max := not i; write max;
	end
