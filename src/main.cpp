#include <string>
using std::string;
#include <baseapi.h>

void TestSetSourceRes();


#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    TestSetSourceRes();
#if 0    
    fprintf(stderr, "back again\n");
    exit(101);
#endif
    return(0);
}


void
TestSetSourceRes()
{
    tesseract::TessBaseAPI api;
    api.SetSourceResolution(200);
//    api.ReadConfigFile("bob");
}

extern "C"
void
R_check()
{
    TestSetSourceRes();
}
