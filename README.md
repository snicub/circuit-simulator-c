# Circuit Simulator in C

This project is a **Circuit Simulator** implemented in C, designed to simulate logic circuits consisting of various gates such as AND, OR, NOT, XOR, NAND, NOR, Multiplexers, and Decoders. The program reads the circuit structure and variable data from an input file, simulates the circuit for all possible input combinations, and outputs the results.

## Features
- **Gate Types Supported:**
  - Logic Gates: AND, OR, NOT, XOR, NAND, NOR
  - Specialized Gates: Multiplexers, Decoders
- **Input/Output Simulation:**
  - Dynamically calculates all possible combinations of inputs and their corresponding outputs.
- **Variable and Gate Representation:**
  - Uses linked lists to manage variables and gates.
  - Supports INPUT, OUTPUT, and TEMP variables.
- **Memory Management:**
  - Allocates and frees memory dynamically for gates, variables, and circuit data structures.

## How It Works
1. **Input Parsing:**
   - Reads the circuit's input and output variables from the input file.
   - Reads the circuit gates and their configurations.
2. **Simulation:**
   - Iterates through all possible combinations of input values.
   - Simulates the behavior of the gates and calculates output values.
3. **Output:**
   - Prints each combination of input variables alongside their computed outputs.
4. **Cleanup:**
   - Frees all dynamically allocated memory after execution.
