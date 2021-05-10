//Import statements
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.ConnectException;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.border.TitledBorder;

/**
 * Maintains Client Frame and handles events
 * 
 * @author Jason MacFarlane
 * @version 1
 * @see default
 * @since 1.8.0_191
 */
public class ClientChatUI extends JFrame implements Accessible {
	
	/** {@value} serial ID */
	private static final long serialVersionUID = -2641550520277117547L;
	/** Text Field used for message input */
	private JTextField message;
	/** Text Field used for the host input */ 
	private JTextField hostText;
	/** Combo box used for port input */
	private JComboBox<String> combo;
	/** Send button displayed by the message input box */
	private JButton sendButton;
	/** Connect button displayed next to the combo box */
	private JButton connectButton;
	/** Display used to display conversations */
	private JTextArea display;
	/** Output stream used for sending messages */
	private ObjectOutputStream outputStream;
	/** socket used for the connection */
	private Socket socket;
	/** Wrapper used for the connection */
	private ConnectionWrapper connection;

	/**
	 * Constructs the Client UI frame and calls the runClient method
	 * 
	 * @param socket the networking socket to be used
	 */
	ClientChatUI(String frameTitle){
		
		this.setTitle(frameTitle); //Set frame title
		runClient(); //Call runClient to build UI
		
	}
	
	/**
	 * Simple getter method for display
	 * 
	 * @return JTextArea used for displaying conversations
	 */
	public JTextArea getDisplay() {
		
		return display;
		
	}
	
	/**
	 * Tries to close the connection and enable connect button again
	 */
	public void closeChat() {
		
		if(!socket.isClosed()) {
			
			try {
				
				connection.closeConnection(); //Close connection if socket is not closed
				
			} catch (IOException e) {
				
				System.out.println("ERROR: Connection failure");
				
			}
			
			enableConnectButton(); //enable connect button
			
		}
		
	}
	
