---
First task: calculate matrix determinant via the Gauss method  
=
<br>
Nothing beyond the task, read the matrix, check that it is square, if it is, then bring it to a step form, remembering how much we multiplied the original determinant. At the end, return the value and display it on the screen
Examples: <br>

```
In:
2 2
3 5
1 4

Out:
Calculated determinant: 7.00
```

```
In:
2 3
1 2 3
4 5 6

Out:
Sorry, invalid matrix sizes :(
```
```
In:
4 4
1 0 2 0
3 1 0 1
0 2 1 0
1 0 3 1

Out:
Calculated determinant: 15.00
```
---
Second task: count tankers on image
=
<br>
Total count: 238

I left the Gaussian blur function in the file, but it is not used in the end (the image with an example of its operation before applying contrast is located in the task folder), as the tankers are "eaten up" after applying other operations

To solve the problem, the image is converted to black and white and its contrast is increased. After that, we go through all the pixels, running BFS from the current one if it is not black and has not yet been processed

The program has a lot of false positives on the shores, so in the BFS function, when a point enters these "dead zones," it causes an early exit from processing that point

If the program considers an area to be a tanker, it will mark it on the image (due to the offset boundaries, the tanker label appears as crosses)

Blur:
![Gaussian blur](second-task-segmentation/picture_after_gauss_blur.png)
After casting to bw:
![After casting to bw](second-task-segmentation/picture_after_casting_to_white_and_black.png)
After contrast:
![After contrast](second-task-segmentation/picture_after_applying_contrast.png)
Before "dead zones" early processing:
![Before "dead zones" early processing](second-task-segmentation/marked_before_finding_dead_zones_and_some_tuning.png)
Final result:
![Final result](second-task-segmentation/input_picture_after_marking_tankers.png)

