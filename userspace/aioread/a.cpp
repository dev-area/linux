#include <sys/types.h>
#include <unistd.h>
#include <aio.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <string.h>

using namespace std;

const int SIZE_TO_READ = 100;

int main()
{
	// open the file
	int file = open("blah.txt", O_RDONLY, 0);
	
	if (file == -1)
	{
		cout << "Unable to open file!" << endl;
		return 1;
	}
	
	// create the buffer
	char* buffer = new char[SIZE_TO_READ];
	
	// create the control block structure
	aiocb cb;
	
	memset(&cb, 0, sizeof(aiocb));
	cb.aio_nbytes = SIZE_TO_READ;
	cb.aio_fildes = file;
	cb.aio_offset = 0;
	cb.aio_buf = buffer;
	
	// read!
	if (aio_read(&cb) == -1)
	{
		cout << "Unable to create request!" << endl;
		close(file);
	}
	
	cout << "Request enqueued!" << endl;
	
	// wait until the request has finished
	while(aio_error(&cb) == EINPROGRESS)
	{
		cout << "Working..." << endl;
	}
	
	// success?
	int numBytes = aio_return(&cb);
	
	if (numBytes != -1)
		cout << "Success!" << endl;
	else
		cout << "Error!" << endl;
		
	// now clean up
	delete[] buffer;
	close(file);
	
	return 0;
}

