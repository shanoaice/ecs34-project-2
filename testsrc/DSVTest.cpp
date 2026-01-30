#include "DSVWriter.h"
#include "StringDataSink.h"
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

std::vector<std::string> row1 = {"Hello", "World"};
std::vector<std::string> row2 = {"Foo|WithDelim", "Bar,WithComma"};
std::vector<std::string> row3 = {"Baz\nWithNewline", "Qux"};

TEST(DSVWriter, Comma) {
  auto Sink = std::make_shared<CStringDataSink>();
  CDSVWriter Writer(Sink, ',');

  Writer.WriteRow(row1);
  EXPECT_EQ(Sink->String(), "Hello,World\n");

  Writer.WriteRow(row2);
  EXPECT_EQ(Sink->String(), R"(Hello,World
Foo|WithDelim,"Bar,WithComma"
)");

  Writer.WriteRow(row3);
  EXPECT_EQ(Sink->String(), R"(Hello,World
Foo|WithDelim,"Bar,WithComma"
"Baz
WithNewline",Qux
)");
}

TEST(DSVWriter, CommaQuoted) {
  auto SinkQuoted = std::make_shared<CStringDataSink>();
  CDSVWriter QuotedWriter(SinkQuoted, ',', true);
  QuotedWriter.WriteRow(row1);
  EXPECT_EQ(SinkQuoted->String(), R"("Hello","World"
)");

  QuotedWriter.WriteRow(row2);
  EXPECT_EQ(SinkQuoted->String(), R"("Hello","World"
"Foo|WithDelim","Bar,WithComma"
)");

  QuotedWriter.WriteRow(row3);
  EXPECT_EQ(SinkQuoted->String(), R"("Hello","World"
"Foo|WithDelim","Bar,WithComma"
"Baz
WithNewline","Qux"
)");
}

TEST(DSVWriter, Delim) {
  auto SinkDelim = std::make_shared<CStringDataSink>();
  CDSVWriter DelimWriter(SinkDelim, '|');
  DelimWriter.WriteRow(row1);
  EXPECT_EQ(SinkDelim->String(), "Hello|World\n");

  DelimWriter.WriteRow(row2);
  EXPECT_EQ(SinkDelim->String(), R"(Hello|World
"Foo|WithDelim"|Bar,WithComma
)");

  DelimWriter.WriteRow(row3);
  EXPECT_EQ(SinkDelim->String(), R"(Hello|World
"Foo|WithDelim"|Bar,WithComma
"Baz
WithNewline"|Qux
)");
}

TEST(DSVWriter, DelimQuoted) {
  auto SinkDelimQuoted = std::make_shared<CStringDataSink>();
  CDSVWriter DelimQuotedWriter(SinkDelimQuoted, '|', true);
  DelimQuotedWriter.WriteRow(row1);
  EXPECT_EQ(SinkDelimQuoted->String(), R"("Hello"|"World"
)");

  DelimQuotedWriter.WriteRow(row2);
  EXPECT_EQ(SinkDelimQuoted->String(), R"("Hello"|"World"
"Foo|WithDelim"|"Bar,WithComma"
)");

  DelimQuotedWriter.WriteRow(row3);
  EXPECT_EQ(SinkDelimQuoted->String(), R"("Hello"|"World"
"Foo|WithDelim"|"Bar,WithComma"
"Baz
WithNewline"|"Qux"
)");
}
