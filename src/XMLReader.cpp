#include "XMLReader.h"
#include "StringUtils.h"
#include <expat.h>
#include <queue>

// <tag attr="val">CharData</tag>

struct CXMLReader::SImplementation{
    std::shared_ptr< CDataSource > DSource;
    XML_Parser DParser;
    std::queue<SXMLEntity> DQueue;

    bool DDone = false;
    bool DParseError = false;

    static void StartElementHandler(void *userData, const XML_Char *name, const XML_Char **atts){
        SImplementation *This = (SImplementation *)userData;
        SXMLEntity NewEntity;

        NewEntity.DType = SXMLEntity::EType::StartElement;
        NewEntity.DNameData = name;
        for(int i = 0; atts[i]; i += 2){
            NewEntity.DAttributes.push_back({atts[i],atts[i+1]});
        }
        This->DQueue.push(NewEntity);
    }

    static void EndElementHandler(void *userData, const XML_Char *name){
        SImplementation *This = (SImplementation *)userData;
        SXMLEntity NewEntity;

        NewEntity.DType = SXMLEntity::EType::EndElement;
        NewEntity.DNameData = name;
        This->DQueue.push(NewEntity);
    }

    static void CharacterDataHandler(void *userData, const XML_Char *cdata, int len){
        SImplementation *This = (SImplementation *)userData;
        SXMLEntity NewEntity;

        NewEntity.DType = SXMLEntity::EType::CharData;
        NewEntity.DNameData = std::string(cdata,len);
        if (!StringUtils::IsAllWhiteSpace(NewEntity.DNameData)) {
        This->DQueue.push(NewEntity);
        }
    }

    SImplementation(std::shared_ptr< CDataSource > src){
        DSource = src;
        DParser = XML_ParserCreate(nullptr);

        XML_SetUserData(DParser,(void *)this);
        XML_SetStartElementHandler(DParser,StartElementHandler);
        XML_SetEndElementHandler(DParser,EndElementHandler);
        XML_SetCharacterDataHandler(DParser,CharacterDataHandler);
    }

    ~SImplementation(){
        XML_ParserFree(DParser);
    }

    bool End() const{
        return DDone && DQueue.empty();
    }

    bool OneTimePump(){
        if(DDone || DParseError){
            return false;
        }

        std::vector<char> Buffer(512);
        DSource->Read(Buffer, Buffer.size());

        const bool IsFinal = DSource->End();

        const int Len = static_cast<int>(Buffer.size());
        const int Status = XML_Parse(
            DParser,
            Buffer.empty() ? "" : Buffer.data(),
            Len,
            IsFinal ? 1 : 0
        );

        if(Status == XML_STATUS_ERROR){
            DParseError = true;
            return false;
        }

        if(IsFinal){
            DDone = true;
        }
        return true;
    }

    bool ReadEntity(SXMLEntity &entity, bool skipcdata){
        while(true){

            if(skipcdata){
                while(!DQueue.empty() && DQueue.front().DType == SXMLEntity::EType::CharData){
                    DQueue.pop();
                }
            }

            if(!DQueue.empty()){
                entity = DQueue.front();
                DQueue.pop();
                return true;
            }

            if(End() || DParseError){
                return false;
            }

            if(!OneTimePump()){
                    return false;
            }
        }

        return false;
    }

};



CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src){
    DImplementation = std::make_unique<SImplementation>(src);
}

CXMLReader::~CXMLReader(){

}

bool CXMLReader::End() const{
    return DImplementation->End();
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){
    return DImplementation->ReadEntity(entity,skipcdata);
}
