program boolean
	var i,j,max :bool;
	var a,b,d :int;
	var c:bool;
	begin
        a := 4; write a;
		b := a + 1; write b;
		a := a * b; write a;
		c := a = b; write c;
		c := a <> b; write c;
		c := b >= a; write c;
		c := b <= a; write c;
		c := b > a; write c;
		c := b < a; write c;
		d := a + b; write d;
		d := d - 2; write d;
    end