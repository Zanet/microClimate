% wstêpne sprawdzenie zawartoœci danych odebranych z portu szeregowego 
%o wymiarach [1 1] 

literki=char(simout);
tresc=' ';
for i=1:1:length(simout),
tresc=strcat(tresc, simout(i));
end
