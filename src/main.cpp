
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <libcellml>

class Options
{
public:
    bool printParserErrors;
};

void usage(const std::string &programName)
{
    std::cout << "Usage: " << programName << " [-p]" << " file" << std::endl;
}

void printErrors(const libcellml::LoggerPtr &l, bool headings = false, bool kinds = false, bool rule = false);
void printErrors(const libcellml::LoggerPtr &l, bool headings, bool kinds, bool rule)
{
    for (size_t i = 0; i < l->errorCount(); ++i) {
        std::cout << "Error " << std::setw(3) << i + 1 << ": ";
        std::cout << l->error(i)->description();
        if (headings) {
            std::cout << ", " << l->error(i)->specificationHeading();
        }
        if (kinds) {
            std::cout << ", " << static_cast<int>(l->error(i)->kind());
        }
        if (rule) {
            std::cout << ", " << static_cast<int>(l->error(i)->rule());
        }
        std::cout << std::endl;
    }
}

void processModel(const std::string &modelFile, const Options &options)
{
    std::ifstream f(modelFile);
    std::stringstream buffer;
    buffer << f.rdbuf();

    std::string stringModel = buffer.str();
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(stringModel);
    if (options.printParserErrors) {
        std::cout << "Printing parser errors ..." << std::endl;
        printErrors(parser);
    }

}

int main(int argc, char *argv[])
{
    std::string modelFile;
    Options options;
    size_t argumentIndex;
    for (argumentIndex = 1; argumentIndex < static_cast<size_t>(argc); argumentIndex++) {
        if (argv[argumentIndex][0] == '-') {
            switch (argv[argumentIndex][1]) {
            case 'p': options.printParserErrors = true; break;
            default:
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
        } else {
            modelFile = std::string(argv[argumentIndex]);
        }

    }

    if (modelFile.empty()) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    std::ifstream myfile(modelFile);
    if (myfile.is_open()) {
      myfile.close();
    } else {
        std::cout << "Unable to open file: " << modelFile << std::endl;
        exit(2);
    }

    std::cout << "Process model: " << modelFile << std::endl;
    processModel(modelFile, options);

    return 0;
}

