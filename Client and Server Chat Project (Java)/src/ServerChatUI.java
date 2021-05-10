//Import Statements
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.Socket;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.border.TitledBorder;


/**
 * Maintains Server Frame and handles events
 * 
 * @author Jason MacFarlane
 * @version 1
 * @see default
 * @since 1.8.0_191
 */
public class ServerChatUI extends JFrame implements Accessible{

	/** {@value} serial ID */
	private static final long serialVersionUID = -3469872269854761835L;
	/** Networking socket to be used */
	private Socket socket; 
	/** Text Field use for the message input on the UI */
	private JTextField message;
	/** Send button displayed next to the message text box */
	private JButton sendButton;
	/** Display used to display conversations */
	private JTextArea display;
	/** Output stream used to send messages */
	private ObjectOutputStream outputStream;
	/** Connection used by the Server UI */
	private ConnectionWrapper connection;

	/**
	 * Constructs the Server UI frame and calls the runClient method
	 * 
	 * @param socket the networking socket to be used
	 */
	public ServerChatUI(Socket socket) {
		
		this.socket = socket;
		setFrame(createUI());
		runClient();
		
	}
	
	/**
	 * Getter method for display
	 * 
	 * @return JTextArea used for the display
	 */
	public JTextArea getDisplay() {
		
		return display;
		
	}
	
	/**
	 * Tries to close the connection and dispose the frame
	 */
	public void closeChat() {
		
		try {
			
			connection.closeConnection();
			
		} catch (IOException e) {

			System.out.println("ERROR: Connection failure");
			
		}
		
		this.dispose();
		
	}
	
	/**
	 * Builds the user interface for the server GUI
	 * 
	 * @return the completed JPanel with the UI
	 */
	public JPanel createUI() {
		
		JPanel mainPanel = new JPanel(new BorderLayout()); /** main panel */
		
		message = new JTextField("Type message"); /** message text field */
		
		//Set message text field properties
		message.setPreferredSize(new Dimension(462,20));
		message.requestFocus();
		message.setCaretPosition(0);
		
		sendButton = new JButton("Send"); /** send button */
		
		//set send button properties
		sendButton.setPreferredSize(new Dimension(79,19));
		sendButton.setMnemonic(KeyEvent.VK_S);
		sendButton.addActionListener(new Controller());
		
		JPanel messagePanel = new JPanel(new FlowLayout()); /** message panel */
		
		//set border and add components
		messagePanel.setBorder(BorderFactory.createTitledBorder(BorderFactory.createLineBorder(Color.BLACK, 10), "MESSAGE"));
		messagePanel.add(message);
		messagePanel.add(sendButton);
		
		display = new JTextArea(30, 45); /** chat text area */
		
		/** scroll pane for the text area */
		JScrollPane scrollPane = new JScrollPane(display, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
		display.setEditable(false);
		
		JPanel chatPanel = new JPanel(new BorderLayout()); /** panel to for the chat display */
		
		//set border and add components
		chatPanel.setBorder(BorderFactory.createTitledBorder(BorderFactory.createLineBorder(Color.BLUE, 10), "CHAT DISPLAY", TitledBorder.CENTER, TitledBorder.DEFAULT_POSITION));
		chatPanel.add(scrollPane);
		
		//add components to main panel
		mainPanel.add(messagePanel, BorderLayout.NORTH);
		mainPanel.add(chatPanel,BorderLayout.CENTER);
		
		return mainPanel; //return completed panel
		
	}
	
	/**
	 * Sets properties for Frame and adds UI
	 * 
	 * @param the completed JPanel with the UI
	 */
	public final void setFrame(JPanel panel) {
		
		//set properties for frame
		this.setSize(new Dimension(588,500));
		this.setResizable(false);
		this.addWindowListener(new WindowController());
		this.setContentPane(panel);
		
	}
	
	/**
	 * Creates the connection and the starts the runnable thread
	 */
	private void runClient() {
		
		connection = new ConnectionWrapper(socket); //Use socket to create connection wrapper
		
		try {
			
			connection.createStreams(); //Try to create streams
			
		} catch (IOException e) {
			
			System.out.println("ERROR: IO Exception");
			
		}

		outputStream = connection.getOutputStream(); //Assign output stream to member variable
		Runnable chat = new ChatRunnable<ServerChatUI>(this, connection); 
		Thread thread = new Thread(chat); //Create new thread
		thread.start(); //Start thread
		
	}
	
	/**
	 * Inherits WindowAdapter and overrides window closing
	 * 
	 * @author Jason MacFarlane
	 * @version 1
	 * @see default
	 * @since 1.8.0_191
	 */
	private class WindowController extends WindowAdapter{
		
		/**
		 * Overrides windowClosing method
		 * 
		 * @param e WindowEvent consumed
		 */
		@Override
		public void windowClosing(WindowEvent e) {
			
			System.out.println("ServerUI Window closing!");
			
			try {
				
				//Try to write chat terminator to output stream
				outputStream.writeObject(ChatProtocolConstants.DISPLACMENT + ChatProtocolConstants.CHAT_TERMINATOR + ChatProtocolConstants.LINE_TERMINATOR);
				
			} catch (IOException e1) {
				
				System.out.println("ERROR: IO Exception");
				
			}finally {
				
				ServerChatUI.this.dispose(); //Dispose frame
				
			}
			
			System.out.println("Closing Chat!");
			
			try {
				
				connection.closeConnection(); //Try to close connection
				
			} catch (IOException e1) {
				
				System.out.println("ERROR: IO Exception");
				
			}finally {

				ServerChatUI.this.dispose(); //Dispose frame
				
			}
			
			ServerChatUI.this.dispose();
			System.out.println("Chat Closed!");
			
		}
		
		/**
		 * Overrides windowClosed method
		 * 
		 * @param e WindowEvent consumed
		 */
		@Override
		public void windowClosed(WindowEvent e) {
			
			System.out.println("Server UI Closed!");
			
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
			
			if(e.getSource() == sendButton) {
				
				send();
				
			}
			
		}
		
		/**
		 * Tries to send message by writing message to output stream
		 */
		private void send() {
			
			String sendMessage = message.getText();
			
			display.append(sendMessage + ChatProtocolConstants.LINE_TERMINATOR);
			
			try {
				
				//Try to write message to output stream
				outputStream.writeObject(ChatProtocolConstants.DISPLACMENT + sendMessage + ChatProtocolConstants.LINE_TERMINATOR);
				
			} catch (IOException e) {
				
				display.append("Error sending message\n");
				
			}
			
		}
		
	}
	
}
