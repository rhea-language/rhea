<p align="center">
    <img src="assets/zhivo-logo.png" width="180" />
</p>
<h1 align="center">Zhivo (Живо)</h1>

<p align="center">
    <img alt="Build CI for Linux" src="https://github.com/nthnn/zhivo/actions/workflows/build_ci_linux.yml/badge.svg" />
    <img alt="Build CI for Darwin" src="https://github.com/nthnn/zhivo/actions/workflows/build_ci_darwin.yml/badge.svg" />
    <img alt="Build CI for Windows" src="https://github.com/nthnn/zhivo/actions/workflows/build_ci_windows.yml/badge.svg" />
    <img alt="VSCode Extension Build" src="https://github.com/nthnn/zhivo/actions/workflows/build_ci_vsce.yml/badge.svg" />
</p>

JIT-compiled dynamic general-purpose programming language utilizing system-available GPU with built-in concurrency and unconventional expressiveness.

<p align="center">
    <a href="https://zhivo.vercel.app"><img src="https://img.shields.io/badge/Learn%20More-007ec6?style=for-the-badge&logoColor=white&logo=Google-Chrome" /></a>
</p>

```zhivo
#!/bin/zhivo

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

## License

Zhivo is distributed under the GNU General Public License v3.0. For further details, refer to the LICENSE file.

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