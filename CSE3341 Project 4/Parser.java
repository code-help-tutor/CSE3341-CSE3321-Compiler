https://tutorcs.com
WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
import java.util.*;

class Parser {
	//scanner is stored here as a static field so it is avaiable to the parse method
	public static Scanner scanner;
	
	//helper method for handling error messages, used by the parse methods
	static void expectedToken(Core expected) {
		if (scanner.currentToken() != expected) {
			System.out.println("ERROR: Expected " + expected + ", recieved " + scanner.currentToken());
			System.exit(0);
		}
	}

}