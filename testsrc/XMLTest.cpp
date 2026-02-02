#include "StringDataSink.h"
#include "StringDataSource.h"
#include "XMLEntity.h"
#include "XMLReader.h"
#include "XMLWriter.h"
// #include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

std::string reference_xml =
    R"(<?xml version="1.0" encoding="UTF-8"?>
<root>
    <element attribute="value">Content goes here</element>
    <special_characters>
        <less_than>&lt;</less_than>
        <greater_than>&gt;</greater_than>
        <ampersand tag="&amp;">&amp;</ampersand>
        <apostrophe tag="&apos;">'</apostrophe>
        <quote tag="&quot;">"</quote>
    </special_characters>
    <nested_structure>
        <item id="1">First Item</item>
        <item id="2">Second Item</item>
    </nested_structure>
    <empty_element/>
</root>
)";

SXMLEntity root_begin = {
    .DType = SXMLEntity::EType::StartElement,
    .DNameData = "root",
    .DAttributes = {},
};

SXMLEntity regular_element_begin = {
    .DType = SXMLEntity::EType::StartElement,
    .DNameData = "element",
    .DAttributes = {{"attribute", "value"}},
};

SXMLEntity regular_element_cdata = {.DType = SXMLEntity::EType::CharData,
                                    .DNameData = "Content goes here",
                                    .DAttributes = {}};

SXMLEntity regular_element_end = {
    .DType = SXMLEntity::EType::EndElement,
    .DNameData = "element",
    .DAttributes = {},
};

SXMLEntity special_characters_begin = {
    .DType = SXMLEntity::EType::StartElement,
    .DNameData = "special_characters",
    .DAttributes = {},
};

SXMLEntity less_than_begin = {
    .DType = SXMLEntity::EType::StartElement,
    .DNameData = "less_than",
    .DAttributes = {},
};

SXMLEntity less_than_cdata = {
    .DType = SXMLEntity::EType::CharData,
    .DNameData = "<",
    .DAttributes = {},
};

SXMLEntity less_than_end = {
    .DType = SXMLEntity::EType::EndElement,
    .DNameData = "less_than",
    .DAttributes = {},
};

SXMLEntity greater_than_begin = {
    .DType = SXMLEntity::EType::StartElement,
    .DNameData = "greater_than",
    .DAttributes = {},
};

SXMLEntity greater_than_cdata = {
    .DType = SXMLEntity::EType::CharData,
    .DNameData = ">",
    .DAttributes = {},
};

SXMLEntity greater_than_end = {
    .DType = SXMLEntity::EType::EndElement,
    .DNameData = "greater_than",
    .DAttributes = {},
};

SXMLEntity ampersand_begin = {
    .DType = SXMLEntity::EType::StartElement,
    .DNameData = "ampersand",
    .DAttributes = {{"tag", "&"}},
};

SXMLEntity ampersand_cdata = {
    .DType = SXMLEntity::EType::CharData,
    .DNameData = "&",
    .DAttributes = {},
};

SXMLEntity ampersand_end = {
    .DType = SXMLEntity::EType::EndElement,
    .DNameData = "ampersand",
    .DAttributes = {},
};

SXMLEntity apostrophe_begin = {
    .DType = SXMLEntity::EType::StartElement,
    .DNameData = "apostrophe",
    .DAttributes = {{"tag", "'"}},
};

SXMLEntity apostrophe_cdata = {
    .DType = SXMLEntity::EType::CharData,
    .DNameData = "'",
    .DAttributes = {},
};

SXMLEntity apostrophe_end = {
    .DType = SXMLEntity::EType::EndElement,
    .DNameData = "apostrophe",
    .DAttributes = {},
};

SXMLEntity quote_begin = {
    .DType = SXMLEntity::EType::StartElement,
    .DNameData = "quote",
    .DAttributes = {{"tag", "\""}},
};

SXMLEntity quote_cdata = {
    .DType = SXMLEntity::EType::CharData,
    .DNameData = "\"",
    .DAttributes = {},
};

SXMLEntity quote_end = {
    .DType = SXMLEntity::EType::EndElement,
    .DNameData = "quote",
    .DAttributes = {},
};

SXMLEntity special_characters_end = {
    .DType = SXMLEntity::EType::EndElement,
    .DNameData = "special_characters",
    .DAttributes = {},
};

SXMLEntity nested_structure_begin = {
    .DType = SXMLEntity::EType::StartElement,
    .DNameData = "nested_structure",
    .DAttributes = {},
};

