
#include "utils.h"

#include <fstream>
#include <sstream>

#include <b64/decode.h>

std::string fileContents(const std::string &fileName)
{
    std::ifstream file(fileName);
    std::stringstream buffer;

    buffer << file.rdbuf();

    return buffer.str();
}

std::string base64Decode(const std::string &base64String)
{
    base64::decoder D;
    std::stringstream outStream;
    std::stringstream inStream;
    inStream << base64String;
    D.decode(inStream, outStream);
    return outStream.str();
}

std::string directoryName (const std::string &filePath)
{
  size_t found = filePath.find_last_of("/\\");
  return filePath.substr(0, found + 1);
}
