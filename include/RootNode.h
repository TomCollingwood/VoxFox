#ifndef ROOTNODE_H
#define ROOTNODE_H

#include <iostream>
#include <vector>
#include "lib/glm/glm/glm.hpp"

#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <ngl/BBox.h>
#include <ngl/Obj.h>

#include "include/LeafNode.h"
#include "include/PrimaryNode.h"

class RootNode
{
public:
  RootNode();
  int getSize();
  void calculatePolys(ngl::Mat4 MV);
  void addVoxel(glm::vec3 _position);
  bool isVoxel(glm::vec3 _position);
  void printVertexes();
  std::vector<float> * getVertexes();
  std::vector<float> * getNormals();
  void loadVBO(GLuint shaderID, GLuint vbo, GLuint nbo);
  float getFloat(int i);
  int getVertexSize();
  void createSphere(glm::vec3 _position, int _radius);
  void createTorus(glm::vec3 _position, glm::vec2 _t);
  void importObj(ngl::Obj * _mesh);
  void drawBox(ngl::Vec3 _min, ngl::Vec3 _max);
  bool full;
private:
  const float unitChildLength = 10.0;
  const int depth = 3;
  float unitVoxelLength = 0.01953125;
  std::vector<LeafNode *> m_leafTable;
  std::vector<PrimaryNode *> m_primChildren;
  LeafNode * m_leafAccessor;
  SecondaryNode * m_secAccessor;
  PrimaryNode * m_primAccessor;
  float numberOfVoxels;
  std::vector<float> * m_vertexes;
  std::vector<float> * m_normals;
};

#endif // ROOTNODE_H
