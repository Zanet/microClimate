#include "mbed.h"
#include "DHT.h"        // temperature and humidity
#include "BMP180.h"     // pressture and temperature
#include "TextLCD.h"

#include "ArduinoAPI.h" // daje warninga, ale jest niezbedne!
#include "ESP8266.h"    // WiFi

//#define ERROR_DEBUG         (0)       // zakomentuj, aby nie smiecilo errorami
#define LADNE_WYSWIETLANIE  (0)       // zakomentuj, aby wyswietlac ciag liczb
#define ALTITUDE            (240)     // wysokosc n.p.m. (Krakow: 188-383) (im nizsza tym niz nizsze cisnienie obliczy)
#define ANALOG_IN_FREQUENCY (0.5f)    // co ile sekund odswierzac wartosc czujnikow
#define BMP180_FREQUENCY    (0.5f)
#define DHT11_FREQUENCY     (2.0f)
#define DISPLAY_FREQUENCY   (1.0f)    // co ile sekund wyswietlic dane
#define GENERAL_FREQUENCY   (0.1f)
#define DIODY_ON            (0.95f)
#define DIODY_OFF           (1.0f)
#define AVERAGE_WIDTH       (5)       //szerokosc sredniej pelzajacej

//#define PASS                "7Px9hmY2" //haslo do Wifi
//#define SSID                "Pressburg"// SSID wifi (jak jest dlugie to nie dziala czasem
#define PASS                "XNTIWCJI" //haslo do Wifi
#define SSID                "UPC7339018"// SSID wifi (jak jest dlugie to nie dziala czasem
#define PORT                "11"        //PORT serwera jako string, Nasz serwer na porcie 21

#define WIFI                (0) // zakomentuj, aby wylaczyc WiFi



PwmOut red(PTE20);    // diody
PwmOut green(PTE21);
PwmOut blue(PTE29);


Serial pc(USBTX, USBRX);        //port szeregowy do komputera (ten sam kabel)

ArduinoSerial uart(PTE22, PTE23);
 
ESP8266 esp(uart);                     // klasa WiFi

TextLCD lcd(PTC7,PTC0, PTC3, PTC4, PTC5, PTC6, TextLCD::LCD20x4); // rs,  e, d4, d5, d6, d7
Timer timer;                    //czas od rozpoczęcia programu

// zmienne, które wymagają dalszej obróbki
AnalogIn MQ3_sensor(A0);    //alcohol sensor
AnalogIn MQ135_sensor(A1);  //gas sensor
AnalogIn luminance_sensor(A2);  //photo sensor
float alcohol_read, gas_read, luminance_read;
float alcohol_bus[AVERAGE_WIDTH], gas_bus[AVERAGE_WIDTH], luminance_bus[AVERAGE_WIDTH];
float alcohol_sum = 0, gas_sum = 0, luminance_sum = 0;
int analog_iterator = 0;

DigitalOut Buzzer(PTB8);
DigitalIn Switch(PTB9);

//zmienne obliczone przez czujnik
DHT dht11(D0,DHT11);
BMP180 bmp180(PTE0, PTE1);
float pressure, T_BMP180, T_DHT11, humidity;
float pressure_bus[AVERAGE_WIDTH], T_BMP180_bus[AVERAGE_WIDTH], T_DHT11_bus[AVERAGE_WIDTH], humidity_bus[AVERAGE_WIDTH];
float pressure_sum = 0, T_BMP180_sum = 0, T_DHT11_sum = 0, humidity_sum = 0;
int BMP180_iterator = 0, DHT11_iterator = 0;

// Tickers
Ticker analogIn_tick;
Ticker bmp180_tick;
Ticker dht11_tick;
Ticker serial_tick;
Ticker WiFi_tick;

// read data from analog Inputs
void analogIn_read()
{
    //Srednia pelzajaca
    alcohol_sum -= alcohol_bus[analog_iterator];
    alcohol_bus[analog_iterator] = MQ3_sensor;
    alcohol_sum += alcohol_bus[analog_iterator];
    alcohol_read = alcohol_sum/AVERAGE_WIDTH;

    gas_sum -= gas_bus[analog_iterator];
    gas_bus[analog_iterator] = MQ135_sensor;
    gas_sum += gas_bus[analog_iterator];
    gas_read = gas_sum/AVERAGE_WIDTH;

    luminance_sum -= luminance_bus[analog_iterator];
    luminance_bus[analog_iterator] = luminance_sensor;
    luminance_sum += luminance_bus[analog_iterator];
    luminance_read = luminance_sum/AVERAGE_WIDTH;

    analog_iterator++;
    if (analog_iterator == (AVERAGE_WIDTH))
        analog_iterator = 0;
}

