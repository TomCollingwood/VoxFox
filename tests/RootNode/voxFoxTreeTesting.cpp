#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include <VoxFoxTree.h>
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/NGLStream.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <Examples.h>

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

//----------------------------------ADD VOXEL----------------------------------------
//--------TREE-------------------
TEST(VoxFoxRootNode,treeAddVoxel)
{
  VoxFoxTree test;
  test.addVoxel(glm::vec3(1.0,2.0,3.0),Voxel());
  PrimaryNode * _testPrim = test.getChildren()->at(0);

  EXPECT_TRUE(_testPrim->getOrigin()[0]<= 1.0 && _testPrim->getOrigin()[0]+test.m_primUnit>1.0 &&
      _testPrim->getOrigin()[1]<= 2.0 && _testPrim->getOrigin()[1]+test.m_primUnit>2.0 &&
      _testPrim->getOrigin()[2]<= 3.0 && _testPrim->getOrigin()[2]+test.m_primUnit>3.0);
}

TEST(VoxFoxRootNode,treeAddVoxel2)
{
  VoxFoxTree test;
  test.addVoxel(glm::vec3(10.0,0.0,0.0),Voxel());
  test.addVoxel(glm::vec3(20.0,0.0,0.0),Voxel());
  test.addVoxel(glm::vec3(30.0,0.0,0.0),Voxel());

  EXPECT_TRUE(test.getChildren()->size()==3 && test.getPrimAccessor()!=nullptr &&
      test.getSecAccessor()!=nullptr && test.getLeafAccessor()!=nullptr);
}

//--------PRIMARY-------------------
TEST(VoxFoxRootNode,primAddVoxel)
{
  PrimaryNode test = PrimaryNode(glm::vec3(1.0f,2.0f,3.0f));
  SecondaryNode * accSec = new SecondaryNode();
  LeafNode * accLeaf = new LeafNode();
  test.addVoxel(glm::vec3(1.0,2.0,3.0),Voxel(),&accSec,&accLeaf);
  SecondaryNode * _testSec = test.m_secChildren.at(0);

  EXPECT_TRUE(_testSec->getOrigin()[0]<= 1.0 && _testSec->getOrigin()[0]+test.m_secUnit>1.0 &&
      _testSec->getOrigin()[1]<= 2.0 && _testSec->getOrigin()[1]+test.m_secUnit>2.0 &&
      _testSec->getOrigin()[2]<= 3.0 && _testSec->getOrigin()[2]+test.m_secUnit>3.0);
}

TEST(VoxFoxRootNode,primAddVoxel2)
{
  PrimaryNode test = PrimaryNode(glm::vec3(10.0f,0.0f,0.0f));
  SecondaryNode * accSec = new SecondaryNode();
  LeafNode * accLeaf = new LeafNode();

  test.addVoxel(glm::vec3(10.0,0.0,0.0),Voxel(),&accSec,&accLeaf);
  test.addVoxel(glm::vec3(10.0+test.m_secUnit,0.0,0.0),Voxel(),&accSec,&accLeaf);
  test.addVoxel(glm::vec3(10.0+test.m_secUnit*2,0.0,0.0),Voxel(),&accSec,&accLeaf);

  EXPECT_TRUE(test.m_secChildren.size()==3 && accSec!=nullptr && accLeaf!=nullptr);
}

//--------SECONDARY-------------------
TEST(VoxFoxRootNode,secAddVoxel)
{
  SecondaryNode test = SecondaryNode(glm::vec3(1.0f,2.0f,3.0f));
  LeafNode * accLeaf = new LeafNode();
  test.addVoxel(glm::vec3(1.0,2.0,3.0),Voxel(),&accLeaf);
  LeafNode * _testLeaf = test.m_leafChildren.at(0);

  EXPECT_TRUE(_testLeaf->getOrigin()[0]<= 1.0 && _testLeaf->getOrigin()[0]+test.m_leafUnit>1.0 &&
      _testLeaf->getOrigin()[1]<= 2.0 && _testLeaf->getOrigin()[1]+test.m_leafUnit>2.0 &&
      _testLeaf->getOrigin()[2]<= 3.0 && _testLeaf->getOrigin()[2]+test.m_leafUnit>3.0);
}

