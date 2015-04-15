function [min, max, treshold] = przedzial(x)
% oblicza przedzia� w jakim znajduj� si� dane
%wektory musz� by� poziome (chyba)

min = 100000;
max = -100000;

rozmiar = size(x,1);
if size(x,2)>rozmiar
    rozmiar = size(x,2);
end;


for i = 1:rozmiar
    if min>x(i)
        min = x(i);
    end;
    
    if max<x(i)
        max = x(i);
    end;
end;

treshold = (min+max)/2;
