/// \file VoxFoxTree.h
/// \brief VoxFoxTree.h  Essentially the rootnode of the tree with all tree functions.
/// \author Thomas Collingwood
/// \version 1.0
/// \date 13/3/17 Updated to NCCA Coding standard
/// Revision History : See https://github.com/TomCollingwood/VoxFox



#ifndef VoxFoxTree_H
#define VoxFoxTree_H

#include <iostream>
#include <vector>
#include <queue>
#include <glm/glm.hpp>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <ngl/BBox.h>
#include <ngl/Obj.h>
#include <ngl/ShaderLib.h>
#include <ngl/BBox.h>
#include <cmath>

#include "LeafNode.h"
#include "PrimaryNode.h"

class VoxFoxTree
{
public:
  VoxFoxTree();
  ~VoxFoxTree();
  VoxFoxTree(const VoxFoxTree & _r);
  VoxFoxTree & operator=(const VoxFoxTree & _r);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief operator |   Union operator that takes two VoxFoxTrees and gives VoxFoxTree of union of both of them
  /// \param[in] _r       RHS of operator
  /// \return             Union of RHS and LHS VoxFoxTree
  //----------------------------------------------------------------------------------------------------------------------
  VoxFoxTree operator|(VoxFoxTree const &_r);
  void operator|=(VoxFoxTree const &_r);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief operator - Subtract set operator
  /// \param[in] _r     Tree to subtract
  /// \return           Subtracted tree
  //----------------------------------------------------------------------------------------------------------------------
  VoxFoxTree operator-(VoxFoxTree const &_r) const;
  void operator-=(VoxFoxTree const &_r);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief operator + Intersection set operator (retains LHS data)
  /// \param[in] _r     The other tree to intersect with.
  /// \return           Intersected tree that retains the LHS voxel data
  //----------------------------------------------------------------------------------------------------------------------
  VoxFoxTree operator+(VoxFoxTree const &_r) const;
  void operator+=(VoxFoxTree const &_r);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief translate          Translates all voxels in the VoxFoxTree by the worldspace input
  /// \param[in] _translation   amount to translate the VoxFoxTree
  //----------------------------------------------------------------------------------------------------------------------
  void translate(glm::vec3 _translation);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief addVoxel       adds Voxel at specified _position to VoxFoxTree
  /// \param[in] _position  position in worldspace of the voxel
  /// \param[in] _data      the information of the voxel to be added (see DataStructs.h)
  //----------------------------------------------------------------------------------------------------------------------
  void addVoxel(glm::vec3 _position, Voxel _data);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief isVoxel        checks if a Voxel is present within the VoxFoxTree at _position
  /// \param[in] _position  position in worldspace of the voxel
  /// \return               if Voxel is present at _position
  //----------------------------------------------------------------------------------------------------------------------
  bool isVoxel(glm::vec3 _position);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief calculatePolys This function calculates m_vertexes, m_normals and m_texturecoords for OpenGL use from VoxelData.
  //----------------------------------------------------------------------------------------------------------------------
  void calculatePolys();

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief getVertexSize  returns how many vertexes there are in m_vertexes
  /// \return               m_vertexes.size()
  //----------------------------------------------------------------------------------------------------------------------
  int getVertexSize();

  int updateVoxCount();

  // ,ø¤°º¤ø,¸¸,ø¤º°`°º¤ø,¸,ø¤°º¤ø,¸ACCESSORS,ø¤°º¤ø,¸¸,ø¤º°`°º¤ø,¸¸,ø¤º°`°º¤ø,¸
  std::vector<float> getVertexes();
  std::vector<float> getNormals();
  std::vector<float> getTextureCoords();
  std::vector<float> getColors();
  std::vector<PrimaryNode *> * getChildren() {return &m_primChildren;}

  // ,ø¤°º¤ø,¸¸,ø¤º°`°º¤ø,¸,ø¤°º¤ø,¸SHAPES,ø¤°º¤ø,¸¸,ø¤º°`°º¤ø,¸¸,ø¤º°`°º¤ø,¸
  //----------------------------------------------------------------------------------------------------------------------
  /// \brief createSphere   Creates a sphere in the tree
  /// \param[in] _position  Centre of sphere
  /// \param[in] _radius    Radius of sphere (worldspace)
  /// \param[in] _color     RGB color of sphere (0.0f-1.0f)
  //----------------------------------------------------------------------------------------------------------------------
  void createSphere(glm::vec3 _position, float _radius, glm::vec3 _color);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief createTorus    Creates a torus / ring of variable radii
  /// \param[in] _position  Centre point of the torus to create
  /// \param[in] _t         The two radius (inner and outter) of the torus
  //----------------------------------------------------------------------------------------------------------------------
  void createTorus(glm::vec3 _position, glm::vec2 _t);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief createCylinder Creates a cylinder in x y or z direction of variable height
  /// \param[in] _position  Centre of cylinder
  /// \param[in] _axis      Direction to create cylinder
  /// \param[in] _radius    Radius of cylinder in worldspace
  /// \param[in] _height    Height of cylinder
  /// \param[in] _color     Color of cylinder (0.0f-1.0f)
  //----------------------------------------------------------------------------------------------------------------------
  void createCylinder(glm::vec3 _position, glm::vec3 _axis, float _radius, float _height, glm::vec3 _color);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief createBox  Creates a box
  /// \param[in] _min   Minimum vertex of box
  /// \param[in] _max   Maximum vertex of box
  /// \param[in] _color Colour of box
  //----------------------------------------------------------------------------------------------------------------------
  void createBox(glm::vec3 _min, glm::vec3 _max, glm::vec3 _color);

