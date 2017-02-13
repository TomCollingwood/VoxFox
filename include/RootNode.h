#ifndef ROOTNODE_H
#define ROOTNODE_H

#include <iostream>
#include <vector>

#include "include/LeafNode.h"
#include "include/PrimaryNode.h"
#include "include/SecondaryNode.h"


class RootNode
{
public:
  RootNode();
  
private:
  std::vector<LeafNode> m_leafTable;
  std::vector<PrimaryNode> m_primChildren;
  std::vector<LeafNode *> m_accessors;
};

#endif // ROOTNODE_H
