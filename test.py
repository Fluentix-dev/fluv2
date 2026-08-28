from __future__ import annotations
import sys

class Position:
    """
    Represents the position of some parts of the source code
    """
    def __init__(self, fn: str, src: str, column: int, line: int) -> None:
        self.fn = fn
        self.src = src
        self.column = column
        self.line = line

class Error:
    """
    Represents the error in the backend
    """
    def __init__(self, type: str, details: str, error_code: int, start: Position, end: Position) -> None:
        self.type = type
        self.details = details
        self.error_code = error_code
        self.start = start
        self.end = end
    
    def print(self) -> None:
        """
        Prints out the error
        """
        print(f"The transpiled code was running but stopped because of error #{self.error_code}")
        lines: list[str] = []
        current_line: str = ""
        for c in self.start.src + "\n":
            if c == "\n":
                lines += [current_line]
                current_line = ""
            else:
                current_line += c
        
        end_digits = len(str(self.end.line))
        if self.start.line == self.end.line:
            print(f"{self.start.line} || {lines[self.start.line-1]}")
            for i in range(1, self.start.column+end_digits+4):
                print(" ", end="")
            
            for i in range(self.start.column, self.end.column):
                print("^", end="")
            
            print()
        else:
            for i in range(self.start.line-1, self.end.line):
                i_digits = len(str(i+1))
                for j in range(i_digits, end_digits):
                    print(0, end="")
                
                print(f"{i_digits} || {lines[i]}")
                for j in range(end_digits+4):
                    print(" ", end="")
                
                if i == self.start.line-1:
                    for j in range(1, self.start.column):
                        print(" ", end="")
                    
                    for j in range(self.start.column, len(lines[i])+1):
                        print("^", end="")
                elif i == self.end.line-1:
                    for j in range(1, self.end.column):
                        print("^", end="")
                else:
                    for c in lines[i]:
                        print("^", end="")
                
                print()
        
        print(f"{self.type}: {self.details}")
        sys.exit(self.error_code)

class RuntimeValue:
    """
    Represents a runtime value
    """
    def __init__(self, type: str, start: Position, end: Position) -> None:
        self.type = type
        self.start = start
        self.end = end
    
    def set_position(self, start: Position, end: Position) -> RuntimeValue:
        """
        Resets the position of something and returns itself
        """
        self.start = start
        self.end = end
        return self

    def add(self, other: RuntimeValue) -> RuntimeValue:
        """
        Addition
        """
        error: Error = Error("Type Error", f"cannot add '{self.type}' to '{other.type}'", 5, self.start, other.end)
        error.print()
    
    def sub(self, other: RuntimeValue) -> RuntimeValue:
        """
        Subtraction
        """
        error: Error = Error("Type Error", f"cannot subtract '{self.type}' to '{other.type}'", 6, self.start, other.end)
        error.print()
    
    def mul(self, other: RuntimeValue) -> RuntimeValue:
        """
        Multiplication
        """
        error: Error = Error("Type Error", f"cannot multiply '{self.type}' to '{other.type}'", 7, self.start, other.end)
        error.print()
    
    def div(self, other: RuntimeValue) -> RuntimeValue:
        """
        Division
        """
        error: Error = Error("Type Error", f"cannot divide '{self.type}' to '{other.type}'", 8, self.start, other.end)
        error.print()
    
    def is_true(self) -> bool:
        """
        Returns if a value is considered a truthy or a falsy value
        """
        return True

    def equals(self, other: RuntimeValue) -> Boolean:
        """
        Equality
        """
        error: Error = Error("Type Error", f"cannot check equality for '{self.type}' to '{other.type}'", 34, self.start, other.end)
        error.print()

    def not_equals(self, other: RuntimeValue) -> Boolean:
        """
        Inequality
        """
        error: Error = Error("Type Error", f"cannot check inequality for '{self.type}' to '{other.type}'", 35, self.start, other.end)
        error.print()

    def greater_than(self, other: RuntimeValue) -> Boolean:
        """
        Greater than
        """
        error: Error = Error("Type Error", f"cannot perform operation '>' on '{self.type}' and '{other.type}'", 36, self.start, other.end)
        error.print()

    def greater_than_or_equals(self, other: RuntimeValue) -> Boolean:
        """
        Greater than or equals
        """
        error: Error = Error("Type Error", f"cannot perform operation '>=' on '{self.type}' and '{other.type}'", 37, self.start, other.end)
        error.print()

    def smaller_than(self, other: RuntimeValue) -> Boolean:
        """
        Smaller than
        """
        error: Error = Error("Type Error", f"cannot perform operation '<' on '{self.type}' and '{other.type}'", 38, self.start, other.end)
        error.print()

    def smaller_than_or_equals(self, other: RuntimeValue) -> Boolean:
        """
        Smaller than or equals
        """
        error: Error = Error("Type Error", f"cannot perform operation '<=' on '{self.type}' and '{other.type}'", 39, self.start, other.end)
        error.print()

