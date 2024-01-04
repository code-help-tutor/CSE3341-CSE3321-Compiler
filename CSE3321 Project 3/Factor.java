https://tutorcs.com
WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
class Factor {
	Id id;
	Expr index;
	int constant;
	Expr expr;
	
	void parse() {
		if (Parser.scanner.currentToken() == Core.ID) {
			id = new Id();
			id.parse();
			if (Parser.scanner.currentToken() == Core.LBRACE) {
				Parser.scanner.nextToken();
				index = new Expr();
				index.parse();
				Parser.expectedToken(Core.RBRACE);
				Parser.scanner.nextToken();
				
			}
		} else if (Parser.scanner.currentToken() == Core.CONST) {
			constant = Parser.scanner.getConst();
			Parser.scanner.nextToken();
		} else if (Parser.scanner.currentToken() == Core.LPAREN) {
			Parser.scanner.nextToken();
			expr = new Expr();
			expr.parse();
			Parser.expectedToken(Core.RPAREN);
			Parser.scanner.nextToken();
		} else {
			System.out.println("ERROR: Expected ID, CONST, LPAREN, or IN, recieved " + Parser.scanner.currentToken());
			System.exit(0);
		}
	}
	
	void print() {
		if (id != null) {
			id.print();
			if (index != null) {
				System.out.print("[");
				index.print();
				System.out.print("]");
			}
		} else if (expr != null) {
			System.out.print("(");
			expr.print();
			System.out.print(")");
		} else {
			System.out.print(constant);
		}
	}
	
	int execute() {
		int value;
		if (id != null && index != null) {
			value = Memory.load(id.getId(), index.execute());
		} else if (id != null) {
			value = Memory.load(id.getId());
		} else if (expr != null) {
			value = expr.execute();
		} else {
			value = constant;
		}
		return value;
	}
}