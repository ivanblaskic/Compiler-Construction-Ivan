/*

	(+	(+	1	1)	(+	2	3))

	- there is a need for + to know where the operators are 

	Rules
	
	- operation take expressions rather then arguments

	- expr evaluates to a number rather then changing the value at memory location or in register

	- infinite variables (there will be need to map them to memory)

	- shadowing :	(let	(x	5	(let	(x	6	x))) --> 6

	R0 --> X0	:	Global Transformation

	R0 --> I0 --> I1 --> I2 --> I3 --> I4 --> X0 
												-- data structure representing instr --> asm-string -- gas/gcc doing this for us --> binary

	R0 --> I0	:	Uniquify Function
	- no duplicate variables any more
	- job is to remove shadowing so variables don't appear multiple times
	- there has to be a mapping solving this problem
	- time stamp counter makes sure each variable gets unique name
	- 2 cases:	var --> lookup mapping
				let	--> add new mapping --> initialization of variable 
					--> [let	(x 10)	(+	x	(let	(x	20)	x))]
	- example:	(+	(let	[x	7]	5)	x)	-->	uniquify will catch variables that are not in scope 
											--> second x consults the mapping, but it's empty so there is an error 
	- preinitialized globals are only mentioned after let

	Implement data structure for R0 and Uniquify!

	Example
	- (+	(+	1	2)	(+	3	4))		-->		x = 1 + 2;	y = 3 + 4;	z = x + y;	ret z;		-->		add_lhs91 = 1+2;	add_rhs92 = 3+4;	ans_to_prog93 = ...;	ret ...;
										-->		names are telling you info where you messed something up if error comes up!

*/

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

#define MAX 50

using namespace std;

#include "R0Language.h"

//"program" matcher
bool R0::check_if_program(string input) {
	string::iterator it;
	int ind = 0, temp_ind = 0;
	if ((input.substr(0, 7)) == "program") {
		return true;
	}
	cout << " - Wrong input - \n";
	return false;
}


//"program" get rider or rather checker
int R0::interpret_input(string input) {
	string::iterator it;
	int ind = 0;

	it = input.begin();

	it++; ind++;

	if (check_if_program(input.substr(ind))) {

		while (*it != '(') {
			it++;
			ind++;
		}

	}

	else {
		cout << "Code has to start with '(program'";
		return -1;
	}

	string expr = input.substr(ind);
	return intrp_R0(expr);
} 


//"read" matcher
bool R0::read_instr(string input) {
	if ((input.substr(0, 4)) == "read") {
		return true;
	}
	return false;
}


//interpreter recursion
int R0::intrp_R0(string input) {

	string::iterator it;

	int ind = 0;
	int ret_val = 0;

	bool integer = false;
	bool read = false;

	it = input.begin();

	if (*it == '(') {
		it++;
		ind++;
	}

	if (read_instr(input.substr(ind))) {
		cout << "Input Value for (read): ";
		cin >> ret_val;
		return ret_val; 
	}

	while ((*it >= '0') && (*it <= '9')) {
		integer = true;
		ret_val *= 10;
		ret_val += (*it - '0');
		ind++;
		it++;
	}

	if (integer) return ret_val;

	if (*it == '+') {
		int arg1, arg2;
		ind++;
		it++;
		
		while (*it == ' ') { 
			ind++;
			it++;
		}
		
		arg1 = intrp_R0(input.substr(ind));
		
		while (*it != ' ') {
			ind++;
			it++;
		}

		while (*it == ' ') {
			ind++;
			it++;
		}
		
		arg2 = intrp_R0(input.substr(ind));
		
		ret_val = arg1 + arg2;

		return (ret_val);
	}

	if (*it == '-') {
		ind++;
		it++;
		while (*it == ' ') {
			ind++;
			it++;
		}
		return (0 - intrp_R0(input.substr(ind)));
	}

}


//partial evaluator - optimizer
string R0::optimizer(string input) {

	string::iterator it;
	int ind = 0;

	int temp_res = 0;

	it = input.begin();

	it++; ind++;

	if (check_if_program(input.substr(ind))) {

		while (*it != '(') {
			it++;
			ind++;
		}

	}

	else {
		cout << "Code has to start with '(program'";
		return "Error input!";
	}

	string expr = input.substr(ind);
	temp_res = optimize_R0(expr);
	if (temp_res == 0) { cout << "Program is already optimized"; }
	return opt_input_expr;
}


