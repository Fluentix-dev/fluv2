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
        if other.type not in ("int", "float"):
            error: Error = Error("Type Error", f"cannot add '{self.type}' to '{other.type}'", 9, self.start, other.end)
            error.print()
        
        result = self.value + other.value
        if other.type == "int":
            return Int(result, self.start, other.end)
        
        return Float(result, self.start, self.end)
    
    def sub(self, other: RuntimeValue) -> RuntimeValue:
        """
        Int subtraction
        """
        if other.type not in ("int",):
            error: Error = Error("Type Error", f"cannot subtract '{self.type}' to '{other.type}'", 10, self.start, other.end)
            error.print()
        
        result = self.value - other.value
        if other.type == "int":
            return Int(result, self.start, other.end)
        
        return Float(result, self.start, other.end)
    
    def mul(self, other: RuntimeValue) -> RuntimeValue:
        """
        Int multiplication
        """
        if other.type not in ("int",):
            error: Error = Error("Type Error", f"cannot multiply '{self.type}' to '{other.type}'", 11, self.start, other.end)
            error.print()
        
        result = self.value * other.value
        if other.type == "int":
            return Int(result, self.start, other.end)
        
        return Float(result, self.start, other.end)
    
    def div(self, other: RuntimeValue) -> RuntimeValue:
        """
        Int division
        """
        if other.type not in ("int", "float"):
            error: Error = Error("Type Error", f"cannot divide '{self.type}' to '{other.type}'", 12, self.start, other.end)
            error.print()
        
        if other.value == 0:
            error: Error = Error("Math Error", "division by 0", 14, self.start, other.end)
            error.print()
        
        if other.type == "int":
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
        if other.type not in ("int", "float"):
            error: Error = Error("Type Error", f"cannot add '{self.type}' to '{other.type}'", 9, self.start, other.end)
            error.print()
        
        result = self.value + other.value
        return Float(result, self.start, other.end)
    
    def sub(self, other: RuntimeValue) -> RuntimeValue:
        """
        Float subtraction
        """
        if other.type not in ("int", "float"):
            error: Error = Error("Type Error", f"cannot subtract '{self.type}' to '{other.type}'", 10, self.start, other.end)
            error.print()
        
        result = self.value - other.value
        return Float(result, self.start, other.end)
    
    def mul(self, other: RuntimeValue) -> RuntimeValue:
        """
        Float multiplication
        """
        if other.type not in ("int", "float"):
            error: Error = Error("Type Error", f"cannot multiply '{self.type}' to '{other.type}'", 11, self.start, other.end)
            error.print()
        
        result = self.value * other.value
        return Float(result, self.start, other.end)
    
    def div(self, other: RuntimeValue) -> RuntimeValue:
        """
        Float division
        """
        if other.type not in ("int", "float"):
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

class Scope:
    """
    A scope where all variables are stored
    """
    def __init__(self, parent: Scope | None = None) -> None:
        self.parent = parent
        self.variables: dict[str, RuntimeValue] = {}
        self.constants: set[str] = set()
    
    def declare(self, is_constant: bool, variable_name: str, value: RuntimeValue, start: Position, end: Position) -> None:
        if variable_name in self.variables:
            error: Error = Error("Variable Error", f"'{variable_name}' is already in the scope, cannot redeclare '{variable_name}'", 23, start, end)
            error.print()
        
        self.variables[variable_name] = value
        if is_constant:
            self.constants.add(variable_name)
    
    def assign(self, variable_name: str, value: RuntimeValue, start: Position, end: Position) -> None:
        scope = self.get_scope_from_variable(variable_name)
        if scope is None:
            error: Error = Error("Variable Error", f"'{variable_name}' does not exist, thus cannot be reassigned", 24, start, end)
            error.print()

        if variable_name in scope.constants:
            error: Error = Error("Variable Error", f"'{variable_name}' is a constant, which cannot be assigned to", 25, start, end)
            error.print()
        
        self.variables[variable_name] = value
    
    def get(self, variable_name: str, start: Position, end: Position) -> None:
        scope = self.get_scope_from_variable(variable_name)
        if scope is None:
            error: Error = Error("Variable Error", f"'{variable_name}' does not exist, so their value cannot be retrieved", 26, start, end)
            error.print()
        
        return scope.variables[variable_name].set_position(start, end)
    
    def get_scope_from_variable(self, variable_name: str) -> Scope | None:
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
        self.scope.declare(False, "a", verifier.verify_int("67", Position(self.fn, self.src, 10, 1), Position(self.fn, self.src, 12, 1)), Position(self.fn, self.src, 1, 1), Position(self.fn, self.src, 1, 2))
        print(self.scope.get("a", Position(self.fn, self.src, 1, 2), Position(self.fn, self.src, 2, 2)).value)
        self.scope.assign("a", verifier.verify_int("67674141", Position(self.fn, self.src, 10, 3), Position(self.fn, self.src, 18, 3)), Position(self.fn, self.src, 1, 3), Position(self.fn, self.src, 18, 3))
        print(self.scope.get("a", Position(self.fn, self.src, 1, 4), Position(self.fn, self.src, 2, 4)).value)

if __name__ == "__main__":
    program = Program("files/main.flu", "let a be 67\na\na is now 67674141\na")
    program.main()