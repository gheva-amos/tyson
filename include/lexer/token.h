#ifndef TYSON_TOKEN_H__
#define TYSON_TOKEN_H__
#include <string>
#include <optional>
#include <cmath>

class Token
{
public:
  enum class Type
  {
    open,
    close,
    quote,
    dot,
    number,
    string,
    symbol,
    if_t,
    nil,
    define,
    set,
    END
  };
  Token(Type type, const std::string& text, size_t line, size_t column,
        std::optional<double> number = std::nullopt);

  Type type() const { return type_; }
  size_t line() const { return line_; }
  size_t column() const { return column_; }
  double number() const { return number_ ? number_.value() : std::nan("no number"); }
  const std::string& string() const { return text_; }
private:
  Type type_;
  std::string text_;
  std::optional<double> number_;
  size_t line_;
  size_t column_;
};

#endif // TYSON_TOKEN_H__
