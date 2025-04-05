#include <iostream>
#include <fstream>
#include <cctype>
#include <string>

using namespace std;

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define GE_comp 25
#define LE_comp 26
#define G_comp 27
#define L_comp 28
#define L_PAREN 29
#define R_PAREN 30
#define R_BRACE 31
#define L_BRACE 32
#define L_BRACKET 33
#define R_BRACKET 34
#define SEMI_COLON 35
#define COMMA 36
#define inc_op 37
#define dec_op 38
#define if_op 39
#define else_op 40

int charClass;
string lexeme;
char nextChar;
int lexLen;
int token;
int nextToken;
ifstream inFile;

void addChar() {
    if (lexLen <= 98) {
        lexeme += nextChar;
        lexLen++;
    }
    else {
        cout << "Error - lexeme is too long" << endl;
    }
}

void getChar() {
    if (inFile.get(nextChar)) {
        if (isalpha(nextChar)) {
            charClass = LETTER;
        }
        else if (isdigit(nextChar)) {
            charClass = DIGIT;
        }
        else {
            charClass = UNKNOWN;
        }
    }
    else {
        charClass = EOF;
    }
}

void getNonBlank() {
    while (isspace(nextChar)) {
        getChar();
    }
}

int lookup(char ch) {
    switch (ch) {
    case '(':
        addChar();
        nextToken = L_PAREN;
        break;
    case ')':
        addChar();
        nextToken = R_PAREN;
        break;
    case '{':
        addChar();
        nextToken = L_BRACKET;
        break;
    case '}':
        addChar();
        nextToken = R_BRACKET;
        break;
    case '[':
        addChar();
        nextToken = L_BRACKET;
        break;
    case ']':
        addChar();
        nextToken = R_BRACKET;
        break;
    case '*':
        addChar();
        nextToken = MULT_OP;
        break;
    case '/':
        addChar();
        nextToken = DIV_OP;
        break;
    case '=':
        addChar();
        nextToken = ASSIGN_OP;
        break;
    case ';':
        addChar();
        nextToken = SEMI_COLON;
        break;
    case ',':
        addChar();
        nextToken = COMMA;
        break;
    case '<': {
        addChar();
        char tmp = inFile.get();
        if (tmp == '=') {
            nextChar = tmp;
            addChar();
            nextToken = LE_comp;
        }
        else {
            inFile.putback(tmp);
            nextToken = L_comp;
        }
        break;
    }
    case '>': {
        addChar();
        char tmp = inFile.get();
        if (tmp == '=') {
            nextChar = tmp;
            addChar();
            nextToken = GE_comp;
        }
        else {
            inFile.putback(tmp);
            nextToken = G_comp;
        }
        break;
    }
    case '+': {
        addChar();
        char tmp = inFile.get();
        if (tmp == '+') {
            nextChar = tmp;
            addChar();
            nextToken = inc_op;
        }
        else {
            inFile.putback(tmp);
            nextToken = ADD_OP;
        }
        break;
    }
    case '-': {
        addChar();
        char tmp = inFile.get();
        if (tmp == '-') {
            nextChar = tmp;
            addChar();
            nextToken = dec_op;
        }
        else {
            inFile.putback(tmp);
            nextToken = SUB_OP;
        }
        break;
    }
    default:
        addChar();
        nextToken = EOF;
    }
    return nextToken;
}

int lex() {
    lexLen = 0;
    lexeme = "";
    getNonBlank();
    switch (charClass) {
    case LETTER:
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT) {
            addChar();
            getChar();
        }
        nextToken = IDENT;
        break;
    case DIGIT:
        addChar();
        getChar();
        while (charClass == DIGIT) {
            addChar();
            getChar();
        }
        nextToken = INT_LIT;
        break;
    case UNKNOWN:
        lookup(nextChar);
        getChar();
        break;
    case EOF:
        nextToken = EOF;
        lexeme = "EOF";
        break;
    }
    cout << "Next token is: " << nextToken << ", Next lexeme is " << lexeme << endl;
    return nextToken;
}

int main() {
    inFile.open("test.txt");
    if (!inFile) {
        cout << "ERROR - cannot open file" << endl;
        return 1;
    }
    else {
        getChar();
        do {
            lex();
        } while (nextToken != EOF);
    }
    inFile.close();
    return 0;
}