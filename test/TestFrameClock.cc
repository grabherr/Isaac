

#include "engine/FrameClock.h"

int main(int argc,char** argv)
{
  FrameClock clock;

  // Half second intervals
  clock.SetFrameRate(2);


  int i;
  cout << "Frame rate 2 " << endl; 
  long long base = clock.GetAbsolute();
  double sec = clock.GetSec();
  for (i=0; i<20; i++) {
    cout << "Elapsed: " << clock.GetAbsolute()-base << endl;
    cout << "Seconds: " << clock.GetSec()-sec << endl;
    //cout << clock.GetAbsolute() << " " << base << endl;
    clock.WaitUntilNextFrame();
  }
  cout << "Frame rate 60 " << endl; 
  clock.SetFrameRate(60);
  for (i=0; i<120; i++) {
    cout << "Elapsed: " << clock.GetAbsolute()-base << endl;
    cout << "Seconds: " << clock.GetSec()-sec << endl;
    clock.WaitUntilNextFrame();
  }
}
