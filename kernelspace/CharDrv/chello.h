#ifndef CHELLO_H_
#define CHELLO_H_

struct hello2_ioctl_args {
	int max_count;
};

/* Use 'M' as magic number */
#define HELLO2_IOC_MAGIC  'M'

#define IOCSMAXCOUNT   _IOR(HELLO2_IOC_MAGIC, 0, struct hello2_ioctl_args *)
#define IOCGMAXCOUNT   _IOR(HELLO2_IOC_MAGIC, 1, struct hello2_ioctl_args *)

#define HELLO2_IOC_MAXNR 1



#endif /*CHELLO_H_*/
