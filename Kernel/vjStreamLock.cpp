/*
 *  File:	    $Name$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */



#include <Kernel/vjStreamLock.h>


ostream& operator<<(ostream& s, const vjStreamLock& streamLock)
{
   streamLock.mMutex.acquire();
   
   return s;
}


ostream& operator<<(ostream& s, const vjStreamUnLock& streamUnLock)
{
   streamUnLock.mMutex.release();
   
   return s;
}