void bmp180_read()
{
    float temp_pressure, temp_temperature;
    if(!bmp180.ReadData(&temp_temperature, &temp_pressure)) {
#if defined(ERROR_DEBUG)
        pc.printf("\nError: BMP180 Sensor failed \r\n");
#endif
    }

    //Srednia pelzajaca
    T_BMP180_sum -= T_BMP180_bus[BMP180_iterator];
    T_BMP180_bus[BMP180_iterator] = temp_temperature;
    T_BMP180_sum += T_BMP180_bus[BMP180_iterator];
    T_BMP180 = T_BMP180_sum/AVERAGE_WIDTH;

    pressure_sum -= pressure_bus[BMP180_iterator];
    pressure_bus[BMP180_iterator] = temp_pressure;
    pressure_sum += pressure_bus[BMP180_iterator];
    pressure = pressure_sum/AVERAGE_WIDTH;

    BMP180_iterator++;
    if (BMP180_iterator == (AVERAGE_WIDTH))
        BMP180_iterator = 0;
}

void dht11_read()
{
    int s;
    s = dht11.readData();
    if(s!= ERROR_NONE) {
#if defined(ERROR_DEBUG)
        pc.printf("\n\rError: DHT11, nr %d \r\n", s);
#endif
    } else {
        //Srednia pelzajaca
        T_DHT11_sum -= T_DHT11_bus[DHT11_iterator];
        T_DHT11_bus[DHT11_iterator] = dht11.ReadTemperature(CELCIUS);
        T_DHT11_sum += T_DHT11_bus[DHT11_iterator];
        T_DHT11 = T_DHT11_sum/AVERAGE_WIDTH;

        humidity_sum -= humidity_bus[DHT11_iterator];
        humidity_bus[DHT11_iterator] = dht11.ReadHumidity();
        humidity_sum += humidity_bus[DHT11_iterator];
        humidity = humidity_sum/AVERAGE_WIDTH;

        DHT11_iterator++;
        if (DHT11_iterator == (AVERAGE_WIDTH))
            DHT11_iterator = 0;
    }
}

#define CO2_A (9751.4068)
#define CO2_B (-1.1928)

//oblicza zawartosc CO2 w ppm  - pomiar BARDZO przybliżony
float calculate_ppm(float u)
{
    // ppm = a*exp(b*ln(5/(odczyt*3.3)-1))
    float RsRo=5/(u*3.3) - 1; //kalibracja względem temperatury
    RsRo=RsRo/(-0.00000173610120258230*T_BMP180*T_BMP180*T_BMP180+   0.0003097099388306526*T_BMP180*T_BMP180-0.018366870747730*T_BMP180+1.25756402180681);
    return (CO2_A *exp(CO2_B*log(RsRo)));
}

#define ALCOHOL_A (3.08)
#define ALCOHOL_B (-0.924)
// oblicza promile we krwi - korelacja danych wynosi 0.3, więc mało.
float calculate_BAC(float u)
{
    /*float RsRo=5/(u*3.3) - 1; //kalibracja względem temperatury
    RsRo=RsRo/( -0.00000049382716*T_BMP180*T_BMP180*T_BMP180+   0.0003185185185185*T_BMP180*T_BMP180-0.02637037037037*T_BMP180+1.40395061728395);
    float temp = 10*(ALCOHOL_A *exp(ALCOHOL_B*log(RsRo)));
    */
    float temp = ALCOHOL_A* u+ALCOHOL_B;

    if (temp<0)
        temp = 0;

    return temp;
}

