package pl.edu.agh.eaiib.uclimate;

import java.util.Random;

/**
 * Class holds current data from server
 * Created by Jakub Szczepankiewicz on 2015-05-15.
 */

public class UClimateData {
    private float luminance=0;
    private float humidity=0;
    private float pressure=0;
    private float temperature_press=0;
    private float gas=0;
    private float alcohol=0;

    /**
     * Parses datastring from server
     * @param s - response from server
     */
    public void parseData(String s){
        String[] separated = s.split(";");

        for(String i : separated){
            switch(i.substring(0,2)){
                case "lm":
                    luminance = Float.valueOf(i.substring(3));
                    break;
                case "t2":
                    temperature_press = Float.valueOf(i.substring(3));
                    break;
                case "hu":
                    humidity = Float.valueOf(i.substring(3));
                    break;
                case "pr":
                    pressure = Float.valueOf(i.substring(3));
                    break;
                case "gr":
                    gas = Float.valueOf(i.substring(3));
                    break;
                case "al":
                    alcohol = Float.valueOf(i.substring(3));
                    break;
                case "cn":
                    break;
                default:
                    break;
            }
        }
    }
    public float getLuminance() {
        return luminance;
    }

    public float getHumidity() {
        return humidity;
    }

    public float getPressure() {
        return pressure;
    }

    public float getTemperature_press() {
        return temperature_press;
    }

    public float getGas() {
        return gas;
    }

    public float getAlcohol() {
        return alcohol;
    }

    public void testing() {

        Random randomizer=new Random();
        int i = randomizer.nextInt(3);
        if (i == 0){
            temperature_press = Float.parseFloat("25.31");

        pressure = Float.parseFloat("1041.11");

        humidity = Float.parseFloat("44.12");
    }
        else         if (i == 1){
            temperature_press = Float.parseFloat("25.00");

            pressure = Float.parseFloat("1012.11");

            humidity = Float.parseFloat("48.32");
        }
        else         if (i == 2){
            temperature_press = Float.parseFloat("24.51");

            pressure = Float.parseFloat("983.20");

            humidity = Float.parseFloat("40.30");
        }

    }
}
