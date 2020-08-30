#include <iostream>
#include <sys/time.h>			// gettimeofday()
#include <time.h>				// localtime(), strftime()
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main(int argc, char const *argv[])
{
	int iterations = 20, i = 0;
	char buffer[30];
	struct timeval tv;
	time_t curtime;

	while(1)
	{
		gettimeofday(&tv, NULL);
		curtime = tv.tv_sec;
		strftime(buffer, 30, "%X.", localtime(&curtime));
		printf("%s%ld\n", buffer, tv.tv_usec);

		i++;
	}

	return 0;
}