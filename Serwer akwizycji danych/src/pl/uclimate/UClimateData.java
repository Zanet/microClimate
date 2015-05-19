package pl.uclimate;

/**
 * UClimateData object and parser. Holds latest data from UClimate to save it in datasets.
 * @author JakubSzczepankiewicz
 *
 */
public class UClimateData {
	private float luminance=0;
	private float humidity=0;
	private float pressure=0;
	private float temperature_press=0;
	private float gas=0;
	private float alcohol=0;
	
	/**
	 * to Android application
	 */
	public String toString(){
		return "lm:"+String.valueOf(luminance)+";t2:"+String.valueOf(temperature_press)+";hu:"+String.valueOf(humidity)+";pr:"+String.valueOf(pressure)+";gr:"+String.valueOf(gas)+";al:"+String.valueOf(alcohol)+"\n" ;
	}
	
	/**
	 * puts latest data from UClimate
	 * @param data - datastring from UClimate
	 */
	public void putData(String data){
		String[] separated = data.split(";");
		
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
	
}
