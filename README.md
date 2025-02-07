<p align="center">
    <img src="https://raw.githubusercontent.com/n8lang/assets/refs/heads/main/n8-logo/n8-logo.png" width="180" />
</p>
<h1 align="center">N8 Programming Language</h1>

<p align="center">
    <img alt="Build CI for Linux" src="https://github.com/n8lang/n8/actions/workflows/build_ci_linux.yml/badge.svg" />
    <img alt="Build CI for Darwin" src="https://github.com/n8lang/n8/actions/workflows/build_ci_darwin.yml/badge.svg" />
    <img alt="Build CI for Windows" src="https://github.com/n8lang/n8/actions/workflows/build_ci_windows.yml/badge.svg" />
    <img alt="Build CI for WebAssembly" src="https://github.com/n8lang/n8/actions/workflows/build_ci_wasm.yml/badge.svg" />
</p>

JIT-compiled dynamic general-purpose programming language utilizing OpenMP parallelism with built-in concurrency and unconventional expressiveness.

<p align="center">
    <a href="https://n8.vercel.app"><img src="https://img.shields.io/badge/Learn%20More-007ec6?style=for-the-badge&logoColor=white&logo=Google-Chrome" /></a>
</p>

```n8
#!/opt/n8lang/bin/n8

val count = 99;

while(count > 0) {
    val bottle = if(count == 1) "bottle" else "bottles";

    render! count + " " + bottle + " of beer on the wall";
    render! count + " " + bottle + " of beer,";
    render! "Take one down, pass it around,";

    count = count - 1;

    if(count > 0)
        render! count + " " + bottle + " of the beer on the wall.\r\n"
    else render! "\r\nNo more " + bottle + " of beer on the wall.";
};
```

> [!WARNING]
> This repository is under a slow development. (Busy on my job)

## Why N8?

- Direct mathematical operations for vectors and/or arrays.
- Syntax-level concurrency and multithreading.
- Language expressiveness with unconventional grammars.
- Easy foreign-function interfacing for native interoperability.
- Simplified ecosystem and project structuing.

## Use Cases

- **High-performance Scientific Computing**

    N8's ability to perform direct mathematical operations on vectors and arrays makes it ideal for scientific computing applications such as simulations, data analysis, and mathematical modeling. Its concurrency support ensures that heavy computational tasks can run efficiently on multi-core CPUs.

- **Data Pipelines and ETL (Extract, Transform, Load)**

    With the N8's expressiveness, it can be used to build efficient data pipelines for ETL processes in big data applications. Its support for complex data structures and built-in concurrency enables scalable, high-throughput data processing, making it ideal for tasks like data aggregation, transformation, and loading in distributed systems.

- **Machine Learning Model Prototyping**

    N8 is ideal for Machine Learning and AI due to its efficient mathematical operations, built-in concurrency, flexible syntax, seamless integration with native libraries, and a simplified project structure, enabling rapid development and optimization of advanced algorithms and applications.

- **Game Development**

    Language's native support for concurrency and vector operations is perfect for game engines that require real-time physics simulations, AI processing, and rendering pipelines. Developers can also easily interface with native libraries for advanced graphical rendering or sound processing.

## Contribution and Feedback

Contributions and feedback are all welcome to enhance this library. If you encounter any issues, have suggestions for improvements, or would like to contribute code, please do so.

## License

N8 is distributed under the GNU General Public License v3.0. For further details, refer to the LICENSE file.

```
This program is free software: you can redistribute it and/or modify  
it under the terms of the GNU General Public License as published by  
the Free Software Foundation, version 3.

This program is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
General Public License for more details.

You should have received a copy of the GNU General Public License 
along with this program. If not, see <http://www.gnu.org/licenses/>.
```
