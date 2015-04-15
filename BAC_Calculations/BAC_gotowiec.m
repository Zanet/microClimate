%% Schemat wykresu z zapisem do pliku
close all;
clear all;

%% Dane ze strony: http://nootropicdesign.com/projectlab/2010/09/17/arduino-breathalyzer/
% Uzy³em ich, poniewa¿ podpiêcie i oporniki s¹ takie same, ale mo¿emy
% zrobiæ w³asne dane!
load('BAC_Arduinodata.mat');

%% Generowanie danych - punkty
x = (1/1023)*BAC(:,2);          %oczytany wynik z Arduino (przerabiamy na (0-1)
y = BAC(:,1);                    % obliczony BAC

%% Obliczanie danych do wykresu - parametryzacja
ile_punktow = 100; %liczba punktow na wykresie
zakres_dodatkowy = 0.1; % procent wystaj¹cy poza punkty danych
[min, max, treshold] = przedzial(x); %rozmiar na x (szerokoœæ)
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
title(['Wykres BAC o R^2 = ', num2str(rsq),'; y = ', num2str(a(1)), 'x + ',num2str(a(2)) ]);
xlabel('Odczyt');
ylabel('BAC, %');
xlim([min, max]);

legend('Punkty', 'Krzywa' ,'Location', 'Best'); %, 'NorthWest'

%% Generowanie plików
saveas(gcf, 'wykres_jpg', 'jpg');
