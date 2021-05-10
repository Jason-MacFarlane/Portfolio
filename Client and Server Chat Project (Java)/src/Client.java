/**
 * File name: Client.java
 * Author: Jason MacFarlane, 040890266
 * Course: CST8221 – JAP, Lab Section: 302
 * Assignment: 2, Part 1
 * Date: December 2nd, 2019
 * Professor: Daniel Cormier
 * Purpose: Creates a simple GUI for the client side of a messaging application
 * Class list: Server.java, ServerChatUI.java, Client.java, ClientChatUI.java, Accessible.java, ChatProtocolConstants.java, ConnectionWrapper.java, ChatRunnable.java
 */

import java.awt.EventQueue;

/**
 * Contains main for client UI, which builds and displays the Frame
 * 
 * @author Jason MacFarlane
 * @version 1
 * @see default
 * @since 1.8.0_191
 */
public class Client {
	
	/**
	 * main function that runs the application by creating and displaying the frame
	 * 
	 * @param  args command line arguments
	 */
	public static void main(String[] args) {
	
		EventQueue.invokeLater(new Runnable() { //Anonymous class
			
			/**
			 * Overrides run and starts the application
			 */
			@Override
			public void run() {
		
				/** new ClientChatUI object which inherits Frame **/
				ClientChatUI clientChat = new ClientChatUI("Jason's ClientChatUI");
				
			}
					
		});
		
	}

}
