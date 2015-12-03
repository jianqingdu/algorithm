#
# using recursive descent parser for simple calculator, e.g.:
# 1 + (12 - 3) / (3 * 3) 
#
INTEGER, PLUS, MINUS, MULTIPLY, DIVIDE, LPAREN, RPAREN, EOF = (\
    'INTEGER', 'PLUS', 'MINUS', 'MULTIPLY', 'DIVIDE', '(', ')', 'EOF')

class Token(object):
    def __init__(self, type, value):
        # INTEGER, PLUS, EOF
        self.type = type
        self.value = value

    def __str__(self):
        return "Token({type}, {value})".format(type=self.type, value=repr(self.value))

    def __repr__(self):
        return self.__str__()

class Lexer(object):
    def __init__(self, text):
        self.text = text
        self.pos = 0
        self.current_char = text[self.pos]

    def error(self):
        raise Exception("parse token failed")

    def skip_whitespace(self):
        while self.current_char is not None and self.current_char.isspace():
            self.advance()

    def advance(self):
        self.pos += 1
        if self.pos > len(self.text) - 1:
            self.current_char = None
        else:
            self.current_char = self.text[self.pos]

    def integer(self):
        r = ''
        while self.current_char is not None and self.current_char.isdigit():
            r += self.current_char
            self.advance()

        return int(r)

    def get_next_token(self):
        text = self.text

        self.skip_whitespace()

        if self.pos > len(text) - 1:
            return Token(EOF, None)

        if self.current_char.isdigit():
            return Token(INTEGER, self.integer());

        if self.current_char == '+':
            self.advance()
            return Token(PLUS, '+')

        if self.current_char == '-':
            self.advance()
            return Token(MINUS, '-')

        if self.current_char == '*':
            self.advance()
            return Token(MULTIPLY, '*')

        if self.current_char == '/':
            self.advance()
            return Token(DIVIDE, '/')

        if self.current_char == '(':
            self.advance()
            return Token(LPAREN, '(')

        if self.current_char == ')':
            self.advance()
            return Token(RPAREN, ')')

        self.error()

class Interpreter(object):
    def __init__(self, lexer):
        self.lexer = lexer
        self.current_token = lexer.get_next_token()

    def error(self):
        raise Exception("parse token failed")

    def eat(self, token_type):
        if self.current_token.type == token_type:
            self.current_token = self.lexer.get_next_token()
        else:
            self.error()

    def factor(self):
        token = self.current_token
        if token.type == INTEGER:
            self.eat(INTEGER)
            return token.value
        elif token.type == LPAREN:
            self.eat(LPAREN)
            result = self.expr()
            self.eat(RPAREN)
            return result


    def term(self):
        result = self.factor()
        while self.current_token.type in (MULTIPLY, DIVIDE):
            op = self.current_token
            if op.type == MULTIPLY:
                self.eat(MULTIPLY)
                result = result * self.factor()
            else:
                self.eat(DIVIDE)
                result = result / self.factor()

        return result

    def expr(self):
        """
            expr   : term ((PLUS | MINUS) term)*
            term   : factor ((MUL | DIV) factor)*
            factor : INTEGER
        """
        result = self.term()

        while self.current_token.type in (PLUS, MINUS):
            op = self.current_token

            if op.type == PLUS:
                self.eat(PLUS)
                result += self.term()
            else:
                self.eat(MINUS)
                result -= self.term()

        return result

def main():
    while True:
        try:
            text = raw_input("calc> ")
        except EOFError:
            break

        if not text:
            continue

        lexer = Lexer(text);
        interpreter = Interpreter(lexer)
        result = interpreter.expr()

        print(result)

if __name__ == '__main__':
    main()

