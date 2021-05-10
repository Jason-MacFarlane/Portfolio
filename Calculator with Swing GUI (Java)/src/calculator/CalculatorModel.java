package calculator; //Package statement

/**
 * Contains logic for mathematical operations of the Calculator
 * 
 * @author Jason MacFarlane
 * @version 1
 * @see calculator
 * @since 1.8.0_191
 */
public class CalculatorModel {
	
	private String operand1; /** value of first operand */
	private String operand2; /** value of second operand */
	private char operator;/** operator for operation */
	private boolean hexMode; /** is hex mode on? */
	private boolean error; /** is the calculator currently in error mode? */
	private int floatPrecision; /** current float precision */
	
	/**
	 * Constructor for Calculator Model
	 */
	public CalculatorModel() {
		
		//set defaults
		this.error = false;
		this.floatPrecision = 2;
		
	}
	
	/**
	 * Set method for operand1
	 * 
	 * @param operand value to set operand1
	 */
	public void setOperand1(String operand) {
		
		operand1 = operand; //Set variable
		
	}
	
	/**
	 * Set method for operand2
	 * 
	 * @param operand value to set operand2
	 */
	public void setOperand2(String operand) {
		
		operand2 = operand; //Set variable
		
	}
	
	/**
	 * Set method for operator
	 * 
	 * @param operator value to set operator
	 */
	public void setOperator(char operator) {
		
		if(operator == '/' || operator == '*' || operator == '+' || operator == '-') { //check for valid operator
		
			this.operator = operator; //Set variable
		
		}else {
			
			System.out.println("Invalid operator, set operation failed"); //error message
			
		}
		
	}
	
	/**
	 * Set method for operational mode
	 * 
	 * @param isSet is Hex mode on?
	 */
	public void setHexMode(boolean isSet) {
		
		hexMode = isSet; //Set variable
		
	}
	
	/**
	 * Set method for floating point precision
	 * 
	 * @param precision number of decimal point precision
	 */
	public void setFloatPercision(int precision) {
		
		floatPrecision = precision; //set variable
		
	}
	
	/**
	 * Get method for the result of the operation
	 * 
	 * @return the result of the operation
	 */
	public String getResult() {
		
			return calculate();
		
	}
	
	/**
	 * Set method for error state
	 * 
	 * @param error new error state
	 */
	public void setError(boolean error) {
		
		this.error = error; //set error variable
		
	}
	
	/**
	 * Get method for error state
	 * 
	 * @return current error state
	 */
	public boolean getError() {
		
		return error; //return error variable
		
	}
	
	/**
	 * Calculates the result based on current operands, operator, and mode
	 */
	private String calculate() {
		
		Float fOperand1; /** Holds Float value for operand 1 */
		Float fOperand2; /** Holds Float value for operand 2 */
		Float fResult = 0f;  /** Holds Float value for the result */
		Integer iOperand1; /** Holds Integer value for operand 1 */
		Integer iOperand2; /** Holds Integer value for operand 2 */
		Integer iResult = 0; /** Holds Integer value for the result */
		
		if(operand2.isEmpty()) { //if a 2nd operator was not specified use copy operand1 into operand2
			
			operand2 = operand1;
			
		}
		
		if(hexMode) {
			
			//Convert operands to Integer objects
			try {
				
				iOperand1 = Integer.valueOf(operand1, 16);
				iOperand2 = Integer.valueOf(operand2, 16);
				
			}catch(NumberFormatException e) {
				
				return "NaN";
				
			}
			
			//Operation depending on operator
			switch(operator) {
			
				case '+':
					
					iResult = iOperand1 + iOperand2;
					break;
					
				case '-':
					
					iResult = iOperand1 - iOperand2;
					break;
					
				case '/':
					
					//Error checking
					if(iOperand1 == 0 && iOperand2 == 0) { 
						
						return "NaN";
						
					}else if(iOperand2 == 0) {
						
						return "DivideZero";
						
					}
					
					iResult = iOperand1 / iOperand2;
					break;
					
				case '*':
					
					iResult = iOperand1 * iOperand2;
					break;
			
			}
			
			return String.format("%X", iResult); //return the result as a String in the correct format
			
		}else {
			
			//Convert operands to Float objects
			fOperand1 = Float.valueOf(operand1);
			fOperand2 = Float.valueOf(operand2);
			
			//Operation depending on operator
			switch(operator) {
			
			case '+':
				
				fResult = fOperand1 + fOperand2;
				break;
				
			case '-':
				
				fResult = fOperand1 - fOperand2;
				break;
				
			case '/':
				
				//Error checking
				if(fOperand1 == 0 && fOperand2 == 0) {
					
					return "NaN";
					
				}else if(fOperand2 == 0) {
					
					return "DivideZero";
					
				}
				
				fResult = fOperand1 / fOperand2;
				break;
				
			case '*':
				
				fResult = fOperand1 * fOperand2;
				break;
		
			}
			
			//Return the result as a String formatted in the desired format
			switch(floatPrecision) {
			
				case 1:
					
					return String.format("%.1f", fResult);
					
				case 2:
					
					return String.format("%.2f", fResult);
					
				case 3:
					
					return String.format("%E", fResult);
					
			}
			
			return ""; //if fail return nothing
			
		}
		
	}

}
