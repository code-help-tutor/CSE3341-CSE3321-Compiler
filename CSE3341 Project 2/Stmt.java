https://tutorcs.com
WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
//Stmt is an interface so we can take advantage of some polymorphism in StmtSeq
interface Stmt {
	void parse();
	void print(int indent);
}