SXMLEntity item_begin = {
    .DType = SXMLEntity::EType::StartElement,
    .DNameData = "item",
    .DAttributes = {{"id", "1"}},
};

SXMLEntity item_cdata = {
    .DType = SXMLEntity::EType::CharData,
    .DNameData = "First Item",
    .DAttributes = {},
};

SXMLEntity item_end = {
    .DType = SXMLEntity::EType::EndElement,
    .DNameData = "item",
    .DAttributes = {},
};

SXMLEntity item2_begin = {
    .DType = SXMLEntity::EType::StartElement,
    .DNameData = "item",
    .DAttributes = {{"id", "2"}},
};

SXMLEntity item2_cdata = {
    .DType = SXMLEntity::EType::CharData,
    .DNameData = "Second Item",
    .DAttributes = {},
};

SXMLEntity item2_end = {
    .DType = SXMLEntity::EType::EndElement,
    .DNameData = "item",
    .DAttributes = {},
};

SXMLEntity nested_structure_end = {
    .DType = SXMLEntity::EType::EndElement,
    .DNameData = "nested_structure",
    .DAttributes = {},
};

SXMLEntity empty_element_complete = {
    .DType = SXMLEntity::EType::CompleteElement,
    .DNameData = "empty_element",
    .DAttributes = {},
};

SXMLEntity empty_element_begin = {
    .DType = SXMLEntity::EType::StartElement,
    .DNameData = "empty_element",
    .DAttributes = {},
};

SXMLEntity empty_element_end = {
    .DType = SXMLEntity::EType::EndElement,
    .DNameData = "empty_element",
    .DAttributes = {},
};

SXMLEntity root_end = {
    .DType = SXMLEntity::EType::EndElement,
    .DNameData = "root",
    .DAttributes = {},
};

std::vector<SXMLEntity> reference = {
    root_begin,
    regular_element_begin,
    regular_element_cdata,
    regular_element_end,
    special_characters_begin,
    less_than_begin,
    less_than_cdata,
    less_than_end,
    greater_than_begin,
    greater_than_cdata,
    greater_than_end,
    ampersand_begin,
    ampersand_cdata,
    ampersand_end,
    apostrophe_begin,
    apostrophe_cdata,
    apostrophe_end,
    quote_begin,
    quote_cdata,
    quote_end,
    special_characters_end,
    nested_structure_begin,
    item_begin,
    item_cdata,
    item_end,
    item2_begin,
    item2_cdata,
    item2_end,
    nested_structure_end,
    empty_element_complete,
    root_end,
};

std::vector<SXMLEntity> reference_reader = {
    root_begin,
    regular_element_begin,
    regular_element_cdata,
    regular_element_end,
    special_characters_begin,
    less_than_begin,
    less_than_cdata,
    less_than_end,
    greater_than_begin,
    greater_than_cdata,
    greater_than_end,
    ampersand_begin,
    ampersand_cdata,
    ampersand_end,
    apostrophe_begin,
    apostrophe_cdata,
    apostrophe_end,
    quote_begin,
    quote_cdata,
    quote_end,
    special_characters_end,
    nested_structure_begin,
    item_begin,
    item_cdata,
    item_end,
    item2_begin,
    item2_cdata,
    item2_end,
    nested_structure_end,
    empty_element_begin,
    empty_element_end,
    root_end,
};

std::vector<SXMLEntity> reference_skipcdata = {
    root_begin,
    regular_element_begin,
    regular_element_end,
    special_characters_begin,
    less_than_begin,
    less_than_end,
    greater_than_begin,
    greater_than_end,
    ampersand_begin,
    ampersand_end,
    apostrophe_begin,
    apostrophe_end,
    quote_begin,
    quote_end,
    special_characters_end,
    nested_structure_begin,
    item_begin,
    item_end,
    item2_begin,
    item2_end,
    nested_structure_end,
    empty_element_begin,
    empty_element_end,
    root_end,
};

TEST(XMLReader, Reference) {
  auto Source = std::make_shared<CStringDataSource>(reference_xml);

  CXMLReader Reader(Source);

  std::vector<SXMLEntity> entities;
  SXMLEntity entity;
  while (Reader.ReadEntity(entity)) {
    entities.push_back(entity);
  }

  EXPECT_EQ(entities.size(), reference_reader.size());

  EXPECT_EQ(entities, reference_reader);

  // auto mismatch_from_reference = std::ranges::mismatch(entities,
  // reference_reader);

  // EXPECT_EQ(mismatch_from_reference.in1, entities.end()); // basically this
  // means no mismatch
}

