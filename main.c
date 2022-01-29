#include <stdlib.h>
#include <crtdbg.h>
#include "getCheckStr.h"

int main(int argc, char* argv[])
{

  GetStr(argc, argv);
  
  _CrtDumpMemoryLeaks();

  return 0;
}
