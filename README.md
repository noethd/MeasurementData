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
