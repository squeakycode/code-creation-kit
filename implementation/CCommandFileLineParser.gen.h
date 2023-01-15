//------------------------------------------------------------------------------
/**
\file
\brief  WARNING CONTAINS GENERATED CODE! ALL CHANGES WILL BE LOST!
*/
//------------------------------------------------------------------------------

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <cwchar>
#include <climits>
#include "StringLiteral.h"
#include "3rdparty/boost/split_winmain.h"

//parses the command line, provides the parameters from the command line, and checks for valid option combinations
template <typename StringT = std::string>
class CCommandFileLineParser
{
    typedef typename StringT::value_type CharT;
    static const size_t cLeftColumnSize = 40;

public:
    ///lists valid option combinations
    enum ECommand
    {
        eGenerate,
        eGenerateUsingIntermediateFile,
        eProcessInlineTemplateFile,
        eResetGenerator,
        eUnloadTable,
        eLoadTable,
        eAddIncludeDirectory,
        eCsvDelimiterChars,
        eCsvCommentChars,
        eCsvQuoteChars,
        eSetLogFile,
        eNoOptionsGiven,
        eOptionsInvalid
    };

    CCommandFileLineParser()
    {
        reset();
    }

    ///reset the parsed data
    void reset()
    {
        m_TableFilePassed = false;
        m_LabelPassed = false;
        m_TopDownPassed = false;
        m_LeftToRightPassed = false;
        m_RowHeaderIndexPassed = false;
        m_ColumnHeaderIndexPassed = false;
        m_PadRowsPassed = false;
        m_LabelsOfTableFilesToUnloadPassed = false;
        m_TemplateFilePassed = false;
        m_OutputFilePassed = false;
        m_ParametersPassed = false;
        m_UseIntermediateOutputFilePassed = false;
        m_IntermediateOutputFileExtensionPassed = false;
        m_MarkupPrefixPassed = false;
        m_MarkupPostfixPassed = false;
        m_MarkupPassed = false;
        m_AppendToFilePassed = false;
        m_InlinePassed = false;
        m_InlinePrefixPassed = false;
        m_InlinePostfixPassed = false;
        m_InlineGeneratedPostfixPassed = false;
        m_InlinePadPassed = false;
        m_RecyclePassed = false;
        m_CanChangeTableListPassed = false;
        m_ResetPassed = false;
        m_IncludeDirectoriesPassed = false;
        m_CsvDelimiterCharsPassed = false;
        m_CsvCommentCharsPassed = false;
        m_CsvQuoteCharsPassed = false;
        m_LogFilePassed = false;

        m_TableFileValue.clear();
        m_LabelValue.clear();
        m_RowHeaderIndexValue = 1;
        m_ColumnHeaderIndexValue = 1;
        m_LabelsOfTableFilesToUnloadValue.clear();
        m_TemplateFileValue.clear();
        m_OutputFileValue = STRING_LITERAL("");
        m_ParametersValue = std::vector<StringT>();
        m_IntermediateOutputFileExtensionValue = STRING_LITERAL(".intermediate");
        m_MarkupPrefixValue = STRING_LITERAL("[");
        m_MarkupPostfixValue = STRING_LITERAL("]");
        m_MarkupValue.clear();
        m_InlinePrefixValue = STRING_LITERAL("//<>");
        m_InlinePostfixValue = STRING_LITERAL("");
        m_InlineGeneratedPostfixValue = STRING_LITERAL("//$");
        m_InlinePadValue = 0;
        m_IncludeDirectoriesValue.clear();
        m_CsvDelimiterCharsValue.clear();
        m_CsvCommentCharsValue.clear();
        m_CsvQuoteCharsValue.clear();
        m_LogFileValue.clear();
    }

