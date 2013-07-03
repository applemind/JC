#include "CtFile.h"

int main(int argc,char *argv[])
{
	CtFile file;

	file.writeonly_open("test.txt");
	file.close();
	return 0;
}
