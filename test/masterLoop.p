program masterLoop
    var i,j:int;
    begin
        i := 0;
        j := 1;
        while i < 4 do
        begin
            j := 1;
            while j < 4 do
            begin
                write i;
                if (i = 2) then
                begin
                    if (j=1) then 
                        write j 
                    else
                        write i;
                end
                else
                    write i+1;
                j := j + 1;
            end;
        i := i + 1;
        end
    end