function [p, rsq] = Rsquare(x,y)
    %Wyznacza wska�nik R^2 oraz aproksymacj� danych zawartych w wektorach x
    %i y

    %n -- rz�d aproksymacji 1=liniowa
    n = 1;
    p = polyfit(x,y,n);
    
    yfit = polyval(p,x); %funkcja fittuj�ca
    
    yresid = y - yfit;
    SSresid = sum(yresid.^2);
    SStotal = (length(y)-1) * var(y);
    rsq = 1 - SSresid/SStotal;

end