%% Plotowanie dla pliku czujniki.slx
figure();

subplot(321)
plot(tout,simout(:,1));
title('temperature DHT11');
subplot(322)
plot(tout,simout(:,2));
title('humidity');
subplot(323)
plot(tout,simout(:,3));
title('pressure');
subplot(324)
plot(tout,simout(:,4));
title('temperature BMP180');
subplot(325)
plot(tout,simout(:,5));
title('gas');
subplot(326)
plot(tout,simout(:,6));
title('alcohol');
