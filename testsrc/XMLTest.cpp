#include "XMLReader.h"
#include "XMLWriter.h"
#include "XMLEntity.h"
#include "StringDataSource.h"
#include "StringDataSink.h"
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

std::string reference_xml = \
R"(<?xml version="1.0" encoding="UTF-8"?>
<root>
    <element attribute="value">Content goes here</element>
    <special_characters>
        <less_than>&lt;</less_than>
        <greater_than>&gt;</greater_than>
        <ampersand>&amp;</ampersand>
        <apostrophe>&apos;</apostrophe>
        <quote>&quot;</quote>
    </special_characters>
    <nested_structure>
        <item id="1">First Item</item>
        <item id="2">Second Item</item>
    </nested_structure>
    <empty_element/>
</root>
)";

TEST(XMLUtils, Regular) {
  auto Source = std::make_shared<CStringDataSource>(reference_xml);

  CXMLReader Reader(Source);

  std::vector<SXMLEntity> entities;
  while (!Reader.End()) {
    SXMLEntity entity;
    Reader.ReadEntity(entity);
    entities.push_back(entity);
  }
}
