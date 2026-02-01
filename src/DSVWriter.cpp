#include "DSVWriter.h"
#include "DataSink.h"
#include "StringUtils.h"
#include <memory>
#include <string>
#include <vector>

struct CDSVWriter::SImplementation {
  std::shared_ptr<CDataSink> DSink;
  char DDelimiter;
  bool DQuoteAll;

  SImplementation(std::shared_ptr<CDataSink> sink, char delim, bool quoteall) {
    DSink = sink;
    DDelimiter = delim;
    DQuoteAll = quoteall;
  }

  bool WriteRow(const std::vector<std::string> &row) {
    bool first_col = true;
    for (const auto &col : row) {
      if (!first_col) {
        DSink->Put(DDelimiter);
      }
      first_col = false;

      if (col.find(DDelimiter) != std::string::npos ||
          col.find("\n") != std::string::npos ||
          DQuoteAll) {
        DSink->Put('"');
        DSink->Write(std::vector<char>(col.cbegin(), col.cend()));
        DSink->Put('"');
      } if (col.find("\"") != std::string::npos) {
        auto escaped = StringUtils::Replace(col, "\"", "\"\"");
        DSink->Put('"');
        DSink->Write(std::vector<char>(escaped.begin(), escaped.end()));
        DSink->Put('"');
      } else {
        DSink->Write(std::vector<char>(col.cbegin(), col.cend()));
      }
    }

    DSink->Put('\n');

    return true;
  };
};

CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter,
                       bool quoteall) {
  DImplementation =
      std::make_unique<SImplementation>(sink, delimiter, quoteall);
}

CDSVWriter::~CDSVWriter() = default;

bool CDSVWriter::WriteRow(const std::vector<std::string> &row) {
  return DImplementation->WriteRow(row);
}
