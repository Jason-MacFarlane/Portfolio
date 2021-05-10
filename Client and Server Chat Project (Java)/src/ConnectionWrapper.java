//Import statements
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

/**
 * Wrapper class used to help maintain the connection between server and client
 * 
 * @author Jason MacFarlane
 * @version 1
 * @see default
 * @since 1.8.0_191
 */
public class ConnectionWrapper {
	
	/** Output Stream of the connection */
	private ObjectOutputStream outputStream;
	/** Input Stream of the connection */
	private ObjectInputStream inputStream;
	/** Socket used for the connection */
	private Socket socket;
	
	/** 
	 * Constructor for a ConnectionWrapper Object
	 * 
	 * @param socket Socket object used for the connection
	 */
	public ConnectionWrapper(Socket socket){
		
		this.socket = socket;
		
	}
	
	/**
	 * Getter method for socket
	 * 
	 * @return Socket used in the connection object
	 */
	public Socket getSocket() {
		
		return socket;
		
	}
	
	/**
	 * Getter method for the output stream
	 * 
	 * @return ObjectOutputStream used for the connection
	 */
	public ObjectOutputStream getOutputStream() {
		
		return outputStream;
		
	}
	
	/**
	 * Getter method for the input stream
	 * 
	 * @return ObjectInputStream used for the connection
	 */
	public ObjectInputStream getInputStream() {
		
		return inputStream;
		
	}
	
	/**
	 * Initializes the input stream of the connection using the socket of the connection
	 * 
	 * @return ObjectInputStream reference to the created input stream
	 * @throws IOException if there is an error getting the input stream from the socket
	 */
	public ObjectInputStream createObjectIStreams() throws IOException {
		
		ObjectInputStream inputStream = new ObjectInputStream(socket.getInputStream());
		
		this.inputStream = inputStream;
		
		return inputStream;
		
	}
	
	/**
	 * Initializes the output stream of the connection using the socket of the connection
	 * 
	 * @return ObjectOutputStream reference to the created output stream
	 * @throws IOException if there is an error getting the output stream from the socket
	 */
	public ObjectOutputStream createObjectOStreams() throws IOException {
		
		ObjectOutputStream outputStream = new ObjectOutputStream(socket.getOutputStream());
		
		this.outputStream = outputStream;
		
		return outputStream;
		
	}
	
	/**
	 * Initializes the output and input streams of the connection
	 * 
	 * @throws IOException if there is an error getting either stream from the socket
	 */
	public void createStreams() throws IOException {
		
		ObjectOutputStream outputStream = new ObjectOutputStream(socket.getOutputStream());
		
		this.outputStream = outputStream;
		
		ObjectInputStream inputStream = new ObjectInputStream(socket.getInputStream());
		
		this.inputStream = inputStream;
		
	}
	
	/**
	 * Closes the input and output streams as well as the socket
	 * 
	 * @throws IOException if there is any issue with closing the connection
	 */
	public void closeConnection() throws IOException{
		
		if(inputStream != null) {
			
			inputStream.close();
			
		}
		
		if(outputStream != null) {
			
			outputStream.close();
			
		}
		
		if(socket != null && !socket.isClosed()) {
				
			socket.close();
			
		}
		
	}
	
}
