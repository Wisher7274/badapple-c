#include <stdio.h>
#include <unistd.h>

int main(void) {
	printf("Sorry i can't write in C well yet.\n");

	const char *BEGIN = "\r";
	char *frames[4] = {
		"|",
		"/",
		"-",
		"\\",
	};
	
	for(int i = 0;;) {
		write(1 , BEGIN , 1);
		write(1 , frames[i] , 1);
		if(i<3) {
			i++;
		} else {
			i = 0;
		}
		usleep(100000);
	}
	return 0;
}
