/// \file LeafNode.h
/// \brief LeafNode   Class for the lowest level node. Contains the voxel data.
/// \author Thomas Collingwood
/// \version 1.0
/// \date 13/3/17 Updated to NCCA Coding standard
/// Revision History : See https://github.com/TomCollingwood/VoxFox

#ifndef LEAFNODE_H
#define LEAFNODE_H

#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Text.h>

#include "glm/glm.hpp"
#include "Voxel.h"

class SecondaryNode;
class VoxFoxTree;

class LeafNode
{
public:
  LeafNode();
  LeafNode(glm::vec3 _origin);
  LeafNode(LeafNode const &_l);
  LeafNode & operator=(LeafNode const &_l) ;

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief operator + Intersection set operator (retains LHS data)
  /// \param[in] _l     The other node to intersect with.
  /// \return           Intersected node that retains the LHS voxel data
  //----------------------------------------------------------------------------------------------------------------------
  LeafNode operator+(LeafNode const & _l) const;

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief operator | Union operator set operator
  /// \param[in] _l     The other node to union with
  /// \return           The unioned node
  //----------------------------------------------------------------------------------------------------------------------
  LeafNode operator|(LeafNode const &_l) const;

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief operator - Subtract set operator
  /// \param[in] _l     Node to subtract
  /// \return           Subtracted node
  //----------------------------------------------------------------------------------------------------------------------
  LeafNode operator-(LeafNode const &_l) const;

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief addVoxel       adds Voxel at specified _position to LeafNode
  /// \param[in] _position  position in worldspace of the voxel
  /// \param[in] _data      the information of the voxel to be added (see Voxel.h)
  /// \return               whether the voxel was successfully inserted or not
  //----------------------------------------------------------------------------------------------------------------------
  bool addVoxel(glm::vec3 _position, Voxel _voxel);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief isVoxel                checks if a Voxel is present within LeafNode at _position
  /// \param[in] _position          position in worldspace of the voxel
  /// \return                       if Voxel is present at _position
  //----------------------------------------------------------------------------------------------------------------------
  bool isVoxel(glm::vec3 _position) const;

  glm::vec3 getOrigin() const {return m_origin; }

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief m_VoxelMap Each bit represents the presence of a voxel within the leafnode. 8*8*8 bits = 8*8 chars
  //----------------------------------------------------------------------------------------------------------------------
  std::array<char,64> m_VoxelMap;

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief m_VoxelData  Voxel data for only the present voxels. Is variable.
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<Voxel> m_VoxelData;

  // Unique ID for the LeafNode within tree. Trees can have LeafNodes with same id.
  // Used for quick compare in set operators
  int idx, idy, idz;
private:
  glm::vec3 m_origin;
  float unitVoxelLength = 0.01953125;
  float unitLeafLength = 0.15625;
  float m_primUnit = 10.0;
  float m_secUnit = 1.25;
  float m_leafUnit = 0.15625;
  float m_voxUnit = 0.01953125;
};

#endif // LEAFNODE_H
