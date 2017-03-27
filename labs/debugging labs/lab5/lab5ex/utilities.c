/*
 * utilittttties.c
 *
 *  Created on: May 15, 2010
 *      Author: root
 */
#include <stdio.h>                  // Needed for printf()
#include <stdlib.h>                 // Needed for rand()

#include "app.h"


//=============================================================================
//=  Compute Internet Checksum for count bytes beginning at location addr     =
//=============================================================================
short checksum(byte *addr, int count)
{
  register int sum = 0;

  // Main summing loop
  while(count > 1)
  {
    sum = sum + *((word16 *) addr);
    addr+=2;
    count = count - 2;
  }

  // Add left-over byte, if any
  if (count > 0)
    sum = sum + *((byte *) addr);

  // Fold 32-bit sum to 16 bits
  while (sum>>16)
    sum = (sum & 0xFFFF) + (sum >> 16);

  return(~sum);
}
