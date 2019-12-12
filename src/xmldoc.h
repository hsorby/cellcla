
#pragma once

#include <libxml/tree.h>
#include <libxml/xmlerror.h>
#include <string>
#include <vector>

class XmlDoc
{
public:
    XmlDoc() noexcept;

    void addXmlError(const std::string &error);
    size_t xmlErrorCount() const;
    std::string xmlError(size_t index) const;
    void parse(const std::string &input);
    xmlNodePtr rootNode() const;
    xmlDocPtr document() const;

private:
    xmlDocPtr mXmlDocPtr;
    std::vector<std::string> mXmlErrors;
};