class Int(RuntimeValue):
    """
    Represents an integer
    """
    def __init__(self, value: int, start: Position, end: Position) -> None:
        super().__init__("int", start, end)
        self.value = value
    
    def add(self, other: RuntimeValue) -> RuntimeValue:
        """
        Int addition
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot add '{self.type}' to '{other.type}'", 9, self.start, other.end)
            error.print()
        
        result = self.value + other.value
        if other.type in ("int", "boolean"):
            return Int(result, self.start, other.end)
        
        return Float(result, self.start, self.end)
    
    def sub(self, other: RuntimeValue) -> RuntimeValue:
        """
        Int subtraction
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot subtract '{self.type}' to '{other.type}'", 10, self.start, other.end)
            error.print()
        
        result = self.value - other.value
        if other.type in ("int", "boolean"):
            return Int(result, self.start, other.end)
        
        return Float(result, self.start, other.end)
    
    def mul(self, other: RuntimeValue) -> RuntimeValue:
        """
        Int multiplication
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot multiply '{self.type}' to '{other.type}'", 11, self.start, other.end)
            error.print()
        
        result = self.value * other.value
        if other.type in ("int", "boolean"):
            return Int(result, self.start, other.end)
        
        return Float(result, self.start, other.end)
    
    def div(self, other: RuntimeValue) -> RuntimeValue:
        """
        Int division
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot divide '{self.type}' to '{other.type}'", 12, self.start, other.end)
            error.print()
        
        if other.value == 0:
            error: Error = Error("Math Error", "division by 0", 14, self.start, other.end)
            error.print()
        
        if other.type in ("int", "boolean"):
            result = self.value // other.value
            return Int(result, self.start, other.end)
        
        result = self.value / other.value
        return Float(result, self.start, other.end)
    
    def positive(self, start: Position) -> RuntimeValue:
        """
        Unary plus on int
        """
        return Int(self.value, start, self.end)
    
    def negative(self, start: Position) -> RuntimeValue:
        """
        Unary subtraction on int
        """
        return Int(-self.value, start, self.end)
    
    def is_true(self) -> bool:
        """
        Returns if an integer is truthy or falsy
        """
        return self.value != 0

    def equals(self, other: RuntimeValue) -> Boolean:
        """
        Integer equality
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot check equality on 'int' and '{other.type}'", 40, self.start, other.end)
            error.print()

        return Boolean(self.value == other.value, self.start, other.end)

    def not_equals(self, other: RuntimeValue) -> Boolean:
        """
        Integer inequality
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot check inequality on 'int' and '{other.type}'", 41, self.start, other.end)
            error.print()

        return Boolean(self.value != other.value, self.start, other.end)

    def greater_than(self, other: RuntimeValue) -> Boolean:
        """
        Greater than
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot perform operation > on 'int' and '{other.type}'", 42, self.start, other.end)
            error.print()

        return Boolean(self.value > other.value, self.start, other.end)

    def greater_than_or_equals(self, other: RuntimeValue) -> Boolean:
        """
        Greater than or equals
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot perform operation >= on 'int' and '{other.type}'", 43, self.start, other.end)
            error.print()

        return Boolean(self.value >= other.value, self.start, other.end)

    def smaller_than(self, other: RuntimeValue) -> Boolean:
        """
        Smaller than
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot perform operation < on 'int' and '{other.type}'", 44, self.start, other.end)
            error.print()

        return Boolean(self.value < other.value, self.start, other.end)

    def smaller_than_or_equals(self, other: RuntimeValue) -> Boolean:
        """
        Smaller than or equals
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot perform operation <= on 'int' and '{other.type}'", 45, self.start, other.end)
            error.print()

        return Boolean(self.value <= other.value, self.start, other.end)

