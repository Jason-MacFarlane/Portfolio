/**
 * File name: CalculatorViewController.java
 * Author: Jason MacFarlane, 040890266
 * Course: CST8221 – JAP, Lab Section: 302
 * Assignment: 1, Part 2
 * Date: October 29th, 2019
 * Professor: Daniel Cormier
 * Purpose: Creates a simple GUI for a calculator application, and add functionality
 * Class list: Calculator.java, CalculatorSplashScreen.java, CalculatorViewController.java, CalculatorModel.java
 */

package calculator; //Package statement

//import statements
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTextField;

/**
 * Creates and maintains the GUI for the calculator application
 * 
 * @author Jason MacFarlane
 * @version 1
 * @see calculator
 * @since 1.8.0_191
 */
public class CalculatorViewController extends JPanel { 
	
	/** {@value} serial ID */
	private static final long serialVersionUID = -5903754761523566115L; 
	private JTextField display1; /** Text field used for top part of calculator display */
	private JTextField display2; /** Text field used for bottom part of calculator display */
	private JLabel error; /** Used for error label */
	private JButton dotButton; /** Used to store reference to dot button */
	private JCheckBox modeSelect; /** modeSelect check box */
	private JRadioButton button2; /** RadioButton 2 */
	private JButton[] hexButtons; /** Used to store reference to hex buttons */
	private final String [] BUTTONS; /** {@value} Array holding the text values for buttons */
	
