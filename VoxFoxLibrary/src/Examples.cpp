#include "Examples.h"

int Examples::runExample(int _whichExample, VoxFoxTree * _toRunOn, std::string _RelativePathToLibrary)
{
  ngl::Obj * dwarfmesh = new ngl::Obj(_RelativePathToLibrary+"/models/dwarf.obj");
  ngl::Obj * deermesh = new ngl::Obj(_RelativePathToLibrary+"/models/deer.obj");
  ngl::Obj * foxmesh = new ngl::Obj(_RelativePathToLibrary+"/models/foxtoy.obj");

  ngl::Image * dwarftex = new ngl::Image(_RelativePathToLibrary+"/images/dwarf.jpg");
  ngl::Image * deertex = new ngl::Image(_RelativePathToLibrary+"/images/deer.jpg");

  ngl::Image * foxtex= new ngl::Image(_RelativePathToLibrary+"/images/foxemoji.png");
  ngl::Image * titletex= new ngl::Image(_RelativePathToLibrary+"/images/title.png");
  ngl::Image * labelstex = new ngl::Image(_RelativePathToLibrary+"/images/labels.png");

  ngl::Image * easteregg = new ngl::Image(_RelativePathToLibrary+"/images/computers.png");

  if(_whichExample==0)
  {
    // EXAMPLE 0: The Title -----------------------------------------------------
    VoxFoxTree theTitleExample;
    glm::vec3 currentPos = glm::vec3(0.0f,1.0f,0.0f);
    theTitleExample.drawFlatImage(currentPos,titletex,3.5f);
    theTitleExample.drawFlatImage(currentPos+glm::vec3(0.0f,-1.0f,0.8f),foxtex,1.0f);
    // Zoom out for a scary surprise
    theTitleExample.drawFlatImage(currentPos+glm::vec3(0.0f,0.0f,5.8f),easteregg,6.0f);
    (*_toRunOn)=theTitleExample;
  }
  else if(_whichExample==1)
  {
    // EXAMPLE 1: Gnome Sphere Binary Operations----------------------------------
    VoxFoxTree gnomeSphereExample;
    float labelsize = 10.0f;
    gnomeSphereExample.drawFlatImage(glm::vec3(0.0f,0.0f,0.0f),labelstex,labelsize);

    float depthh = 1.0f;
    VoxFoxTree dwarf, sphered, dwarfunion, dwarfminus, dwarfintersect;
    dwarf.importTexturedObj(dwarfmesh,dwarftex,2.0f);
    sphered.createSphere(glm::vec3(0.0f,1.5f,0.0f),0.7f,glm::vec3(1.0f,0.0f,0.0f));
    //dwarf |= sphered;


    glm::vec3 dwarfstart = glm::vec3(-0.85f*labelsize/2.0f,0.3f*labelsize/2.0f,depthh);

    dwarfunion = sphered | dwarf;
    dwarfunion.translate(dwarfstart+glm::vec3(4.0f,0.0f,0.0f));
    dwarfminus = dwarf - sphered;
    dwarfminus.translate(dwarfstart+glm::vec3(6.0f,0.0f,0.0f));
    dwarfintersect = dwarf + sphered;
    dwarfintersect.translate(dwarfstart+glm::vec3(8.0f,0.0f,0.0f));

    dwarf.translate(dwarfstart);
    sphered.translate(dwarfstart+glm::vec3(2.0f,0.0f,0.0f));

    gnomeSphereExample |= dwarf | sphered | dwarfunion | dwarfminus | dwarfintersect ;
    gnomeSphereExample.translate(glm::vec3(4.5f,-3.0f,0.0f));
    (*_toRunOn)=gnomeSphereExample;
  }
  else if(_whichExample==2)
  {
    // EXAMPLE 2: The CSG TREE ------------------------------------------------
    VoxFoxTree theCSGTree;// = title;
    float labelsize = 10.0f;
    float depthh = 1.0f;

    theCSGTree.drawFlatImage(glm::vec3(0.0f,0.0f,0.0f),labelstex,labelsize);

    float scale = 0.7f;
    glm::vec3 ourcol = glm::vec3(0.0f,0.7,1.0f);
    VoxFoxTree sphere, cube, cylinder1, cylinder2, cylinder3, CSGfinal, CSG2cylinder, CSG3cylinder, CSGcubesphere;

    cylinder1.createCylinder(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f),0.2f*scale,1.1f*scale,ourcol);
    cylinder2.createCylinder(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f),0.2f*scale,1.1f*scale,ourcol);
    cylinder3.createCylinder(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,0.0f,0.0f),0.2f*scale,1.1f*scale,ourcol);

    sphere.createSphere(glm::vec3(0.0f,0.0f,0.0f),0.6f*scale,ourcol);
    cube.createBox(glm::vec3(-0.5f,-0.5f,-0.5f)*scale,glm::vec3(0.5f,0.5,0.5f)*scale,ourcol);

    CSG2cylinder = cylinder3 | cylinder2;
    CSG3cylinder = cylinder1 | cylinder2 | cylinder3;
    CSGcubesphere = cube + sphere;

    CSGfinal = ((cube + sphere) - (cylinder1 | cylinder2 | cylinder3)) ;

    cube.translate(glm::vec3(((21.0f-256.0f)/512.0f)*labelsize,((256.0f-411.0f)/512.0f)*labelsize,depthh));
    sphere.translate(glm::vec3(((112.0f-256.0f)/512.0f)*labelsize,((256.0f-411.0f)/512.0f)*labelsize,depthh));
    cylinder1.translate(glm::vec3(((153.0f-256.0f)/512.0f)*labelsize,((256.0f-411.0f)/512.0f)*labelsize,depthh));
    cylinder2.translate(glm::vec3(((292.0f-256.0f)/512.0f)*labelsize,((256.0f-507.0f)/512.0f)*labelsize,depthh));
    cylinder3.translate(glm::vec3(((196.0f-256.0f)/512.0f)*labelsize,((256.0f-507.0f)/512.0f)*labelsize,depthh));
    CSG2cylinder.translate(glm::vec3(((230.0f-256.0f)/512.0f)*labelsize,((256.0f-411.0f)/512.0f)*labelsize,depthh));
    CSG3cylinder.translate(glm::vec3(((181.0f-256.0f)/512.0f)*labelsize,((256.0f-317.0f)/512.0f)*labelsize,depthh));
    CSGcubesphere.translate(glm::vec3(((66.0f-256.0f)/512.0f)*labelsize,((256.0f-317.0f)/512.0f)*labelsize,depthh));
    CSGfinal.translate(glm::vec3(((123.0f-256.0f)/512.0f)*labelsize,((256.0f-234.0f)/512.0f)*labelsize,depthh));

    theCSGTree |= cube | sphere | cylinder1 | cylinder2 | cylinder3 | CSG2cylinder | CSG3cylinder | CSGcubesphere | CSGfinal;
    theCSGTree.translate(glm::vec3(2.5f,2.0f,0.0f));
    (*_toRunOn)=theCSGTree;
  }
  else if(_whichExample==3)
  {
    // EXAMPLE 3: The Cascade ------------------------------------------------
    VoxFoxTree theCascade;

    // These guys change how they're shaded each iteration.

    VoxFoxTree deercascade;
    for(int i =0; i<19; ++i)
    {
      bool normals = i%3;
      bool interpo = i%4;
      deercascade.importTexturedObj(deermesh,deertex,2.0f-(i*0.1f),normals,!interpo);
      deercascade.translate(glm::vec3((2.0f-(i*0.1f))*1.0f,0.0f,0.0f));
    }

    VoxFoxTree gnomecascade;
    for(int i =1; i<20; ++i)
    {
      bool normals = i%3;
      bool interpo = i%4;
      gnomecascade.importTexturedObj(dwarfmesh,dwarftex,2.0f-(i*0.1f),normals,!interpo);
      gnomecascade.translate(glm::vec3(-(2.0f-(i*0.1f))*0.7,0.0f,0.0f));
    }

    theCascade = gnomecascade | deercascade;
    (*_toRunOn)=theCascade;
  }
  else if(_whichExample==4)
  {
    // EXAMPLE 3: The Cascade ------------------------------------------------
    VoxFoxTree theHornedDeerGnome;

    float gnomedeerscale =1.0f;
    for(int i=1; i<20; ++i)
    {
      bool normals = i%3;
      bool interpo = i%4;
      gnomedeerscale=i*0.05f;
      VoxFoxTree gnomehead, sphere1, deer;
      gnomehead.importTexturedObj(dwarfmesh,dwarftex,1.6f*gnomedeerscale,normals,!interpo);
      sphere1.createSphere(glm::vec3(0.0f,1.55f,0.0f)*gnomedeerscale,0.7f*gnomedeerscale,glm::vec3(1.0f,0.0f,0.0f));
      gnomehead += sphere1;
      deer.importTexturedObj(deermesh,deertex,2.0f*gnomedeerscale,normals,!interpo);
      gnomehead.translate(glm::vec3(-0.65f,0.4f,0.2f)*gnomedeerscale);
      deer |= gnomehead; // imagine placing a gnome head on a deer
      VoxFoxTree horns;
      horns.createCylinder(glm::vec3(-0.75f,2.0f,0.3f)*gnomedeerscale,glm::vec3(0.0f,1.0f,0.0f),0.05f*gnomedeerscale,0.8f*gnomedeerscale,glm::vec3(1.0f,0.0f,0.0f));
      horns.createCylinder(glm::vec3(-0.42f,2.0f,0.3f)*gnomedeerscale,glm::vec3(0.0f,1.0f,0.0f),0.05f*gnomedeerscale,0.8f*gnomedeerscale,glm::vec3(1.0f,0.0f,0.0f));
      theHornedDeerGnome |= deer | horns;
      theHornedDeerGnome.translate(glm::vec3(2.0f*gnomedeerscale*1.1f,0.0f,0.0f));
    }
    (*_toRunOn)=theHornedDeerGnome;
  }
  else if(_whichExample==5)
  {
    // EXAMPLE 5: The Gnome ------------------------------------------------
    VoxFoxTree theGnome;
    theGnome.importTexturedObj(dwarfmesh,dwarftex,2.0f);
    (*_toRunOn)=theGnome;
    //Number of Voxels:               131,103
    //Number of Polygons              298,152
  }
  else if(_whichExample==6)
  {
    // EXAMPLE 6: The RGB Gnomes ------------------------------------------------
    VoxFoxTree theRGBGnomes;
    theRGBGnomes.importObj(dwarfmesh,glm::vec3(1.0f,0.3f,0.0f),1.5f);
    theRGBGnomes.translate(glm::vec3(1.0f,0.0f,0.0f));
    theRGBGnomes.importObj(dwarfmesh,glm::vec3(0.3f,1.0f,0.0f),2.0f);
    theRGBGnomes.translate(glm::vec3(0.85f,0.0f,0.0f));
    theRGBGnomes.importObj(dwarfmesh,glm::vec3(0.0f,0.3f,1.0f),1.1f);
    theRGBGnomes.translate(glm::vec3(-0.85f,0.0f,0.0f));
    (*_toRunOn)=theRGBGnomes;
    //Number of Voxels:               258,706
    //Number of Polygons              555,252
  }
  else if(_whichExample==7)
  {
    // EXAMPLE 7: Idiotically Massive Gnome -------------------------------------
    VoxFoxTree theGnome;
    theGnome.importTexturedObj(dwarfmesh,dwarftex,10.0f);
    (*_toRunOn)=theGnome;
//    -------------------------------------------------
//    Number of Voxels:               3,200,806
//    Number of Polygons              7,464,780
//    -------------------------------------------------
  }
  else if(_whichExample==8)
  {
    // EXAMPLE 8: Idiotically Massive Deer -------------------------------------
    VoxFoxTree theDeer;
    theDeer.importTexturedObj(deermesh,deertex,10.0f);
    (*_toRunOn)=theDeer;
//    -------------------------------------------------
//    Number of Voxels:               1,277,268
//    Number of Polygons              4,006,668
//    -------------------------------------------------
  }
}
