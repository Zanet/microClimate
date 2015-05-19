package pl.uclimate;

import java.awt.EventQueue;
import java.awt.GridLayout;

import javax.swing.JPanel;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.plot.XYPlot;
import org.jfree.data.xy.XYDataset;
import org.jfree.ui.ApplicationFrame;
import org.jfree.ui.RefineryUtilities;

public class MainWindow extends ApplicationFrame {
	private static final long serialVersionUID = 1L;
    
	//Serial port connected with uClimate
    private static final String COMPORT = "COM100";
    
    //Showed charts objects
	JFreeChart luminanceChart;
	JFreeChart humidityChart;
	JFreeChart pressureChart;
	JFreeChart temperaturePressChart;
	JFreeChart gasChart;
	JFreeChart alcoholChart;
	
	/**
	 * Main panel
	 */
	JPanel panel;
	
	/**
	 * Main UClimate object
	 */
	private UClimate uclimate;
	
	public MainWindow(String title) {
		super(title);
		
		uclimate = new UClimate(COMPORT);
       
        gasChart = createChart(uclimate.getChartDataset().getGasDataset(), "Carbon dioxide", "ppm", 200, 3000);
        humidityChart = createChart(uclimate.getChartDataset().getHumidityDataset(), "Humidity", "%", 0, 100);
        pressureChart = createChart(uclimate.getChartDataset().getPressureDataset(), "Pressure", "hPa", 900, 1100);
        temperaturePressChart = createChart(uclimate.getChartDataset().getTemperatureDataset(), "Temperature", "C", 10, 30);
        alcoholChart = createChart(uclimate.getChartDataset().getAlcoholDataset(), "Alcohol", "‰", 0, 15);
        luminanceChart = createChart(uclimate.getChartDataset().getLuminanceDataset(), "Luminance", "%", 0, 100);
        
        this.setLayout(new GridLayout(3,2));
        this.add(new ChartPanel(gasChart));
        this.add(new ChartPanel(humidityChart));
        this.add(new ChartPanel(pressureChart));
        this.add(new ChartPanel(temperaturePressChart));
        this.add(new ChartPanel(alcoholChart));
        this.add(new ChartPanel(luminanceChart));
	}
	
	/**
	 * Creates new XY chart with starting data
	 * @param dataset - starting dataset for chart
	 * @param title - chart title
	 * @param valueName - Y value title
	 * @param min - minimum Y value
	 * @param max - maximum Y value
	 * @return
	 */
	private JFreeChart createChart(final XYDataset dataset, String title, String valueName, float min, float max) { //TODO specyfikacja wykresow
	        final JFreeChart result = ChartFactory.createTimeSeriesChart(title, "time", valueName, dataset, true, true, false);
	        final XYPlot plot = result.getXYPlot();
	        ValueAxis domain = plot.getDomainAxis();
	        domain.setAutoRange(true);
	        ValueAxis range = plot.getRangeAxis();
	        range.setRange(min, max);
	        return result;
	}
	
	/**
	 * Starts reading data from serial and starts local server for Android application
	 */
	public void start(){
		uclimate.startReadingData();
		uclimate.startServer();
	}
    
    public static void main(String[] args) {
        EventQueue.invokeLater(new Runnable() {
            @Override
            public void run() {
                MainWindow application = new MainWindow("uClimate");
                application.pack();
                RefineryUtilities.centerFrameOnScreen(application);
                application.setVisible(true);
                application.start();
            }
        });
    }
}
