# Sequence Pair
Programming Asignment #2 of Physical Design for Nanometer ICs, Spring 2022

|系級|學號|姓名|日期|
|:---:|:---:|:---:|:---:|
|電子所碩一|r10943109|Shiuan-Yun Ding|2022-4-9|

[Read my report on HackMD](https://hackmd.io/@mirkat1206/BJZx_JuQc)

## How to compile?

To generate an executable binary `./bin/fp`, simply type `make`.

```
make
```

## How to execute?

Use the following command format to execute.

```
./bin/fp <alpha value> <input.block file name> <input.net file name> <output file name>
```

## How to plot?
Use the following command format to plot every resulting floorplan in the directory `./output_pa2/`.

```
python3 plot.py
```


## How to verify?
Use the following command format to verify the output.

- NTU checker:
```
./checker/checker <input.block file name> <input.net file name> <output file name> <alpha value>
```
- NCTU verifier:
```
./checker/verifier  <alpha value> <input.block file name> <input.net file name> <output file name>
```

## Simple commands
Simple commands to solve/plot/check/verify all testcases.
```
sh run            // solve all testcase in input_pa2
sh run_plot       // generate plots for all output.rpt in output_pa2
sh run_checker    // check all output.rpt (NTU)
sh run_verifier   // verify all output.rpt (NCTU)
```

## Directory Structure
```bash
├─ .
    ├─ bin/         
    ├─ checker/
    ├─ input_pa2/
    ├─ output_pa2/
    ├─ plot/   
    ├─ src/         
```

## Program Structure
```bash
├─ src
    ├─ main.cpp  // Main
    ├─ module.h  // Terminal/Block header file
    ├─ sp.h      // Sequence Pair header file
    ├─ sp.cpp    // Sequence Pair 
    ├─ veb.h     // Van Emde Boas Tree header file
    ├─ veb.cpp   // Van Emde Boas Tree
    ├─ plot.py   // Plot floorplan
```

## Features
- Implement Sequence Pair.
- Follow the paper *[FAST-SP: A Fast Algorithm for Block Placement based on Sequence Pair](https://ieeexplore.ieee.org/document/913361)* to compute the coordinates of every block with the longest common subsequence algorithm.
- Generate 100 random partition for initail partition
- Utilize [Van Emde Boas Tree](https://www.geeksforgeeks.org/van-emde-boas-tree-set-1-basics-and-construction/) structure which support search, successor, predecessor, insert and delete operations in $O(\lg\lg n)$ time.
- Following [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

## Issues
- Cost function only considers area and does not condsider wirelength.
- Random shuffle is not really random in `./src/sp.cpp` line 205, 206. This may be handle by shuffle with random engine in `./src/sp.cpp` line 203, 204. However, the testcases perform better with random shuffle, I did not use shuffle with random engine.
    - There is no hidden case in this PA 🙃.

## Fixed Outline Floorplanning
### Problem Description
Given a set of rectangular blocks and a set of nets, generate a floorplan of all blocks within a given boundary size and without any overlaps.

### Input.block File Format
```
Outline: <outline width> <outline height>
NumBlocks: <number of blocks>
NumTerminals: <number of terminals>

<block name> <block width> <block height>
... more blocks ...

<terminal name> terminal <terminal x coordinate> <terminal y coordinate>
... more terminals ...
```

Example:

```
Outline: 100 100
NumBlocks: 4
NumTerminals: 2

A 40 50
B 60 50
C 60 50
D 40 50

VSS terminal 0 0
VDD terminal 100 0
```

### Input.nets File Format
```
NumNets: <number of nets>

NetDegree: <number of terminals in this net>
<terminal name>
... more terminal names ...

... more "NetDegree" and "terminal names"
```

Example:

```
NumNets: 2 
NetDegree: 3 
A 
C 
D 
NetDegree: 2 
B 
D 
```

### Output File Format
```
<final cost>
<total wirelength>
<chip area>
<chip width> <chip height>
<program runtime>

<block name> <x1> <y1> <x2> <y2>
... more blocks ...
```

Note that $cost = \alpha A+ (1 - \alpha)W$, where $A$ is the total area and $W$ is the total wirelength.

Example:

```
5085 
170 
10000 
100  100 
0.24 
A  0   50  40  100 
B  40  50  100 100 
C  0   0   60  50 
D  60  0   100 50 
```

## Experiment
- All testcase can find a legal solution.
- My optimization only considers area and does not condsider wirelength. That is, the alpha value does not affect my result.
- The solutions of the same testcase may vary from each execution because of the random operations in Simulated Annealing.
- For testcase *ami49*, it may take much longer time to find a legal solution when the random operations are with bad luck.

### Results
|testcase|outline </br> width|outline </br> height|chip </br> width|chip </br> height|chip </br> area|total </br> wirelength|cost </br> $\alpha = 0.5$|runtime </br> (s)|
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|ami33|1326|1205|1204|1078|1297912|124551.5|711231.75|12.54|
|ami49|5336|7673|5068|7448|37746464|1892576.0|19819520.0|24.41|
|apte|11894|6314|9478|5490|52034220|997334.0|26515776.5|0.53|
|hp|5412|3704|3892|2520|9807840|314478.0|5061159.5|0.62|
|xerox|6937|5379|5264|3885|20450640|686979.0|10568810.0|0.57|

### Floorplan visualization
I wrote a python program `plot.py` to visualize the generated floorplan.

|![](https://i.imgur.com/jx5Vq86.png)|
|:---:|
|ami33|

|![](https://i.imgur.com/CWXyi7d.png)|
|:---:|
|ami49|

|![](https://i.imgur.com/Kk8YvOX.png)|
|:---:|
|apte|

|![](https://i.imgur.com/9tvvAKy.png)|
|:---:|
|hp|

|![](https://i.imgur.com/eKQVM8w.png)|
|:---:|
|xerox|

## Roadmap
- [x] Understand Sequence Pair
- [x] Implement Sequence Pair
    - [x] Parse input files
    - [x] Print output format
    - [x] Data Structure
        - [x] module.h
            - [x] Terminal
            - [x] Block
            - [x] Net
        - [x] veb .h
            - [x] Van Emde Boas Tree
        - [x] sp.h
    - [x] Coordinates Computation
    - [x] Cost Computation 
        - [x] Wirelength
            - [x] HPWL
        - [x] Area
    - [x] Simulation Annealing
- [x] GUI to show the floorplannning
    - [x] Simple Plot