TEST(XMLReader, ReferenceSkipcdata) {
  auto Source = std::make_shared<CStringDataSource>(reference_xml);

  CXMLReader Reader(Source);

  std::vector<SXMLEntity> entities;
  SXMLEntity entity;
  while (Reader.ReadEntity(entity)) {
    entities.push_back(entity);
  }

  EXPECT_EQ(entities.size(), reference_skipcdata.size());

  EXPECT_EQ(entities, reference_skipcdata);

  // auto mismatch_from_reference = std::ranges::mismatch(entities,
  // reference_reader);

  // EXPECT_EQ(mismatch_from_reference.in1, entities.end()); // basically this
  // means no mismatch
}

TEST(XMLWriter, WholeDocument) {
  auto Sink = std::make_shared<CStringDataSink>();
  CXMLWriter Writer(Sink);

  for (const auto &entity : reference) {
    Writer.WriteEntity(entity);
  }

  auto Source = std::make_shared<CStringDataSource>(Sink->String());
  CXMLReader Reader(Source);

  std::vector<SXMLEntity> entities;
  SXMLEntity entity;
  while (Reader.ReadEntity(entity)) {
    entities.push_back(entity);
  }

  EXPECT_EQ(entities.size(), reference_reader.size());

  EXPECT_EQ(entities, reference_reader);
}

TEST(XMLWriter, FlushTags) {
  auto Sink = std::make_shared<CStringDataSink>();
  CXMLWriter Writer(Sink);

  SXMLEntity entity_begin = {
      .DType = SXMLEntity::EType::StartElement,
      .DNameData = "entity",
      .DAttributes = {},
  };

  SXMLEntity entity_attributes_begin = {
      .DType = SXMLEntity::EType::StartElement,
      .DNameData = "entity_attributes",
      .DAttributes = {{"tag", "attributes"}},
  };

  SXMLEntity entity_end = {
      .DType = SXMLEntity::EType::EndElement,
      .DNameData = "entity",
      .DAttributes = {},
  };

  SXMLEntity entity_attributes_end = {
      .DType = SXMLEntity::EType::EndElement,
      .DNameData = "entity_attributes",
      .DAttributes = {},
  };

  std::vector reference_output = {entity_begin, entity_attributes_begin,
                                  entity_attributes_end, entity_end};

  Writer.WriteEntity(entity_begin);
  Writer.WriteEntity(entity_attributes_begin);
  Writer.Flush();

  auto Source = std::make_shared<CStringDataSource>(Sink->String());
  CXMLReader Reader(Source);

  std::vector<SXMLEntity> entities;
  SXMLEntity entity;
  while (Reader.ReadEntity(entity)) {
    entities.push_back(entity);
  }

  EXPECT_EQ(reference_output.size(), entities.size());
  EXPECT_EQ(reference_output, entities);
}

TEST(XMLWriter, RejectMalformedEntities) {
  auto Sink = std::make_shared<CStringDataSink>();
  CXMLWriter Writer(Sink);

  SXMLEntity malformed_begin_entity = {
      .DType = SXMLEntity::EType::StartElement,
      .DNameData = "",
      .DAttributes = {},
  };

  SXMLEntity malformed_end_entity = {
      .DType = SXMLEntity::EType::EndElement,
      .DNameData = "",
      .DAttributes = {},
  };

  SXMLEntity malformed_complete_entity = {
      .DType = SXMLEntity::EType::CompleteElement,
      .DNameData = "",
      .DAttributes = {},
  };

  SXMLEntity malformed_entity_type = {
      .DType = (SXMLEntity::EType)4,
      .DNameData = "malformed",
      .DAttributes = {},
  };

  EXPECT_FALSE(Writer.WriteEntity(malformed_begin_entity));
  EXPECT_FALSE(Writer.WriteEntity(malformed_end_entity));
  EXPECT_FALSE(Writer.WriteEntity(malformed_complete_entity));
  EXPECT_FALSE(Writer.WriteEntity(malformed_entity_type));
}

TEST(XMLWriter, RejectMismatchedTags) {
  auto Sink = std::make_shared<CStringDataSink>();
  CXMLWriter Writer(Sink);

  SXMLEntity mismatched_begin_entity = {
      .DType = SXMLEntity::EType::StartElement,
      .DNameData = "mismatched",
      .DAttributes = {},
  };

  SXMLEntity mismatched_end_entity = {
      .DType = SXMLEntity::EType::EndElement,
      .DNameData = "mismatch",
      .DAttributes = {},
  };

  Writer.WriteEntity(mismatched_begin_entity);

  EXPECT_FALSE(Writer.WriteEntity(mismatched_end_entity));
}