class Float(RuntimeValue):
    """
    Represents a float
    """
    def __init__(self, value: float, start: Position, end: Position) -> None:
        super().__init__("float", start, end)
        self.value = value
    
    def add(self, other: RuntimeValue) -> RuntimeValue:
        """
        Float addition
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot add '{self.type}' to '{other.type}'", 9, self.start, other.end)
            error.print()
        
        result = self.value + other.value
        return Float(result, self.start, other.end)
    
    def sub(self, other: RuntimeValue) -> RuntimeValue:
        """
        Float subtraction
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot subtract '{self.type}' to '{other.type}'", 10, self.start, other.end)
            error.print()
        
        result = self.value - other.value
        return Float(result, self.start, other.end)
    
    def mul(self, other: RuntimeValue) -> RuntimeValue:
        """
        Float multiplication
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot multiply '{self.type}' to '{other.type}'", 11, self.start, other.end)
            error.print()
        
        result = self.value * other.value
        return Float(result, self.start, other.end)
    
    def div(self, other: RuntimeValue) -> RuntimeValue:
        """
        Float division
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot divide '{self.type}' to '{other.type}'", 12, self.start, other.end)
            error.print()
        
        if other.value == 0:
            error: Error = Error("Math Error", "division by 0", 15, self.start, other.end)
            error.print()

        result = self.value / other.value
        return Float(result, self.start, other.end)
    
    def positive(self, start: Position) -> RuntimeValue:
        """
        Unary plus on float
        """
        return Float(self.value, start, self.end)
    
    def negative(self, start: Position) -> RuntimeValue:
        """
        Unary subtraction on float
        """
        return Float(-self.value, start, self.end)
    
    def is_true(self) -> bool:
        """
        Returns if a float is truthy or falsy
        """
        return self.value != 0

    def equals(self, other: RuntimeValue) -> Boolean:
        """
        Float equality
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot check equality on 'float' and '{other.type}'", 46, self.start, other.end)
            error.print()

        return Boolean(self.value == other.value, self.start, other.end)

    def not_equals(self, other: RuntimeValue) -> Boolean:
        """
        Integer inequality
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot check inequality on 'float' and '{other.type}'", 47, self.start, other.end)
            error.print()

        return Boolean(self.value != other.value, self.start, other.end)

    def greater_than(self, other: RuntimeValue) -> Boolean:
        """
        Greater than
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot perform operation > on 'float' and '{other.type}'", 48, self.start, other.end)
            error.print()

        return Boolean(self.value > other.value, self.start, other.end)

    def greater_than_or_equals(self, other: RuntimeValue) -> Boolean:
        """
        Greater than or equals
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot perform operation >= on 'float' and '{other.type}'", 49, self.start, other.end)
            error.print()

        return Boolean(self.value >= other.value, self.start, other.end)

    def smaller_than(self, other: RuntimeValue) -> Boolean:
        """
        Smaller than
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot perform operation < on 'float' and '{other.type}'", 50, self.start, other.end)
            error.print()

        return Boolean(self.value < other.value, self.start, other.end)

    def smaller_than_or_equals(self, other: RuntimeValue) -> Boolean:
        """
        Smaller than or equals
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot perform operation <= on 'float' and '{other.type}'", 51, self.start, other.end)
            error.print()

        return Boolean(self.value <= other.value, self.start, other.end)

