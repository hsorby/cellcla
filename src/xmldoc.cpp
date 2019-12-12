

#include "xmldoc.h"

void structuredErrorCallback(void *userData, xmlErrorPtr error)
{
    std::string errorString = std::string(error->message);
    // Swap libxml2 carriage return for a period.
    if (errorString.substr(errorString.length() - 1) == "\n") {
        errorString.replace(errorString.end() - 1, errorString.end(), ".");
    }
    auto context = reinterpret_cast<xmlParserCtxtPtr>(userData);
    auto doc = reinterpret_cast<XmlDoc *>(context->_private);
    doc->addXmlError(errorString);
}

XmlDoc::XmlDoc() noexcept
{

}

void XmlDoc::parse(const std::string &input)
{
    xmlInitParser();
    xmlParserCtxtPtr context = xmlNewParserCtxt();
    context->_private = reinterpret_cast<void *>(this);
    xmlSetStructuredErrorFunc(context, structuredErrorCallback);
    mXmlDocPtr = xmlCtxtReadDoc(context, reinterpret_cast<const xmlChar *>(input.c_str()), "/", nullptr, 0);
    xmlFreeParserCtxt(context);
    xmlSetStructuredErrorFunc(nullptr, nullptr);
    xmlCleanupParser();
    xmlCleanupGlobals();
}

void XmlDoc::addXmlError(const std::string &error)
{
    mXmlErrors.push_back(error);
}

xmlDocPtr XmlDoc::document() const
{
    return mXmlDocPtr;
}

xmlNodePtr XmlDoc::rootNode() const
{
    return xmlDocGetRootElement(mXmlDocPtr);
}

size_t XmlDoc::xmlErrorCount() const
{
    return mXmlErrors.size();
}
std::string XmlDoc::xmlError(size_t index) const
{
    return mXmlErrors.at(index);
}
