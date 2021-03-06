#ifndef NOOP_PARSER_H
#define NOOP_PARSER_H

#include <noop.h>
#include <noop_io.h>
#include <noop_context.h>

#include <sstream>
#include <vector>

namespace noop {

namespace TokenType {

enum {
  EndOfSource, /* End of source code */
  Keyword,
  Identifier,
  BooleanLiteral,
  NullLiteral,
  NumericLiteral,
  StringLiteral,
  Punctuator
};

} // namespace TokenType

struct Token {
  int type, start, end;
  Token(int type, int start, int end) : type(type), start(start), end(end) {

  }
};

struct StringLiteralToken: Token {
  String value;
  StringLiteralToken(int type, int start, int end): Token(type, start, end) {
    value = U"";
  }
};

struct IdentifierToken: Token {
  String name;
  IdentifierToken(int type, int start, int end): Token(type, start, end)  {
    name = U"";
  }
};

struct PunctuatorToken: Token {
  String value;
  PunctuatorToken(int type, int start, int end): Token(type, start, end)  {
    value = U"";
  }
};

struct NumericLiteralToken: Token {
  Number value;
  NumericLiteralToken(int type, int start, int end): Token(type, start, end)  {
    value = 0.0;
  }
};

namespace SyntaxTreeNodeType {

enum {
  NullLiteral,
  BooleanLiteral,
  StringLiteral,
  NumericLiteral,
  Identifier,
  ThisExpression,
  MemberExpression,
  CallExpression,
  AssignmentExpression,
  ObjectExpression,
  ArrayExpression,
  SequenceExpression,
  BinaryExpression,
  FunctionExpression,
  VariableDeclarator,
  VariableStatement,
  IfStatement,
  WhileStatement,
  ExpressionStatement,
  BlockStatement,
  Body,
  Program
};

} // namespace SyntaxTreeNodeType

/* Just an interface */
struct SyntaxTreeNode {
  int type;
  String node_id;
  static size_t node_id_;
  virtual int Execute() = 0;
  SyntaxTreeNode() {
    std::string _tmp = "";
    std::stringstream sio;
    sio << ".node_" << node_id_;
    sio >> _tmp;
    node_id = Encoding::UTF8ToUTF32(_tmp);
    DEBUG << "[id] " << node_id << std::endl;
    ++node_id_;
  }
};

/* SyntaxTreeNode */

struct Expression: SyntaxTreeNode {
  virtual int Execute() = 0;
};

struct Literal: Expression {
  /* Just an interface */
};

struct NumericLiteral: Literal {
  Number value;
  NumericLiteral() {
    type = SyntaxTreeNodeType::NumericLiteral;
  }
  int Execute() override;
};

struct StringLiteral: Literal {
  String value;
  StringLiteral() {
    type = SyntaxTreeNodeType::StringLiteral;
  }
  int Execute() override;
};

struct BooleanLiteral: Literal {
  bool value;
  BooleanLiteral() {
    type = SyntaxTreeNodeType::BooleanLiteral;
  }
  int Execute() override;
};

struct NullLiteral: Literal {
  NullLiteral() {
    type = SyntaxTreeNodeType::NullLiteral;
  }
  int Execute() override;
};

struct Identifier: Expression {
  String name;
  Identifier() {
    type = SyntaxTreeNodeType::Identifier;
  }
  int Execute() override;
};

struct ThisExpression: Expression {
  ThisExpression() {
    type = SyntaxTreeNodeType::ThisExpression;
  }
  int Execute() override;
};

struct MemberExpression: Expression {
  String _operator;
  Expression* left;
  Expression* right;
  MemberExpression() {
    type = SyntaxTreeNodeType::MemberExpression;
  }
  int Execute() override;
};

struct CallExpression: Expression {
  Expression* callee;
  std::vector<Expression*> arguments;
  CallExpression() {
    type = SyntaxTreeNodeType::CallExpression;
  }
  int Execute() override;
};

struct AssignmentExpression: Expression {
  String _operator;
  Expression* left;
  Expression* right;
  AssignmentExpression() {
    type = SyntaxTreeNodeType::AssignmentExpression;
  }
  int Execute() override;
};

struct ObjectExpression: Expression {
  std::vector<std::pair<StringLiteral*, Expression*>> properties;
  ObjectExpression() {
    type = SyntaxTreeNodeType::ObjectExpression;
  }
  int Execute() override;
};

struct ArrayExpression: Expression {
  std::vector<Expression*> elements;
  ArrayExpression() {
    type = SyntaxTreeNodeType::ArrayExpression;
  }
  int Execute() override;
};

struct SequenceExpression: Expression {
  std::vector<Expression *> expressions;
  SequenceExpression() {
    type = SyntaxTreeNodeType::SequenceExpression;
  }
  int Execute() override;
};

struct BinaryExpression: Expression {
  String _operator;
  Expression* left;
  Expression* right;
  BinaryExpression() {
    type = SyntaxTreeNodeType::BinaryExpression;
  }
  int Execute() override;
};

/* Just an interface */
struct Statement: SyntaxTreeNode {
  virtual int Execute() = 0;
};

struct VariableDeclarator: SyntaxTreeNode {
  Identifier* id;
  Expression* init;
  VariableDeclarator() {
    type = SyntaxTreeNodeType::VariableDeclarator;
  }
  int Execute() override;
};

struct VariableStatement: Statement {
  std::vector<VariableDeclarator*> declarations;
  String kind;
  VariableStatement() {
    type = SyntaxTreeNodeType::VariableStatement;
  }
  int Execute() override;
};

struct ExpressionStatement: Statement {
  Expression* expression;
  ExpressionStatement() {
    type = SyntaxTreeNodeType::ExpressionStatement;
  }
  int Execute() override;
};

struct BlockStatement: Statement {
  std::vector<Statement*> statements;
  BlockStatement() {
    type = SyntaxTreeNodeType::BlockStatement;
  }
  int Execute() override;
};

struct IfStatement: Statement {
  Expression* condition;
  Statement* consequent;
  Statement* alternate;
  IfStatement() {
    type = SyntaxTreeNodeType::IfStatement;
  }
  int Execute() override;
};

struct WhileStatement: Statement {
  Expression* test;
  Statement* body;
  WhileStatement() {
    type = SyntaxTreeNodeType::WhileStatement;
  }
  int Execute() override;
};

struct FunctionExpression: Expression {
  Identifier* id;
  std::vector<Expression*> params;
  Statement* body;
  FunctionExpression() {
    type = SyntaxTreeNodeType::FunctionExpression;
  }
  int Execute() override;
};

struct Body: Statement {
  std::vector<Statement*> statements;
  Body() {
    type = SyntaxTreeNodeType::Body;
  }
  int Execute() override;
};

struct Program: SyntaxTreeNode {
  Body* body;
  Program() {
    type = SyntaxTreeNodeType::Program;
  }
  int Execute() override;
};

/* SyntaxTreeResolver */
class SyntaxTree {
public:
  Literal* CreateLiteral(Token* token);
  Identifier* CreateIdentifier(Token* token);
  ThisExpression* CreateThisExpression();
  MemberExpression* CreateMemberExpression(String op, Expression* expr,
                                                      Expression* property);
  CallExpression* CreateCallExpression(Expression* expr, std::vector<Expression*> args);
  AssignmentExpression* CreateAssignmentExpression(String op, Expression* left,
                                                              Expression* right);
  ObjectExpression* CreateObjectExpression(std::vector<std::pair<StringLiteral*, Expression*>> properties);
  ArrayExpression* CreateArrayExpression(std::vector<Expression*> elements);
  SequenceExpression* CreateSequenceExpression(std::vector<Expression*> expressions);
  BinaryExpression* CreateBinaryExpression(String op, Expression* left,
                                                      Expression* right);
  FunctionExpression* CreateFunctionExpression(Identifier* id,
                              std::vector<Expression*> params, Statement* body);
  VariableDeclarator* CreateVariableDeclarator(Identifier* id,
                                               Expression* init);
  VariableStatement* CreateVariableStatement(String kind,
                              std::vector<VariableDeclarator*> declarations);
  IfStatement* CreateIfStatement(Expression* expr, Statement* consequent,
                                                   Statement* alternate);
  WhileStatement* CreateWhileStatement(Expression* test, Statement* body);
  BlockStatement* CreateBlockStatement(std::vector<Statement*> statements);
  ExpressionStatement* CreateExpressionStatement(Expression* expr);
  Program* CreateProgram(Body* body);
};

class Parser {
private:
  String source;
  int index;
  int length;
  Token* look_ahead;
public:
  Token* LookAhead();
  void Peek();
  Token* Lex();
  bool IsPunctuation(const String& value);
  bool IsKeyword(const String& value);
  bool IsIdentifierName(Token* token);
  void SkipUselessness();
  void GetKeyword(const String& keyword);
  void GetSemicolon();
  StringLiteralToken* GetStringLiteral();
  NumericLiteralToken* GetNumericLiteral();
  IdentifierToken* GetIdentifier();
  PunctuatorToken* GetPunctuator();
  VariableDeclarator* ParseVariableDeclarator();
  Expression* ParsePrimaryExpression();
  Identifier* ParseLiteralProperty();
  Expression* ParseComputedProperty();
  std::vector<Expression*> ParseArguments();
  Expression* ParsePostfixExpression();
  Expression* ParseObjectExpression();
  Expression* ParseArrayExpression();
  Expression* ParseBinaryExpression();
  Expression* ParseAssignmentExpression();
  Expression* ParseFunctionExpression();
  Expression* ParseExpression();
  std::vector<VariableDeclarator*> ParseVariableDeclarationList();
  VariableStatement* ParseVariableStatement();
  BlockStatement* ParseBlock();
  IfStatement* ParseIfStatement();
  WhileStatement* ParseWhileStatement();
  BlockStatement* ParseBlockStatement();
  Statement* ParseStatement();
  Body* ParseBody();
  Program* ParseProgram(String code);
};

extern SyntaxTree delegate;
} // namespace noop

#endif // NOOP_PARSER_H
