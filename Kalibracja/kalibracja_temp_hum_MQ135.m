% RsRo33=[1.37 1.25 1.14 1.07 0.98 0.89 0.83 0.8 0.79];
% RsRo85=[1.6 1.45 1.35 1.23 1.12 1 0.93 0.89 0.87];
RsRo33=[1.7 1.28 1 0.82];
RsRo85=[1.58 1.15 0.91 0.82];
temp=[-10 5 20 50];
plot(temp, RsRo33,temp, RsRo85);

N=3;
p1=polyfit(temp, RsRo33, N) %wspó³czynniki wielomianu
x1=-10:0.1:50;
y1=0;


for m=1:N+1
y1=y1+p1(m)*x1.^(N-m+1);
end;

p2=polyfit(temp, RsRo85, N) %wspó³czynniki wielomianu
x2=-10:0.1:50;
y2=0;


for m=1:N+1
y2=y2+p2(m)*x2.^(N-m+1);
end;
close all;

figure();
%wykres punktów i funkcji

plot(temp, RsRo33, 'or', x1, y1,'r',temp, RsRo85, 'og', x2, y2,'g')
hold on;
T_BMP180=-10:5:50;
plot(T_BMP180,-0.00000049382716.*T_BMP180.*T_BMP180.*T_BMP180+   0.0003185185185185.*T_BMP180.*T_BMP180-0.02637037037037.*T_BMP180+1.40395061728395);


