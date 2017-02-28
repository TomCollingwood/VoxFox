#ifndef ROOTNODE_H
#define ROOTNODE_H

#include <iostream>
#include <vector>
#include "lib/glm/glm/glm.hpp"

#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <ngl/BBox.h>
#include <ngl/Obj.h>
#include <ngl/ShaderLib.h>

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
  //void loadVBO(ngl::ShaderLib* shader, constexpr auto shaderProgram, GLuint vbo, GLuint nbo, float * normals);
  float getFloat(int i);
  int getVertexSize();
  void createSphere(glm::vec3 _position, int _radius);
  void createTorus(glm::vec3 _position, glm::vec2 _t);
  void importObj(ngl::Obj * _mesh);
  void importAccurateObj(ngl::Obj * _mesh);
  void drawBox(ngl::Vec3 _min, ngl::Vec3 _max);

  void fill();
  bool full;
  bool empty = true;
private:
  glm::vec3 min, max;
  const float unitChildLength = 10.0;
  const int depth = 3;
  float unitVoxelLength = 0.01953125;
  std::vector<LeafNode *> m_leafTable;
  std::vector<PrimaryNode *> m_primChildren;
  LeafNode * m_leafAccessor =nullptr;
  SecondaryNode * m_secAccessor =nullptr;
  PrimaryNode * m_primAccessor =nullptr;
  int numberOfVoxels=0;
  std::vector<float> * m_vertexes;
  std::vector<float> * m_normals;
};

#endif // ROOTNODE_H