    void printDescription(std::ostream& stream)
    {
        stream << "Command Options" << std::endl;
        stream << "Load Table" << ":" << std::endl;
        printHelpCommandText("  --load-table arg", stream);
        stream << "Name of the table file to load. Default" << std::endl;
        stream << "                                        when the option name is omitted." << std::endl;
        printHelpCommandText("  -a [ --label ] arg", stream);
        stream << "Specifies the label used for" << std::endl;
        stream << "                                        identifying the table when unloading." << std::endl;
        stream << "                                        The label defaults to the name of the" << std::endl;
        stream << "                                        table passed." << std::endl;
        printHelpCommandText("  -t [ --top-down ]", stream);
        stream << "Indicates that the table is to be read" << std::endl;
        stream << "                                        top down. If no direction is explicitly" << std::endl;
        stream << "                                        specified all directions will be read" << std::endl;
        stream << "                                        by default." << std::endl;
        printHelpCommandText("  -l [ --left-to-right ]", stream);
        stream << "Indicates that the table is to be read" << std::endl;
        stream << "                                        from left to right. If no direction is" << std::endl;
        stream << "                                        explicitly specified all directions" << std::endl;
        stream << "                                        will be read by default." << std::endl;
        printHelpCommandText("  -w [ --row-header-index ] arg", stream);
        stream << "Specifies the one based index of the" << std::endl;
        stream << "                                        column containing the row names. If" << std::endl;
        stream << "                                        zero is passed the one based index of" << std::endl;
        stream << "                                        the row is used as row name." << std::endl;
        printHelpCommandText("  -n [ --column-header-index ] arg", stream);
        stream << "Specifies the one based index of the" << std::endl;
        stream << "                                        row containing the column names. If" << std::endl;
        stream << "                                        zero is passed the one based index of" << std::endl;
        stream << "                                        the column is used as column name." << std::endl;
        printHelpCommandText("  --pad-rows", stream);
        stream << "Automatically fills missing row items" << std::endl;
        stream << "                                        at the end of a row with an empty" << std::endl;
        stream << "                                        string." << std::endl;
        stream << "Unload Table" << ":" << std::endl;
        printHelpCommandText("  -x [ --unload-table ] arg", stream);
        stream << "Unload table file identified by its" << std::endl;
        stream << "                                        label. This option accepts multiple" << std::endl;
        stream << "                                        parameters for unloading more than one" << std::endl;
        stream << "                                        table. Tables having the same label are" << std::endl;
        stream << "                                        unloaded in the reverse order they are" << std::endl;
        stream << "                                        loaded." << std::endl;
        stream << "Generate File" << ":" << std::endl;
        printHelpCommandText("  -s [ --template-source-file ] arg", stream);
        stream << "Specifies the file containing the" << std::endl;
        stream << "                                        template." << std::endl;
        printHelpCommandText("  -o [ --output-file ] arg", stream);
        stream << "Specifies the file to output to." << std::endl;
        printHelpCommandText("  -p [ --parameter ] arg", stream);
        stream << "Optional parameters used by the" << std::endl;
        stream << "                                        template forming an additional table." << std::endl;
        stream << "                                        This option accepts multiple" << std::endl;
        stream << "                                        parameters. Therefore it must be" << std::endl;
        stream << "                                        provided last." << std::endl;
        printHelpCommandText("  -u [ --use-intermediate-output-file ]", stream);
        stream << "Indicates that the output shall be" << std::endl;
        stream << "                                        written to an intermediate file first." << std::endl;
        stream << "                                        This file replaces the primary output" << std::endl;
        stream << "                                        file when the generation succeeded and" << std::endl;
        stream << "                                        the intermediate and the primary output" << std::endl;
        stream << "                                        files are different otherwise it is" << std::endl;
        stream << "                                        being removed." << std::endl;
        printHelpCommandText("  -e [ --intermediate-file-extension ] arg", stream);
        stream << "The name of the intermediate file is" << std::endl;
        stream << "                                        produced by adding the given extension" << std::endl;
        stream << "                                        to the name of the output file. The" << std::endl;
        stream << "                                        extension defaults to \'.intermediate\'" << std::endl;
        stream << "                                        when omitted." << std::endl;
        printHelpCommandText("  --markup-prefix arg", stream);
        stream << "Sets the initial tag markup prefix." << std::endl;
        printHelpCommandText("  --markup-postfix arg", stream);
        stream << "Sets the initial tag markup postfix." << std::endl;
        printHelpCommandText("  -m [ --markup ] arg", stream);
        stream << "Sets the initial tag markup prefix and" << std::endl;
        stream << "                                        postfix. This switch overrides the" << std::endl;
        stream << "                                        switches markup-prefix and" << std::endl;
        stream << "                                        markup-postfix." << std::endl;
        printHelpCommandText("  --append-to-file", stream);
        stream << "The output is appended to the target" << std::endl;
        stream << "                                        file. This option is ignored when used" << std::endl;
        stream << "                                        together with the" << std::endl;
        stream << "                                        use-intermediate-output-file option." << std::endl;
        printHelpCommandText("  --inlined", stream);
        stream << "Indicates that a file with inline" << std::endl;
        stream << "                                        templates is processed. An intermediate" << std::endl;
        stream << "                                        file is automatically used when" << std::endl;
        stream << "                                        processing files with inline templates" << std::endl;
        stream << "                                        if no output file is provided. Use this" << std::endl;
        stream << "                                        option carefully to prevent data loss." << std::endl;
        stream << "                                        Consider using the recycle option." << std::endl;
        printHelpCommandText("  -b [ --inline-prefix ] arg", stream);
        stream << "A prefix that marks a line of an inline" << std::endl;
        stream << "                                        template file as template content. This" << std::endl;
        stream << "                                        string must not be empty." << std::endl;
        printHelpCommandText("  -c [ --inline-postfix ] arg", stream);
        stream << "A postfix that marks a line of an" << std::endl;
        stream << "                                        inline template file as template" << std::endl;
        stream << "                                        content. This string can be empty." << std::endl;
        printHelpCommandText("  -d [ --inline-generated-postfix ] arg", stream);
        stream << "A postfix that marks a line of an" << std::endl;
        stream << "                                        inline template file as generated" << std::endl;
        stream << "                                        content. This string must not be" << std::endl;
        stream << "                                        empty." << std::endl;
        printHelpCommandText("  --inline-pad arg", stream);
        stream << "The number of characters a generated" << std::endl;
        stream << "                                        line is padded up to with spaces before" << std::endl;
        stream << "                                        the generated postfix is appended." << std::endl;
        printHelpCommandText("  -y [ --recycle ]", stream);
        stream << "Used together with inlined option.  If" << std::endl;
        stream << "                                        possible the target file is moved to" << std::endl;
        stream << "                                        the recycle bin of the system before it" << std::endl;
        stream << "                                        is replaced by the intermediate file. " << std::endl;
        printHelpCommandText("  --can-change-table-list", stream);
        stream << "Allows a template to change the table" << std::endl;
        stream << "                                        list provided to the generator, e.g. by" << std::endl;
        stream << "                                        adding or removing tables. The default" << std::endl;
        stream << "                                        setting is off. A template can always" << std::endl;
        stream << "                                        use temporary tables during" << std::endl;
        stream << "                                        processing." << std::endl;
        stream << "Reset Generator" << ":" << std::endl;
        printHelpCommandText("  -r [ --reset ]", stream);
        stream << "Reset the generator to defaults." << std::endl;
        stream << "Add Include Directories" << ":" << std::endl;
        printHelpCommandText("  -i [ --add-include-directory ] arg", stream);
        stream << "Adds an include directory. This option" << std::endl;
        stream << "                                        accepts multiple parameters." << std::endl;
        stream << "CSV Table Properties" << ":" << std::endl;
        printHelpCommandText("  --csv-delimiter-chars arg", stream);
        stream << "Specifies a list of delimiter" << std::endl;
        stream << "                                        characters for the next CSV-files to" << std::endl;
        stream << "                                        load. Use \"\\t\" for tab separated items." << std::endl;
        stream << "                                        Use an empty string for no delimiter." << std::endl;
        printHelpCommandText("  --csv-comment-chars arg", stream);
        stream << "Specifies a list of characters as" << std::endl;
        stream << "                                        string that mark commented lines in" << std::endl;
        stream << "                                        CSV-files when found at the beginning" << std::endl;
        stream << "                                        of a line. The default is no commenting" << std::endl;
        stream << "                                        characters used." << std::endl;
        printHelpCommandText("  --csv-quote-chars arg", stream);
        stream << "Specifies a list of characters as" << std::endl;
        stream << "                                        string  that are used for quoting text" << std::endl;
        stream << "                                        items in CSV-files. The default is the" << std::endl;
        stream << "                                        double quote character." << std::endl;
        stream << "Logging" << ":" << std::endl;
        printHelpCommandText("  --log-file arg", stream);
        stream << "Sets up logging as follows: \'none\' is" << std::endl;
        stream << "                                        off, \'-\' is log to stdout, and any" << std::endl;
        stream << "                                        other parameter value is the name of a" << std::endl;
        stream << "                                        log file." << std::endl;
    }

