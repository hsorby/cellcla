
#include "manipulate.h"

bool isCellML(const std::string &url, std::string &out, std::string &msg, std::ifstream &xslt)
{
    bool success = false;
    bool isCellmlFile = false;
    if (success) {// && out->contains("model")) {
        isCellmlFile = true;
    }

    return isCellmlFile;
}

bool isCellML1(const std::string &url, std::string &out, std::string &msg)
{
    std::ifstream xslt(":/xslt/iscellml1.xsl");
    return isCellML(url, out, msg, xslt);
}

bool isCellML2(const std::string &url, std::string &out, std::string &msg)
{
    std::ifstream xslt(":/xslt/iscellml2.xsl");
    return isCellML(url, out, msg, xslt);
}

