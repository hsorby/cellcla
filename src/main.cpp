
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <libcellml>

#include "manipulate.h"
#include "utils.h"

class Options
{
public:
    bool printParserErrors;
    bool validate;
    bool generateC;
    bool generatePython;
};

enum EXIT_CODES
{
    EXIT_FILE_OPEN = 2,
    EXIT_CELLML_1_OR_2,
    EXIT_1_TO_2_TRANSFORM,
};

void usage(const std::string &programName)
{
    std::cout << "Usage: " << programName << " [-pvu]" << "[--generate-c,--generate-python]" << " file" << std::endl;
    std::cout << "    Options:" << std::endl;
    std::cout << "      -u Print out this usage information and exit." << std::endl;
    std::cout << "      -p Parse and print out any errors." << std::endl;
    std::cout << "      -v Validate and print out any errors." << std::endl;
    std::cout << "      --generate-c Generate C code [--gc]." << std::endl;
    std::cout << "      --generate-python Generate Python code [--gp]." << std::endl;

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
    std::string out;
    std::string msg;
    bool isCellML1Model = isCellML1(modelFile, out, msg);
    out.clear();
    msg.clear();
    bool isCellML2Model = isCellML2(modelFile, out, msg);
    out.clear();
    msg.clear();
    std::string stringModel;
    if (isCellML1Model) {
        std::cout << "deal with CellML 1.0 file." << std::endl;
        if (transformToCellML2(modelFile, out, msg)) {
            stringModel = out;
        } else {
            std::cout << "Failed to transform '" << modelFile << "' CellML 1.0 model to CellML 2.0.";
            exit(EXIT_1_TO_2_TRANSFORM);
        }
    } else if (isCellML2Model) {
        stringModel = fileContents(modelFile);
    } else {
        std::cout << "File: '" << modelFile << "' is not a CellML 1.0 or CellML 2.0 model" << std::endl;
        exit(EXIT_CELLML_1_OR_2);
    }

    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(stringModel);
    if (options.printParserErrors) {
        if (parser->errorCount()) {
            std::cout << "The model was parsed with no errors." << std::endl;
        } else {
            std::cout << "Parsing errors:" << std::endl;
            printErrors(parser);
        }
    }
    if (options.validate) {
        auto validator = libcellml::Validator::create();
        validator->validateModel(model);
        if (validator->errorCount() == 0) {
            std::cout << "The model is valid." << std::endl;
        } else {
            std::cout << "Validation errors:" << std::endl;
            printErrors(validator);
        }
    }
    if (options.generateC || options.generatePython) {
        if (model->hasImports()) {
            std::string resourcePath = directoryName(modelFile);
            model->resolveImports(resourcePath);
            model->flatten();
        }
        auto generator = libcellml::Generator::create();
        generator->processModel(model);
        if (options.generateC) {
            std::cout << generator->interfaceCode();
            std::cout << generator->implementationCode();

        }
        if (options.generatePython) {
            auto profile = libcellml::GeneratorProfile::create(libcellml::GeneratorProfile::Profile::PYTHON);
            generator->setProfile(profile);
            std::cout << generator->implementationCode();
        }
    }

}

int main(int argc, char *argv[])
{
    std::string modelFile;
    Options options;
    size_t argumentIndex;
    for (argumentIndex = 1; argumentIndex < static_cast<size_t>(argc); ++argumentIndex) {
        std::string arg = argv[argumentIndex];
        if (arg.size() > 2 && arg[0] == '-' && arg[1] == '-') {
            arg = arg.substr(2, arg.size());
            if (arg == "gc" || arg == "generate-c") {
                options.generateC = true;
            } else if (arg == "gp" || arg == "generate-python") {
                options.generatePython = true;
            } else {
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
        } else if (arg[0] == '-') {
            switch (arg[1]) {
            case 'p': options.printParserErrors = true; break;
            case 'v': options.validate = true; break;
            case 'u':
                usage(argv[0]);
                exit(EXIT_SUCCESS);
                break;
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
        exit(EXIT_FILE_OPEN);
    }

    processModel(modelFile, options);

    return 0;
}