    ///parses standard command line parameters
    void parse(int argc, CharT* args[])
    {
        std::vector<const CharT*> argv;
        argv.reserve(argc + 1);
        for (int i = 0; i < argc; ++i)
        {
            argv.push_back(args[i]);
        }
        argv.push_back(NULL);
        parse(argc, argv.data());
    }
    
    ///parses command line parameters
    void parse(int argc, const CharT* argv[])
    {
        reset();

        for (int i = 1; i < argc; ++i)
        {
            //get current argument
            bool argumentConsumed = false;
            const CharT* arg = argv[ i ];
            if (arg && arg[0] == '-')
            {
                if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("load-table"))))
                {
                    argumentConsumed = true;
                    m_TableFilePassed = true;
                    parseArg(arg, argc, argv, ++i, m_TableFileValue, false);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("label"))) || isEqual(arg + 1, STRING_LITERAL("a")))
                {
                    argumentConsumed = true;
                    m_LabelPassed = true;
                    parseArg(arg, argc, argv, ++i, m_LabelValue, false);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("top-down"))) || isEqual(arg + 1, STRING_LITERAL("t")))
                {
                    argumentConsumed = true;
                    m_TopDownPassed = true;
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("left-to-right"))) || isEqual(arg + 1, STRING_LITERAL("l")))
                {
                    argumentConsumed = true;
                    m_LeftToRightPassed = true;
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("row-header-index"))) || isEqual(arg + 1, STRING_LITERAL("w")))
                {
                    argumentConsumed = true;
                    m_RowHeaderIndexPassed = true;
                    parseArg(arg, argc, argv, ++i, m_RowHeaderIndexValue, false);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("column-header-index"))) || isEqual(arg + 1, STRING_LITERAL("n")))
                {
                    argumentConsumed = true;
                    m_ColumnHeaderIndexPassed = true;
                    parseArg(arg, argc, argv, ++i, m_ColumnHeaderIndexValue, false);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("pad-rows"))))
                {
                    argumentConsumed = true;
                    m_PadRowsPassed = true;
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("unload-table"))) || isEqual(arg + 1, STRING_LITERAL("x")))
                {
                    argumentConsumed = true;
                    m_LabelsOfTableFilesToUnloadPassed = true;
                    parseArgs(arg, argc, argv, ++i, m_LabelsOfTableFilesToUnloadValue, 1, SIZE_MAX);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("template-source-file"))) || isEqual(arg + 1, STRING_LITERAL("s")))
                {
                    argumentConsumed = true;
                    m_TemplateFilePassed = true;
                    parseArg(arg, argc, argv, ++i, m_TemplateFileValue, false);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("output-file"))) || isEqual(arg + 1, STRING_LITERAL("o")))
                {
                    argumentConsumed = true;
                    m_OutputFilePassed = true;
                    parseArg(arg, argc, argv, ++i, m_OutputFileValue, false);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("parameter"))) || isEqual(arg + 1, STRING_LITERAL("p")))
                {
                    argumentConsumed = true;
                    m_ParametersPassed = true;
                    parseArgs(arg, argc, argv, ++i, m_ParametersValue, 1, SIZE_MAX);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("use-intermediate-output-file"))) || isEqual(arg + 1, STRING_LITERAL("u")))
                {
                    argumentConsumed = true;
                    m_UseIntermediateOutputFilePassed = true;
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("intermediate-file-extension"))) || isEqual(arg + 1, STRING_LITERAL("e")))
                {
                    argumentConsumed = true;
                    m_IntermediateOutputFileExtensionPassed = true;
                    parseArg(arg, argc, argv, ++i, m_IntermediateOutputFileExtensionValue, false);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("markup-prefix"))))
                {
                    argumentConsumed = true;
                    m_MarkupPrefixPassed = true;
                    parseArg(arg, argc, argv, ++i, m_MarkupPrefixValue, false);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("markup-postfix"))))
                {
                    argumentConsumed = true;
                    m_MarkupPostfixPassed = true;
                    parseArg(arg, argc, argv, ++i, m_MarkupPostfixValue, false);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("markup"))) || isEqual(arg + 1, STRING_LITERAL("m")))
                {
                    argumentConsumed = true;
                    m_MarkupPassed = true;
                    parseArg(arg, argc, argv, ++i, m_MarkupValue, false);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("append-to-file"))))
                {
                    argumentConsumed = true;
                    m_AppendToFilePassed = true;
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("inlined"))))
                {
                    argumentConsumed = true;
                    m_InlinePassed = true;
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("inline-prefix"))) || isEqual(arg + 1, STRING_LITERAL("b")))
                {
                    argumentConsumed = true;
                    m_InlinePrefixPassed = true;
                    parseArg(arg, argc, argv, ++i, m_InlinePrefixValue, false);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("inline-postfix"))) || isEqual(arg + 1, STRING_LITERAL("c")))
                {
                    argumentConsumed = true;
                    m_InlinePostfixPassed = true;
                    parseArg(arg, argc, argv, ++i, m_InlinePostfixValue, false);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("inline-generated-postfix"))) || isEqual(arg + 1, STRING_LITERAL("d")))
                {
                    argumentConsumed = true;
                    m_InlineGeneratedPostfixPassed = true;
                    parseArg(arg, argc, argv, ++i, m_InlineGeneratedPostfixValue, false);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("inline-pad"))))
                {
                    argumentConsumed = true;
                    m_InlinePadPassed = true;
                    parseArg(arg, argc, argv, ++i, m_InlinePadValue, false);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("recycle"))) || isEqual(arg + 1, STRING_LITERAL("y")))
                {
                    argumentConsumed = true;
                    m_RecyclePassed = true;
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("can-change-table-list"))))
                {
                    argumentConsumed = true;
                    m_CanChangeTableListPassed = true;
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("reset"))) || isEqual(arg + 1, STRING_LITERAL("r")))
                {
                    argumentConsumed = true;
                    m_ResetPassed = true;
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("add-include-directory"))) || isEqual(arg + 1, STRING_LITERAL("i")))
                {
                    argumentConsumed = true;
                    m_IncludeDirectoriesPassed = true;
                    parseArgs(arg, argc, argv, ++i, m_IncludeDirectoriesValue, 1, SIZE_MAX);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("csv-delimiter-chars"))))
                {
                    argumentConsumed = true;
                    m_CsvDelimiterCharsPassed = true;
                    parseArg(arg, argc, argv, ++i, m_CsvDelimiterCharsValue, false);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("csv-comment-chars"))))
                {
                    argumentConsumed = true;
                    m_CsvCommentCharsPassed = true;
                    parseArg(arg, argc, argv, ++i, m_CsvCommentCharsValue, false);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("csv-quote-chars"))))
                {
                    argumentConsumed = true;
                    m_CsvQuoteCharsPassed = true;
                    parseArg(arg, argc, argv, ++i, m_CsvQuoteCharsValue, false);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("log-file"))))
                {
                    argumentConsumed = true;
                    m_LogFilePassed = true;
                    parseArg(arg, argc, argv, ++i, m_LogFileValue, false);
                }
            }
            
            if (!argumentConsumed && !m_TableFilePassed)
            {
                argumentConsumed = true;
                m_TableFilePassed = true;
                parseArg(STRING_LITERAL("--load-table"), argc, argv, i, m_TableFileValue, false);
            }
            if (!argumentConsumed)
            {
                throw std::runtime_error( std::string("Error unknown program option '") + arg + "'." );
            }
        }
    }

    void parse(const StringT& commandLine)
    {
        std::vector<StringT> args = boost::program_options::split_winmain(commandLine);
        std::vector<const CharT*> argv;
        argv.reserve(args.size() + 2);
        argv.push_back("");
        for (const StringT& arg : args)
        {
            argv.push_back(arg.c_str());
        }
        argv.push_back(NULL);
        parse(static_cast<int>(args.size() + 1), argv.data());
    }

    ///determines the command by checking the combination of parameters provided
    ECommand getCommand() const
    {
        if (
               m_TableFilePassed == false
            && m_LabelPassed == false
            && m_TopDownPassed == false
            && m_LeftToRightPassed == false
            && m_RowHeaderIndexPassed == false
            && m_ColumnHeaderIndexPassed == false
            && m_PadRowsPassed == false
            && m_LabelsOfTableFilesToUnloadPassed == false
            && m_TemplateFilePassed == true
            && m_OutputFilePassed == true
            && m_UseIntermediateOutputFilePassed == false
            && m_IntermediateOutputFileExtensionPassed == false
            && m_InlinePassed == false
            && m_InlinePrefixPassed == false
            && m_InlinePostfixPassed == false
            && m_InlineGeneratedPostfixPassed == false
            && m_InlinePadPassed == false
            && m_RecyclePassed == false
            && m_ResetPassed == false
            && m_IncludeDirectoriesPassed == false
            && m_CsvDelimiterCharsPassed == false
            && m_CsvCommentCharsPassed == false
            && m_CsvQuoteCharsPassed == false
            && m_LogFilePassed == false
        )
        {
            return eGenerate;
        }
        
        if (
               m_TableFilePassed == false
            && m_LabelPassed == false
            && m_TopDownPassed == false
            && m_LeftToRightPassed == false
            && m_RowHeaderIndexPassed == false
            && m_ColumnHeaderIndexPassed == false
            && m_PadRowsPassed == false
            && m_LabelsOfTableFilesToUnloadPassed == false
            && m_TemplateFilePassed == true
            && m_OutputFilePassed == true
            && m_UseIntermediateOutputFilePassed == true
            && m_AppendToFilePassed == false
            && m_InlinePassed == false
            && m_InlinePrefixPassed == false
            && m_InlinePostfixPassed == false
            && m_InlineGeneratedPostfixPassed == false
            && m_InlinePadPassed == false
            && m_RecyclePassed == false
            && m_ResetPassed == false
            && m_IncludeDirectoriesPassed == false
            && m_CsvDelimiterCharsPassed == false
            && m_CsvCommentCharsPassed == false
            && m_CsvQuoteCharsPassed == false
            && m_LogFilePassed == false
        )
        {
            return eGenerateUsingIntermediateFile;
        }
        
        if (
               m_TableFilePassed == false
            && m_LabelPassed == false
            && m_TopDownPassed == false
            && m_LeftToRightPassed == false
            && m_RowHeaderIndexPassed == false
            && m_ColumnHeaderIndexPassed == false
            && m_PadRowsPassed == false
            && m_LabelsOfTableFilesToUnloadPassed == false
            && m_TemplateFilePassed == true
            && m_InlinePassed == true
            && m_ResetPassed == false
            && m_IncludeDirectoriesPassed == false
            && m_CsvDelimiterCharsPassed == false
            && m_CsvCommentCharsPassed == false
            && m_CsvQuoteCharsPassed == false
            && m_LogFilePassed == false
        )
        {
            return eProcessInlineTemplateFile;
        }
        
        if (
               m_TableFilePassed == false
            && m_LabelPassed == false
            && m_TopDownPassed == false
            && m_LeftToRightPassed == false
            && m_RowHeaderIndexPassed == false
            && m_ColumnHeaderIndexPassed == false
            && m_PadRowsPassed == false
            && m_LabelsOfTableFilesToUnloadPassed == false
            && m_TemplateFilePassed == false
            && m_OutputFilePassed == false
            && m_ParametersPassed == false
            && m_UseIntermediateOutputFilePassed == false
            && m_IntermediateOutputFileExtensionPassed == false
            && m_MarkupPrefixPassed == false
            && m_MarkupPostfixPassed == false
            && m_MarkupPassed == false
            && m_AppendToFilePassed == false
            && m_InlinePassed == false
            && m_InlinePrefixPassed == false
            && m_InlinePostfixPassed == false
            && m_InlineGeneratedPostfixPassed == false
            && m_InlinePadPassed == false
            && m_RecyclePassed == false
            && m_CanChangeTableListPassed == false
            && m_ResetPassed == true
            && m_IncludeDirectoriesPassed == false
            && m_CsvDelimiterCharsPassed == false
            && m_CsvCommentCharsPassed == false
            && m_CsvQuoteCharsPassed == false
            && m_LogFilePassed == false
        )
        {
            return eResetGenerator;
        }
        
        if (
               m_TableFilePassed == false
            && m_LabelPassed == false
            && m_TopDownPassed == false
            && m_LeftToRightPassed == false
            && m_RowHeaderIndexPassed == false
            && m_ColumnHeaderIndexPassed == false
            && m_PadRowsPassed == false
            && m_LabelsOfTableFilesToUnloadPassed == true
            && m_TemplateFilePassed == false
            && m_OutputFilePassed == false
            && m_ParametersPassed == false
            && m_UseIntermediateOutputFilePassed == false
            && m_IntermediateOutputFileExtensionPassed == false
            && m_MarkupPrefixPassed == false
            && m_MarkupPostfixPassed == false
            && m_MarkupPassed == false
            && m_AppendToFilePassed == false
            && m_InlinePassed == false
            && m_InlinePrefixPassed == false
            && m_InlinePostfixPassed == false
            && m_InlineGeneratedPostfixPassed == false
            && m_InlinePadPassed == false
            && m_RecyclePassed == false
            && m_CanChangeTableListPassed == false
            && m_ResetPassed == false
            && m_IncludeDirectoriesPassed == false
            && m_CsvDelimiterCharsPassed == false
            && m_CsvCommentCharsPassed == false
            && m_CsvQuoteCharsPassed == false
            && m_LogFilePassed == false
        )
        {
            return eUnloadTable;
        }
        
        if (
               m_TableFilePassed == true
            && m_LabelsOfTableFilesToUnloadPassed == false
            && m_TemplateFilePassed == false
            && m_OutputFilePassed == false
            && m_ParametersPassed == false
            && m_UseIntermediateOutputFilePassed == false
            && m_IntermediateOutputFileExtensionPassed == false
            && m_MarkupPrefixPassed == false
            && m_MarkupPostfixPassed == false
            && m_MarkupPassed == false
            && m_AppendToFilePassed == false
            && m_InlinePassed == false
            && m_InlinePrefixPassed == false
            && m_InlinePostfixPassed == false
            && m_InlineGeneratedPostfixPassed == false
            && m_InlinePadPassed == false
            && m_RecyclePassed == false
            && m_CanChangeTableListPassed == false
            && m_ResetPassed == false
            && m_IncludeDirectoriesPassed == false
            && m_CsvDelimiterCharsPassed == false
            && m_CsvCommentCharsPassed == false
            && m_CsvQuoteCharsPassed == false
            && m_LogFilePassed == false
        )
        {
            return eLoadTable;
        }
        
        if (
               m_TableFilePassed == false
            && m_LabelPassed == false
            && m_TopDownPassed == false
            && m_LeftToRightPassed == false
            && m_RowHeaderIndexPassed == false
            && m_ColumnHeaderIndexPassed == false
            && m_PadRowsPassed == false
            && m_LabelsOfTableFilesToUnloadPassed == false
            && m_TemplateFilePassed == false
            && m_OutputFilePassed == false
            && m_ParametersPassed == false
            && m_UseIntermediateOutputFilePassed == false
            && m_IntermediateOutputFileExtensionPassed == false
            && m_MarkupPrefixPassed == false
            && m_MarkupPostfixPassed == false
            && m_MarkupPassed == false
            && m_AppendToFilePassed == false
            && m_InlinePassed == false
            && m_InlinePrefixPassed == false
            && m_InlinePostfixPassed == false
            && m_InlineGeneratedPostfixPassed == false
            && m_InlinePadPassed == false
            && m_RecyclePassed == false
            && m_CanChangeTableListPassed == false
            && m_ResetPassed == false
            && m_IncludeDirectoriesPassed == true
            && m_CsvDelimiterCharsPassed == false
            && m_CsvCommentCharsPassed == false
            && m_CsvQuoteCharsPassed == false
            && m_LogFilePassed == false
        )
        {
            return eAddIncludeDirectory;
        }
        
        if (
               m_TableFilePassed == false
            && m_LabelPassed == false
            && m_TopDownPassed == false
            && m_LeftToRightPassed == false
            && m_RowHeaderIndexPassed == false
            && m_ColumnHeaderIndexPassed == false
            && m_PadRowsPassed == false
            && m_LabelsOfTableFilesToUnloadPassed == false
            && m_TemplateFilePassed == false
            && m_OutputFilePassed == false
            && m_ParametersPassed == false
            && m_UseIntermediateOutputFilePassed == false
            && m_IntermediateOutputFileExtensionPassed == false
            && m_MarkupPrefixPassed == false
            && m_MarkupPostfixPassed == false
            && m_MarkupPassed == false
            && m_AppendToFilePassed == false
            && m_InlinePassed == false
            && m_InlinePrefixPassed == false
            && m_InlinePostfixPassed == false
            && m_InlineGeneratedPostfixPassed == false
            && m_InlinePadPassed == false
            && m_RecyclePassed == false
            && m_CanChangeTableListPassed == false
            && m_ResetPassed == false
            && m_IncludeDirectoriesPassed == false
            && m_CsvDelimiterCharsPassed == true
            && m_CsvCommentCharsPassed == false
            && m_CsvQuoteCharsPassed == false
            && m_LogFilePassed == false
        )
        {
            return eCsvDelimiterChars;
        }
        
        if (
               m_TableFilePassed == false
            && m_LabelPassed == false
            && m_TopDownPassed == false
            && m_LeftToRightPassed == false
            && m_RowHeaderIndexPassed == false
            && m_ColumnHeaderIndexPassed == false
            && m_PadRowsPassed == false
            && m_LabelsOfTableFilesToUnloadPassed == false
            && m_TemplateFilePassed == false
            && m_OutputFilePassed == false
            && m_ParametersPassed == false
            && m_UseIntermediateOutputFilePassed == false
            && m_IntermediateOutputFileExtensionPassed == false
            && m_MarkupPrefixPassed == false
            && m_MarkupPostfixPassed == false
            && m_MarkupPassed == false
            && m_AppendToFilePassed == false
            && m_InlinePassed == false
            && m_InlinePrefixPassed == false
            && m_InlinePostfixPassed == false
            && m_InlineGeneratedPostfixPassed == false
            && m_InlinePadPassed == false
            && m_RecyclePassed == false
            && m_CanChangeTableListPassed == false
            && m_ResetPassed == false
            && m_IncludeDirectoriesPassed == false
            && m_CsvDelimiterCharsPassed == false
            && m_CsvCommentCharsPassed == true
            && m_CsvQuoteCharsPassed == false
            && m_LogFilePassed == false
        )
        {
            return eCsvCommentChars;
        }
        
        if (
               m_TableFilePassed == false
            && m_LabelPassed == false
            && m_TopDownPassed == false
            && m_LeftToRightPassed == false
            && m_RowHeaderIndexPassed == false
            && m_ColumnHeaderIndexPassed == false
            && m_PadRowsPassed == false
            && m_LabelsOfTableFilesToUnloadPassed == false
            && m_TemplateFilePassed == false
            && m_OutputFilePassed == false
            && m_ParametersPassed == false
            && m_UseIntermediateOutputFilePassed == false
            && m_IntermediateOutputFileExtensionPassed == false
            && m_MarkupPrefixPassed == false
            && m_MarkupPostfixPassed == false
            && m_MarkupPassed == false
            && m_AppendToFilePassed == false
            && m_InlinePassed == false
            && m_InlinePrefixPassed == false
            && m_InlinePostfixPassed == false
            && m_InlineGeneratedPostfixPassed == false
            && m_InlinePadPassed == false
            && m_RecyclePassed == false
            && m_CanChangeTableListPassed == false
            && m_ResetPassed == false
            && m_IncludeDirectoriesPassed == false
            && m_CsvDelimiterCharsPassed == false
            && m_CsvCommentCharsPassed == false
            && m_CsvQuoteCharsPassed == true
            && m_LogFilePassed == false
        )
        {
            return eCsvQuoteChars;
        }
        
        if (
               m_TableFilePassed == false
            && m_LabelPassed == false
            && m_TopDownPassed == false
            && m_LeftToRightPassed == false
            && m_RowHeaderIndexPassed == false
            && m_ColumnHeaderIndexPassed == false
            && m_PadRowsPassed == false
            && m_LabelsOfTableFilesToUnloadPassed == false
            && m_TemplateFilePassed == false
            && m_OutputFilePassed == false
            && m_ParametersPassed == false
            && m_UseIntermediateOutputFilePassed == false
            && m_IntermediateOutputFileExtensionPassed == false
            && m_MarkupPrefixPassed == false
            && m_MarkupPostfixPassed == false
            && m_MarkupPassed == false
            && m_AppendToFilePassed == false
            && m_InlinePassed == false
            && m_InlinePrefixPassed == false
            && m_InlinePostfixPassed == false
            && m_InlineGeneratedPostfixPassed == false
            && m_InlinePadPassed == false
            && m_RecyclePassed == false
            && m_CanChangeTableListPassed == false
            && m_ResetPassed == false
            && m_IncludeDirectoriesPassed == false
            && m_CsvDelimiterCharsPassed == false
            && m_CsvCommentCharsPassed == false
            && m_CsvQuoteCharsPassed == false
            && m_LogFilePassed == true
        )
        {
            return eSetLogFile;
        }
        
        
        if (
               !m_TableFilePassed
            && !m_LabelPassed
            && !m_TopDownPassed
            && !m_LeftToRightPassed
            && !m_RowHeaderIndexPassed
            && !m_ColumnHeaderIndexPassed
            && !m_PadRowsPassed
            && !m_LabelsOfTableFilesToUnloadPassed
            && !m_TemplateFilePassed
            && !m_OutputFilePassed
            && !m_ParametersPassed
            && !m_UseIntermediateOutputFilePassed
            && !m_IntermediateOutputFileExtensionPassed
            && !m_MarkupPrefixPassed
            && !m_MarkupPostfixPassed
            && !m_MarkupPassed
            && !m_AppendToFilePassed
            && !m_InlinePassed
            && !m_InlinePrefixPassed
            && !m_InlinePostfixPassed
            && !m_InlineGeneratedPostfixPassed
            && !m_InlinePadPassed
            && !m_RecyclePassed
            && !m_CanChangeTableListPassed
            && !m_ResetPassed
            && !m_IncludeDirectoriesPassed
            && !m_CsvDelimiterCharsPassed
            && !m_CsvCommentCharsPassed
            && !m_CsvQuoteCharsPassed
            && !m_LogFilePassed
        )
        {
            return eNoOptionsGiven;
        }
        
        return eOptionsInvalid;
    }
    
    StringT getTableFile() const
    {
        return m_TableFileValue;
    }

    StringT getLabel() const
    {
        return m_LabelValue;
    }

    bool getTopDown() const
    {
        return m_TopDownPassed;
    }
    
    bool getLeftToRight() const
    {
        return m_LeftToRightPassed;
    }
    
    unsigned int getRowHeaderIndex() const
    {
        return m_RowHeaderIndexValue;
    }

    unsigned int getColumnHeaderIndex() const
    {
        return m_ColumnHeaderIndexValue;
    }

    bool getPadRows() const
    {
        return m_PadRowsPassed;
    }
    
    std::vector<StringT> getLabelsOfTableFilesToUnload() const
    {
        return m_LabelsOfTableFilesToUnloadValue;
    }

    StringT getTemplateFile() const
    {
        return m_TemplateFileValue;
    }

    StringT getOutputFile() const
    {
        return m_OutputFileValue;
    }

    std::vector<StringT> getParameters() const
    {
        return m_ParametersValue;
    }

    bool getUseIntermediateOutputFile() const
    {
        return m_UseIntermediateOutputFilePassed;
    }
    
    StringT getIntermediateOutputFileExtension() const
    {
        return m_IntermediateOutputFileExtensionValue;
    }

    StringT getMarkupPrefix() const
    {
        return m_MarkupPrefixValue;
    }

    StringT getMarkupPostfix() const
    {
        return m_MarkupPostfixValue;
    }

    StringT getMarkup() const
    {
        return m_MarkupValue;
    }

    bool getAppendToFile() const
    {
        return m_AppendToFilePassed;
    }
    
    bool getInline() const
    {
        return m_InlinePassed;
    }
    
    StringT getInlinePrefix() const
    {
        return m_InlinePrefixValue;
    }

    StringT getInlinePostfix() const
    {
        return m_InlinePostfixValue;
    }

    StringT getInlineGeneratedPostfix() const
    {
        return m_InlineGeneratedPostfixValue;
    }

    unsigned int getInlinePad() const
    {
        return m_InlinePadValue;
    }

    bool getRecycle() const
    {
        return m_RecyclePassed;
    }
    
    bool getCanChangeTableList() const
    {
        return m_CanChangeTableListPassed;
    }
    
    bool getReset() const
    {
        return m_ResetPassed;
    }
    
    std::vector<StringT> getIncludeDirectories() const
    {
        return m_IncludeDirectoriesValue;
    }

    StringT getCsvDelimiterChars() const
    {
        return m_CsvDelimiterCharsValue;
    }

    StringT getCsvCommentChars() const
    {
        return m_CsvCommentCharsValue;
    }

    StringT getCsvQuoteChars() const
    {
        return m_CsvQuoteCharsValue;
    }

    StringT getLogFile() const
    {
        return m_LogFileValue;
    }

    
    bool hasTableFile() const
    {
        return m_TableFilePassed;
    }
    
    bool hasLabel() const
    {
        return m_LabelPassed;
    }
    
    bool hasTopDown() const
    {
        return m_TopDownPassed;
    }
    
    bool hasLeftToRight() const
    {
        return m_LeftToRightPassed;
    }
    
    bool hasRowHeaderIndex() const
    {
        return m_RowHeaderIndexPassed;
    }
    
    bool hasColumnHeaderIndex() const
    {
        return m_ColumnHeaderIndexPassed;
    }
    
    bool hasPadRows() const
    {
        return m_PadRowsPassed;
    }
    
    bool hasLabelsOfTableFilesToUnload() const
    {
        return m_LabelsOfTableFilesToUnloadPassed;
    }
    
    bool hasTemplateFile() const
    {
        return m_TemplateFilePassed;
    }
    
    bool hasOutputFile() const
    {
        return m_OutputFilePassed;
    }
    
    bool hasParameters() const
    {
        return m_ParametersPassed;
    }
    
    bool hasUseIntermediateOutputFile() const
    {
        return m_UseIntermediateOutputFilePassed;
    }
    
    bool hasIntermediateOutputFileExtension() const
    {
        return m_IntermediateOutputFileExtensionPassed;
    }
    
    bool hasMarkupPrefix() const
    {
        return m_MarkupPrefixPassed;
    }
    
    bool hasMarkupPostfix() const
    {
        return m_MarkupPostfixPassed;
    }
    
    bool hasMarkup() const
    {
        return m_MarkupPassed;
    }
    
    bool hasAppendToFile() const
    {
        return m_AppendToFilePassed;
    }
    
    bool hasInline() const
    {
        return m_InlinePassed;
    }
    
    bool hasInlinePrefix() const
    {
        return m_InlinePrefixPassed;
    }
    
    bool hasInlinePostfix() const
    {
        return m_InlinePostfixPassed;
    }
    
    bool hasInlineGeneratedPostfix() const
    {
        return m_InlineGeneratedPostfixPassed;
    }
    
    bool hasInlinePad() const
    {
        return m_InlinePadPassed;
    }
    
    bool hasRecycle() const
    {
        return m_RecyclePassed;
    }
    
    bool hasCanChangeTableList() const
    {
        return m_CanChangeTableListPassed;
    }
    
    bool hasReset() const
    {
        return m_ResetPassed;
    }
    
    bool hasIncludeDirectories() const
    {
        return m_IncludeDirectoriesPassed;
    }
    
    bool hasCsvDelimiterChars() const
    {
        return m_CsvDelimiterCharsPassed;
    }
    
    bool hasCsvCommentChars() const
    {
        return m_CsvCommentCharsPassed;
    }
    
    bool hasCsvQuoteChars() const
    {
        return m_CsvQuoteCharsPassed;
    }
    
    bool hasLogFile() const
    {
        return m_LogFilePassed;
    }
    

    
