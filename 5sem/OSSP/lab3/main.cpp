#include "critical_section.h"

// There is a problem: boost does not seem to be easy to install in
//   Debian/Ubuntu.
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>

#include <stdio.h>

int main(int argc, char **argv)
{
  std::unique_ptr<CriticalSection>
    pCriticalSection(new CriticalSection("mySection"));

  pCriticalSection.reset();
  return 0;
}
