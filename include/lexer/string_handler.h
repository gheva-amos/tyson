#ifndef TYSON_STRING_HANDLER_H__
#define TYSON_STRING_HANDLER_H__
#include <string>
#include <cstddef>

class StringHandler
{
public:
  StringHandler(std::string text);
  virtual ~StringHandler() = default;

  size_t line() const { return line_; }
  size_t column() const { return column_; }

  // Return true if reading forward chars will be past the end of the inoput text
  bool eof(size_t forward=0) const;
  char peek(size_t forward=0) const;
  char next();
  void skip_space();

  bool is_space() const;
protected:
  const std::string& text() const { return text_; }
private:
  std::string text_;
  size_t index_;
  size_t line_;
  size_t column_;
};
#endif // TYSON_STRING_HANDLER_H__
