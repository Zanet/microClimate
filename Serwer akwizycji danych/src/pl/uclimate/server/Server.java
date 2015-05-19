package pl.uclimate.server;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

import pl.uclimate.UClimateData;

/**
 * Server object responsible for hosting server socket for Android application. Implements Runnable.
 * @author JakubSzczepankiewicz
 *
 */
public class Server implements Runnable {
	protected int serverPort = 11111;
	protected ServerSocket serverSocket = null;
	protected boolean isStopped = false;
	protected Socket s = null;

	protected UClimateData data;

	public Server(UClimateData data){
		this.data = data;
	}
	
	/**
	 * Starts the server and accepts clients
	 */
	public void run() {
	    try {
	      serverSocket = new ServerSocket(serverPort);
	    } catch (IOException e) {
	      e.printStackTrace();
	    }
	    while(!isStopped){
		    try {
		        s = serverSocket.accept();
		        PrintWriter output = new PrintWriter(s.getOutputStream(), true);
		        output.write(data.toString());
		      } catch (IOException e) {
		        e.printStackTrace();
		      }
	    }
	}

	/**
	 * Stops the server
	 */
	public void stop() {
		this.isStopped = true;
		try {
			this.serverSocket.close();
		} catch (IOException e) {
			throw new RuntimeException("Error closing server", e);
		}
	}

}
