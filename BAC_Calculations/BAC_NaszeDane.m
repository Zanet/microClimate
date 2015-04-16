%% Schemat wykresu z zapisem do pliku
close all;
clear all;

%% Nasze w�asne dane
% pierwsza kolumna - wynik z Freescale
% druga kolumna  - obliczony BAC w %
% znalaz�em stron�:
% http://celtickane.com/projects/blood-alcohol-content-bac-calculator/ 
% (ma mas� w kg)
BAC = [ 
    0.7564 3.5;
    0.7664 3.6;
    ];

%% Generowanie danych - punkty
x = BAC(:,2);           % obliczony BAC
y = BAC(:,1);           %oczytany wynik z Freescale

%% Obliczanie danych do wykresu - parametryzacja
ile_punktow = 100; %liczba punktow na wykresie
zakres_dodatkowy = 0.1; % procent wystaj�cy poza punkty danych
[min, max, treshold] = przedzial(x); %rozmiar na x (szeroko��)
min = min - treshold*zakres_dodatkowy;
max = max + treshold*zakres_dodatkowy;
zmiennax = zeros(ile_punktow+1,1);
zmiennay = zeros(ile_punktow+1,1);
zmiennax = min:(max-min)/ile_punktow:max;
%twoja funkcja na y
[a, rsq] = Rsquare(x, y);
zmiennay = polyval(a,zmiennax);

[a, rsq] = Rsquare(x, y);
display(['Wspolczynnik R^2 wynosi ', num2str(rsq)]);
display(['Wielomian: y = ', num2str(a(1)), 'x + ',num2str(a(2)) ]);

figure();
set(gcf, 'Position', [100 100 600 400]);
plot(x,y, 'g+', zmiennax,zmiennay,'r','LineWidth',1,'MarkerSize',10);
title(['Wykres BAC, R^2 = ', num2str(rsq),'; y = ', num2str(a(1)), 'x + ',num2str(a(2)) ]);
xlabel('Odczyt');
ylabel('BAC, %');
xlim([min, max]);

legend('Punkty', 'Krzywa' ,'Location', 'Best'); %, 'NorthWest'

%% Generowanie plik�w
saveas(gcf, 'wykres_jpg', 'jpg');