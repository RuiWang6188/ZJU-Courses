#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *a[2], *b[2], *c[2];
	char *protect, *recatch;
	
	a[0] = (char *)malloc(0x8);
	a[1] = (char *)malloc(0x8);

	/* debug checkpoint - 0 */

	b[0] = (char *)malloc(0x18);
	b[1] = (char *)malloc(0x18);

	/* debug checkpoint - 1 */

        c[0] = (char *)malloc(0x20);
        c[1] = (char *)malloc(0x20);

	/* debug checkpoint - 2 */

	protect = malloc(0x100);

	free(a[0]);
	free(a[1]);
	free(b[0]);
	free(b[1]);

	/* debug checkpoint - 3 */

	recatch = malloc(0x10);

	/* debug checkpoint - 4 */

        free(c[0]);
        free(c[1]);

	/* debug checkpoint - 5 */

	free(protect);

        a[0] = (char *)malloc(0x500);
        a[1] = (char *)malloc(0x500);

	protect = malloc(0x500);

	/* debug checkpoint - 6 */

        free(a[0]);
        free(a[1]);

	/* debug checkpoint - 7 */

	exit(0);
}
