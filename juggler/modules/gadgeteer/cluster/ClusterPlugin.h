/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _CLUSTER_PLUGIN_H
#define _CLUSTER_PLUGIN_H

#include <gadget/gadgetConfig.h>

#include <jccl/RTRC/ConfigChunkHandler.h>
#include <jccl/Config/ConfigChunkPtr.h>
#include <string> 

namespace vpr
{
   class GUID;
}

namespace cluster
{
   class Packet;
   class ClusterNode;

class ClusterPlugin : public jccl::ConfigChunkHandler
{
public:
   ClusterPlugin();
   virtual ~ClusterPlugin();

   /**
    * Get the GUID associated with this plugin.
    */
   virtual vpr::GUID getPluginGUID() = 0;

   /**
    * Handle a incoming packet.
    */
   virtual void handlePacket(Packet* packet, ClusterNode* node) = 0;
   
   virtual void load();
   void setActive(bool active);
   bool isActive();

   
   virtual void preDraw() = 0;
   virtual void postPostFrame() = 0;
   virtual std::string getManagerName() = 0;
   
   virtual bool createBarrier()
   {
      return false;
   }
   
   virtual bool isPluginReady() = 0;
   virtual bool configCanHandle(jccl::ConfigChunkPtr chunk) = 0;
   virtual bool configAdd(jccl::ConfigChunkPtr chunk) = 0;
   virtual bool configRemove(jccl::ConfigChunkPtr chunk) = 0;

private:   
   bool  mActive;
};

} // end namespace gadget

#endif
