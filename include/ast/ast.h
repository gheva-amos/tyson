#ifndef TYSON_AST_H__
#define TYSON_AST_H__
#include <cstddef>
#include <string>
#include <vector>
#include <memory>
#include <ostream>

class Token;

class AST
{
public:
  enum class Type
  {
    number,
    string,
    boolean,
    nil,
    symbol,
    list,
    start,
    unknown
  };
  AST(Token& token);
  virtual ~AST() = default;
  virtual void add_child(std::unique_ptr<AST> child) {};
  virtual AST* get_child() { return nullptr; }
  Type type() const { return type_; }
  static std::unique_ptr<AST> factory(Token& token);
//  virtual bool operator()() = 0;
  const std::string& str() const { return print_value_; }
  virtual std::ostream& output(std::ostream& out) const;
protected:
  size_t line_;
  size_t column_;
  Type type_;
  const std::string print_value_;
private:
  static std::unique_ptr<AST> symbol_factory(Token& token);
};

class ASTStart : public AST
{
public:
  ASTStart(Token& token) : AST{token} {type_ = AST::Type::start;}
  virtual void add_child(std::unique_ptr<AST> child) override;
  virtual AST* get_child() override { return root_.get(); }
private:
  std::unique_ptr<AST> root_;
};

class ASTNumber : public AST
{
public:
  ASTNumber(Token& token);
  double value() const { return value_; }
  virtual std::ostream& output(std::ostream& out) const;
private:
  double value_;
};

class ASTString : public AST
{
public:
  ASTString(Token& token);
  const std::string& value() const { return value_; }
  virtual std::ostream& output(std::ostream& out) const;
private:
  std::string value_;
};

class ASTBool : public AST
{
public:
  ASTBool(Token& token);
  bool value() const { return value_; }
  virtual std::ostream& output(std::ostream& out) const;
private:
  bool value_;
};

class ASTList : public AST
{
public:
  ASTList(Token& token);
  void add_child(std::unique_ptr<AST> child) override;
  virtual std::ostream& output(std::ostream& out) const;
private:
  std::vector<std::unique_ptr<AST>> value_;
};

class ASTSymbol : public AST
{
public:
  ASTSymbol(Token& token);
  const std::string& value() const { return value_; }
  virtual std::ostream& output(std::ostream& out) const;
private:
  std::string value_;
};

class ASTNil : public AST
{
public:
  ASTNil(Token& token);
  const std::string& value() const;
private:
  std::string value_;
};

#endif // TYSON_AST_H__
