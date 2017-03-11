#ifndef VOXELDATA_H
#define VOXELDATA_H
#include <vector>
#include <cstring>

class Voxel
{
public:
  Voxel()
  {
    r=1.0f;
    g=0.0f;
    b=0.0f;
    u=0;
    v=0;
    nx=0;
    ny=0;
    nz=0;
    index=0;
  }
  float nx;
  float ny;
  float nz;
  float u;
  float v;
  float r;
  float g;
  float b;
  int index;
};

#endif // VOXELDATA_H
