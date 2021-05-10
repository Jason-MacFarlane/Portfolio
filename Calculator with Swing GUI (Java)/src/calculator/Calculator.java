/**
 * File name: Calculator.java
 * Author: Jason MacFarlane, 040890266
 * Course: CST8221 – JAP, Lab Section: 302
 * Assignment: 1, Part 2
 * Date: October 29th, 2019
 * Professor: Daniel Cormier
 * Purpose: Creates a simple GUI for a calculator application, and add functionality
 * Class list: Calculator.java, CalculatorSplashScreen.java, CalculatorViewController.java, CalculatorModel.java
 */

package calculator; //Package statement


//Import statements
import java.awt.Dimension;
import java.awt.EventQueue;
import javax.swing.JFrame;

/**
 * Contains main function that runs the application by creating the frame and a splash screen
 * 
 * @author Jason MacFarlane
 * @version 1
 * @see calculator
 * @since 1.8.0_191
 */
public class Calculator {
	
	/**
	 * main function that runs the application by creating the frame and a splash screen
	 * 
	 * @param  args command line arguments
	 */
	public static void main(String[] args) {
		
		int splashScreenTime = 5000; /** Time to display splash screen */
		CalculatorSplashScreen splashScreen = new CalculatorSplashScreen(splashScreenTime); /** Splash screen object */
		
		splashScreen.showSplashWindow(); //display splash screen
		
		EventQueue.invokeLater(new Runnable() { //Anonymous class
			
			/**
			 * Overrides run and starts the application
			 * @param N/A
			 * @return N/A
			 */
			@Override
			public void run() {
				
				JFrame frame = new JFrame("Calculator"); /** create new frame */
				
				//set properties for frame
				frame.setMinimumSize(new Dimension(380, 540)); 
		        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		        frame.setContentPane(new CalculatorViewController());
		        frame.setLocationByPlatform(true);
		        frame.setVisible(true);
				
			}
					
		});
		
	}

}