private:    
    bool isOption(const CharT* arg)
    {
        bool result = false;
        if (arg && arg[0] == '-')
        {
            if (arg[1] == '-')
            {
                if(
                       isEqual(arg + 2, STRING_LITERAL("load-table"))
                    || isEqual(arg + 2, STRING_LITERAL("label"))
                    || isEqual(arg + 2, STRING_LITERAL("top-down"))
                    || isEqual(arg + 2, STRING_LITERAL("left-to-right"))
                    || isEqual(arg + 2, STRING_LITERAL("row-header-index"))
                    || isEqual(arg + 2, STRING_LITERAL("column-header-index"))
                    || isEqual(arg + 2, STRING_LITERAL("pad-rows"))
                    || isEqual(arg + 2, STRING_LITERAL("unload-table"))
                    || isEqual(arg + 2, STRING_LITERAL("template-source-file"))
                    || isEqual(arg + 2, STRING_LITERAL("output-file"))
                    || isEqual(arg + 2, STRING_LITERAL("parameter"))
                    || isEqual(arg + 2, STRING_LITERAL("use-intermediate-output-file"))
                    || isEqual(arg + 2, STRING_LITERAL("intermediate-file-extension"))
                    || isEqual(arg + 2, STRING_LITERAL("markup-prefix"))
                    || isEqual(arg + 2, STRING_LITERAL("markup-postfix"))
                    || isEqual(arg + 2, STRING_LITERAL("markup"))
                    || isEqual(arg + 2, STRING_LITERAL("append-to-file"))
                    || isEqual(arg + 2, STRING_LITERAL("inlined"))
                    || isEqual(arg + 2, STRING_LITERAL("inline-prefix"))
                    || isEqual(arg + 2, STRING_LITERAL("inline-postfix"))
                    || isEqual(arg + 2, STRING_LITERAL("inline-generated-postfix"))
                    || isEqual(arg + 2, STRING_LITERAL("inline-pad"))
                    || isEqual(arg + 2, STRING_LITERAL("recycle"))
                    || isEqual(arg + 2, STRING_LITERAL("can-change-table-list"))
                    || isEqual(arg + 2, STRING_LITERAL("reset"))
                    || isEqual(arg + 2, STRING_LITERAL("add-include-directory"))
                    || isEqual(arg + 2, STRING_LITERAL("csv-delimiter-chars"))
                    || isEqual(arg + 2, STRING_LITERAL("csv-comment-chars"))
                    || isEqual(arg + 2, STRING_LITERAL("csv-quote-chars"))
                    || isEqual(arg + 2, STRING_LITERAL("log-file"))
                )
                {
                    result = true;
                }
                
            }
            else if(
                   isEqual(arg + 1, STRING_LITERAL("a"))
                || isEqual(arg + 1, STRING_LITERAL("t"))
                || isEqual(arg + 1, STRING_LITERAL("l"))
                || isEqual(arg + 1, STRING_LITERAL("w"))
                || isEqual(arg + 1, STRING_LITERAL("n"))
                || isEqual(arg + 1, STRING_LITERAL("x"))
                || isEqual(arg + 1, STRING_LITERAL("s"))
                || isEqual(arg + 1, STRING_LITERAL("o"))
                || isEqual(arg + 1, STRING_LITERAL("p"))
                || isEqual(arg + 1, STRING_LITERAL("u"))
                || isEqual(arg + 1, STRING_LITERAL("e"))
                || isEqual(arg + 1, STRING_LITERAL("m"))
                || isEqual(arg + 1, STRING_LITERAL("b"))
                || isEqual(arg + 1, STRING_LITERAL("c"))
                || isEqual(arg + 1, STRING_LITERAL("d"))
                || isEqual(arg + 1, STRING_LITERAL("y"))
                || isEqual(arg + 1, STRING_LITERAL("r"))
                || isEqual(arg + 1, STRING_LITERAL("i"))
            )
            {
                result = true;
            }
        }
        return result;
    }
    
    bool isEqual(const char* a, const char* b)
    {
        bool result = (strcmp(a,b) == 0);
        return result;
    }

    bool isEqual(const wchar_t* a, const wchar_t* b)
    {
        bool result = (wcscmp(a,b) == 0);
        return result;
    }

    void printHelpCommandText(const char* text, std::ostream& stream)
    {
        size_t textSize = strlen(text);
        if (textSize > (cLeftColumnSize - 1) )
        {
            stream << text << std::endl;
            stream << std::string(cLeftColumnSize, ' ');
        }
        else
        {
            stream << text << std::string(cLeftColumnSize - textSize, ' ');
        }
    }

    template <typename T>
    bool parseArg(const CharT* option, int argc, const CharT** argv, int& index, T& parsedValue, bool valueOptional)
    {
        const char* argValue = argv[ index ];
        
        if ( index >= argc || !argValue || isOption(argValue))
        {
            if (valueOptional)
            {
                return false;
            }
            else
            {
                throw std::runtime_error( std::string("Option '") + option + "' requires a value.");
            }
        }
        if ( !convertTo( parsedValue, argValue))
        {
            throw std::runtime_error( std::string("Error parsing value '") + argValue + "' of option '" + option + "'.");
        }
        return true;
    }

    template <typename T>
    void parseArgs(const CharT* option, int argc, const CharT** argv, int& index, T& container, size_t minCount, size_t maxCount)
    {
        size_t argsParsed = 0;
        for (; argsParsed <= maxCount && index < argc; ++argsParsed, ++index)
        {
            typename T::value_type parsedValue;
            if (!parseArg(option, argc, argv, index, parsedValue, true))
            {
                --index;
                break;
            }
            container.push_back(parsedValue);
        }
        if (argsParsed < minCount)
        {
            throw std::runtime_error( std::string("Option '") + option + "' requires more values.");
        }
    }

    bool convertTo( std::string& value, const char* arg)
    {
        value = arg;
        return true;
    }
    
    bool convertTo( std::wstring& value, const wchar_t* arg)
    {
        value = arg;
        return true;
    }

    //helper function for converting parameters
    template <typename T>
    bool convertTo( T& value, const CharT* arg)
    {
        std::stringstream s;
        s << arg;
        s >> value;
        return s.eof();
    }

