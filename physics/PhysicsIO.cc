#include "physics/PhysicsIO.h"
#include "base/FileParser.h"

void PhysicsIO::SetCoordsOffset(const Coordinates & c)
{
  m_offset = c;
}

void PhysicsIO::Read(PhysObject & p, const string & fileName)
{
 FlatFileParser parser;


 parser.Open(fileName);
  while (parser.ParseLine()) {
    if (parser.GetItemCount() == 0)
      continue;
    const string & s = parser.AsString(0);
    if (s[0] == '#')
      continue;
    
    if (s == "<point>") {
      PhysMinimal min;
      while (parser.ParseLine()) {
	if (parser.GetItemCount() == 0)
	  continue;
	const string & ss = parser.AsString(0);
	if (ss[0] == '#')
	  continue;
	
	if (ss == "Mass") {
	  min.SetMass(parser.AsFloat(1));
	}
	if (ss == "Coordinates") {
	  Coordinates c;
	  c[0] = parser.AsFloat(1) + m_offset[0];
	  c[1] = parser.AsFloat(2) + m_offset[1];
	  c[2] = parser.AsFloat(3) + m_offset[2];
	  min.SetPosition(c);
	}
	if (ss == "Velocity") {
	  Coordinates c;
	  c[0] = parser.AsFloat(1);
	  c[1] = parser.AsFloat(2);
	  c[2] = parser.AsFloat(3);
	  min.SetVelocity(c);
	}
	
	if (ss == "</point>") {
	  p.Add(min);
	  break;
	}
      }
    }

  }
  p.Fixate();
}

void PhysicsIO::Write(const PhysObject & p, const string & fileName)
{
  FILE * pF = fopen(fileName.c_str(), "w");

  fprintf(pF, "<object>\n");
  for (int i=0; i<p.isize(); i++) {
    fprintf(pF, "<point>\n");
    const PhysMinimal & m = p[i];
    fprintf(pF, "Mass %f\n", m.GetMass());
    const Coordinates & cc = m.GetPosition();
    const Coordinates & vv = m. GetVelocity();
   
    fprintf(pF, "Coordinates %f %f %f\n", cc[0], cc[1], cc[2]);
    fprintf(pF, "Velocity %f %f %f\n", vv[0], vv[1], vv[2]);

    fprintf(pF, "</point>\n\n");
  }
  fprintf(pF, "</object>\n");
  fclose(pF);
}