TEST(VoxFoxRootNode,secAddVoxel2)
{
  SecondaryNode test = SecondaryNode(glm::vec3(10.f,0.0f,0.0f));
  LeafNode * accLeaf = new LeafNode();

  test.addVoxel(glm::vec3(10.0,0.0,0.0),Voxel(),&accLeaf);
  test.addVoxel(glm::vec3(10.0+test.m_leafUnit,0.0,0.0),Voxel(),&accLeaf);
  test.addVoxel(glm::vec3(10.0+test.m_leafUnit*2,0.0,0.0),Voxel(),&accLeaf);

  EXPECT_TRUE(test.m_leafChildren.size()==3 && accLeaf!=nullptr);
}

//-----------LEAF----------------
TEST(VoxFoxRootNode,leafAddVoxel)
{
  LeafNode test = LeafNode(glm::vec3(0.0f,2.0f,3.0f));
  Voxel ourVox = Voxel(-1.0f,2.0f,-3.0f,4.0,-5.0f); // nx ny nz u v
  ourVox.r = 2.0f;
  ourVox.g = -1.0f;
  ourVox.b = 0.001f;
  test.addVoxel(glm::vec3(test.m_voxUnit,2.0,3.0),ourVox);

  // Never do a == to floats!!
  EXPECT_TRUE((test.m_VoxelData.size()==1)&&
              (test.m_VoxelMap[8] && 1) &&
              test.m_VoxelData[0].r>1.0f &&
              test.m_VoxelData[0].g<0.0f &&
              test.m_VoxelData[0].b>0.0f &&
              test.m_VoxelData[0].nx<0.0f &&
              test.m_VoxelData[0].ny>1.0f &&
              test.m_VoxelData[0].nz<-2.5f &&
              test.m_VoxelData[0].u>3.0f &&
              test.m_VoxelData[0].v<-4.0f
      );
}

TEST(VoxFoxRootNode,leafAddVoxel2)
{
  LeafNode test = LeafNode(glm::vec3(0.0f,0.0f,0.0f));

  test.addVoxel(glm::vec3(test.m_voxUnit,0.0,0.0),Voxel());
  test.addVoxel(glm::vec3(test.m_voxUnit,test.m_voxUnit,0.0),Voxel());
  test.addVoxel(glm::vec3(test.m_voxUnit,test.m_voxUnit,test.m_voxUnit*7),Voxel());

  EXPECT_TRUE((test.m_VoxelData.size()==3)&&
              (test.m_VoxelMap[8] && 1)&&
              (test.m_VoxelMap[8+1] && 1)&&
             (test.m_VoxelMap[8+1] && (7<<1))
      );
}


//----------------------------------IS VOXEL / IS NODE -------------------------------

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

//----------------------------------SHAPES AND POLYS-------------------------------

TEST(VoxFoxRootNode,createBox)
{
  VoxFoxTree cube;
  cube.createBox(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(cube.m_voxUnit*10,cube.m_voxUnit*5,cube.m_voxUnit*15),glm::vec3(1.0f,0.0f,0.0f));
  EXPECT_TRUE(cube.updateVoxCount()==10*5*15);
}


TEST(VoxFoxRootNode,createSphere)
{
  VoxFoxTree sphere;
  sphere.createSphere(glm::vec3(0.0f,0.0f,0.0f),1.0f,glm::vec3(1.0f,0.0f,0.0f));
  EXPECT_TRUE(sphere.updateVoxCount()==562460);
}

TEST(VoxFoxRootNode,createSphere2)
{
  VoxFoxTree sphere2;
  sphere2.createSphere(glm::vec3(0.0f,0.0f,0.0f),1.0f,glm::vec3(1.0f,0.0f,0.0f));
  sphere2.createSphere(glm::vec3(3.0f,0.0f,0.0f),1.0f,glm::vec3(1.0f,0.0f,0.0f));
  EXPECT_TRUE(sphere2.updateVoxCount()==562460*2);
}




