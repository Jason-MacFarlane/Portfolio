//Import statements
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.net.SocketException;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

import javax.swing.JFrame;
import javax.swing.JTextArea;

/**
 * Runnable for either the Client or Server objects
 * 
 * @author Jason MacFarlane
 * @version 1
 * @see default
 * @since 1.8.0_191
 */
public class ChatRunnable<T extends JFrame & Accessible> implements Runnable {

	/** User interface of either the Server or Client */
	final T ui;
	/** The socket of the connection */
	final Socket socket;
	/** The input stream of the connection */
	final ObjectInputStream inputStream;
	/** The output stream of the connection */
	final ObjectOutputStream outputStream;
	/** The display of either the Server or Client */
	final JTextArea display;
	
	/**
	 * Constructor initializes the class member variables with the parameters 
	 * 
	 * @param ui User interface of either the Server of the Client
	 * @param connection ConnectionWrapper for the connection
	 */
	public ChatRunnable(T ui, ConnectionWrapper connection) {
		
		this.socket = connection.getSocket();
		this.inputStream = connection.getInputStream();
		this.outputStream = connection.getOutputStream();
		this.ui = ui;
		this.display = ui.getDisplay();
		
	}
	
	/**
	 * Overriding the run() method of the runnable interface
	 */
	@Override
	public void run() {
		
		String strin = ""; //String variable to hold the message to be read from the input stream
		DateTimeFormatter dateFormat = DateTimeFormatter.ofPattern("MMMM dd, HH:mm a"); //Pattern used to display the timestamp on messages
		LocalDateTime date; //Variable to hold the value of the current date and time
	
		while(true) { //Infinite loop of for the running of the thread
			
			if(socket.isClosed()) {//Break the loop if the socket is closed 
				
				break;
				
			}
				
			try {
				
				strin = (String) inputStream.readObject(); //Get input
					
			} catch(SocketException e){
				
				System.out.println("ERROR: Connection reset");
				break;
		
			} catch(ClassNotFoundException e) {
				
				System.out.println("ERROR: Class Not Found Exception");
				break;

			}catch(IOException e) {
				
				System.out.println("IOException");
				break;

			}
				
			date = LocalDateTime.now(); //Update time stamp
				
			if(strin.trim().equals(ChatProtocolConstants.CHAT_TERMINATOR)) { //check for "bye"
					
				final String terminate = ChatProtocolConstants.DISPLACMENT + dateFormat.format(date) + ChatProtocolConstants.LINE_TERMINATOR + strin;
					
				display.append(terminate); //Format and append string to display
					
				break; // Terminate the loop
					
			}else {
					
				final String append = ChatProtocolConstants.DISPLACMENT + dateFormat.format(date) + ChatProtocolConstants.LINE_TERMINATOR + strin;
					
				display.append(append); //Format and append message to display
					
			}
				
		}
		
		if(!socket.isClosed()) {
			
			try { //if loop was broken but the socket is not closed
				
				// Write formatted chat terminator to the output stream
				outputStream.writeObject(ChatProtocolConstants.DISPLACMENT + ChatProtocolConstants.CHAT_TERMINATOR + ChatProtocolConstants.LINE_TERMINATOR);
				
			} catch(SocketException e) {
				
				System.out.println("ERROR: Could not write to socket");
				
			}catch (IOException e) {
				
				System.out.println("ERROR: IO Exception");
				
			}
			
		}
		
		ui.closeChat(); //Close chat
		
	}

}