//optimizer recursion function
int R0::optimize_R0(string input) {

	opt_input_expr += "(program";

	cout << " - RECURSION CALL - ";
	 
	cout << " - " << opt_input_expr << " - \n";

	string::iterator it;

	int ind = 0;
	int ret_val = 0;
	int tmp_cnt = 0;
	int i_temp = 0;
	string s_temp;

	bool integer = false;
	bool read = false;

	it = input.begin();

	while (*it == ' ') {
		opt_input_expr += ' ';
		it++;
		ind++;
	}

	while (*it == ')') {
		opt_input_expr += ')';
		it++;
		ind++;
	}

	while (*it == '(') {
		opt_input_expr += "(";
		it++;
		ind++;
	}

	if (read_instr(input.substr(ind))) {
		opt_input_expr += "read";
		return 0;
	}

	while ((*it >= '0') && (*it <= '9')) {
		integer = true;
		ret_val *= 10;
		ret_val += (*it - '0');
		ind++;
		it++;
	}

	if (integer) return ret_val;
	//else return 0;					//(read) case

	if (*it == '+') {
		opt_input_expr += '+';
		int arg1, arg2;
		ind++;
		it++;

		while (*it == ' ') {
			opt_input_expr += ' ';
			ind++;
			it++;
		}

		arg1 = intrp_R0(input.substr(ind));

		while (*it != ' ') {
			ind++;
			it++;
		}

		while (*it == ' ') {
			ind++;
			it++;
		}

		arg2 = intrp_R0(input.substr(ind));

		if ((arg1 != 0) && (arg2 != 0)) {
			ret_val = arg1 + arg2;					// 512
			while (ret_val > 0) {					// 512 > 0, 51 > 0, 5 > 0, 0 = 0
				i_temp = ret_val % 10;				// 2, 1, 5
				ret_val /= 10;						// 51, 5, 0
				s_temp = "0" + i_temp;				// s_temp: '2','1','5' --> "215"
				tmp_cnt++;							// 1,2,3
			}
			for (int i = (tmp_cnt-1); tmp_cnt >= 0; tmp_cnt--) {		// i = 2
				opt_input_expr += s_temp[tmp_cnt];						// s_temp[2] = '5', opt_input_expr = "...512", s_temp [1] = '1', s_temp [0] = '2'  
			}
		}

		else if (arg1 != 0) {
			while (arg1 > 0) {					// 512 > 0, 51 > 0, 5 > 0, 0 = 0
				i_temp = arg1 % 10;				// 2, 1, 5
				arg1 /= 10;						// 51, 5, 0
				s_temp = "0" + i_temp;				// s_temp: '2','1','5' --> "215"
				tmp_cnt++;							// 1,2,3
			}
			for (int i = (tmp_cnt - 1); tmp_cnt >= 0; tmp_cnt--) {		// i = 2
				opt_input_expr += s_temp[tmp_cnt];						// s_temp[2] = '5', opt_input_expr = "...512", s_temp [1] = '1', s_temp [0] = '2'  
			}
		}

		else if (arg2 != 0) {
			while (arg2 > 0) {					// 512 > 0, 51 > 0, 5 > 0, 0 = 0
				i_temp = arg2 % 10;				// 2, 1, 5
				arg2 /= 10;						// 51, 5, 0
				s_temp = "0" + i_temp;				// s_temp: '2','1','5' --> "215"
				tmp_cnt++;							// 1,2,3
			}
			for (int i = (tmp_cnt - 1); tmp_cnt >= 0; tmp_cnt--) {		// i = 2
				opt_input_expr += s_temp[tmp_cnt];						// s_temp[2] = '5', opt_input_expr = "...512", s_temp [1] = '1', s_temp [0] = '2'  
			}
		}

		return (ret_val);
	}

	if (*it == '-') {
		ind++;
		it++;
		while (*it == ' ') {
			ind++;
			it++;
		}
		int arg1 = intrp_R0(input.substr(ind));
		if (arg1 != 0) {
			arg1 = 0 - intrp_R0(input.substr(ind));
			while (arg1 > 0) {					// 512 > 0, 51 > 0, 5 > 0, 0 = 0
				i_temp = arg1 % 10;				// 2, 1, 5
				arg1 /= 10;						// 51, 5, 0
				s_temp = "0" + i_temp;				// s_temp: '2','1','5' --> "215"
				tmp_cnt++;							// 1,2,3
			}
			for (int i = (tmp_cnt - 1); tmp_cnt >= 0; tmp_cnt--) {		// i = 2
				opt_input_expr += s_temp[tmp_cnt];						// s_temp[2] = '5', opt_input_expr = "...512", s_temp [1] = '1', s_temp [0] = '2'  
			}
			return (arg1);
			
		}
		else
			return 0;
	}

}


//class tester 
int main() {

	R0("(program (+ 5 3))");

	cout << "\n\tLADIES N GENTLEMEN MY INTERPRETER IS WORKING PROPERLY! \n";
	system("pause");

}

