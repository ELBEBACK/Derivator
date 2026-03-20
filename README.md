# Derivative Calculator

*A derivative calculator featuring tokenization, recursive parsing, differentiation, and simplification, including Abstract Syntax Tree (AST) graph visualization, all implemented in C*

----
## Installation process

- **Clone the repository**
```bash
git clone git@github.com:ELBEBACK/LIRS-Cache.git
```
- **Navigate to the repository directory** 
- **Build the project using the following sequence of commands:**
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build 
```
----
## How to use 

- **To run the program navigate to the `build` directory and proceed with the following command**
```bash
./derivator <input_file>
```
- **The input requires the sequence of terminals that represents mathematical expression featuring a '$' literal in the end**

> sin( x ) + cos( x ^ 2 - 4 ) $ 

- **The output consists of 3 files: `ast_original.dot` `ast_derivated_raw.dot` `ast_derivated`**
- **These represent the ast structure of original, derivated without simplification and derived after simplification expressions**

- **As a sample there is an input file `bibubig.txt` in the `assets/input/`, and its processed resulting `.dot` files in the `assets/dot/`**

- **To turn those `.dot` files into images install the `graphviz` packages and run the following command**

```bash
dot -Tpng <src> -o <target.png> 
```
- **There are pre-converted images for sample in the `assets/png/`**

#### Original
![sample](assets/png/sample_original.png)
#### Derived before simplification
![sample](assets/png/sample_derivated_raw.png)
#### Derived after simplification
![sample](assets/png/sample_derivated.png)

----