void serial_write()
{
#ifdef LADNE_WYSWIETLANIE

    pc.printf("lm:%3.2f;", luminance_read*100);
    pc.printf("hu:%3.2f;", 40.10);
    pc.printf("pr:%5.2f;", pressure);
    pc.printf("t2:%3.2f;", T_BMP180);
    pc.printf("gr:%3.2f;", calculate_ppm(gas_read));
    pc.printf("al:%3.2f;", calculate_BAC(alcohol_read));
    pc.printf("cn:%4.2f\n", timer.read());
#else
    pc.printf("%07.2f\n", luminance_read*100);
    pc.printf("%07.2f\n", humidity);
    pc.printf("%07.2f\n", pressure);
    pc.printf("%07.2f\n", T_BMP180);
    pc.printf("%07.2f\n", calculate_ppm(gas_read));
    pc.printf("%07.2f\n", calculate_BAC(alcohol_read));
    pc.printf("%07.2f\n", timer.read());

#endif
}

// Zainicjowanie tablicy sredniej pelzajacej
int initialize()
{
    int i = 0;
    for (i = 0; i<AVERAGE_WIDTH; i++) {
        analogIn_read();
        bmp180_read();
        dht11_read();
        wait(0.2f);
    }
    return 0;
}

#ifdef WIFI
void receiveIP(String target, uint32_t timeout, int &q,int &w,int &e,int &r)
{
    uart.flush();
    uart.println("AT+CIFSR");
    
    String  data;
    char a;
    unsigned long start = millis();
    while (millis() - start < timeout) {
        while(uart.available() > 0) {
            a = uart.readChr();
            data +=a;
            //pc.printf("%c",a);
            }
        if (data.indexOf(target) != -1) {
            break;
        }   
        }   
    //pc.printf("Wypisz cale otrzymane dane: \r\n %s \r\n",data);
    
    String IP = data.substring(data.indexOf("+CIFSR:STAIP"), data.indexOf("+CIFSR:STAMAC"));
    //pc.printf("IP String: %s \r\n",IP);
    String num;
    int p = 14;
    num = IP.substring(p+0, p+3);
    q = num.toInt();
    num = IP.substring(p+4,p+ 7);
    w = num.toInt();    
    num = IP.substring(p+8,p+ 11);
    e = num.toInt();    
    num = IP.substring(p+12,p+ 15);
    r = num.toInt();
    //pc.printf("IP num: %d %d %d %d \r\n",q,w,e,r);
}

void senddata(uint32_t timeout,uint8_t channel, String dane)
{
    String komenda = "AT+CIPSEND=";
    komenda+=channel;
    komenda+=","; 
    komenda+= (dane.length());
    
    uart.flush();
    esp.clear_serial();
    uart.println(komenda);
    
    String  data;
    char a;
    unsigned long start;
    start = millis();
    while (millis() - start < 1000) { 
        while(uart.available() > 0) {
            a = uart.readChr();
            data +=a;
        }
        if ((data.indexOf(">") != -1)){
          break;
        }
    }
    //pc.printf("Karetka gotowa\r\n");
        
    uart.println(dane);
    
    data="";
    start = millis();
    while (millis() - start < 1000) {
        while(uart.available() > 0) {
            a = uart.readChr();
            data +=a;
        }
        if ((data.indexOf("OK") != -1)){
          //pc.printf("Dane wyslane\r\n");
          break;
        }
    }  
}

void WiFi_write()
{
    String dane;
    dane = "";
    dane += " D:";
    dane += (int)(100*T_DHT11);
    dane += " H:";
    dane += (int)(100*humidity);
    dane += " P:";
    dane += (int)(100*pressure);
    dane += " B:";
    dane += (int)(100*T_BMP180);
    dane += " G:";
    dane += (int)(100*100*gas_read);
    dane += " A:";
    dane += (int)(100*100*alcohol_read);
    dane += " T:";
    dane += (int)(100*timer.read());
    dane += " \r\n";
    
    
    senddata(1000,3,dane);
//    senddata(1000,3,'D',T_DHT11);
//    senddata(1000,3,'H',humidity);
//    senddata(1000,3,'P',pressure);
//    senddata(1000,3,'B',T_BMP180);
//    senddata(1000,3,'G',100*gas_read);
//    senddata(1000,3,'A',100*alcohol_read);
//    senddata(1000,3,'T',timer.read());
}

#endif

void buzz(int count, float time)
{
    for(int i=0; i<count; i++) {
        Buzzer=!Buzzer;
        wait(time);
        Buzzer=!Buzzer;
        wait(time);
    }
}

void alert(char* text, float time)
{
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("%s", text);
    wait(time);
    lcd.cls();
}

