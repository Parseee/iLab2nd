# ARC caching implementation in C++. 

`Query format`: \<cache_size\> \<number_of_requests\> (\<request\>)* ... \
`Answer format`: \<ARC\> | \<Belady\>

## To run arc-cache
```bash
cmake -S . -B build && cmake --build build
./build/arc-cache
```

## To run belady-cache
```bash
cmake -S . -B build && cmake --build build --target belady-cache
./build/belady-cache
```

## To run tests (tbd move to ctest)
`python testcomp.py`

## Ideal to ARC caching comparisson
test case | ARC cache hits | Ideal cache hits |
|-------- | -------------- | ---------------- |
| 1       |       4        |       9          |
| 2       |       5        |       5          |
| 3       |       15       |       16         |
| 4       |       8        |       16         |
| 5       |       9        |       18         |
| 6       |       16       |       25         |
| 7       |       9        |       12         |
| 8       |       4        |       8          |
| 9       |       10       |       15         |
| 10      |       6        |       8          |

## additions
- add ci (github actions)