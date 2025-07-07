# 🐚 Minishell

[![42 School](https://img.shields.io/badge/42-School-000000?style=flat&logo=42&logoColor=white)](https://42.fr/)
[![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

> A minimal shell implementation in C that mimics the behavior of bash, featuring command execution, built-in commands, pipes, redirections, and signal handling.

## 📋 Table of Contents

- [About](#about)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Supported Features](#supported-features)
- [Project Structure](#project-structure)
- [Technical Implementation](#technical-implementation)
- [Examples](#examples)
- [Testing](#testing)
- [Contributing](#contributing)
- [Authors](#authors)

## 🎯 About

Minishell is a 42 School project that challenges students to create their own shell from scratch. This implementation provides a functional command-line interface with many features found in modern shells like bash, including:

- **Command execution** with PATH resolution
- **Built-in commands** (cd, echo, pwd, export, unset, env, exit)
- **Input/Output redirections** (`<`, `>`, `>>`)
- **Pipes** for command chaining (`|`)
- **Here documents** (`<<`)
- **Environment variables** and expansion
- **Signal handling** (Ctrl+C, Ctrl+D, Ctrl+\)
- **Quote handling** (single and double quotes)

## ✨ Features

### Core Shell Functionality
- ✅ **Interactive prompt** with command history
- ✅ **Command execution** with external program support
- ✅ **PATH resolution** for executables
- ✅ **Environment variable management**
- ✅ **Signal handling** (SIGINT, SIGQUIT)
- ✅ **Exit status management**

### Built-in Commands
- ✅ `echo` with `-n` option
- ✅ `cd` with relative/absolute paths and `~`, `-` support
- ✅ `pwd` for current directory
- ✅ `export` for setting environment variables
- ✅ `unset` for removing environment variables
- ✅ `env` for displaying environment
- ✅ `exit` with numeric argument support

### Advanced Features
- ✅ **Pipes** (`|`) for command chaining
- ✅ **Input redirection** (`<`)
- ✅ **Output redirection** (`>`, `>>`)
- ✅ **Here documents** (`<<`) with expansion
- ✅ **Quote handling** (single `'` and double `"` quotes)
- ✅ **Variable expansion** (`$VAR`, `$?`)
- ✅ **Wildcard expansion** (basic globbing)

## 🚀 Installation

### Prerequisites
- GCC compiler
- Make
- Readline library (`libreadline-dev` on Ubuntu/Debian)

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/yourusername/minishell.git
cd minishell

# Compile the project
make

# Run the shell
./minishell
```

### Make Targets
```bash
make        # Compile the project
make clean  # Remove object files
make fclean # Remove object files and executable
make re     # Rebuild the project
```

## 💻 Usage

### Starting the Shell
```bash
./minishell
```

You'll see the minishell prompt:
```
minishell> 
```


## 🔧 Built-in Commands

| Command | Description | Examples |
|---------|-------------|----------|
| `echo` | Display text | `echo "Hello"`, `echo -n "No newline"` |
| `cd` | Change directory | `cd ~`, `cd ..`, `cd /path` |
| `pwd` | Print working directory | `pwd` |
| `export` | Set environment variables | `export VAR=value` |
| `unset` | Remove environment variables | `unset VAR` |
| `env` | Display environment | `env` |
| `exit` | Exit the shell | `exit`, `exit 42` |

## 🎛️ Supported Features

### Redirections
```bash
# Output redirection
minishell$ echo "Hello" > file.txt
minishell$ echo "World" >> file.txt

# Input redirection
minishell$ cat < file.txt

# Here document
minishell$ cat << EOF
> Line 1
> Line 2
> EOF
```

### Pipes
```bash
# Simple pipe
minishell$ ls | grep .c

# Multiple pipes
minishell$ cat file.txt | grep pattern | wc -l
```

### Variable Expansion
```bash
# Environment variables
minishell$ echo $HOME
minishell$ echo $USER

# Exit status
minishell$ echo $?

# Quoted expansion
minishell$ echo "User: $USER"
minishell$ echo 'Literal: $USER'
```

### Quotes
```bash
# Single quotes (literal)
minishell$ echo 'Hello $USER'

# Double quotes (with expansion)
minishell$ echo "Hello $USER"

# Mixed quotes
minishell$ echo "It's a 'great' day"
```

## 📁 Project Structure

```
minishell/
├── Makefile                    # Build configuration
├── README.md                   # Project documentation
├── include/
│   ├── minishell.h            # Main header file
│   └── libft/                 # Custom C library
├── src/
│   ├── main.c                 # Main program entry
│   ├── lexer/                 # Tokenization and lexical analysis
│   │   ├── lexer.c
│   │   ├── lexer_segments.c
│   │   ├── lexer_segments_utils.c
│   │   └── lexer_token_status.c
│   ├── parser/                # Command parsing and validation
│   │   ├── parser.c
│   │   ├── parser_utils.c
│   │   └── parser_validation.c
│   ├── expand/                # Variable and quote expansion
│   │   ├── expander.c
│   │   └── expander_utils.c
│   ├── execution/             # Command execution engine
│   │   ├── execution.c
│   │   ├── execution_utils.c
│   │   ├── run_builtins.c
│   │   ├── signals.c
│   │   ├── signals_handlers.c
│   │   ├── utils.c
│   │   ├── heredoc/           # Here document handling
│   │   ├── pipe/              # Pipeline execution
│   │   └── redirections/      # I/O redirection handling
│   ├── builtins/              # Built-in command implementations
│   │   ├── cd.c
│   │   ├── echo.c
│   │   ├── env.c
│   │   ├── exit.c
│   │   ├── export.c
│   │   ├── export_utils.c
│   │   ├── pwd.c
│   │   └── unset.c
│   └── env/                   # Environment management
│       ├── environment.c
│       └── env_to_envp.c
```

## 🛠️ Technical Implementation

### Architecture Overview

The minishell follows a modular architecture with clear separation of concerns:

1. **Lexical Analysis** - Tokenizes input into meaningful units
2. **Parsing** - Builds command structures from tokens
3. **Expansion** - Handles variable and quote expansion
4. **Execution** - Executes commands with proper I/O handling

### Key Data Structures

```c
// Token structure for lexical analysis
typedef struct s_token {
    char        *value;      // Token value
    t_segment   **segments;  // Quoted segments
    int         seg_count;   // Number of segments
    int         quoted;      // Quote status
} t_token;

// Command structure
typedef struct s_cmd {
    char        **args;         // Command arguments
    char        *infile;        // Input file
    char        *outfile;       // Output file
    int         append;         // Append mode flag
    int         heredoc;        // Here document flag
    int         heredoc_fd;     // Here document file descriptor
    struct s_cmd *next;         // Next command in pipeline
} t_cmd;

// Environment variable structure
typedef struct s_env {
    char        *key;    // Variable name
    char        *value;  // Variable value
    struct s_env *next;  // Next variable
} t_env;
```

### Process Flow

1. **Input Reading** - Uses readline for interactive input
2. **Lexical Analysis** - Tokenizes input handling quotes and operators
3. **Expansion** - Expands variables and handles quotes
4. **Parsing** - Builds command pipeline from tokens
5. **Execution** - Forks processes and handles I/O
6. **Cleanup** - Frees memory and restores state

## 📚 Examples

### Basic Command Execution
```bash
minishell$ ls -la
minishell$ cat /etc/passwd | grep root
minishell$ echo "Current directory: $(pwd)"
```

### File Operations
```bash
# Create and edit files
minishell$ echo "Hello World" > hello.txt
minishell$ cat hello.txt
minishell$ echo "Goodbye" >> hello.txt

# Use here documents
minishell$ cat << END > script.sh
#!/bin/bash
echo "This is a script"
END
```

### Environment Management
```bash
# Set variables
minishell$ export NAME="John"
minishell$ export AGE=25

# Use variables
minishell$ echo "Hello $NAME, you are $AGE years old"

# Check environment
minishell$ env | grep NAME
```

### Pipeline Examples
```bash
# Text processing
minishell$ cat large_file.txt | grep "pattern" | sort | uniq -c

# System monitoring
minishell$ ps aux | grep minishell | wc -l
```

### Coding Standards
- Follow 42 School norm
- Handle memory leaks (use valgrind)

## 👥 Authors

- **aysadeq** - *parsing part* - [@DKOK](https://github.com/DKOK01)
- **ael-mans** - *execution part* - [@aziddine](https://github.com/aziddineelm)

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- 42 School for the project specifications
- The bash manual for reference implementation
- The readline library for input handling

## 📚 Additional Resources

- [Bash Manual](https://www.gnu.org/software/bash/manual/)
- [Advanced Bash Scripting Guide](https://tldp.org/LDP/abs/html/)
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/rltop.html)

---

*Made with ❤️ at 42 School*