	/**
	 * Builds the user interface for the client GUI
	 * 
	 * @return the completed JPanel with the UI
	 */
	public JPanel createClientUI() {
		
		Controller listener = new Controller();
		JPanel mainPanel = new JPanel(new BorderLayout()); /** main panel for the UI */

		JPanel connection = new JPanel(new BorderLayout()); /** Panel for the connection section */
		connection.setBorder(BorderFactory.createTitledBorder(BorderFactory.createLineBorder(Color.RED, 10), "CONNECTION")); //Add border
		
		JLabel host = new JLabel("Host:"); /** Host label */
		
		//set host label properties
		host.setDisplayedMnemonic(KeyEvent.VK_H);
		host.setPreferredSize(new Dimension(35, 30));
		
		hostText = new JTextField("localhost");  /** Host text field */
		
		//set host text properties
		host.setLabelFor(hostText);
		hostText.setPreferredSize(new Dimension(508,20));
		hostText.setBackground(Color.WHITE);
		hostText.requestFocus();
		hostText.setCaretPosition(0);
		hostText.setMargin(new Insets(0,5,0,0));
		
		JPanel hostPanel = new JPanel(new FlowLayout()); /** Panel to contain host label and host textbox */
		
		//Add to host panel and and to connection panel
		hostPanel.add(host);
		hostPanel.add(hostText);
		connection.add(hostPanel, BorderLayout.NORTH);
		
		JLabel port = new JLabel("Port:"); /** Port label */
		
		//set port label properties
		port.setPreferredSize(new Dimension(35,30));
		port.setDisplayedMnemonic(KeyEvent.VK_P);
		
		String[] ports = {"","8089", "65000", "65535"}; /** string array to hold contents of combo box */
		combo = new JComboBox<String>(ports); /** combo box for ports */
		
		//set combo box properties
		combo.setBackground(Color.WHITE);
		combo.setPreferredSize(new Dimension(100, 20));
		combo.setEditable(true);
		port.setLabelFor(combo);
		
		connectButton = new JButton("Connect"); /** connection button */
		
		//set button properties
		connectButton.setBackground(Color.RED);
		connectButton.setPreferredSize(new Dimension(100, 20));
		connectButton.setMnemonic(KeyEvent.VK_C);
		connectButton.addActionListener(listener);
		
		JPanel portPanel = new JPanel(new FlowLayout()); /** port panel */
		
		//add to port panel and add to connection panel
		portPanel.add(port);
		portPanel.add(combo);
		portPanel.add(connectButton);
		portPanel.setBorder(BorderFactory.createEmptyBorder(0, -2, 0, 0));
		connection.add(portPanel, BorderLayout.WEST);
		
		message = new JTextField("Type message"); /** message text box */
		message.setPreferredSize(new Dimension(462,20)); //set size
		
		sendButton = new JButton("Send"); /** send button */
		
		//set send button properties
		sendButton.setPreferredSize(new Dimension(79,19));
		sendButton.setMnemonic(KeyEvent.VK_S);
		sendButton.setEnabled(false);
		sendButton.addActionListener(listener);
		
		JPanel messagePanel = new JPanel(new FlowLayout()); /** message panel */
		
		//set border and add components
		messagePanel.setBorder(BorderFactory.createTitledBorder(BorderFactory.createLineBorder(Color.BLACK, 10), "MESSAGE"));
		messagePanel.add(message);
		messagePanel.add(sendButton);
		
		JPanel topUI = new JPanel(new BorderLayout()); /** panel for the top part of the UI */
		
		//add panels
		topUI.add(connection, BorderLayout.CENTER);
		topUI.add(messagePanel, BorderLayout.SOUTH);
		
		display = new JTextArea(30, 45); /** text area */
		
		/** scroll pane for text area */
		JScrollPane scrollPane = new JScrollPane(display, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
		display.setEditable(false);
		
		JPanel chatPanel = new JPanel(new BorderLayout()); /** panel to display chat */
		
		//set border and add the text area
		chatPanel.setBorder(BorderFactory.createTitledBorder(BorderFactory.createLineBorder(Color.BLUE, 10), "CHAT DISPLAY", TitledBorder.CENTER, TitledBorder.DEFAULT_POSITION));
		chatPanel.add(scrollPane, BorderLayout.CENTER);
		
		//add to main panel
		mainPanel.add(topUI,BorderLayout.NORTH);
		mainPanel.add(chatPanel, BorderLayout.CENTER);
		
		return mainPanel; //return completed panel
		
	}
	
	/**
	 * Adds content to Frame and adds window listener
	 * 
	 */
	private void runClient() {
		
		this.setSize(new Dimension(588,500));
		this.setResizable(false);
		this.setLocationByPlatform(true);
		this.setContentPane(createClientUI()); //set content pane with new UI
		this.addWindowListener(new WindowController()); //add window listener
		this.setVisible(true);
		
	}
	
	/**
	 * Enables to connect button and sets its properties accordingly
	 */
	private void enableConnectButton() {
		
		connectButton.setEnabled(true);
		connectButton.setBackground(Color.RED);
		sendButton.setEnabled(false);
		hostText.requestFocus();
		
	}
	
	/**
	 * Inherits WindowAdapter and overrides window closing
	 * 
	 * @author Jason MacFarlane
	 * @version 1
	 * @see default
	 * @since 1.8.0_191
	 */
	private class WindowController extends WindowAdapter {
		
		/**
		 * Overrides windowClosing method
		 * 
		 * @param e WindowEvent consumed
		 */
		@Override
		public void windowClosing(WindowEvent e) {
			
			if(outputStream != null) {
			
				try {
					
					//Try to write bye to output stream
					outputStream.writeObject(ChatProtocolConstants.CHAT_TERMINATOR);
					
				} catch (IOException e1) {
				
					System.out.println("ERROR: Connection error");
					System.exit(0);
					
				}
				
			}
			
			System.exit(0);
			
		}
		
	}
	
	/**
	 * Implements ActionListener and overrides actionPerformed
	 * 
	 * @author Jason MacFarlane
	 * @version 1
	 * @see default
	 * @since 1.8.0_191
	 */
	private class Controller implements ActionListener{
		
		/**
		 * Overrides actionPerformed method
		 * 
		 * @param e ActionEvent consumed
		 */
		@Override
		public void actionPerformed(ActionEvent e) {
			
			boolean connected = false; //To tell if currently connected
			int port = 0; //Initialize port to 0
			
			if(e.getSource() == connectButton) {
				
				String host = hostText.getText(); //Gets host text field
				String portText = (String) combo.getSelectedItem(); //Gets value from combo box
				
				if(portText.equals("") || portText == null) {
					
					portText = "65535"; //Default port
					
				}
				
				try {
				
					port = Integer.parseInt(portText); //Convert port string to int
					connected = connect(host,port); // Try to start connection
				
				}catch (NumberFormatException ex) {
					
					display.append("ERROR: Please enter a properly formated socket" + ChatProtocolConstants.LINE_TERMINATOR);
					
				}catch(IllegalArgumentException ex) {
					
					display.append("ERROR: Port number out of range 0-65535" + ChatProtocolConstants.LINE_TERMINATOR);
					
				}
				
				if(connected) { //If connection is successful
					
					//Disable the connect button and enable send button
					connectButton.setEnabled(false);
					connectButton.setBackground(Color.BLUE);
					sendButton.setEnabled(true);
					message.requestFocus();
					
					Runnable chat = new ChatRunnable<ClientChatUI>(ClientChatUI.this, connection); 
					Thread thread = new Thread(chat);
					thread.start(); //Start thread
					
				}else {
					
					return;
					
				}
				
			}else if(e.getSource() == sendButton) {
				
				send();
				
			}
			
		}
		
		/**
		 * Tries to create a connection with the host and port given
		 * @param host for the connection
		 * @param port for the connection
		 * @return boolean if the connection was successful
		 */
		private boolean connect(String host, int port) {
			
			Socket timeoutSocket = null; //Reference for timeout socket
			
			try {
				
				timeoutSocket = new Socket(host, port); //Try to create time-out socket
				
			} catch (ConnectException e) {
				
				display.append("ERROR: Connection refused: server is not available. Check port or restart server." + ChatProtocolConstants.LINE_TERMINATOR);
				return false;
				
			} catch (UnknownHostException e) {
				
				display.append("ERROR: Host not found" + ChatProtocolConstants.LINE_TERMINATOR);
				return false;
				
			} catch (IOException e) {
				
				display.append("ERROR: IO Exception" + ChatProtocolConstants.LINE_TERMINATOR);
				return false;
				
			}
					
			socket = timeoutSocket;
				
			try { //Set properties of socket
					
				if(socket.getSoLinger()!= -1) {
						
					socket.setSoLinger(true,5);
						
				}
				
				if(!socket.getTcpNoDelay()) {
						
					socket.setTcpNoDelay(true);
						
				}
					
			}catch(IOException e) {
					
				System.out.println("ERROR: IO Exception");
					
			}
				
			display.append(socket + ChatProtocolConstants.LINE_TERMINATOR); //Display socket information on chat display
				
			connection = new ConnectionWrapper(socket); //Initialize the connection member variable
				
			try {
					
				connection.createStreams(); //Try to create streams
					
			}catch(SocketException e){
					
				display.append("ERROR: Connection refused: server is not available. Check port or restart server." + ChatProtocolConstants.LINE_TERMINATOR);
				return false;
					
			}catch (IOException e) {
					
				display.append("ERROR: IOException" + ChatProtocolConstants.LINE_TERMINATOR);
				return false;
					
			}
				
			outputStream = connection.getOutputStream(); //Initialize output stream
			
			return true; //Return true for successful connection
			
		}
		
		/**
		 * Attempts to send the text currently in the message field
		 */
		private void send() {
			
			String sendMessage = message.getText(); //Get the text from message text field
			
			display.append(sendMessage + ChatProtocolConstants.LINE_TERMINATOR); //Display the message
			
			try {
				
				//Attempt to write the message to the stream
				outputStream.writeObject(ChatProtocolConstants.DISPLACMENT + sendMessage + ChatProtocolConstants.LINE_TERMINATOR);
				
			} catch (IOException e) {
				
				enableConnectButton();
				display.append("Error writing to output stream" + ChatProtocolConstants.LINE_TERMINATOR);
					
			}
			
		}
		
	}

}
