/// \file PrimaryNode.h
/// \brief PrimaryNode.h  The highest node level that RootNode has.
///                       PrimaryNode contains the SecondaryNodes (up to 8^3)
/// \author Thomas Collingwood
/// \version 1.0
/// \date 13/3/17 Updated to NCCA Coding standard
/// Revision History : See https://github.com/TomCollingwood/VoxFox

#ifndef PRIMARYNODE_H
#define PRIMARYNODE_H

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "LeafNode.h"
#include "SecondaryNode.h"
class RootNode;

class PrimaryNode
{
public:
  PrimaryNode();
  ~PrimaryNode();
  PrimaryNode(glm::vec3 _origin);
  PrimaryNode(PrimaryNode const &p);

  PrimaryNode operator +(PrimaryNode const &_p) const;

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief addVoxel             tries to add voxel to the PrimaryNode
  /// \param[in] _position        position in worldspace of voxel
  /// \param[in] _voxel           the information of the voxel to be added
  /// \param[in] _secAccessor     the secondary accessor to change (last accessed SecondaryNode)
  /// \param[out] o_leafAccessor  the leaf accessor to change (last accessed LeafNode)
  //----------------------------------------------------------------------------------------------------------------------
  void addVoxel(glm::vec3 _position, Voxel _voxel, SecondaryNode ** o_secAccessor, LeafNode ** _leafAccessor);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief isVoxel              checks if a Voxel is present within PrimaryNode at _position
  /// \param[in] _position        position in worldspace of the voxel
  /// \param[out] o_secAccessor   the secondary accessor to change (last accessed SecondaryNode)
  /// \param[out] o_leafAccessor  the leaf accessor to change (last accessed LeafNode)
  /// \return                     if Voxel is present at _position
  //----------------------------------------------------------------------------------------------------------------------
  bool isVoxel(glm::vec3 _position, SecondaryNode ** _secAccessor, LeafNode ** _leafAccessor);

  glm::vec3 getOrigin() const {  return m_origin; }
  bool isLeaf(glm::vec3 _position, SecondaryNode ** _sec, LeafNode ** _leaf);
  bool isSecondary(glm::vec3 _position, SecondaryNode ** _secondary);
  void shiftOrigin(int _x, int _y, int _z);


  std::vector<SecondaryNode *> m_secChildren;
  int idx, idy, idz;
private:
  glm::vec3 m_origin;
  float unitChildLength = 1.25;
  const float m_primUnit = 10.0;
  const float m_secUnit = 1.25;
  const float m_leafUnit = 0.15625;
  const float m_voxUnit = 0.01953125;
};

#endif // PRIMARYNODE_H
