//Import statement
import javax.swing.JTextArea;

/**
 * Used to make a Clients or Servers chat display accessible as well as
 * what happens when one of the parties closes their side of the chat
 * 
 * @author Jason MacFarlane
 * @version 1
 * @see default
 * @since 1.8.0_191
 */
public interface Accessible {
	
	/** 
	 * Simple getter method 
	 * 
	 * @return JTextArea used for the chat
	 */
	JTextArea getDisplay();
	
	/** 
	 * Defines the classes behavior when the Server or Client closes their chat 
	*/
	void closeChat();

}
