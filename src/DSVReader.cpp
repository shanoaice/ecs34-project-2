#include "DSVReader.h"
#include "DataSource.h"
#include <memory>

struct CDSVReader::SImplementation {
  std::shared_ptr<CDataSource> DSource;
  char DDelimiter;

  SImplementation(std::shared_ptr<CDataSource> source, char delimiter) {
    DSource = source;
    DDelimiter = delimiter;
  }

  bool ParseValue(std::string &value) {
    return false;
  }

  bool ReadRow(std::vector<std::string> &row) {
    while (!DSource->End()) {}
    return false;
  }
};

CDSVReader::CDSVReader(std::shared_ptr<CDataSource> source, char delimiter) {
  DImplementation =
      std::make_unique<SImplementation>(source, delimiter);
}

CDSVReader::~CDSVReader() = default;

bool CDSVReader::ReadRow(std::vector<std::string> &row) {
  return DImplementation->ReadRow(row);
}