////----------------------------------SET OPERATORS-------------------------------
TEST(VoxFoxRootNode,MinusOperator)
{
  VoxFoxTree sphere1, sphere2;
  sphere1.createSphere(glm::vec3(0.0f,0.0f,0.0f),1.0f,glm::vec3(1.0f,0.0f,0.0f));
  sphere2.createSphere(glm::vec3(0.3f,0.0f,0.0f),1.0f,glm::vec3(1.0f,0.0f,0.0f));
  sphere1-=sphere2;
  EXPECT_TRUE(sphere1.updateVoxCount()==245106);
}

TEST(VoxFoxRootNode,IntersectionOperator)
{
  VoxFoxTree sphere1, sphere2;
  sphere1.createSphere(glm::vec3(0.0f,0.0f,0.0f),1.0f,glm::vec3(1.0f,0.0f,0.0f));
  sphere2.createSphere(glm::vec3(0.3f,0.0f,0.0f),1.0f,glm::vec3(1.0f,0.0f,0.0f));
  sphere1+=sphere2;

  EXPECT_TRUE(sphere1.updateVoxCount()==439907);
}

TEST(VoxFoxRootNode,UnionOperator)
{
  VoxFoxTree sphere1, sphere2;
  sphere1.createSphere(glm::vec3(0.0f,0.0f,0.0f),1.0f,glm::vec3(1.0f,0.0f,0.0f));
  sphere2.createSphere(glm::vec3(0.3f,0.0f,0.0f),1.0f,glm::vec3(1.0f,0.0f,0.0f));
  sphere1|=sphere2;
printf("SIZE:::%d",sphere1.updateVoxCount());

  EXPECT_TRUE(sphere1.updateVoxCount()==685013);
}
// These next one span a few primary nodes
// This way we can test if it work for larger objects
// cylinders are tall and skinny so they're quick large objects
TEST(VoxFoxRootNode,MinusOperator2)
{
  VoxFoxTree sphere1, sphere2;
  sphere1.createCylinder(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,0.0f,0.0f),0.03f,20.0f,glm::vec3(1.0f,0.0f,0.0f));
  sphere2.createCylinder(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f),0.03f,20.0f,glm::vec3(1.0f,0.0f,0.0f));
  sphere1-=sphere2;
  printf("SIZE:::%d",sphere1.updateVoxCount());

  EXPECT_TRUE(sphere1.updateVoxCount()==4592);
}

TEST(VoxFoxRootNode,IntersectionOperator2)
{
  VoxFoxTree sphere1, sphere2;
  sphere1.createCylinder(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,0.0f,0.0f),0.03f,20.0f,glm::vec3(1.0f,0.0f,0.0f));
  sphere2.createCylinder(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f),0.03f,20.0f,glm::vec3(1.0f,0.0f,0.0f));
  sphere1+=sphere2;
  printf("SIZE:::%d",sphere1.updateVoxCount());

  EXPECT_TRUE(sphere1.updateVoxCount()==8);
}

TEST(VoxFoxRootNode,UnionOperator2)
{
  VoxFoxTree sphere1, sphere2;
  sphere1.createCylinder(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,0.0f,0.0f),0.03f,20.0f,glm::vec3(1.0f,0.0f,0.0f));
  sphere2.createCylinder(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f),0.03f,20.0f,glm::vec3(1.0f,0.0f,0.0f));
  sphere1|=sphere2;

  EXPECT_TRUE(sphere1.updateVoxCount()==8184);
}

////--------------------POLYGON CALCULATE-----------------------
TEST(VoxFoxRootNode,calculatePolys)
{
  VoxFoxTree test;
  test.addVoxel(glm::vec3(1.0,2.0,3.0),Voxel());
  test.calculatePolys();
  std::vector<float> testVerts = test.getVertexes();

  EXPECT_TRUE(testVerts.size()==108);
}

