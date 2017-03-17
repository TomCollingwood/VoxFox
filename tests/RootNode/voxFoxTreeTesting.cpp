#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include <VoxFoxTree.h>

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(VoxFoxRootNode,addVoxel)
{
  VoxFoxTree test;
  test.addVoxel(glm::vec3(1.0,2.0,3.0),Voxel());
  PrimaryNode * _testPrim = test.getChildren()->at(0);

  EXPECT_TRUE(_testPrim->getOrigin()[0]<= 1.0 && _testPrim->getOrigin()[0]+test.m_primUnit>1.0 &&
      _testPrim->getOrigin()[1]<= 2.0 && _testPrim->getOrigin()[1]+test.m_primUnit>2.0 &&
      _testPrim->getOrigin()[2]<= 3.0 && _testPrim->getOrigin()[2]+test.m_primUnit>3.0);
}

TEST(VoxFoxRootNode,addVoxel2)
{
  VoxFoxTree test;
  test.addVoxel(glm::vec3(10.0,0.0,0.0),Voxel());
  test.addVoxel(glm::vec3(20.0,0.0,0.0),Voxel());
  test.addVoxel(glm::vec3(30.0,0.0,0.0),Voxel());

  EXPECT_TRUE(test.getChildren()->size()==3);
}

TEST(VoxFoxRootNode,isVoxel)
{
  VoxFoxTree test;
  test.addVoxel(glm::vec3(1.0,2.0,3.0),Voxel());
  EXPECT_TRUE(test.isVoxel(glm::vec3(1.0,2.0,3.0)));
}

TEST(VoxFoxRootNode,isVoxel2)
{
  VoxFoxTree test;
  EXPECT_FALSE(test.isVoxel(glm::vec3(0.0,2.0,1.0)));
}

TEST(VoxFoxRootNode,isPrimary)
{
  VoxFoxTree test;
  test.addVoxel(glm::vec3(1.0,2.0,3.0),Voxel());
  EXPECT_TRUE(test.isPrimary(glm::vec3(1.0,2.0,3.0)));
}

TEST(VoxFoxRootNode,isSecondary)
{
  VoxFoxTree test;
  test.addVoxel(glm::vec3(1.0,2.0,3.0),Voxel());
  EXPECT_TRUE(test.isSecondary(glm::vec3(1.0,2.0,3.0)));
}

TEST(VoxFoxRootNode,isLeaf)
{
  VoxFoxTree test;
  test.addVoxel(glm::vec3(1.0,2.0,3.0),Voxel());
  EXPECT_TRUE(test.isLeaf(glm::vec3(1.0,2.0,3.0)));
}

TEST(VoxFoxRootNode,calculatePolys)
{
  VoxFoxTree test;
  test.addVoxel(glm::vec3(1.0,2.0,3.0),Voxel());
  test.calculatePolys();
  std::vector<float> testVerts = test.getVertexes();
  EXPECT_TRUE(testVerts.size()==108);
}

TEST(VoxFoxRootNode,plusOperator)
{
  ngl::Obj * m_meshdwarf = new ngl::Obj("../../models/dwarf.obj");
  ngl::Obj * m_meshdeer = new ngl::Obj("../../models/deer.obj");

  ngl::Image * mytexturedwarf = new ngl::Image("../../images/dwarf.jpg");
  ngl::Image * mytexturedeer = new ngl::Image("../../images/deer.jpg");

  VoxFoxTree myDeer, myDwarf, deerDwarf;

  myDeer.importObjRGB(m_meshdeer,mytexturedeer,0.5f);
  myDwarf.importObjRGB(m_meshdwarf,mytexturedwarf,0.5f);
  myDwarf.translate(glm::vec3(2.0f,0.0f,0.0f));

  myDwarf.calculatePolys();
  myDeer.calculatePolys();

  int sizeToCheck = myDwarf.getVertexSize() + myDwarf.getVertexSize();

  deerDwarf = VoxFoxTree(myDwarf+myDeer);

  EXPECT_TRUE(deerDwarf.getVertexSize()==sizeToCheck);
}
