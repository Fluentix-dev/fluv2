# fluv2
Flu is a programming language that the Fluentix community is trying to revive

# Commands
- `flu help` to get all of the commands Flu supports
- `flu --version` to get the current version of Flu
- `flu conv <file_name> <dest_name>` to transpile the source code to Python code into destination name
- `flu clean` to clear the terminal

# Error codes
- Error `#1`: There is a number with more than 1 decimal point. Classified as a Syntax Error
- Error `#2`: Unexpected character. Classified as a Syntax Error
- Error `#3`: Unexpected token, basically a syntax error. Classified as a Syntax Error
- Error `#4`: Expecting `)` to close off `(`. Classified as a Syntax Error
- Error `#5`: Cannot add something to something. Classified as a Type Error
- Error `#6`: Cannot subtract something to something. Classified as a Type Error
- Error `#7`: Cannot multiply something to something. Classified as a Type Error
- Error `#8`: Cannot divide something to something. Classfied as a Type Error
- Error `#9`: Cannot add an `int` to a non-number. Classified as a Type Error
- Error `#10`: Cannot subtract an `int` to a non-number. Classified as a Type Error
- Error `#11`: Cannot multiply an `int` to a non-number. Classified as a Type Error
- Error `#12`: Cannot divide an `int` to a non-number. Classified as a Type Error
- Error `#13`: Value literal for `int` is overflowed. Classified as an Overflow Error
- Error `#14`: Division by 0 where the dividend is an `int`. Classified as a Math Error
- Error `#15`: Division by 0 where the dividend is a `float`. Classified as a Math Error
- Error `#16`: Expecting a variable name in the declaration `variable <var_name> is <assigned_val>`. Classified as a Syntax Error
- Error `#17`: Expecting `is` in the declaration `variable <var_name> is <assigned_val>`. Classified as a Syntax Error
- Error `#18`: Expecting a variable name in the declaration `let <var_name> be <assigned_val>`. Classified as a Syntax Error
- Error `#19`: Expecting `be` in the declaration `let <var_name> be <assigned_val>`. Classified as a Syntax Error
- Error `#20`: Expecting a variable name in the declaration `constant <var_name> is <assigned_val>`. Classified as a Syntax Error
- Error `#21`: Expecting `is` in the declaration `constant <var_name> is <assigned_val>`. Classified as a Syntax Error
- Error `#22`: Expecting a newline at the end of each statement. Classified as a Syntax Error
- Error `#23`: Cannot redeclare a variable. Classified as a Variable Error
- Error `#24`: Cannot assign to a variable that isn't already declared/existed. Classified as a Variable Error
- Error `#25`: Cannot assign to a constant variable. Classified as a Variable Error
- Error `#26`: Cannot get a variable that does not exist. Classfied as a Variable Error
- Error `#27`: Assignment gone wrong. Classified as a Syntax Error