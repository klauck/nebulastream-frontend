Getting Started
=================

## Requirements

**Requirements for development (Windows, Mac, Linux):**
* C++14 (or clang 3.4+)
* [CMake](https://cmake.org/download/) 
* [Bison (Optional)](https://www.gnu.org/software/bison/)

## Steps to Build
1. **Clone the Repository**  
   Start by cloning the Nebula SQL Parser repository:

   ```sh
   git clone https://github.com/nebulastream/sql-parser.git
   ```
2. **Build the Project**  
   Use the following commands to build your project and run the executable:

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```
3. **Build the Project**  
   Run the following commands inside build folder to run the tests:
    ```shell
    ./nebulastream_tests/parser_tests/parser_tests
    ```
## Extending the Grammar
This sections provides an overview of how to extend the grammar.

In order to extend the grammar, you have to change the files in `/src/libraries/pg_query/grammar` directory.

The grammar for each statement is defined in a separate file e.g, `select statement` is defined in `select.y` in **statements** directory file. Each rule's return type is defined in corresponding `.yh` file e.g, `select.yh`.

