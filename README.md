![logo_master_thesis](https://github.com/DVerbakel/MasterThesis/assets/77973924/e1317b2d-763d-4b6c-969f-eeaa6384fe65)
# Designing q-ary Transformations for Symmetric Cryptography

In this master thesis, the following research question will be answered:
*How can we design an efficient q-ary transformation with good propagation properties?*


## Software Implementation
The folder SoftwareTools holds the implementation of a certain instance of a ternary transformation as discussed in Chapter 8 of the master thesis.

### Install

```
$ cd SoftwareTools
$ make
```

The executable will be located in the same directory. One can run the main by entering `./main`.

### Delete

```
$ make clean
```

This comment will delete all binaries and object files.



## Tools for differential and linear trail search
The folder TrailSearchTools holds the algorithms 2, 3 and 4 as discussed in Chapter 5 of the master thesis.
Note that the algorithms in the files `find_compatible_inputs_general.py` and `find_compatible_outputs_general.py` are implemented to work for ternary transformations (transformations over the field F3).

### Install

No installation needed. Run the Python files by issuing the following command:
```
python3 [some_python_file.py]
```

### Data

The results of the files `find_compatible_inputs_general.py` and `find_compatible_outputs_general.py` are saved in the `data` folder as respectively `compatible_inputs_general.txt` and `compatible_outputs_general.txt`. 
