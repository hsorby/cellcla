
#pragma once

#include <string>

bool isCellML1(const std::string &url, std::string &out, std::string &msg);

bool isCellML2(const std::string &url, std::string &out, std::string &msg);

bool transformToCellML2(const std::string &url, std::string &out, std::string &msg);
