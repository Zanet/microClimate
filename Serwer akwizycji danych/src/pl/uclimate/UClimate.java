package pl.uclimate;

import jssc.SerialPort;
import jssc.SerialPortException;
import pl.uclimate.server.Server;

/**
 * Main UClimate Objects. Holds datasets for charts and serial port reader object
 * @author JakubSzczepankiewicz
 *
 */
public class UClimate {
	private SerialPort port;
	private UClimateData data = new UClimateData();
	private SerialPortReader portreader;
	
	private UClimateChartsDataset udataset;
	
	private Server server;

	public void startServer(){
		server = new Server(data);
		new Thread(server).start();
	}
	public SerialPort getPort() {
		return port;
	}

	public UClimateData getData() {
		return data;
	}
	
	public UClimateChartsDataset getChartDataset() {
		return udataset;
	}

	private int mask = SerialPort.MASK_RXCHAR + SerialPort.MASK_CTS + SerialPort.MASK_DSR; // Prepare mask

	UClimate(String portname) {
		port = new SerialPort(portname);
		udataset = new UClimateChartsDataset();
		portreader = new SerialPortReader(port,data,udataset);
	}

	public void startReadingData() {
		try {
			port.openPort();// Open port
			port.setParams(9600, 8, 1, 0);// Set params
			port.setEventsMask(mask);// Set mask
			port.addEventListener(portreader);// Add SerialPortEventListener
		} catch (SerialPortException ex) {
			System.out.println(ex);
		}
	}

	public void stopReadingData() {
		try {
			if (port.isOpened())
				port.closePort();
		} catch (SerialPortException e) {
			e.printStackTrace();
		}
	}
	
}
