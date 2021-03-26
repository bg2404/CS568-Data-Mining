# CS568: Data Mining

## Group 7: dataHacks

This repository houses all the code related to both incremental and static implementation of SUBCLU of CS568: Data Mining Project.

### How to check accuracy

```BASH
$ python3 accuracy.py {static_file} {incr_file} {updated_file}
```

`static_file` - The file containing clusters formed by static SUBCLU in a particular subspace

`incr_file` - The file containing clusters formed by incremental SUBCLU in the same subspace as above

`updated_file` - The file containing the remaining points after points are inserted/deleted by incremental SUBCLU.
