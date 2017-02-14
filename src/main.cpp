#include <iostream>
#include "include/RootNode.h"

using namespace std;

int main(int argc, char *argv[])
{
  cout << "Hello World!" << endl;
  RootNode * myRootNode = new RootNode();
  myRootNode->addVoxel(glm::vec3(5,5,5));
  myRootNode->addVoxel(glm::vec3(5.5,5,5));
  myRootNode->addVoxel(glm::vec3(15,5,5));
  myRootNode->draw();
  myRootNode->printVertexes();
  cout << myRootNode->getSize() << endl;
  return 0;
}
