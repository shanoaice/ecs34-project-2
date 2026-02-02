#ifndef DSVREADER_H
#define DSVREADER_H

#include "DataSource.h"
#include <memory>
#include <string>

class CDSVReader {
  private:
    struct SImplementation;
    std::unique_ptr<SImplementation> DImplementation;

  public:
    CDSVReader(std::shared_ptr<CDataSource> src, char delimiter);
    ~CDSVReader();

    bool End() const;
    bool ReadRow(std::vector<std::string> &row);
};

#endif