int main()
{
    pc.printf("Witaj w systemie uClimate!\r\n");
    pc.printf("Inicjalizacja zmiennych...\r\n");

    bmp180.Initialize(ALTITUDE, BMP180_OSS_NORMAL); //ALTITUDE - aktualna wysokosc n.p.m.
    initialize();

    pc.printf("Podlaczenie zegarow...\r\n");
    analogIn_tick.attach(&analogIn_read, ANALOG_IN_FREQUENCY);
    bmp180_tick.attach(&bmp180_read, BMP180_FREQUENCY);
    dht11_tick.attach(&dht11_read, DHT11_FREQUENCY);
    serial_tick.attach(&serial_write, DISPLAY_FREQUENCY);

    red.period(0.001f);
    green.period(0.001f);
    blue.period(0.001f);

    timer.start();

    int event_alcohol=1;
    int event_light=1;
    int event_gas=1;
    int Switch_Old=0;
    int Switch_New=0;
    float w=GENERAL_FREQUENCY;
    int MAX_DIV =(DISPLAY_FREQUENCY/GENERAL_FREQUENCY-1);
    int it=0;
    int div=0;

#ifdef WIFI
    pc.printf("Laczenie z siecia (instrukcje wyswietlaja sie po drodze)...\r\n");
    //zmienna pozytywnego zaakonczenia   
    bool happy_end = false;
    
    int status = esp.getIPStatus();
    pc.printf("\t Status Wifi: %d\r\n", status);
    
    pc.printf("1. Restartowanie plytki ESP8266...\r\n");
    happy_end = esp.restart();
    if (happy_end==true)
        pc.printf("\t Reset udany\r\n");
    else
        pc.printf("\t Reset NIEudany\r\n");
    
    pc.printf("2. Sprawdzenie, czy plytka odpowiada...\r\n");   
    happy_end = esp.kick();
    if (happy_end==true)
        pc.printf("\t Plytka zyje! \r\n");
    else
        pc.printf("\t Plytka NIEzyje! \r\n"); 
 
    pc.printf("3. Przelaczenie sie na tryb serwera + klienta\r\n");       
    happy_end = esp.setOprToStationSoftAP();
    if (happy_end==true)
        pc.printf("\t MODE: Serwer + Klient \r\n");
    else
        pc.printf("\t MODE ERR \r\n");     
    
    pc.printf("4. Laczenie z siecia %s (max. 50 prob)\r\n", SSID);
    //blue = 0;   // laczenie z wifi
    for (int i = 1;i<=50;i++){      
        delay(1000);
        esp.clear_serial();
        happy_end = esp.joinAP((String)SSID, (String)PASS);            
        if (happy_end==true){
            pc.printf("\t Polaczono z siecia %s w probie %d\r\n", SSID, i);
            blue = 1;       //polaczono
            break;
            }
        else{
            pc.printf("\t Niepolaczono, ponawianie proba %d \r\n", i); 
            } 
    }  
    
    pc.printf("\t Status Wifi: %d\r\n", esp.getIPStatus());
    
    pc.printf("5. IP Serwera + Multimode \r\n"); 
    int Serwer_1, Serwer_2, Serwer_3, Serwer_4; // IP serwera
    receiveIP("OK",10000,Serwer_1, Serwer_2, Serwer_3, Serwer_4 );   
    pc.printf("\tIP serwera to: %d.%d.%d.%d \r\n", Serwer_1, Serwer_2, Serwer_3, Serwer_4);  
    
    
    happy_end = esp.enableMUX();
    if (happy_end==true)
        pc.printf("\tMultimode ON\r\n");
    else
        pc.printf("\tMultimode ERR \r\n"); 

    int Client_1 = 192, Client_2=168, Client_3=123, Client_4=100;
    String addr = (String)(Client_1);
    addr+=".";
    addr += (String)Client_2;
    addr+=".";
    addr += (String)Client_3;
    addr+=".";
    addr += (String)Client_4;
    pc.printf("\tIP klienta to: %s \r\n", addr);

    pc.printf("6. Rozpoczynanie polaczenia z klientem.\r\n");    
    //blue = 0; red = 0; // fiolet - laczenie z zewnetrznym serwerm
    for (int i = 1;i<=5;i++){      
        delay(10000); 
        esp.clear_serial();
        happy_end = esp.createTCP((uint8_t)3, addr, (uint8_t) 21); 
        if (happy_end==true){
            pc.printf("\tTCP ON, IP:%s, port: %d\r\n", addr, (int)21);
            break;
            }
        else
            pc.printf("\tTCP ERR, proba nr:%d\r\n",i); 
    }    
#endif


    while (true) {

        // Komunikaty o przekroczeniu limitów

        if((luminance_read)<0.5) {
            if(event_light) {
                buzz(2,0.1);
                alert("Slabe oswietlenie\nZapal swiatlo \nby chronic wzrok",5.0);
                event_light=0;
                if(div>3)
                    div-=4;
                else
                    div=div+9-4;
            }
        } else
            event_light=1;


        if(calculate_BAC(alcohol_read)>0.2) {
            if(event_alcohol) {
                buzz(1,1);
                alert("Alkohol ponad 0.2\%.\nPiles? Nie jedz!",5.0);
                event_alcohol=0;
                /*if(div>3)
                    div-=4;
                else
                    div=div+9-4;
                */
            }
        } else
            event_alcohol=1;

        if(calculate_ppm(gas_read)>700) {
            if(event_gas) {
                buzz(4,0.2);
                alert("Duze stezenie CO2 \nlub zanieczyszczen.\nPrzewietrz pokoj!",5.0);
                event_gas=0;
                /*if(div>3)
                    div-=4;
                else
                    div=div+9-4;
                */
            }
        } else
            event_gas=1;


// obsługa przycisku
        Switch_New=!Switch.read();

        if(Switch_New && !Switch_Old) {
            div=0;
            lcd.cls();
            if(it<6)
                it++;
            else
                it=0;
        }

        switch(it) {



            case 1:
                if(div==0) {
                    red=DIODY_ON;
                    green=DIODY_OFF;
                    blue=DIODY_OFF;
                    //lcd.cls();
                    lcd.locate(0,1);
                    if(humidity<0.01)
                        lcd.printf("Wilgotnosc:\n %3.2f; [%c]", 45.0 ,37);
                    else
                        lcd.printf("Wilgotnosc:\n %3.2f; [%c]", humidity,37);
                }
                break;

            case 2:
                if(div==0) {
                    red=DIODY_OFF;
                    green=DIODY_ON;
                    blue=DIODY_OFF;
                    //lcd.cls();
                    lcd.locate(0,1);
                    lcd.printf("Cisnienie:\n%5.2f; [hPa]", pressure);
                }
                break;

            case 3:
                if(div==0) {
                    red=DIODY_OFF;
                    green=DIODY_OFF;
                    blue=DIODY_ON;
                    lcd.locate(0,1);
                    lcd.printf("Temperatura:\n %3.2f [%cC];", T_BMP180,0xDF);
                }
                break;

            case 4:
                if(div==0) {
                    red=DIODY_OFF;
                    green=DIODY_ON;
                    blue=DIODY_ON;
                    lcd.locate(0,1);
                    lcd.printf("Stezenie CO2:\n %3.2f [ppm]\n(norma: ok.400ppm);", calculate_ppm(gas_read));
                }
                break;

            case 5:
                if(div==0) {
                    red=DIODY_ON;
                    green=DIODY_OFF;
                    blue=DIODY_ON;
                    //lcd.cls();
                    lcd.locate(0,1);
                    lcd.printf("Alkohol:\n  %3.2f [%c.];", calculate_BAC(alcohol_read), 37);
                }
                break;

            case 6:
                if(div==0) {
                    red=DIODY_ON;
                    green=DIODY_ON;
                    blue=DIODY_OFF;
                    lcd.locate(0,1);
                    lcd.printf("Oswietlenie:\n %3.6f [%c];",luminance_read*100,37);
                }
                break;

            default:
                red=DIODY_ON;
                green=DIODY_ON;
                blue=DIODY_ON;
                lcd.locate(0,0);
                lcd.printf("  Witaj w systemie! \nNacisnij przycisk \naby przejsc dalej.");

                break;
        }



        Switch_Old=Switch_New;

        if(div < MAX_DIV )
            div++;
        else
            div=0;

        wait(w);
        
#ifdef WIFI
//wyslij dane poprzez WiFi
WiFi_write();
#endif

    }



}
