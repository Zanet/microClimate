package pl.uclimate;

import org.jfree.data.time.DynamicTimeSeriesCollection;
import org.jfree.data.time.Second;

/**
 * Holds dynamic datasets for charts
 * @author JakubSzczepankiewicz
 *
 */
public class UClimateChartsDataset {
    private static final int COUNT = 2 * 60;
	
	private DynamicTimeSeriesCollection gasDataset;
	private DynamicTimeSeriesCollection pressureDataset;
	private DynamicTimeSeriesCollection humidityDataset;
	private DynamicTimeSeriesCollection temperatureDataset;
	private DynamicTimeSeriesCollection alcoholDataset;
	private DynamicTimeSeriesCollection luminanceDataset;

	UClimateChartsDataset(){
		gasDataset = new DynamicTimeSeriesCollection(1, COUNT, new Second());
		pressureDataset = new DynamicTimeSeriesCollection(1, COUNT, new Second());
		humidityDataset = new DynamicTimeSeriesCollection(1, COUNT, new Second());
		temperatureDataset = new DynamicTimeSeriesCollection(1, COUNT, new Second());
		alcoholDataset = new DynamicTimeSeriesCollection(1, COUNT, new Second());
		luminanceDataset = new DynamicTimeSeriesCollection(1, COUNT, new Second());
		
        gasDataset.setTimeBase(new Second(0, 0, 0, 1, 1, 2011));
        gasDataset.addSeries(generateStartingData(), 0, "Gas");
        
        pressureDataset.setTimeBase(new Second(0, 0, 0, 1, 1, 2011));
        pressureDataset.addSeries(generateStartingData(), 0, "Pressure");
        
        humidityDataset.setTimeBase(new Second(0, 0, 0, 1, 1, 2011));
        humidityDataset.addSeries(generateStartingData(), 0, "Humidity");
        
        temperatureDataset.setTimeBase(new Second(0, 0, 0, 1, 1, 2011));
        temperatureDataset.addSeries(generateStartingData(), 0, "Temperature");
        
        alcoholDataset.setTimeBase(new Second(0, 0, 0, 1, 1, 2011));
        alcoholDataset.addSeries(generateStartingData(), 0, "Alcohol");
        
        luminanceDataset.setTimeBase(new Second(0, 0, 0, 1, 1, 2011));
        luminanceDataset.addSeries(generateStartingData(), 0, "Luminance");
	}
	
	/**
	 * Generates starting data for charts (only zeros)
	 * @return
	 */
	private float[] generateStartingData(){
        float[] a = new float[COUNT];
        for (int i = 0; i < a.length; i++) {
            a[i] = 0;
        }
        return a;
	}
	
	public DynamicTimeSeriesCollection getGasDataset() {
		return gasDataset;
	}

	public DynamicTimeSeriesCollection getPressureDataset() {
		return pressureDataset;
	}

	public DynamicTimeSeriesCollection getHumidityDataset() {
		return humidityDataset;
	}

	public DynamicTimeSeriesCollection getTemperatureDataset() {
		return temperatureDataset;
	}

	public DynamicTimeSeriesCollection getAlcoholDataset() {
		return alcoholDataset;
	}
	
	public DynamicTimeSeriesCollection getLuminanceDataset() {
		return luminanceDataset;
	}
	
	/**
	 * Advances time in dynamic datasets
	 */
	public void advanceTime() {
		gasDataset.advanceTime();
		pressureDataset.advanceTime();
		humidityDataset.advanceTime();
		temperatureDataset.advanceTime();
		alcoholDataset.advanceTime();
		luminanceDataset.advanceTime();
	}
	
	/**
	 * Appends new data from UClimate to datasets
	 * @param data
	 */
	public void appendData(UClimateData data) {
		appendGasData(data.getGas());
		appendPressureData(data.getPressure());
		appendHumidityData(data.getHumidity());
		appendTemperatureData(data.getTemperature_press());
		appendAlcoholData(data.getAlcohol());
		appendLuminanceData(data.getLuminance());
	}
	
	private void appendGasData(float f) { 
    	float[] tmp = new float[1];
    	tmp[0] = f;
		gasDataset.appendData(tmp);
	}
	
	private void appendPressureData(float f) {
    	float[] tmp = new float[1];
    	tmp[0] = f;
		pressureDataset.appendData(tmp);
	}
	
	private void appendHumidityData(float f) {
    	float[] tmp = new float[1];
    	tmp[0] = f;
		humidityDataset.appendData(tmp);
	}
	
	private void appendTemperatureData(float f) {
    	float[] tmp = new float[1];
    	tmp[0] = f;
		temperatureDataset.appendData(tmp);
	}
	
	private void appendAlcoholData(float f) {
    	float[] tmp = new float[1];
    	tmp[0] = f;
		alcoholDataset.appendData(tmp);
	}
	
	private void appendLuminanceData(float f) {
    	float[] tmp = new float[1];
    	tmp[0] = f;
		luminanceDataset.appendData(tmp);
	}
}
