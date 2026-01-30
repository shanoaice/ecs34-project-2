#include "DSVWriter.h"
#include "StringDataSink.h"
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

TEST(DSVWriter, Comma) {
  auto Sink = std::make_shared<CStringDataSink>();
  CDSVWriter Writer(Sink, ',');

  std::vector<std::string> row1 = {"Hello", "World"};
  std::vector<std::string> row2 = {"Foo", "Bar,WithDelim"};
  std::vector<std::string> row3 = {"Baz\nWithNewline", "Qux"};

  Writer.WriteRow(row1);
  EXPECT_EQ(Sink->String(), "Hello,World\n");

  Writer.WriteRow(row2);
  EXPECT_EQ(Sink->String(), R"(Hello,World
Foo,"Bar,WithDelim"
)");

  Writer.WriteRow(row3);
  EXPECT_EQ(Sink->String(), R"(Hello,World
Foo,"Bar,WithDelim"
"Baz
WithNewline",Qux
)");

  auto SinkQuoted = std::make_shared<CStringDataSink>();
  CDSVWriter QuotedWriter(SinkQuoted, ',', true);
  QuotedWriter.WriteRow(row1);
  EXPECT_EQ(SinkQuoted->String(), R"("Hello","World"
)");

  QuotedWriter.WriteRow(row2);
  EXPECT_EQ(SinkQuoted->String(), R"("Hello","World"
"Foo","Bar,WithDelim"
)");

  QuotedWriter.WriteRow(row3);
  EXPECT_EQ(SinkQuoted->String(), R"("Hello","World"
"Foo","Bar,WithDelim"
"Baz
WithNewline","Qux"
)");
}
