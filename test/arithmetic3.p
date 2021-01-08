program arithmetic3
    var i,j :int;
    var res :int

    begin
        i := 1;
        j := 4;
        res := (j + i) * ((j+2) / (i+3)) - j^2;
        write res;
        j := j^(-2);
        write j;
        i := 0;
        write i^7;
        i := 4;
        write i^0;
    end