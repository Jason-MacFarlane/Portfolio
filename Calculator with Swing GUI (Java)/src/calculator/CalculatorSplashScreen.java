package calculator; //Package statement

//Import statements
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Toolkit;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JWindow;

/**
 * Creates and maintains a splash screen for a desired amount of time
 * 
 * @author Jason MacFarlane
 * @version 1
 * @see calculator
 * @since 1.8.0_191
 */
public class CalculatorSplashScreen extends JWindow {
	
	/** {@value} serial ID */
	private static final long serialVersionUID = 9045815293941836390L; 
	private final int time; /** {@value} time to display screen */
	
	/**
	 * Constructs splash screen object and set the desired time to display
	 * 
	 * @param time The amount of time to display the splash screen
	 */
	public CalculatorSplashScreen(int time) {
		
		this.time = time;
		
	}
	
	/**
	 * Constructs the interface and displays the splash screen
	 */
	public void showSplashWindow() {
		 
		JPanel panel = new JPanel(new BorderLayout()); /** Initialize panel for splash screen */
		Dimension size = Toolkit.getDefaultToolkit().getScreenSize(); /** Get hosts screen size */
		int w = 500+10; /** Width of picture */
		int h = 333+10; /** Height of picture */
		int xCoord = (size.width-w)/2; /** calculate x coordinate */
		int yCoord = (size.height-h)/2; /** calculate y coordinate */
		JLabel image = new JLabel(new ImageIcon("calculator/Splash.jpeg")); /** get image */
		JLabel information = new JLabel("Jason MacFarlane 040890266", JLabel.CENTER); /** information for splash screen */
		 
		setBounds(xCoord, yCoord ,w ,h); //set location for screen
		 
		panel.add(image, BorderLayout.CENTER); // add image to panel
		panel.add(information, BorderLayout.SOUTH); //add information to panel
		 
		setContentPane(panel); //add panel to content pane of JWindow
		setVisible(true); // show
		 
		try {
		    	
			Thread.sleep(time); //sleep for specified amount of time
	    	 
		} catch (InterruptedException e) { //if error do nothing
	    	
	    	
	    }
		 
		dispose(); //release resources for splash screen
		
	 }

}
