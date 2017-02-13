#ifndef SECONDARYNODE_H
#define SECONDARYNODE_H

#include <iostream>
#include <vector>

#include "include/LeafNode.h"
#include "include/RootNode.h"
#include "include/PrimaryNode.h"

class SecondaryNode
{
public:
  SecondaryNode();
private:
  PrimaryNode * m_parent;
  RootNode * m_grandparent;
  std::vector<LeafNode *> m_leafChildren;
};

#endif // SECONDARYNODE_H