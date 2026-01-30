#include "DSVWriter.h"
#include <vector>

struct CDSVWriter::SImplementation {
  std::shared_ptr<CDataSink> DSink;
  char DDelimiter;
  bool DQuoteAll;

  bool WriteRow(const std::vector<std::string> &row) {
    bool firstCol = true;
    for (const auto &col : row) {
      if (!firstCol) {
        DSink->Put(DDelimiter);
      }
      firstCol = false;

      std::vector<char> buf(col.begin(), col.end());

      if (col.find(DDelimiter) != std::string::npos ||
          col.find("\"") != std::string::npos ||
          col.find("\n") != std::string::npos ||
          DQuoteAll) {
        DSink->Put('"');
        DSink->Write(buf);
        DSink->Put('"');
      } else {
        DSink->Write(buf);
      }
    }

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
