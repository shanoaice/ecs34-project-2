#ifndef XMLREADER_H
#define XMLREADER_H

#include "DataSource.h"
#include "XMLEntity.h"
#include <memory>

class CXMLReader {
  private:
    struct SImplementation;
    std::unique_ptr<SImplementation> DImplementation;

  public:
    CXMLReader(std::shared_ptr<CDataSource> src);
    ~CXMLReader();

    bool End() const;
    bool ReadEntity(SXMLEntity &entity, bool skipcdata = false);
};

#endif
