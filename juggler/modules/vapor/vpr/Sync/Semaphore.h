/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_Semaphore_h_
#define _VPR_Semaphore_h_

#include <vpr/vprConfig.h>

#if VPR_THREAD_DOMAIN_INCLUDE == VPR_DOMAIN_IRIX_SPROC
#   include <ulocks.h>
#   include <vpr/md/SPROC/Sync/SemaphoreSGI.h>
#elif VPR_THREAD_DOMAIN_INCLUDE == VPR_DOMAIN_POSIX
#ifndef _POSIX_C_SOURCE
#   define _POSIX_C_SOURCE VPR_POSIX_C_SOURCE
#endif

#   include <vpr/md/POSIX/Sync/SemaphorePosix.h>
#elif VPR_THREAD_DOMAIN_INCLUDE == VPR_DOMAIN_NSPR
#  include <vpr/md/NSPR/Sync/SemaphoreNSPR.h>
#endif


#endif	/* _VPR_Sempahore_h_ */
