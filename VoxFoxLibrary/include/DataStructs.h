#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

struct Voxel{
  float nx;
  float ny;
  float nz;
  float u;
  float v;
  float r;
  float g;
  float b;
  int index;
} Voxel_Default = {0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0};

struct PrimAccessor{
  int primIndex;
  bool initialized;
};

struct SecAccessor{
  int primIndex;
  int secIndex;
  bool initialized;
};

struct LeafAccessor{
  int primIndex;
  int secIndex;
  int leafIndex;
  bool initialized;
};

#endif // DATASTRUCTS_H
