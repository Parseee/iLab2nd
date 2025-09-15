The program implements ARC caching. The implementation supposes we have twice as much cache space as we input. This was chosen because of divisibility problems. It's easier to assume that we have double capacity.

# To run app
`cmake -B build && cmake --build build`

# To run tests
`python testcomp.py`

# Ideal to ARC caching comparisson

test case | ARC cache hits | Ideal cache hits |
|-------- | -------------- | ---------------- |
| 1       |       4        |        7         |
| 2       |       5        |        5         |
| 3       |       15       |       16         |
| 4       |       1        |        1         |
| 5       |       11       |       11         |
| 6       |       7        |       12         |
| 7       |       13       |       14         |
| 8       |       8        |        8         |
| 9       |       1        |        1         |
| 10      |       4        |        4         |