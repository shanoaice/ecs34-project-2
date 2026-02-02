#include "XMLWriter.h"
#include "StringUtils.h"
#include "XMLEntity.h"
#include <expat.h>

#include <string>
#include <vector>
#include <memory>
#include <utility>

struct CXMLWriter::SImplementation{
    std::shared_ptr<CDataSink> DSink;
    std::vector<std::string> DOpenStack;

    static std::string EscapeText(const std::string &s) {
        auto escaped = StringUtils::Replace(s, "&", "&amp;");
        escaped = StringUtils::Replace(escaped, "<", "&lt;");
        escaped = StringUtils::Replace(escaped, ">", "&gt;");

        return escaped;
    }

    static std::string EscapeTag(const std::string &s) {
        auto escaped = StringUtils::Replace(s, "&", "&amp;");
        escaped = StringUtils::Replace(escaped, "<", "&lt;");
        escaped = StringUtils::Replace(escaped, ">", "&gt;");
        escaped = StringUtils::Replace(escaped, "\"", "&quot;");
        escaped = StringUtils::Replace(escaped, "\\", "&apos;");

        return escaped;
    }

    bool WriteRaw(const std::string &out) { // 输出原始字符串
        std::vector<char> buf(out.begin(), out.end());
        return DSink->Write(buf);
    }

    bool Flush(){
        // 没 close 的 tag 全部补齐 LIFO
        while (!DOpenStack.empty()) {
            std::string name = DOpenStack.back();
            DOpenStack.pop_back();

            std::string out = "</";
            out += name;
            out += ">";

            if (!WriteRaw(out)) return false;
        }
        return true;
    }

    std::string BuildTagCore(const TAttributes &attrs) {
        std::string out;
        for (const auto &pair : attrs) {
            out += " ";
            out += pair.first;
            out += "=\"";
            out += EscapeTag(pair.second);
            out += "\"";
        }
        return out;
    }

    bool WriteEntity(const SXMLEntity &entity){
        using EType = SXMLEntity::EType;

        switch (entity.DType) {
            case EType::StartElement: {
                if (entity.DNameData.empty()) {
                    return false;
                }

                std::string out = "<";
                out += entity.DNameData;
                out += BuildTagCore(entity.DAttributes);
                out += ">";

                if (!WriteRaw(out)) return false;
                DOpenStack.push_back(entity.DNameData);
                return true;
            }
            case EType::EndElement: {
                if (entity.DNameData.empty()) {
                    return false;
                }

                if (DOpenStack.back() != entity.DNameData) { // 标签不匹配
                    return false;
                }


                DOpenStack.pop_back();

                std::string out = "</";
                out += entity.DNameData;
                out += ">";

                return WriteRaw(out);
            }
            case EType::CharData: {
                return WriteRaw(EscapeText(entity.DNameData));
            }
            case EType::CompleteElement: {
                if (entity.DNameData.empty()) return false;

                std::string out = "<";
                out += entity.DNameData;
                out += BuildTagCore(entity.DAttributes);
                out += "/>";

                return WriteRaw(out);
            }
        }

        return false;
    }

};

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink) {
    DImplementation = std::make_unique<SImplementation>(std::move(sink));
}

CXMLWriter::~CXMLWriter() = default;

bool CXMLWriter::Flush() {
    return DImplementation->Flush();
}

bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    return DImplementation->WriteEntity(entity);
}