	/**
	 * Constructs a GUI for a simple calculator
	 */
	public CalculatorViewController() {
		
		BUTTONS =  new String[] {"A", "B", "C", "D", "E", "F", "7", "8", "9", "4", "5", "6", "1", "2", "3", ".", "0", "\u00B1"}; /** Initializes buttons values */
		hexButtons = new JButton[6]; /** initialize hex buttons array */
		Controller listener = new Controller(); /** Creates a new Controller object */
		JPanel topCalc = new JPanel(new BorderLayout()); /** Panel for top part of calculator set to BorderLayout */
		JPanel displays = new JPanel(new GridLayout(2,1)); /** Set panels for displays */
		JButton backSpace = new JButton("\u21DA"); /** Initialize BackSpace button */
		JPanel mode = new JPanel(new BorderLayout()); /** Initialize mode Panel */
		modeSelect = new JCheckBox("Hex"); /** Initialize modeSelect check box */
		JRadioButton button1 = new JRadioButton(".0"); /** Initialize RadioButton 1 */
		button2 = new JRadioButton(".00"); /** Initialize RadioButton 2 */
		JRadioButton button3 = new JRadioButton("Sci"); /** Initialize RadioButton 3 */
		ButtonGroup precision = new ButtonGroup(); /** Initialize button group */
		JPanel radioButtons = new JPanel(new GridLayout()); /** Initialize panel to radio buttons */
		JPanel topCalcWithMode = new JPanel(new BorderLayout()); /** Initialize label to hold mode and top calc */
		JPanel leftCalc = new JPanel(new GridLayout(2, 1, 3, 3)); /** Initialize panel for left side of calc */
		JPanel rightCalc = new JPanel(new GridLayout(2, 1, 3, 3)); /** Initialize panel for right side of calc */
		JPanel centerCalc = new JPanel(new BorderLayout()); /** Initialize panel for center of calc */
		JButton multiButton = createButton("*", "*", Color.BLACK, Color.CYAN, listener); /** Initialize button for multiplication */
		JButton diviButton = createButton("/", "/", Color.BLACK, Color.CYAN, listener); /** Initialize button for division */
		JButton addButton = createButton("+", "+", Color.BLACK, Color.CYAN, listener); /** Initialize button for addition */
		JButton subButton = createButton("-", "-", Color.BLACK, Color.CYAN, listener); /** Initialize button for subtraction */
		JButton clearButton = createButton("C", "Cl", Color.BLACK, Color.RED, listener); /** Initialize clear button */
		JButton equalButton = createButton("=", "=", Color.BLACK, Color.YELLOW, listener); /** Initialize equal button */
		JPanel numericButtons = new JPanel(new GridLayout(6,3,3,3)); /** Initialize panel for numericButtons */
		JButton temp; /** declare temp reference for buttons */
		
		this.setLayout(new BorderLayout()); //Sets main panel's layout
		this.setBorder(BorderFactory.createMatteBorder(5, 5, 5, 5, Color.black)); //Sets Border for main panel
		
		topCalc.setBackground(Color.YELLOW); //Set background color
		
		//Set display1's properties
		display1 = new JTextField(14); 
		display1.setEditable(false); 
		display1.setHorizontalAlignment(JTextField.RIGHT); 
		display1.setBackground(Color.WHITE); 
		display1.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 0)); 
		display1.setSize(display1.getWidth(), 30); 
		
		displays.add(display1); //add to panel
		
		//Set display2's properties
		display2 = new JTextField("0.0",14); 
		display2.setEditable(false); 
		display2.setHorizontalAlignment(JTextField.RIGHT); 
		display2.setBackground(Color.WHITE); 
		display2.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 0)); 
		display2.setSize(display2.getWidth(), 30); 
		
		displays.add(display2); //add to panel
		
		topCalc.add(displays, BorderLayout.CENTER);//add displays to topCalc panel
		
		//Set error label properties
		error = new JLabel("F", JLabel.CENTER); 
		error.setPreferredSize(new Dimension(52,55)); 
		error.setBackground(Color.YELLOW); 
		error.setOpaque(true); 
		error.setBorder(BorderFactory.createMatteBorder(0, 1, 0, 5, Color.BLACK)); 
		error.setFont(error.getFont().deriveFont(20f)); 
		
		topCalc.add(error,BorderLayout.WEST); //add error to topCalc panel
		
		//Set backSpace button properties
		backSpace.setPreferredSize(new Dimension(52,55)); 
		backSpace.setBackground(Color.YELLOW);
		backSpace.setOpaque(false);
		backSpace.setContentAreaFilled(false);
		backSpace.setBorder(BorderFactory.createMatteBorder(0, 5, 0, 1, Color.BLACK));
		backSpace.setFont(backSpace.getFont().deriveFont(20f));
		backSpace.setToolTipText("Backspace (Alt-B)");
		backSpace.setActionCommand("<");
		backSpace.addActionListener(listener);
		backSpace.setMnemonic(KeyEvent.VK_B); 
		
		topCalc.add(backSpace, BorderLayout.EAST);//Add backSpace to topCalc panel
		
		//set mode label's properties
		mode.setBorder(BorderFactory.createEmptyBorder(10, 1, 10, 1));
		mode.setBackground(Color.BLACK);
		
		//set modeSelects properties
		modeSelect.setBackground(Color.GREEN);
		modeSelect.setPreferredSize(new Dimension(47,0));
		modeSelect.setActionCommand("Hex");
		modeSelect.addActionListener(listener);
		
		mode.add(modeSelect, BorderLayout.WEST);//add modeSelect to mode label
		
		//Set properties for radio buttons
		button1.setActionCommand(".0");
		button1.addActionListener(listener);
		button1.setActionCommand(".0");
		button1.setOpaque(false);
		button2.setActionCommand(".00");
		button2.addActionListener(listener);
		button2.setActionCommand(".00");
		button2.setOpaque(false);
		button2.setSelected(true);
		button3.setActionCommand("Sci");
		button3.addActionListener(listener);
		button3.setActionCommand("Sci");
		button3.setOpaque(false);
		
		//Add radio buttons to button group
		precision.add(button1);
		precision.add(button2);
		precision.add(button3);
		
		//Add radio buttons to grid layout 
		radioButtons.add(button1);
		radioButtons.add(button2);
		radioButtons.add(button3);
		radioButtons.setBackground(Color.YELLOW);
		
		mode.add(radioButtons, BorderLayout.EAST);//add radio buttons to mode label
		
		//Add topCalc and mode to another border layout 
		topCalcWithMode.add(topCalc, BorderLayout.NORTH);
		topCalcWithMode.add(mode, BorderLayout.SOUTH);
		
		//Set leftCalc properties
		leftCalc.setBackground(Color.BLACK);
		leftCalc.setBorder(BorderFactory.createMatteBorder(0, 1, 0, 5, Color.BLACK));
		
		
		multiButton.setPreferredSize(new Dimension(48,45)); //Set size for multi button
		diviButton.setPreferredSize(new Dimension(48,45)); //Set size for divi button
		
		//Add both buttons to left side of calc
		leftCalc.add(multiButton);
		leftCalc.add(diviButton);
		
		//set rightCalc properties
		rightCalc.setBackground(Color.BLACK);
		rightCalc.setBorder(BorderFactory.createMatteBorder(0, 5, 0, 1, Color.BLACK));
		
		//Set size and add buttons to right side of calc
		addButton.setPreferredSize(new Dimension(48,45));
		subButton.setPreferredSize(new Dimension(48,45));
		rightCalc.add(addButton);
		rightCalc.add(subButton);

		//Add clear and equal buttons to center
		centerCalc.add(clearButton, BorderLayout.NORTH);
		centerCalc.add(equalButton, BorderLayout.SOUTH);

		numericButtons.setBackground(Color.WHITE); //set bg color to white

		//For loop to create numeric buttons
		for(int i = 0; i < 18; i++) {
		
			temp = createButton(BUTTONS[i], BUTTONS[i], Color.BLACK, Color.BLUE, listener); //Initialize button
			
			if(i < 6) { //save hex buttons in array and disable them
				
				temp.setEnabled(false);		
				hexButtons[i] = temp;
				
			}
			
			if(i == 15) { //if dot button set color and save reference
				
				temp.setBackground(Color.MAGENTA);
				dotButton = temp;
				
			}
			
			if(i == 17) { // if plus/minus button set color
				
				temp.setBackground(Color.MAGENTA);
				
			}
			
			numericButtons.add(temp); //add button to panel
			
		}
		
		centerCalc.add(numericButtons, BorderLayout.CENTER);//add numeric buttons to center calc
	
		//add all pieces to calculator
		this.add(topCalcWithMode, BorderLayout.NORTH);
		this.add(leftCalc, BorderLayout.WEST);
		this.add(rightCalc, BorderLayout.EAST);
		this.add(centerCalc, BorderLayout.CENTER);
		
	}
	
	/**
	 * Constructs a JButton object with the desired attributes
	 * 
	 * @param text Text to be displayed on button
	 * 	      ac String for desired Action Command
	 * 	      fg Color for the foreground
	 * 	      bg Color for the background
	 * 	      handler Desired ActionListener for this button
	 * @return the completed JButton object
	 */
	private JButton createButton(String text, String ac, Color fg, Color bg, ActionListener handler) {
		
		JButton tempButton= new JButton(text);//Initialize tempButton
		
		//Set properties
		tempButton.setBackground(bg);
		tempButton.setForeground(fg);
		tempButton.setFont(tempButton.getFont().deriveFont(20f));
		tempButton.addActionListener(handler);
		
		if(ac != null) { //set action command only if ac is not null
			
			tempButton.setActionCommand(ac);
			
		}
		
		return tempButton; //Return button
		
	}
	
	/**
	 * Controller to handle Action Events
	 * 
	 * @author Jason MacFarlane
	 * @version 1
	 * @see calculator
	 * @since 1.8.0_191
	 */
	private class Controller implements ActionListener {
		
		private String display1Content = new String();/** String for what is on the top display */
		private String display2Content = new String(); /** String showing what is inputed through the calculator  */
		private CalculatorModel model = new CalculatorModel(); /** model holding the calculator object */
		private char mode = 'F'; /** current mode of the calculator */
		private boolean operand1IsSet = false; /** indicator for the if the first operand is set */
		private boolean operand2IsSet = false; /** indicator for the if the second operand is set */
		private boolean operatorIsSet = false; /** indicator for the if the operator is set */
		private boolean resultDisplay = false; /** indicator for the if the current display is a result */
		
		/**
		 * Handles events for when a numerical button is pressed
		 * 
		 * @param ac Action command from button

		 */
		private void numericalButton(String ac) {
			
			if(resultDisplay) {
				
				resultDisplay = false; //enable backspace again after result has been shown
				clear();//reset calculator
				
			}
			
			display2Content += ac; //Add it to string 
			
			if(display2Content.equals(".")) {
				
				display2Content = "0."; 
				
			}
			
			display2.setText(display2Content); //Display it
			
		}
		
		/**
		 * Handles events for when an operator button is pressed
		 * 
		 * @param ac Action command from button
		 */
		private void operatorButton(String ac) {
			
			if(resultDisplay) { //if the current display is result, set to false
				
				resultDisplay = false;
				
			}
			
			if(!operand1IsSet && !operatorIsSet) {
			
				if(display2Content.equals("")) { //if no number is inputed default input to 0
					
					display2Content = "0";
					
				}
				
				model.setOperand1(display2Content); //set model operand
				model.setOperator(ac.charAt(0)); //set model operator
				display1Content = display2Content + " " + ac; //concatenate
				display1.setText(display1Content); //display on display1
				display2Content = ""; //reset display2
			
			}else if(operand1IsSet && !operand2IsSet && display2Content.isEmpty()) {
				
				model.setOperator(ac.charAt(0)); //set model operator to new operator
				display1Content = display1Content.substring(0, display1Content.length()-1); //remove old operator from display
				display1Content = display1Content + ac; //add new operator to display
				display1.setText(display1Content); //display
				
			}
			
			//set boolean variables
			operand1IsSet = true;
			operatorIsSet = true;
			
		}
		
		/**
		 * Handles events for when the backspace button is pressed
		 * @param N/A
		 * @return N/A
		 */
		private void backSpaceButton() {
			
			if(!display2Content.isEmpty() && !resultDisplay) { //if display2 has contents
				
				display2Content = display2Content.substring(0, (display2Content.length()-1)); //remove the last char of the string
				display2.setText(display2Content); //display new content
				
				if(display2Content.isEmpty() || display2Content.length() == 1 && display2Content.charAt(0) == '-') { //if nothing is left after backspace
					
					if(mode == 'H') {
						
						display2.setText("0"); //display 0
						
					}else {
						
						display2.setText("0.0"); //display 0.0
						
					}
					
					display2Content = "";
					
				}
				
			}
			
		}
		
		/**
		 * Handles events for with modeSelect check box is selected/unselected
		 * @param N/A
		 * @return N/A
		 */
		private void hexButton() {
			
			if(model.getError()) {
				
				//if in error mode change modes but don't reset calculator until clear function has happened
				if(mode == 'F') {
					
					mode = 'H';
					
				}else {
					
					mode = 'F';
					
				}
				
			}else {
			
				if(mode == 'F') { //if currently in Float mode
					
					//Change properties to Hex mode
					error.setText("H");
					error.setBackground(Color.GREEN);
					mode = 'H';
					
					dotButton.setEnabled(false); //disable dot button
					model.setHexMode(true);
					
					for(int i = 0; i < 6; i++) { //enable all hex buttons
						
						hexButtons[i].setEnabled(true);
						
					}
					
				}else { //if currently in Hex mode
					
					//Change properties to Float mode
					error.setText("F");
					error.setBackground(Color.YELLOW);
					
					if(!modeSelect.isSelected()) {
						
						model.setFloatPercision(2);
						button2.setSelected(true);
						
					}
					modeSelect.setSelected(false);
					mode = 'F';
					
					dotButton.setEnabled(true); //enable dot button
					model.setHexMode(false);
					
					for(int i = 0; i < 6; i++) {
						
						hexButtons[i].setEnabled(false); //disable all hex buttons
						
					}	
					
				}
				
				clear();
			
			}
			
		}
		
		/**
		 * Handles event for when the plus minus button
		 */
		private void plusMinusButton() {
			
			if(!display2Content.isEmpty()) { //if display2 is not empty
			
				if(display2Content.charAt(0) == '-') { //if the number is currently negative
					
					display2Content = display2Content.substring(1, display2Content.length()); //make positive by removing the sign
					
				}else{ //if the number is not negative
					
					display2Content = "-" + display2Content; //add the minus symbol
					
				}
				
				display2.setText(display2Content); //set new text to display
			
			}
			
		}
		
		/**
		 * Handles event for equal button 
		 */
		private void equalButton() {
			
			if(resultDisplay) { //if the equal button is hit again while the result is still showing assign result to operand 1
				
				model.setOperand1(display2Content);
				operatorIsSet = true;
				
			}else {
				
				model.setOperand2(display2Content);
				
			}
			
			if(operatorIsSet) {
				
				//display result
				display2Content = model.getResult();
				
				//Error checking
				if(display2Content.equals("DivideZero")) {
					
					display2.setText("Cannot divide by zero");
					errorMode();
					
				}else if(display2Content.equals("NaN")){
					
					display2.setText("Result is undefined");
					errorMode();
					
				}else if(display2Content.length() > 35){
					
					display2.setText("Result is too long");
					errorMode();
					
				}else if(display2Content.equals("Infinity")){
					
					display2.setText(display2Content);
					errorMode();
					
				}else {
				
					display2.setText(display2Content);
					display1.setText("");
					
				}
				
				//reset operand/operator indicators
				operand1IsSet = false;
				operand2IsSet = false;
				operatorIsSet = false;
				
				resultDisplay = true; // while result is displayed
			
			}
			
		}
		
		/**
		 * Handles events for precision buttons 
		 * @param [String ac] action command
		 * @return N/A
		 */
		private void precisionButton(String ac) {
			
			switch(ac) { //set float precision based on action command
			
			case ".0":
				
				model.setFloatPercision(1);
				break;
				
			case ".00":
				
				model.setFloatPercision(2);
				break;
				
			case "Sci":
				
				model.setFloatPercision(3);
				break;
			
			}
			
			if(!model.getError()) { //if in error mode don't reset calculator until clear function
				
				clear();
				
			}
			
			if(mode == 'H') { //if currently in hex mode switch to float
				
				hexButton();
				
			}		
			
		}
		
		/**
		 * Handles events for clear button
		 * @param N/A
		 * @return N/A
		 */
		private void clear() {
			
			if(model.getError()) {
				
				model.setError(false);
				
				//reverse modes for the purpose of using the hexButton method to set calculator 
				if(mode == 'H') {
					
					mode = 'F';
					hexButton();
					
				}else {
					
					mode = 'H';
					hexButton();
					
				}
				
			}
			
			//reset displays and content
			display1Content = "";
			display2Content = "";
			display1.setText("");
			
			if(mode == 'H') {
				
				display2.setText("0"); //if hex mode
				
			}else {
				
				display2.setText("0.0"); //if float mode
				
			}
			
			model.setOperand1("");
			model.setOperand2("");
			operand1IsSet = false;
			operand2IsSet = false;
			operatorIsSet = false;
			
		}
		
		private void errorMode() {
			
			error.setText("E");
			error.setBackground(Color.RED);
			model.setError(true);
			
		}

		/**
		 * Handles ActionEvents for buttons
		 * @param [ActionEvent e] The action event detected
		 * @return N/A
		 */
		@Override
		public void actionPerformed(ActionEvent e) {
			
			// if in error mode, disable functionality of almost all buttons
			if(!model.getError()) {
			
				switch(e.getActionCommand()) { //Switch statement for different button presses

					case "0":
					case "1":
					case "2":
					case "3":
					case "4":
					case "5":
					case "6":
					case "7":
					case "8":
					case "9":
					case "A":
					case "B":
					case "C":
					case "D":
					case "E":
					case "F":
							
						numericalButton(e.getActionCommand());
						break;
						
					case ".":
							
						if(!display2Content.contains(".")) {
								
							numericalButton(e.getActionCommand());
								
						}
						break;
						
					case "+":
					case "/":
					case "-":
					case "*":
							
						operatorButton(e.getActionCommand());
						break;
							
					case "<":
							
						backSpaceButton();
						break;
							
					case "\u00B1":
							
						plusMinusButton();
						break;
							
					case "=":
							
						equalButton();
						break;
						
						
					case "Hex":
						
						hexButton();
						break;
						
					case ".0":
					case ".00":
					case "Sci":
						
						precisionButton(e.getActionCommand());
						break;
						
					case "Cl":
						
						clear();
						break;
				
		
				}
			
			}else {
				
				switch(e.getActionCommand()) {
				
					case "Hex":
						
						hexButton();
						break;
				
					case ".0":
					case ".00":
					case "Sci":
						
						precisionButton(e.getActionCommand());
						break;
						
					case "Cl":
						
						clear();
						break;
				
				}
				
			}

		}

	}
	
}