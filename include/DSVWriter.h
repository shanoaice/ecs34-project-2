#ifndef DSVWRITER_H
#define DSVWRITER_H

#include "DataSink.h"
#include <memory>
#include <string>

class CDSVWriter {
  private:
    struct SImplementation;
    std::unique_ptr<SImplementation> DImplementation;

  public:
    CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter,
               bool quoteall = false);
    ~CDSVWriter();

    bool WriteRow(const std::vector<std::string> &row);
};

#endif
