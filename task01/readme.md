The program implements ARC caching. the answer has format \<ARC\> | \<Belady\>

# To run app
`cmake -B build && cmake --build build`

# To run tests
`python testcomp.py`

# Ideal to ARC caching comparisson

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