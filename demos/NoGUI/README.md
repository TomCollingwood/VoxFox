# VoxFox
Voxel Library for Modern OpenGL

Initial design based on SimpleNGL by Jon Macey. The two shader files are written by Richard Southern.

See my initial ideas here:
[https://drive.google.com/file/d/0B3Bu_KdSBGhibENXZksyVjFCeUk/view?usp=sharing](https://drive.google.com/file/d/0B3Bu_KdSBGhibENXZksyVjFCeUk/view?usp=sharing)

TODO:

1. Fill the object
  a. Make isLeaf and isSec and isPrim
  b. Make union between RootNodes
  b. Strip mine the MainRootNode putting fill voxels in FillRootNode using the 'is' functions above
  c. Then check each strip for neigbours in MainRootNode and FillRootNode
  d. If voxel is not fully enclosed get rid of that strip and adjascent strips and kill those adjascent strips.
  e. Otherwise leave the voxel
  f. Union the two RootNodes.

2. Accurate Polygon to Voxel (my own method woop woop)
  a. Pick an edge and find a step size below voxel unit that fits perfectly in the edge.
  b. For each step create a vector to the opposite vertex.
  c. For first vector make voxels up to vertex.
  d. So every other edge goes half way to centre.
  e. Then every fourth edge from 3rd
