# AST Optimization for Boolean Expressions in C Compiler

## Overview
This project implements a Boolean optimization pass for a subset of the C programming language, targeting the abstract syntax tree (AST) to enhance both compilation efficiency and runtime performance. It focuses on optimizing Boolean expressions using Boolean Algebra and nested loops unrolling.

## Authors
- Khanh Khuat (ltk30)
- Matt Le (dml122)
- Nofer Xue (sxx261)

## Institution
Case Western Reserve University

## Features
- **Optimization Techniques**: Includes constant folding and application of Boolean identities to simplify expressions.
- **AST Manipulation**: Direct manipulation of the AST to streamline expressions before code generation.

## Implementation
The optimization pass identifies inefficient Boolean expressions within the AST and simplifies them using predefined rules and pattern matching. Modifications include node replacement, removal, and addition to optimize overall code execution.

## Testing Methodology
- **Performance Metrics**: Evaluation based on AST structures, compiled binary size, compilation time, and runtime memory and CPU resource usage.
- **Test Environment**: Tests conducted using the Windows Subsystem for Linux (WSL).

## Results and Conclusion
Example:
```c
int puts(string str);
int main()
{
    if ((5 > 3 || 10 > 7) || (4 >= 5 && 10 <= 12))
    {
        puts("Hello World!");
    }

    return 0;
}
```
<table>
  <tr>
    <td style="vertical-align: top;">
      <p>Before Optimization:</p>
      <img src="noOptimization.png" alt="AST Before Optimization" style="width:100%">
    </td>
    <td style="vertical-align: top;">
      <p>After Optimization:</p>
      <img src="afterOptimization.png" alt="AST After Optimization" style="width:100%">
    </td>
  </tr>
</table>

After optimization, the if condition would be just True because (5 > 3) returns True -> (5 > 3 || 10 > 7) would be True without validating (10 > 7) which would make the whole if condition True without validating (4 >= 5 && 10 <= 12). This is the Identity Law from Boolean Algebra.    

## Acknowledgments
Special thanks to Professor Vipin Chaudhary and the Teaching Assistants for their guidance on LLVM implementation.

## Repository Contents
- We include an Optimization method for logical and, logical or and if statement: MyOptznPass() where we implement our optimization pass
- We have some test cases in the tests folder that the TAs could use to see our implementation results

## How to Run
- Run build.sh to build the project
- Run executeTests.sh to run the test cases
- Check the AST structure for each test cases like the ones we showcase above. You could run the same test cases on the initial Programming 4 without Optimization to see the differences.  

