
%% Schemat wykresu z zapisem do pliku
close all;
clear all;

%% Generowanie danych - punkty
t = 0:1:10;
xx = exp(t);

%% Obliczanie danych do wykresu - parametryzacja
ile_punktow = 100; %liczba punktow na wykresie
zakres_dodatkowy = 0.01; % procent wystaj¹cy poza punkty danych
[min, max, treshold] = przedzial(t); %rozmiar na x (szerokoœæ)
min = min - treshold*zakres_dodatkowy;
max = max + treshold*zakres_dodatkowy;
zmiennax = zeros(ile_punktow+1,1);
zmiennay = zeros(ile_punktow+1,1);
zmiennax = min:(max-min)/ile_punktow:max;
%twoja funkcja na y
zmiennay = exp(zmiennax); 

[a, rsq] = Rsquare(zmiennax, zmiennay);
display(['Wspolczynnik R^2 wynosi ', num2str(rsq)]);

figure();
set(gcf, 'Position', [100 100 600 400]);
plot(t,xx, 'g+', zmiennax,zmiennay,'r','LineWidth',1,'MarkerSize',10);
title('Wykres zaleznosci \Lambda od c^{1/2} dla CH_3COONa');
xlabel('c^{1/2}');
ylabel('\Lambda');
xlim([min, max]);

legend('Punkty', 'Krzywa' ,'Location', 'Best'); %, 'NorthWest'

%% Generowanie plików
saveas(gcf, 'wykres_jpg', 'jpg');
%latex
matlab2tikz('wykres_latex.tikz', 'height', '\figureheight', 'width', '\figurewidth');
