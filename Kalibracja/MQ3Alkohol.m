%% Schemat wykresu z zapisem do pliku
close all;
clear all;
clc

%% Nasze w³asne dane
% Blood alcohol content BAC
%BAC = [0.000001, 0.0000001, 2, 3];
% odczyty z MQ3
%Vo = [0.1, 0.22, 0.34, 0.4];

% dane znalezionych pomiarow dla Arduino
load('BAC_Arduinodata.mat');
Vo = BAC(:,2)/1024;
BAC = BAC(:,1);

%% Trochê teorii
% BAC mozemy obliczyc analogicznie do czujnika MQ135 (to tez jest
% electrochemical gas sensor)
% BAC = a*(Rs/Ro)^b
% Dodajemy ropornik Ro
Ro = 10000; %%10kOhm
% Mierzymy napiecie na Ro (dziwne, ale na to wychodzi)

%z instrukcji:
% Ro: sensor resistance at 0.4mg/L of
% Alcohol in air at 33%RH and 20 ?
% Rs: sensor resistance at 0.4mg/L of
% Alcohol at different temperatures
% and humidities.


% Z dzielnika Vo = Vcc * (Ro/(Rs+Ro))
Vcc = 5; %5V - mo¿na zmienic na 3,3!!!
Rs = Ro*(Vcc./(Vo.*3.3)-1);

% Przeksztalcajac rownanie na ppm dostajemy rownanie liniowe:
% ln(ppm/a)  = b* ln(Rs/Ro)
% ln(ppm) = b*ln(Rs/Ro) + ln(a)
% Rs/Ro = (Vcc./(Vo.*3.3)-1)
stosunek_Rs_Ro = (Vcc./(Vo.*3.3)-1);
kalibracja_temp_hum_MQ3;
stosunek_Rs_Ro=stosunek_Rs_Ro./(p2(1)*T.^3+p2(2)*T.^2+p2(3)*T + p2(4));


%% Generowanie danych - punkty
y = log(BAC);
x = log(stosunek_Rs_Ro);

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

%% Wykres liniowy
figure();
set(gcf, 'Position', [100 100 600 400]);
plot(x,y, 'g+', zmiennax,zmiennay,'r','LineWidth',1,'MarkerSize',10);
title(['Wykres liniowy, R^2 = ', num2str(rsq),'; y = ', num2str(a(1)), 'x + ',num2str(a(2)) ]);
xlabel('log(Rs/Ro)');
ylabel('log(BAC)');
xlim([min, max]);

legend('Punkty', 'Krzywa' ,'Location', 'Best'); %, 'NorthWest'

%% Wykres prawdziwy
A = exp(a(2));
B = a(1);
ile_punktow = 100; %liczba punktow na wykresie
zakres_dodatkowy = 0.1; % procent wystaj¹cy poza punkty danych
[min, max, treshold] = przedzial(Vo);
min = min - treshold*zakres_dodatkowy;
max = max + treshold*zakres_dodatkowy;
zmiennax = min:(max-min)/ile_punktow:max;
zmiennay = A*((Vcc./(zmiennax*3.3) -1)./ (p1(1)*T.^3+p1(2)*T.^2+p1(3)*T + p1(4))).^B;

figure();
set(gcf, 'Position', [100 100 600 400]);
plot(Vo,BAC, 'g+', zmiennax,zmiennay,'r','LineWidth',1,'MarkerSize',10);
title(['Wykres BAC(Vo);  ppm = ', num2str(A), '* (',num2str(Vcc),'/(3.3x) - 1) ^{',num2str(B),'}' ]);
xlabel('Vo');
ylabel('BAC,%');
xlim([min, max]);

legend('Dane', 'Krzywa dopasowana' ,'Location', 'Best'); %, 'NorthWest'

display(['BAC = A*(Vcc/(Vo*3.3)-1)^B ']);
display(['A = ', num2str(A)]);
display(['B = ', num2str(B)]);

%% Generowanie plików
% saveas(gcf, 'wykres_jpg', 'jpg');
