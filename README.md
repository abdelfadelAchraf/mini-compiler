# Lexical and Syntax Analyzer for a Custom Programming Language

Welcome to the **Lexical and Syntax Analyzer** project! This C program is designed to analyze source code written in a custom programming language. It performs lexical analysis to check if words exist in a predefined dictionary and syntax analysis to ensure that the code follows the rules of the language.

The analyzer identifies errors in the source code and generates a comprehensive report in `errors.txt`. This project is useful for understanding the basics of language processing, including tokenization, syntax validation, and error handling.


![Chaîne_de_compilation](https://github.com/user-attachments/assets/f6fc6643-980f-4b25-9c41-13a53d21289f)


## 🚀 Features

- **Lexical Analysis**:  
  - Checks if words in the source code match entries in a predefined dictionary.
  - Identifies invalid tokens or unrecognized words in the code.

- **Syntax Analysis**:  
  - Verifies the structure of various statements such as:
    - `debut` (start of the program block)
    - `var` (variable declaration)
    - `ecrire` (output statement)
    - `lire` (input statement)
    - `Si` (if statement)
  - Ensures the program adheres to basic programming language rules.

- **Error Reporting**:  
  - Generates an `errors.txt` file to log any detected lexical or syntax errors.
  - Provides clear and detailed error messages with line numbers.

---

## 🛠 Installation and Usage

### 1. Clone the Repository

Start by cloning the repository to your local machine:

```bash
git clone https://github.com/abdelfadelAchraf/mini-compiler.git
cd mini-compiler
