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