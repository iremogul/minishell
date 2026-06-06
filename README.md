*This project has been created as part of the 42 curriculum by iogul and skaraosm.*
Minishell - As Beautiful As a Shell
Description

Minishell is a simple, interactive UNIX shell project designed to mimic the core behaviors of the bash (Bourne Again SHell) program. The primary goal of this project is to learn and practice fundamental operating system concepts, including process creation and synchronization, file descriptor manipulation, pipe implementations, and asynchronous signal handling. By strictly adhering to C programming standards, a highly secure system has been built to completely prevent memory leaks.
Architecture and Technical Choices

    Linked List over AST: Instead of using complex Abstract Syntax Trees (AST) to process commands, a Doubly Linked List architecture was chosen, as it better suits the linear pipeline logic. The workflow is entirely modular, consisting of: Lexer (Lexical Analysis), Parser (Syntax Analysis), Expander (Variable Expansion), and Executer.

    Garbage Collector (GC): A custom memory management system replaces the traditional malloc and free cycle. All memory allocations are performed using a custom gc_malloc function and tracked within a static linked list. Upon program termination, all allocated memory is safely returned to the operating system at once without any leaks. Standard free calls have been completely eliminated from the codebase.

Features

    Built-in Commands: Supports echo (with the -n option), cd, pwd, export, unset, env, and exit.

    Redirections and Pipes: Handles input (<), output (> and >>), heredoc (<<), and the pipe (|) operator to connect the inputs and outputs of different processes.

    Signal Handling: In interactive mode, ctrl-C (SIGINT), ctrl-D (EOF), and ctrl-\ (SIGQUIT) signals are accurately handled. To comply with strict norm rules, a single global variable is used solely to track the signal state.

    Variables and Quotes: Environment variables ($VAR) and exit status codes ($?) are dynamically expanded, strictly respecting the rules of single (') and double (") quotes.

Instructions

To compile and run the system, you need a C compiler (such as GCC or Clang), the make utility, and the Readline library installed on your system.
Bash

# Clone the repository
git clone <repository_url> 
cd minishell

# Compile the project
make

# Launch the shell
./minishell

Resources

    GNU Bash Reference Manual: Used as the primary reference for quoting rules, variable expansions, and the standard behaviors of built-in commands.

    Linux Programmer's Manual (man pages): Consulted for the proper implementation of system calls such as fork, execve, pipe, dup2, waitpid, and sigaction.

    GNU Readline Documentation: Referenced for implementing the interactive command prompt and managing command history.

    AI Usage: Artificial Intelligence tools (LLMs) were utilized to brainstorm potential edge cases during the Lexer phase (e.g., complex unclosed quote scenarios) and to structure this README documentation according to 42 standards. The project's core logic, Garbage Collector architecture, and all functional code implementations were developed entirely independently.