private:    
    bool m_TableFilePassed;
    bool m_LabelPassed;
    bool m_TopDownPassed;
    bool m_LeftToRightPassed;
    bool m_RowHeaderIndexPassed;
    bool m_ColumnHeaderIndexPassed;
    bool m_PadRowsPassed;
    bool m_LabelsOfTableFilesToUnloadPassed;
    bool m_TemplateFilePassed;
    bool m_OutputFilePassed;
    bool m_ParametersPassed;
    bool m_UseIntermediateOutputFilePassed;
    bool m_IntermediateOutputFileExtensionPassed;
    bool m_MarkupPrefixPassed;
    bool m_MarkupPostfixPassed;
    bool m_MarkupPassed;
    bool m_AppendToFilePassed;
    bool m_InlinePassed;
    bool m_InlinePrefixPassed;
    bool m_InlinePostfixPassed;
    bool m_InlineGeneratedPostfixPassed;
    bool m_InlinePadPassed;
    bool m_RecyclePassed;
    bool m_CanChangeTableListPassed;
    bool m_ResetPassed;
    bool m_IncludeDirectoriesPassed;
    bool m_CsvDelimiterCharsPassed;
    bool m_CsvCommentCharsPassed;
    bool m_CsvQuoteCharsPassed;
    bool m_LogFilePassed;

    StringT m_TableFileValue;
    StringT m_LabelValue;
    unsigned int m_RowHeaderIndexValue;
    unsigned int m_ColumnHeaderIndexValue;
    std::vector<StringT> m_LabelsOfTableFilesToUnloadValue;
    StringT m_TemplateFileValue;
    StringT m_OutputFileValue;
    std::vector<StringT> m_ParametersValue;
    StringT m_IntermediateOutputFileExtensionValue;
    StringT m_MarkupPrefixValue;
    StringT m_MarkupPostfixValue;
    StringT m_MarkupValue;
    StringT m_InlinePrefixValue;
    StringT m_InlinePostfixValue;
    StringT m_InlineGeneratedPostfixValue;
    unsigned int m_InlinePadValue;
    std::vector<StringT> m_IncludeDirectoriesValue;
    StringT m_CsvDelimiterCharsValue;
    StringT m_CsvCommentCharsValue;
    StringT m_CsvQuoteCharsValue;
    StringT m_LogFileValue;
};
