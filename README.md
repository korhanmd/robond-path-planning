# RoboND Path Planning Search Algorithms
This is the C++ implementation of path planning lectures of Udacity's Robotics Software Engineer Nanodegree

There are four C++ files. `minkowski.cpp` file includes implementation of Minkowski Sum. `bfs.cpp` file includes implementation of BFS algorithm on a grid map.
`a_star.cpp` file includes implementation of A* algorithm on the same grid map.
`a_star_rwm.cpp` file includes modified version of `a_star.cpp` to make it work on a real-world map and visualization of the path.
It uses matplotlib library of Python to visualize map.

### Compiling
To compile A* on real-world maps write below command to terminal:

```sh
$ g++ a_star_rwm.cpp -std=c++11 -I/usr/include/python2.7 -lpython2.7
```

To compile other files write below command to terminal:

```sh
$ g++ file_name.cpp
```

### Visualization of Path

![alt text](Images/Path.png)
