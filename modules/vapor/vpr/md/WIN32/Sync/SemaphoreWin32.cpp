/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>

#include <sstream>
#include <assert.h>

#include <vpr/Util/ResourceException.h>
#include <vpr/Util/Error.h>
#include <vpr/md/WIN32/Sync/SemaphoreWin32.h>


namespace vpr
{

SemaphoreWin32::SemaphoreWin32(const int initialValue)
{
   // ----- Allocate the semaphore ----- //
   mSema = CreateSemaphore(NULL, initialValue, 99, NULL);

   if ( mSema == NULL || mSema == INVALID_HANDLE_VALUE )
   {
      std::ostringstream msg_stream;
      msg_stream << "Semaphore allocation failed: "
                 << vpr::Error::getCurrentErrorMsg();
      throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
   }
}

SemaphoreWin32::~SemaphoreWin32()
{
   // ---- Delete the semaphore --- //
   const int result = CloseHandle(mSema);
   assert(result);
}

void SemaphoreWin32::reset(const int val)
{
   const BOOL result = CloseHandle(mSema);
   assert(result);

   mSema = CreateSemaphore(NULL, val, 99, NULL);

   if ( mSema == NULL || mSema == INVALID_HANDLE_VALUE )
   {
      std::ostringstream msg_stream;
      msg_stream << "Semaphore re-initialization failed: "
                 << vpr::Error::getCurrentErrorMsg();
      throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
   }
}

} // End of vpr namespace
