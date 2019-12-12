
#include "manipulate.h"

#include <b64/decode.h>
#include <sstream>

#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
#include <libxml/DOCBparser.h>
#include <libxml/xinclude.h>
#include <libxml/catalog.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include "datastore.h"
#include "utils.h"
#include "xmldoc.h"

bool applyXslt(const std::string &xslt, const std::string &xml, std::string &out, std::string &msg)
{
    bool success = false;
    const char *params[16 + 1];
    int nbparams = 0;
    xmlChar *xmlBuffer;
    int size;

    params[nbparams] = nullptr;

    XmlDoc docTool;
    docTool.parse(xslt);
    for (size_t index = 0; index < docTool.xmlErrorCount(); ++index) {
        msg += docTool.xmlError(index);
    }

    xmlSubstituteEntitiesDefault(1);
    xmlLoadExtDtdDefaultValue = 1;

    xmlDocPtr xsltDoc = docTool.document();
    xsltStylesheetPtr cur = xsltParseStylesheetDoc(xsltDoc);

    docTool.parse(xml);
    for (size_t index = 0; index < docTool.xmlErrorCount(); ++index) {
        msg += docTool.xmlError(index);
    }

    xmlDocPtr doc = docTool.document();
    xmlDocPtr res = xsltApplyStylesheet(cur, doc, params);

    int transformResult = xsltSaveResultToString(&xmlBuffer, &size, res, cur);
    if (transformResult == 0 && size > 0) {
        out.assign((char *)xmlBuffer, size);
    }
    success = true ? transformResult == 0 : false;

    xsltFreeStylesheet(cur);
    xmlFreeDoc(res);
    xmlFreeDoc(doc);
    xmlFree(xmlBuffer);

    xsltCleanupGlobals();
    xmlCleanupParser();

    return success;
}

bool transformToCellML2(const std::string &url, std::string &out, std::string &msg)
{
    std::string xslt = base64Decode(DataStore::CELLML1TO2XSLTBASE64);
    std::string urlContents = fileContents(url);

    return applyXslt(xslt, urlContents, out, msg);
}

bool isCellML(const std::string &url, std::string &out, std::string &msg, const std::string &xslt)
{
    bool success = false;
    bool isCellmlFile = false;

    std::string urlContents = fileContents(url);

    success = applyXslt(xslt, urlContents, out, msg);
    if (success && out.size() > 0) {
        if (out.find("model") != std::string::npos) {
            isCellmlFile = true;
        }
    }

    return isCellmlFile;
}

bool isCellML1(const std::string &url, std::string &out, std::string &msg)
{
    std::string xslt = base64Decode(DataStore::CELLML1XSLTBASE64);
    return isCellML(url, out, msg, xslt);
}

bool isCellML2(const std::string &url, std::string &out, std::string &msg)
{
    std::string xslt = base64Decode(DataStore::CELLML2XSLTBASE64);
    return isCellML(url, out, msg, xslt);
}