class Boolean(RuntimeValue):
    """
    Represents a boolean
    """
    def __init__(self, value: bool, start: Position, end: Position) -> None:
        super().__init__("boolean", start, end)
        self.value = value
    
    def add(self, other: RuntimeValue) -> RuntimeValue:
        """
        Boolean addition
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot add '{self.type}' to '{other.type}'", 9, self.start, other.end)
            error.print()
        
        result = self.value + other.value
        if other.type in ("int", "boolean"):
            return Int(result, self.start, other.end)
        
        return Float(result, self.start, self.end)
    
    def sub(self, other: RuntimeValue) -> RuntimeValue:
        """
        Boolean subtraction
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot subtract '{self.type}' to '{other.type}'", 10, self.start, other.end)
            error.print()
        
        result = self.value - other.value
        if other.type in ("int", "boolean"):
            return Int(result, self.start, other.end)
        
        return Float(result, self.start, other.end)
    
    def mul(self, other: RuntimeValue) -> RuntimeValue:
        """
        Boolean multiplication
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot multiply '{self.type}' to '{other.type}'", 11, self.start, other.end)
            error.print()
        
        result = self.value * other.value
        if other.type in ("int", "boolean"):
            return Int(result, self.start, other.end)
        
        return Float(result, self.start, other.end)
    
    def div(self, other: RuntimeValue) -> RuntimeValue:
        """
        Boolean division
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot divide '{self.type}' to '{other.type}'", 12, self.start, other.end)
            error.print()
        
        if other.value == 0:
            error: Error = Error("Math Error", "division by 0", 14, self.start, other.end)
            error.print()
        
        if other.type in ("int", "boolean"):
            result = self.value // other.value
            return Int(result, self.start, other.end)
        
        result = self.value / other.value
        return Float(result, self.start, other.end)
    
    def positive(self, start: Position) -> RuntimeValue:
        """
        Unary plus on Boolean
        """
        return Int(self.value, start, self.end)
    
    def negative(self, start: Position) -> RuntimeValue:
        """
        Unary subtraction on Boolean
        """
        return Int(-self.value, start, self.end)
    
    def is_true(self) -> bool:
        """
        Returns if a boolean is truthy or falsy
        """
        return self.value

    def equals(self, other: RuntimeValue) -> Boolean:
        """
        Boolean equality
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot check equality on 'boolean' and '{other.type}'", 52, self.start, other.end)
            error.print()

        return Boolean(self.value == other.value, self.start, other.end)

    def not_equals(self, other: RuntimeValue) -> Boolean:
        """
        Integer inequality
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot check inequality on 'boolean' and '{other.type}'", 53, self.start, other.end)
            error.print()

        return Boolean(self.value != other.value, self.start, other.end)

    def greater_than(self, other: RuntimeValue) -> Boolean:
        """
        Greater than
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot perform operation > on 'boolean' and '{other.type}'", 54, self.start, other.end)
            error.print()

        return Boolean(self.value > other.value, self.start, other.end)

    def greater_than_or_equals(self, other: RuntimeValue) -> Boolean:
        """
        Greater than or equals
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot perform operation >= on 'boolean' and '{other.type}'", 55, self.start, other.end)
            error.print()

        return Boolean(self.value >= other.value, self.start, other.end)

    def smaller_than(self, other: RuntimeValue) -> Boolean:
        """
        Smaller than
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot perform operation < on 'boolean' and '{other.type}'", 56, self.start, other.end)
            error.print()

        return Boolean(self.value < other.value, self.start, other.end)

    def smaller_than_or_equals(self, other: RuntimeValue) -> Boolean:
        """
        Smaller than or equals
        """
        if other.type not in ("int", "float", "boolean"):
            error: Error = Error("Type Error", f"cannot perform operation <= on 'boolean' and '{other.type}'", 57, self.start, other.end)
            error.print()

        return Boolean(self.value <= other.value, self.start, other.end)

class Scope:
    """
    A scope where all variables are stored
    """
    def __init__(self, parent: Scope | None = None) -> None:
        self.parent = parent
        self.variables: dict[str, RuntimeValue] = {}
        self.constants: set[str] = set()

        if self.parent is None:
            self.variables["true"] = Boolean(True, Position("", "", 0, 0), Position("", "", 0, 0))
            self.constants.add("true")

            self.variables["false"] = Boolean(False, Position("", "", 0, 0), Position("", "", 0, 0))
            self.constants.add("false")

    def declare(self, is_constant: bool, variable_name: str, value: RuntimeValue, start: Position, end: Position) -> None:
        """
        Declares a variable
        """
        if variable_name in self.variables:
            error: Error = Error("Variable Error", f"'{variable_name}' is already in the scope, cannot redeclare '{variable_name}'", 23, start, end)
            error.print()
        
        self.variables[variable_name] = value
        if is_constant:
            self.constants.add(variable_name)
    
    def assign(self, variable_name: str, value: RuntimeValue, start: Position, end: Position) -> None:
        """
        Assigns a variable
        """
        scope = self.get_scope_from_variable(variable_name)
        if scope is None:
            error: Error = Error("Variable Error", f"'{variable_name}' does not exist, thus cannot be reassigned", 24, start, end)
            error.print()

        if variable_name in scope.constants:
            error: Error = Error("Variable Error", f"'{variable_name}' is a constant, which cannot be assigned to", 25, start, end)
            error.print()
        
        self.variables[variable_name] = value
    
    def get(self, variable_name: str, start: Position, end: Position) -> None:
        """
        Retrieves the value of a variable
        """
        scope = self.get_scope_from_variable(variable_name)
        if scope is None:
            error: Error = Error("Variable Error", f"'{variable_name}' does not exist, so their value cannot be retrieved", 26, start, end)
            error.print()
        
        return scope.variables[variable_name].set_position(start, end)
    
    def get_scope_from_variable(self, variable_name: str) -> Scope | None:
        """
        Finds which scope the variable lives in
        """
        if variable_name in self.variables:
            return self
        
        if self.parent is None:
            return None
        
        return self.parent.get_scope_from_variable(variable_name)

class Verifier:
    """
    A verifier for each primitive data type
    """
    def __init__(self) -> None:
        pass
    
    def verify_int(self, literal: str, start: Position, end: Position) -> Int:
        """
        Verifies an int literal
        """
        try:
            return Int(int(literal), start, end)
        except Exception:
            error: Error = Error("Overflow Error", f"value '{literal}' exceeds the integer limit", 13, start, end)
            error.print()
    
    def verify_float(self, literal: str, start: Position, end: Position) -> Float:
        """
        Verifies a float literal
        """
        return Float(float(literal), start, end)

verifier = Verifier()

class Program:
    """
    Represents the program
    """
    def __init__(self, fn: str, src: str) -> None:
        self.fn = fn
        self.src = src
        self.scope = Scope()
    
    def main(self) -> None:
        """
        Your code
        """
        self.scope.declare(False, "a", verifier.verify_int("1", Position(self.fn, self.src, 10, 1), Position(self.fn, self.src, 11, 1)), Position(self.fn, self.src, 1, 1), Position(self.fn, self.src, 1, 2))
        self.scope.declare(False, "b", verifier.verify_int("2", Position(self.fn, self.src, 10, 2), Position(self.fn, self.src, 11, 2)), Position(self.fn, self.src, 1, 2), Position(self.fn, self.src, 1, 3))
        if self.scope.get("a", Position(self.fn, self.src, 4, 3), Position(self.fn, self.src, 5, 3)).equals(self.scope.get("b", Position(self.fn, self.src, 8, 3), Position(self.fn, self.src, 9, 3))).is_true():
            print(self.scope.get("a", Position(self.fn, self.src, 5, 4), Position(self.fn, self.src, 6, 4)).value)
            print(self.scope.get("b", Position(self.fn, self.src, 5, 5), Position(self.fn, self.src, 6, 5)).value)
        while not self.scope.get("a", Position(self.fn, self.src, 15, 7), Position(self.fn, self.src, 16, 7)).smaller_than(self.scope.get("b", Position(self.fn, self.src, 19, 7), Position(self.fn, self.src, 20, 7))).is_true():
            print(self.scope.get("a", Position(self.fn, self.src, 5, 8), Position(self.fn, self.src, 6, 8)).value)
            print(self.scope.get("b", Position(self.fn, self.src, 5, 9), Position(self.fn, self.src, 6, 9)).value)


if __name__ == "__main__":
    program = Program("files/main.flu", "let a be 1\nlet b be 2\nif a = b then\n    a\n    b\n\nrepeat: until a < b\n    a\n    b")
    program.main()