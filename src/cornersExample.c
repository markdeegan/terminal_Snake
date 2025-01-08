/* MD20250105-04 Sun  5 Jan 2025 12:34:17 GMT
Example replacing ewcape codes with more readable #defines */

#include <stdio.h>
#include "primitives.h"

int main(int argc, char** argv)
{
	printf("\e(0j\e(B");//prints Bottom Right Corner
	printf(BRC);

	printf("\e(0k\e(B");//prints Top    Right Corner
	printf(TRC);

	printf("\e(0l\e(B");//prints Top    Left  Corner
	printf(TLC);

	printf("\e(0m\e(B");//prints Bottom Left  Corner
	printf(BLC);

	printf("\n");
	
	return 0;
}