  // ,ø¤°º¤ø,¸¸,ø¤º°`°º¤ø,¸,ø¤°º¤ø,¸OBJ and TEXTURE IMPORT,ø¤°º¤ø,¸¸,ø¤º°`°º¤ø,¸¸,ø¤º°`°º¤ø,¸
  //----------------------------------------------------------------------------------------------------------------------
  /// \brief importQuickObj Quickly imports an polygonal .obj file to the VoxFoxTree as voxel data
  ///                       It is done by simply drawing in the bounding boxes of the polygons
  /// \param[in] _mesh      The ngl::Obj object of the mesh
  /// \param[in] _size      Height of the object in the worldspace
  //----------------------------------------------------------------------------------------------------------------------
  void importQuickObj(ngl::Obj * _mesh, float _size);


  //----------------------------------------------------------------------------------------------------------------------
  /// \brief importObj          Imports a polygonal .obj file to the VoxFoxTree as voxel data.
  /// \param[in] _mesh          The ngl::Obj object of the mesh
  /// \param[in] _color         The color of the object
  /// \param[in] _normals       This tells us we want normals used in shader and calculated
  /// \param[in] _interpnormals This tells us to interpolate the normals in fragments between vertexes
  //----------------------------------------------------------------------------------------------------------------------
  void importObj(ngl::Obj * _mesh, glm::vec3 _color,float _size, bool _normals =true, bool _interpnormals =true);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief importObj          Imports a polygonal .obj file to the VoxFoxTree as voxel data.
  /// \param[in] _mesh          The ngl::Obj object of the mesh
  /// \param[in] _texture       The texture of the object
  /// \param[in] _normals       This tells us we want normals used in shader and calculated
  /// \param[in] _interpnormals This tells us to interpolate the normals in fragments between vertexes
  //----------------------------------------------------------------------------------------------------------------------
  void importTexturedObj(ngl::Obj * _mesh, ngl::Image * _texture,float _size, bool _normals =true, bool _interpnormals=true);


  //----------------------------------------------------------------------------------------------------------------------
  /// \brief drawFlatImage  Draws a flat 1 voxel thick square with a textured image on
  /// \param _position      Centre of image plane
  /// \param _texture       texture to use
  /// \param _height        The height of the square
  //----------------------------------------------------------------------------------------------------------------------
  void drawFlatImage(glm::vec3 _position, ngl::Image *_texture, float _height);

  // This increases after every set operation.
  int numberOfObjects=0;


  bool isLeaf(glm::vec3 _position);
  bool isSecondary(glm::vec3 _position);
  bool isPrimary(glm::vec3 _position);

  PrimaryNode * getPrimAccessor() {return m_primAccessor;}
  SecondaryNode * getSecAccessor() {return m_secAccessor;}
  LeafNode * getLeafAccessor() {return m_leafAccessor;}

  float m_primUnit = 10.0;
  float m_secUnit = 1.25;
  float m_leafUnit = 0.15625;
  float m_voxUnit = 0.01953125;

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief m_primChildren The vector of PrimaryNode children.
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<PrimaryNode *> m_primChildren;

private:
  glm::vec3 min, max;
  // The accessors speed up importing larger obj by up to four times
  //----------------------------------------------------------------------------------------------------------------------
  /// \brief m_leafAccessor The last accessed LeafNode. Is updated in addVoxel and isVoxel
  //----------------------------------------------------------------------------------------------------------------------
  LeafNode * m_leafAccessor =nullptr;
  //----------------------------------------------------------------------------------------------------------------------
  /// \brief m_secAccessor  The last accessed SecondaryNode. Is updated in addVoxel and isVoxel
  //----------------------------------------------------------------------------------------------------------------------
  SecondaryNode * m_secAccessor =nullptr;
  //----------------------------------------------------------------------------------------------------------------------
  /// \brief m_primAccessor The last accessed PrimaryNode. Is updated in addVoxel and isVoxel
  //----------------------------------------------------------------------------------------------------------------------
  PrimaryNode * m_primAccessor =nullptr;

  int numberOfVoxels=0;


  //----------------------------------------------------------------------------------------------------------------------
  /// \brief m_vertexes Vector of floats that are the x y z coordinates of the vertexes. Calculated in calculatePolygons()
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<float> m_vertexes;
  //----------------------------------------------------------------------------------------------------------------------
  /// \brief m_normals  Vector of floats that are x y z normals of vertexes. Made in calculatePolygons from Voxel data
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<float> m_normals;
  //----------------------------------------------------------------------------------------------------------------------
  /// \brief m_textureCoords  Vector of floats that are u v coordinates of vertexes. Calculated in calculatePolygons()
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<float> m_textureCoords;
  //----------------------------------------------------------------------------------------------------------------------
  /// \brief m_colors Vector of floats that are r g b colour values of vertexes. Made in calculatePolygons from voxel data
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<float> m_colors;
};

#endif // VoxFoxTree_H
