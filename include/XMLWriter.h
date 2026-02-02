#ifndef XMLWRITER_H
#define XMLWRITER_H

#include "DataSink.h"
#include "XMLEntity.h"
#include <memory>

class CXMLWriter {
  private:
    struct SImplementation;
    std::unique_ptr<SImplementation> DImplementation;

  public:
    CXMLWriter(std::shared_ptr<CDataSink> sink);
    ~CXMLWriter();

    bool Flush();
    bool WriteEntity(const SXMLEntity &entity);
};

#endif
