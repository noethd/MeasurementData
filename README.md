# MeasurementData
Implementation of the C-code test of calculating the sum and average of a matrix in constant time.

Simply run the main function, there will be created a MD object, calculated the sum and avg, took a deep copy and free all the memory at last.
At the end there is also provided a function which handles some edge cases and shows the robustness of the implementation.

Create a Buffer object with
```
const unsigned char buffer[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 1, 2, 3, 4};

MD *pBuffer = MD_create(buffer, 4, 4);
```

get statistics 
```
unsigned int sum = MD_getSum(pBuffer, 1,1,3,3);
double avg = MD_getAverage(pBuffer, 1,1,3,3);
printf("Sum: %d Avg: %f\n", sum, avg);
```

create deep copy of original buffer
```
MD *pCopy = MD_copy(pBuffer);
```
delete both buffers
```
MD_delete(pBuffer);
MD_delete(pCopy);
```


The code in the main will produce following output:
```
0 1 2 3 
4 5 6 7 
8 9 10 0 
1 2 3 4 
Sum: 46 Avg: 5.111111

Running some examples of edge cases and more with this matrix:
1 1 1 1 
1 1 1 1 
1 1 1 1 
1 1 1 1 
Sum and average of complete J matrix should be 16 and 1:
Sum: 16 Avg: 1.000000
Sum and average of (0,0) (0,0) submatrix should be both 1:
Sum: 1 Avg: 1.000000
Sum and average of (2,2) (4,4) submatrix should be 4 and 0.4444 
Sum: 4 Avg: 0.444444
Sum and average of (2,3) (1,1) inverted rectangle should be 6 and 1 
Sum: 6 Avg: 1.000000
Sum and average of (3,1) (1,2) inverted rectangle should be 6 and 1 
Sum: 6 Avg: 1.000000
Sum and average of (4,3) (4,4) 'out of bounds' should be both 0 
Sum: 0 Avg: 0.000000
Sum and average of (4,2) (4,3) 'out of bounds' should be both 0 
Sum: 0 Avg: 0.000000

Process finished with exit code 0

```

## Possible improvements
- Change x0,y0,x1,y1 to a coordinate or rectangle Type
- better null checking or creating matrices in seperate functions
