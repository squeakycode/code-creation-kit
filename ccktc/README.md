## General
- Error Handling: Classes throw strongly typed exceptions derived from std::exception.
- Modules are loosely coupled using duck-typing.

## Overview

``` mermaid
graph TD
    subgraph Generator

        TemplateLoader
        Tokenizer
        subgraph TemplateProcessor
            Preprocessor
            subgraph ProcessingLevelControl
              subgraph "ProcessingLevel n=32"
                Parser
                LineCollector
              end
            end
        BackEndTokenizer
        TemplateProvidedTableLoader
        MacroProcessor["MacroProcessor\n(holds table list)"]
      end
    end
  
  Parser-- table info -->TemplateProvidedTableLoader
  Parser-- macros -->MacroProcessor
  LineCollector-- lines -->BackEndTokenizer
  TemplateLoader-- lines -->Tokenizer
  MacroProcessor-- text --> LineCollector
  Tokenizer-- tokens --> Preprocessor
  BackEndTokenizer-- tokens --> Preprocessor
  Preprocessor-- tokens --> Parser
  Preprocessor-- filename --> TemplateLoader
  ProcessingLevelControl-- lines -->OutputStream
```

### Template Loader
The template loader handles loading templates from the right directories. It knows the currently used directory and handles additional include directories.

### Tokenizers
#### Frontend
The tokenizer parsed the template lines and detects macro tags outputs the result as tokens that can be parsed.
#### Backend
The backend tokenizer additionally removes the delay dot marks in the tags.
