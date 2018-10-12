/*

	exp ::= int |
			(read) |
			(- exp) |
			(+ exp exp) |
			var |
			(let ([var exp]) exp)
	R1 ::= (program exp)

	Example Program:
	main : here is the input program --> get result
		(program
			(let	([x (+12 20)])	(+10 x)))

	Compiled Program:


	(program (let (x (+ (read) 20)) (+ 10 x)))
	
*/

#pragma once

#ifndef R0LANGUAGE_H
#define R0LANGUAGE_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

class R0 {

private:

	string input_expr;						//program stored
	int result;								//result
	string opt_input_expr;					//optimized input expression
	
	bool check_if_program(string input); 	// checking if command is "program"
	int interpret_input(string input);		//program interpreter
	bool read_instr(string input);			//helper for read
	int intrp_R0(string input);				//recursion interp
	string optimizer(string input);			//partial evaluator
	int optimize_R0(string input);		//recursion partial evaluator

public:

	//constructor
	R0(string input) {
		input_expr = input;
		result = interpret_input(input);
		opt_input_expr = optimizer(input);
		cout << "\nProgram:  " << input_expr << "  \n\tInterpreted has value: " << result << " \n";
		cout << "\nOptimized Program: " << opt_input_expr << "\n";
	}

};

#endif // !R0LANGUAGE_H




