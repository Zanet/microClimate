%% Schemat wykresu z zapisem do pliku
close all;
clear all;
clc

%% Nasze w³asne dane
% stê¿enia dwutlenku wegla
ppm = [450, 2000];
% odczyty z MQ135
Vo = [0.12, 0.35];
T = [30 30];

%% Trochê teorii
% Ste¿enie dwutlenku wegla w czujniku mo¿e byæ opisan erównaniem potêgowym
% ppm = a*(Rs/Ro)^b
% problem jest taki, ¿e nie znamy Ro, bo Chiñczyki go nie podali. Zak³adam
% go jako:
Ro = 22000; %%22kOhm
% Mierzymy napiecie na Ro (dziwne, ale na to wychodzi)

% Z dzielnika Vo = Vcc * (Ro/(Rs+Ro))
Vcc = 5; %5V - mo¿na zmienic na 3,3!!!
Rs = Ro*(Vcc./(Vo.*3.3)-1);

% Przeksztalcajac rownanie na ppm dostajemy rownanie liniowe:
% ln(ppm/a)  = b* ln(Rs/Ro)
% ln(ppm) = b*ln(Rs/Ro) + ln(a)
% ppm = a*exp(b*ln(5/(odczyt*3.3)-1))
% Rs/Ro = (Vcc./(Vo.*3.3)-1)
stosunek_Rs_Ro = (Vcc./(Vo.*3.3)-1);
kalibracja_temp_hum_MQ135;
stosunek_Rs_Ro=stosunek_Rs_Ro./(p1(1)*T.^3+p1(2)*T.^2+p1(3)*T + p1(4));

%% Generowanie danych - punkty
y = log(ppm);
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
ylabel('log(ppm)');
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
T=30;
zmiennay = A*((Vcc./(zmiennax*3.3) -1)/(p1(1)*T.^3+p1(2)*T.^2+p1(3)*T + p1(4))).^B;

figure();
set(gcf, 'Position', [100 100 600 400]);
plot(Vo,ppm, 'g+', zmiennax,zmiennay,'r','LineWidth',1,'MarkerSize',10);
title(['Wykres ppm(Vo);  ppm = ', num2str(A), '* (',num2str(Vcc),'/(3.3x) - 1) ^{',num2str(B),'}' ]);
xlabel('Vo');
ylabel('ppm');
xlim([min, max]);

legend('Dane', 'Krzywa dopasowana' ,'Location', 'Best'); %, 'NorthWest'

display(['ppm = A*(Vcc/(Vo*3.3)-1)^B ']);
display(['A = ', num2str(A)]);
display(['B = ', num2str(B)]);

%% Generowanie plików
% saveas(gcf, 'wykres_jpg', 'jpg');
