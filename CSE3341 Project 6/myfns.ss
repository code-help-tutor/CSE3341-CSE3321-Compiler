https://tutorcs.com
WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
(define (myinterpreter x)
	;(display x) 
	;(newline)
	(mainEval (cadr x))
)

(define (mainEval x) 
	;(display x) 
	;(newline)
	(cond
		((integer? x) x)
	)
)