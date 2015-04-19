#ifndef IAU_H
#define IAU_H

#include <string>

using namespace std;

// ============================================================
// Class to communicate 3-dim coordinates. 
// ============================================================
class RelativeCoordinates
{
 public:
  RelativeCoordinates() {
    m_x = 0.;
    m_y = 0.;
    m_z = 0.;    
  }
  RelativeCoordinates(double x, double y, double z) {
    m_x = x;
    m_y = y;
    m_z = z;    
  }

  void Set(double x, double y, double z) {
    m_x = x;
    m_y = y;
    m_z = z;    
  }

  double X() const {return m_x;}
  double Y() const {return m_y;}
  double Z() const {return m_z;}

  void operator += (const RelativeCoordinates & p) {
    m_x += p.m_x;
    m_y += p.m_y;
    m_z += p.m_z;
  }
  void operator -= (const RelativeCoordinates & p) {
    m_x -= p.m_x;
    m_y -= p.m_y;
    m_z -= p.m_z;
  }

 private:
  // Coordinates
  double m_x;
  double m_y;
  double m_z;   
};

// ============================================================
// Each element has a list of properties, which is stored in a 
// vector of doubles. Properties can be freely defined, e.g. 
// wet, poisonous, nasty, slick, color, etc., but is fixed in 
// the number of entries. Also, the range is freely definable, 
// but impacts their relative weight.
// ============================================================
#define PROP_COUNT 8

class ElementProperties
{
 public:
  ElementProperties() {
    for (int i=0; i<PROP_COUNT; i++)
      m_prop[i] = 0.;
  }

  int isize() const {return PROP_COUNT;}
  unsigned int size() const {return PROP_COUNT;}

  double & operator [] (int i) {return m_prop[i];}
  const double & operator [] (int i) const {return m_prop[i];}

 private:
  double m_prop[PROP_COUNT];
};

// ============================================================
// Elements are everything that is visible in the map. They
// include static objects, as well as other entities, such as
// players and IAUs.
// Each element needs a (non-unique) name and a unique ID.
// ============================================================
class Element
{
 public:
  Element() {
    m_id = -1;
  }

  Element(const string & s) {
    m_name = s;
    m_id = -1;
  }

  Element(const string & s, int id) {
    m_name = s;
    m_id = id;
  }
  
  Element(const string & s, 
	  int id, 
	  const RelativeCoordinates & pos, 
	  const RelativeCoordinates & face) {
    m_name = s;
    m_id = id;
    m_pos = pos;
    m_face = face;
  }

  Element(const string & s, 
	  int id, 
	  const RelativeCoordinates & pos, 
	  const RelativeCoordinates & face,
	  const ElementProperties & prop) {
    m_name = s;
    m_id = id;
    m_pos = pos;
    m_face = face;
    m_prop = prop;
  }

  void SetName(const string & s) {m_name = s;}

  void SetID(int i) {m_id = i;}

  void SetCoordinates(const RelativeCoordinates & p) {
    m_pos = p;
  }

  void SetFaceCoordinates(const RelativeCoordinates & p) {
    m_face = p;
  }

  void SetProperties(const ElementProperties & p) {
    m_prop = p;
  }

  const RelativeCoordinates & GetCoordinates() const {return m_pos;}
  const RelativeCoordinates & GetFaceCoordinates () const {return m_face;}
  const string & GetName() const {return m_name;}
  int GetID() const {return m_id;}
  const ElementProperties & GetProperties() const {return m_prop;}

 private:
  RelativeCoordinates m_pos;
  RelativeCoordinates m_face;
  ElementProperties m_prop;
  string m_name;
  int m_id;
};


// ============================================================
// Public interface to the IAU.
//
// The game engine needs to inform the IAU about:
// - the IAU's additional abilities (use weapons etc.)
// - the IAU's current direction it is facing.
// - each element that is visible to the IAU, in relative
//   coordinates to the IAU's position and direction.
// - the IAU's status, i.e. whether it got hurt, hurt other
//   things etc., as well as its health, death, etc.
// 
// The game engine queries the IAU for:
// - its (proposed) action
// ============================================================
class IAU
{
 public:
  
  virtual ~IAU() {}

  // Set up the IAU
  // Tell it what capabilities it has. It will learn/has learned 
  // what those are. The status decribes how many of it the IAU has
  // at its disposal.
  virtual void AddCapability(const string & cap, double status) = 0;

  // Give it some initial idea of what to expect, to speed up the
  // learning process
  virtual void Hint(const Element & e,
		    double pain,
		    double reward,
		    double collision) = 0;

  // -----------------------------------------------------------------
  // Functions to be continously called to update the IAU:
  //
  // Call this to indicate transfer of new status data
  virtual void StartFrame() = 0;

  // Direction the IAU is facing
  virtual void SetIAUDirection(const RelativeCoordinates & dir) = 0;

  // Immediate effect of interactions with the IAU, on a scale from 0 to 1.,
  // and death as a bool.
  virtual void SetIAUStatus(double pain, 
			    double reward, 
			    double collision,
			    bool bDeath = false) = 0;

  // Health status
  virtual void SetIAUHealth(double health) = 0;

  // Capability status info, e.g. ammo, on a scale from 0 to 1.
  virtual void SetIAUCapability(const string & inv, double status) = 0;

  // Elements in the map, one at a time 
  virtual void AddElement(const Element & e) = 0;
 
  // Done for now.
  virtual void CloseFrame() = 0;
 
  // -----------------------------------------------------------------
  // Functions requesting actions by the IAU:
  //
  // Get relative change in direction (as in mouse movements)
  virtual RelativeCoordinates GetDirection() = 0;

  // Get movement request
  virtual RelativeCoordinates GetMovement() = 0;

  // Get number of capability requests
  virtual int GetNumCapabilities() = 0;
  
  // IAU requests to use a capability. Return false if not
  // possible, true if succesful. The parameter "intensity"
  // specifies as which intensity the capability is to be
  // applied (0 would e.g. just indicate a weapon switch)
  virtual bool ApplyCapability(string & cap, double & intensity) = 0;
  

};


// Request a new IAU from the system
IAU * CreateIAU(const string & datafile = "");

// IAU no longer in use - only call when the IAU is out of the game
void DestroyIAU(IAU * p, bool bSaveData = true);




#endif // IAU_H

