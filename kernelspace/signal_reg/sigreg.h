#ifndef SIGREGH
#define SIGREGH

#define SIGNAL_MAGIC 'g'
#define SIGNAL_REGISTER 	_IOW(SIGNAL_MAGIC, 0x09, int) 
#define SIGNAL_SETSIG 		_IOW(SIGNAL_MAGIC, 0x0a, int) 
#define SIGNAL_BROADCAST 	_IOW(SIGNAL_MAGIC, 0x0b, int)

#endif
