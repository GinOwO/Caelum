# C/C++ Project Style Guide

## Table of Contents

1. [Introduction](#introduction)
2. [Formatting and Code Style](#formatting-and-code-style)
   - [Indentation](#indentation)
   - [Line Length](#line-length)
   - [Naming Conventions](#naming-conventions)
   - [Braces](#braces)
3. [Documentation](#documentation)
   - [README](#readme)
4. [Git and GitHub Guidelines](#git-and-github-guidelines)
   - [Branching Strategy](#branching-strategy)
   - [Commit Messages](#commit-messages)
   - [Pull Requests](#pull-requests)

## 1. Introduction

Welcome to the C/C++ Project Style Guide. This guide outlines the coding standards and best practices to be followed by contributors to maintain consistency and high-quality code across the project.

## 2. Formatting and Code Style

### Indentation

Use 4 spaces for indentation; do not use tabs.

### Line Length

Limit each line of code to a maximum of 80 characters for improved readability.

### Naming Conventions

- Identifiers (functions, variables): Use `camelCase`.
- Classes/Structs: Begin with a capital letter and use `PascalCase`.
- Functions and variables: Begin with lowercase letters and use `camelCase`.

### Braces

Use the K&R style for braces:
```c
if (condition) {
    // code
} 
else {
    // code
}
```

### Main Code in Header Files

Avoid writing program code in header files. Place only declarations and small inline functions in headers.
Main code should be in the corresponding source files (.c or .cpp) to ensure modularity and prevent potential issues.

Add headers to CMakeLists.txt when required

### Header Guards

All header files must include header guards to prevent multiple inclusions. Use the following format:

```c

#ifndef HEADER_NAME_H
#define HEADER_NAME_H

// Content of the header

#endif // HEADER_NAME_H
```

## 3. Documentation

### README

Maintain an up-to-date and informative README.md at the root of the repository. Include details such as:
- Project overview
- Installation instructions
- Usage examples
- Plans
- Configuration details
- Contribution guidelines
- License information

## 4. Git and GitHub Guidelines

### Commit Messages

- Write clear and descriptive commit messages.
- For minor patches, a concise message is acceptable.
- Use the rebase strategy to keep commit history clean before merging.

### Pull Requests

- Create pull requests from feature/bugfix branches to `main`.
- Provide detailed descriptions of changes.
- Require code reviews before merging.
