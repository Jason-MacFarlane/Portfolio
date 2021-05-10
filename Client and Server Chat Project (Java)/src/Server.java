/**
 * File name: Server.java
 * Author: Jason MacFarlane, 040890266
 * Course: CST8221 – JAP, Lab Section: 302
 * Assignment: 2, Part 1
 * Date: Decemeber 2nd, 2019
 * Professor: Daniel Cormier
 * Purpose: Creates a simple GUI for the server side of a messaging application
 * Class list: Server.java, ServerChatUI.java, Client.java, ClientChatUI.java, Accessible.java, ChatProtocolConstants.java, ConnectionWrapper.java, ChatRunnable.java
 */

//Import statements
import java.awt.EventQueue;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.nio.channels.IllegalBlockingModeException;

/**
 * Contains main for server UI, which builds and displays the Frame
 * 
 * @author Jason MacFarlane
 * @version 1
 * @see default
 * @since 1.8.0_191
 */
public class Server {

	/**
	 * main function that runs the application by calling the launchClient method
	 * 
	 * @param  args command line arguments
	 */
	@SuppressWarnings("resource") //In order to stop the server (close ServerSocket) the user must terminate the Java Virtual Machine (JVM) (Ctrl-C).
	public static void main(String[] args) {
		
		boolean connectSuccess = false; //Connection boolean
		int portNum; //Holds the socket port number
		int friend = 0; //Holds the number of friends created
		Socket socket = null; //Socket reference
		ServerSocket serverSocket = null; //ServerSocket reference;
		
		if(args.length != 0) { // if command line arguments were provided
			
			try {
				
				portNum = Integer.parseInt(args[0]); //Attempt to convert argument to int
				
				if(portNum > 65535 || portNum < 0) { //Check for valid range
					
					System.out.println("Invalid port, using default port: 65535");
					portNum = 65535;
					
				}else {
					
					System.out.println("Using Port " + portNum);
					
				}
				
			}catch (NumberFormatException e) { //If convert to int failed 
				
				System.out.println("Invalid port, using default port: 65535");
				portNum = 65535;
				
			}
			
		}else { //If no argument was specified use default
			
			portNum = 65535;
			System.out.println("Using default port: 65535");
			
		}
		
		try {
			
			serverSocket = new ServerSocket(portNum); //Attempt to open ServerSocket with port
			connectSuccess = true;
			
		} catch (SecurityException e) {
			
			System.out.println("Error: Security error");
			
		} catch (IOException e) {
			
			System.out.println("Error: Could not open server socket, try different port");
			
		}
		
		if(connectSuccess) {
			
			while(true) { //Endless loop
				
				try {
					
					socket = serverSocket.accept();	//Wait for a connection to accpet
				
					//Settings for accepted socket
					if(socket.getSoLinger() != -1) {
						
						socket.setSoLinger(true,5);
						
					}
					
					if(!socket.getTcpNoDelay()) {
						
						socket.setTcpNoDelay(true);
						
					}
				
				}catch (IllegalBlockingModeException e) {
					
					System.out.println("Error: Channel is in a non-blocking mode, could not accept");
					
				}catch(SocketTimeoutException e){
				
					System.out.println("Error: Socket timed out");
					
				}catch(SecurityException e) {
					
					System.out.println("Error: Security error");
					
				}catch (IOException e) {
				
					System.out.println("Error: Could not open server socket");
					
				}
				
				System.out.println("Connecting to a client " + socket); //Print socket information to console
				
				friend++;
				
				final String title = "Jason's Friend " + friend; //Create title for new friend
				
				launchClient(socket, title); //Use socket and title to launch new Client
				
			}
		
		}
		
	}
	
	/**
	 *  Creates and displays the GUI Frame for the server UI
	 * 
	 * @param  socket networking socket to use
	 * @param  title desired title for the frame
	 */
	public static void launchClient(Socket socket, String title) {
		
		EventQueue.invokeLater(new Runnable() { //Anonymous class
			
			/**
			 * Overrides run and starts the application
			 * @param N/A
			 * @return N/A
			 */
			@Override
			public void run() {
				
				/** new ServerChatUI object which inherits from Frame **/
				ServerChatUI serverChat = new ServerChatUI(socket);

				/** Sets to UI accordingly **/
				serverChat.setTitle(title);
				serverChat.setLocationByPlatform(true);
				serverChat.setVisible(true);

				
			}
					
		});
		
	}
	
}
