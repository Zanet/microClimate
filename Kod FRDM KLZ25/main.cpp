#include "mbed.h"
#include "DHT.h"        // temperature and humidity
#include "BMP180.h"     // pressture and temperature
#include "TextLCD.h"



#define ERROR_DEBUG         0       // zakomentuj, aby nie smiecilo errorami
#define LADNE_WYSWIETLANIE  0       // zakomentuj, aby wyswietlac ciag liczb
#define ALTITUDE            240     // wysokosc n.p.m. (Krakow: 188-383) (im nizsza tym niz nizsze cisnienie obliczy)
#define ANALOG_IN_FREQUENCY 0.5f    // co ile sekund odswierzac wartosc czujnikow
#define BMP180_FREQUENCY    0.5f
#define DHT11_FREQUENCY     2.0f
#define DISPLAY_FREQUENCY   1.0f    // co ile sekund wyswietlic dane

//DigitalOut myled(LED_GREEN);    // dioda
Serial pc(USBTX, USBRX);        //port szeregowy do komputera (ten sam kabel)
TextLCD lcd(PTC7,PTC0, PTC3, PTC4, PTC5, PTC6); // rs,  e, d4, d5, d6, d7
Timer timer;                    //czas od rozpoczęcia programu

// zmienne, które wymagają dalszej obróbki
AnalogIn MQ3_sensor(A0);    //alcohol sensor
AnalogIn MQ135_sensor(A1);  //gas sensor
AnalogIn Photoresistor(A2);
float alcohol_read, gas_read, light_read;

DigitalOut Buzzer(PTC12);

//zmienne obliczone przez czujnik
DHT dht11(D0,DHT11);
BMP180 bmp180(PTE0, PTE1);
float pressure, temp_BMP180, temp_DHT11, humidity;

// Tickers
Ticker analogIn_tick;
Ticker bmp180_tick;
Ticker dht11_tick;
Ticker serial_tick;

// read data from analog Inputs
void analogIn_read()
{
    alcohol_read = MQ3_sensor;
    gas_read = MQ135_sensor;
    light_read= Photoresistor;
}

void bmp180_read()
{
    if(!bmp180.ReadData(&temp_BMP180, &pressure)) {
#if defined(ERROR_DEBUG)
        pc.printf("\nError: BMP180 Sensor failed \r\n");
#endif
    }
}

void dht11_read()
{
    int s;
    s = dht11.readData();
    if(s!= ERROR_NONE) {
#if defined(ERROR_DEBUG)
        //pc.printf("\n\rError: DHT11, nr %d \r\n", s);
#endif
    } else {
        temp_DHT11 = dht11.ReadTemperature(CELCIUS);
        humidity = dht11.ReadHumidity();
    }
}

void serial_write()
{
#ifdef LADNE_WYSWIETLANIE
//    pc.printf("T_DHT11:%3.2f[C], ", temp_DHT11);
//    pc.printf("RH:%3.2f[%%], ", humidity);
//    pc.printf("p:%5.2f[hPa], ", pressure);
//    pc.printf("T_BMP180:%3.2f[C], ", temp_BMP180);
//    pc.printf("CO2:%3.2f[%%], ", 100*gas_read);
//    pc.printf("Alcohol:%3.2f[%%], ", 100*alcohol_read);
//    pc.printf("Time:%4.2f[s]", timer.read());
//    pc.printf("\r\n");
    pc.printf("t1:%3.2f;", temp_DHT11);
    pc.printf("hu:%3.2f;", humidity);
    pc.printf("pr:%5.2f;", pressure);
    pc.printf("t2:%3.2f;", temp_BMP180);
    pc.printf("gr:%3.2f;", 100*gas_read);
    pc.printf("al:%3.2f;", 100*alcohol_read);
    pc.printf("cn:%4.2f\n", timer.read());
#else
    pc.printf("%07.2f\n", temp_DHT11);
    pc.printf("%07.2f\n", humidity);
    pc.printf("%07.2f\n", pressure);
    pc.printf("%07.2f\n", temp_BMP180);
    pc.printf("%07.2f\n", 100*gas_read);
    pc.printf("%07.2f\n", 100*alcohol_read);
    pc.printf("%07.2f\n", timer.read());

#endif
}

void buzz(int count, float time)
{
    for(int i=0; i<count; i++) {
        Buzzer=!Buzzer;
        wait(time);
        Buzzer=!Buzzer;
        wait(time);
    }
}

int main()
{
    bmp180.Initialize(ALTITUDE, BMP180_OSS_NORMAL); //ALTITUDE - aktualna wysokosc n.p.m.

    analogIn_tick.attach(&analogIn_read, ANALOG_IN_FREQUENCY);
    bmp180_tick.attach(&bmp180_read, BMP180_FREQUENCY);
    dht11_tick.attach(&dht11_read, DHT11_FREQUENCY);
    serial_tick.attach(&serial_write, DISPLAY_FREQUENCY);

    pc.printf("Witaj w systemie!\r\n");

    //time_t startTime = time(NULL); //ta komenda NIE dziala (nie wiem czemu)

    timer.start();

    int event=1;
    // mruganie diodą :)
    // w programie musi być petla while(1), chociaz niczego nie musi robićc. Cala robote wykonuja za nas tickery.
    while (true) {
        lcd.cls();
        lcd.printf("oswietlenie:\n %5.5f",light_read);
        wait(1);

        if((light_read)>0.5) {
            if(event) {
                buzz(2,0.1);
                event=0;
            }
        } else
            event=1;
        /*
        lcd.printf("t1:%3.2f;", temp_DHT11);
        lcd.printf("hu:%3.2f;", humidity);
        lcd.printf("pr:%5.2f;", pressure);
        wait(5);
        lcd.cls();
        lcd.printf("t2:%3.2f;", temp_BMP180);
        lcd.printf("gr:%3.2f;", 100*gas_read);
        lcd.printf("al:%3.2f", 100*alcohol_read);
        wait(5);*/
        //wait(0.2f); // wait a small period of time
        //myled = !myled; // toggle a led
    }
    //timer.stop();


}
