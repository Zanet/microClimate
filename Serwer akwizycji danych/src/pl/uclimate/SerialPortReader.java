package pl.uclimate;

import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;

public class SerialPortReader implements SerialPortEventListener{
	String buffer = "";
	SerialPort port;
	UClimateData data;
	private UClimateChartsDataset dataset;
	
	SerialPortReader(SerialPort port, UClimateData data, UClimateChartsDataset dataset){
		this.port = port;
		this.data = data;
		this.dataset = dataset;
	}
	
    public void serialEvent(SerialPortEvent event) {
        if(event.isRXCHAR()){//If data is available
                try {
                    buffer = buffer + port.readString();
                    if(buffer.endsWith("\n")){
                    	System.out.print(buffer);
                    	System.out.println("---------------------");
                    	//buffer = buffer.substring(0, buffer.length()-1);
                    	data.putData(buffer);

                    	dataset.advanceTime();
                    	dataset.appendData(data);
                    	
                    	buffer = "";
                    }
                }
                catch (SerialPortException ex) {
                    System.out.println(ex);
                }
        }
        else if(event.isCTS()){//If CTS line has changed state
            if(event.getEventValue() == 1){//If line is ON
                System.out.println("CTS - ON");
            }
            else {
                System.out.println("CTS - OFF");
            }
        }
        else if(event.isDSR()){///If DSR line has changed state
            if(event.getEventValue() == 1){//If line is ON
                System.out.println("DSR - ON");
            }
            else {
                System.out.println("DSR - OFF");
            }
        }
    }
}