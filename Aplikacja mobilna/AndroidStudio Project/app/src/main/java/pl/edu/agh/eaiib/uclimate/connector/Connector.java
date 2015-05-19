package pl.edu.agh.eaiib.uclimate.connector;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ConnectException;
import java.net.InetAddress;
import java.net.Socket;

import pl.edu.agh.eaiib.uclimate.UClimateData;

/**
 * Class responsible for connecting with data server on PC (static ip address)
 * Created by Jakub Szczepankiewicz on 2015-05-17.
 */
public class Connector {
    private final String SERVERADDRESS = "192.168.0.18";
    private final int SERVERPORT = 11111;

    private Socket server;
    private BufferedReader read = null;
    String response = "";
    UClimateData data;

    public Connector(UClimateData data){
        this.data=data;
    }

    /**
     * Method gets new data from server and puts it in UClimateData object
     */
    public void getData(){
        try{
            InetAddress addr = null;
            addr = InetAddress.getByName(SERVERPORT);
            //connect("192.168.0.18", SERVERPORT);
            connect(addr,SERVERPORT);
            data.parseData(response);
            disconnect();
        }catch(Exception ex){
            ex.printStackTrace();
        }
    }

    /**
     * Connects with PC using Socket
     * @param addr
     * @param port
     * @throws IOException
     */
    private void connect(InetAddress addr, int port) throws IOException{
        server = new Socket(addr,port);
        read = new BufferedReader(new InputStreamReader(server.getInputStream()));
        response = read.readLine();
    }

    /**
     * Connects with PC using Socket
     * @param host
     * @param port
     * @throws ConnectException
     * @throws IOException
     */
    private void connect(String host, int port) throws ConnectException, IOException{
        server = new Socket(host,port);
        read = new BufferedReader(new InputStreamReader(server.getInputStream()));
        response = read.readLine();
    }


    /**
     * Closes socket and reading buffer
     */
    private void disconnect(){
        try {
            read.close();
            server.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
