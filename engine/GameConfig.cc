#include "graphics/Messages.h"
#include "engine/GameConfig.h"


void GameConfig::Read(const string & fileName)
{
  FlatFileParser parser;
  parser.Open(fileName);

  MapConfig * pMap = NULL;
  AnimatedSceneNode * pAnim = NULL;
  SceneNode * pNode = NULL;

  string MapName;

  string Terrain;
  string Texture1;
  string Texture2;
  string SkyUp;
  string SkyDown;
  string SkyLeft;
  string SkyRight;
  string SkyFront;
  string SkyBack;
  string SkyDome;

  double x = 0;
  double y = 0;
  double z = 0;
  double xd = 1;
  double yd = 0;
  double zd = 0;
  string MD2Model;
  string MDTexture;
  string MDTexture1;
  string MDTexture2;
  string Mesh;
  string Animation;
  double AnimationSpeed = 40;
  string Name;
  string Type;
  string Physics;
  string Control;

  Properties prop;

  bool bCompound = false;

  while (parser.ParseLine()) {
    if (parser.GetItemCount() == 0)
      continue;
    const string & s = parser.AsString(0);
    if (s[0] == '#')
      continue;

    // Properties
    if (s == "<properties>") {
      while (parser.ParseLine()) {
	if (parser.GetItemCount() == 0)
	  continue;
	const string & ss = parser.AsString(0);
 	if (ss == "</properties>")
	  break;
	prop.Add(parser.AsString(1), 0.);
      }
      m_template.SetProperties(prop);
    }
    // Actions
    if (s == "<actions>") {
      while (parser.ParseLine()) {
	if (parser.GetItemCount() == 0)
	  continue;
	const string & ss = parser.AsString(0);
 	if (ss == "</actions>")
	  break;
	m_actions.Add(parser.AsString(1), 0.);
      }
    }
    // Entities
    if (s == "<entity>") {
      Properties tmp = m_template.GetProperties();
      string name;
      while (parser.ParseLine()) {
	if (parser.GetItemCount() == 0)
	  continue;
	const string & ss = parser.AsString(0);
 	if (ss == "</entity>")
	  break;
	if (s == "EntName")
	  name = parser.AsString(1);
	else
	  tmp.Add(parser.AsString(1), 0.);
      }
      Entity nn;
      nn.SetProperties(tmp);
      nn.SetName(name);
      m_all.push_back(nn);
    }


    if (s == "GraphicsEngine") {
      m_basics.m_graphicsEng = parser.AsString(1);
    }

    if (s == "MD2Model") 
       MD2Model = parser.AsString(1);
    if (s == "MDTexture") 
      MDTexture = parser.AsString(1);
    if (s == "MDTexture1") 
      MDTexture1 = parser.AsString(1);
    if (s == "MDTexture2") 
      MDTexture2 = parser.AsString(1);
    if (s == "Mesh") 
      Mesh = parser.AsString(1);
    if (s == "Animation") 
      Animation = parser.AsString(1);
    if (s == "Name") 
      Name = parser.AsString(1);
    if (s == "Type") 
      Type = parser.AsString(1);
    if (s == "Physics") 
      Physics = parser.AsString(1);
    if (s == "Control") 
      Control = parser.AsString(1);


    if (s == "AnimationSpeed") 
      AnimationSpeed= parser.AsFloat(1);
    if (s == "X") 
      x = parser.AsFloat(1);
    if (s == "Y") 
      y = parser.AsFloat(1);
    if (s == "Z") 
      z = parser.AsFloat(1);
    if (s == "XD") 
      xd = parser.AsFloat(1);
    if (s == "YD") 
      yd = parser.AsFloat(1);
    if (s == "ZD") 
      zd = parser.AsFloat(1);



    if (s == "MapName") 
      MapName = parser.AsString(1);
    if (s == "Terrain") 
      Terrain = parser.AsString(1);
    if (s == "Texture1") 
      Texture1 = parser.AsString(1);
    if (s == "Texture2") 
      Texture2 = parser.AsString(1);
    if (s == "SkyUp") 
      SkyUp  = parser.AsString(1);
    if (s == "SkyDown") 
      SkyDown = parser.AsString(1);
    if (s == "SkyLeft") 
      SkyLeft = parser.AsString(1);
    if (s == "SkyRight") 
      SkyRight = parser.AsString(1);
    if (s == "SkyFront") 
      SkyFront = parser.AsString(1);
    if (s == "SkyBack") 
      SkyBack = parser.AsString(1);
    if (s == "SkyDome") 
      SkyDome  = parser.AsString(1);
 
    if (s == "</map>") {
      pMap->m_terrain.Set(Terrain,
			  Texture1,
			  Texture2,
			  SkyUp,
			  SkyDown,
			  SkyLeft,
			  SkyRight,
			  SkyFront,
			  SkyBack,
			  SkyDome);
      pMap->m_name = MapName;
    }

    if (s == "</animated>" || s == "</physics>") {
      Coordinates cc2;
      cc2[0] = x;
      cc2[1] = y;
      cc2[2] = z;
      pAnim->SetCoordinates(cc2);
      Coordinates dir2;
      dir2[0] = xd;
      dir2[1] = yd;
      dir2[2] = zd;
      pAnim->SetDirection(dir2);
      
      pAnim->SetModel(MD2Model);
      pAnim->SetTexture(MDTexture);
      pAnim->SetAnimation(Animation);
      pAnim->SetAnimationSpeed(AnimationSpeed);
      pAnim->SetName(Name);     
      pAnim->SetType(Type);     
      pAnim->SetPhysics(Physics);     
      pAnim->SetControl(Control);     
      x = y = z = yd = zd = 0;
      xd = 1;
      Name = "";
      Type = "";
      MDTexture = "";
      MDTexture1 = "";
      MDTexture2 = "";
      Physics = "";
      Control = "";
    }
    if (s == "</node>") {    
      Coordinates cc1;
      cc1[0] = x;
      cc1[1] = y;
      cc1[2] = z;
      pNode->SetCoordinates(cc1);
      Coordinates dir2;
      dir2[0] = xd;
      dir2[1] = yd;
      dir2[2] = zd;
      pNode->SetDirection(dir2);
       
      pNode->SetMesh(Mesh);
      pNode->SetTexture1(MDTexture1);
      pNode->SetTexture2(MDTexture2);
      pNode->SetName(Name);     
      pNode->SetType(Type);     
      pNode->SetPhysics(Physics);     
      pNode->SetControl(Control);     
      Name = "";
      Type = "";
      MDTexture = "";
      MDTexture1 = "";
      MDTexture2 = "";
      Physics = "";
      Control = "";
      x = y = z = yd = zd = 0;
      xd = 1;
    }

    if (s == "<compound>") {
      bCompound = true;
    }
    if (s == "</compound>") {
      bCompound = false;
    }


    if (s == "<map>") {
      MapConfig m;
      m_maps.push_back(m);
      pMap = &m_maps[m_maps.isize()-1];
    }
    if (s == "<animated>") {
      AnimatedSceneNode tmp;
      pMap->m_animNodes.push_back(tmp);      
      pAnim = &pMap->m_animNodes[pMap->m_animNodes.isize()-1];     
    }
    if (s == "<physics>") {
      AnimatedSceneNode tmp;
      pMap->m_physNodes.push_back(tmp);      
      pAnim = &pMap->m_physNodes[pMap->m_physNodes.isize()-1];     
    }
    if (s == "<node>") {
      SceneNode tmp;
      pMap->m_nodes.push_back(tmp);      
      pNode = &pMap->m_nodes[pMap->m_nodes.isize()-1];
    }
  }

}
