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

#ifndef _CLUSTER_APPLICATION_DATA_H
#define _CLUSTER_APPLICATION_DATA_H

#include <cluster/Plugins/PluginConfig.h>
#include <vpr/IO/SerializableObject.h>

#include <cluster/Plugins/ApplicationDataManager/ApplicationDataManager.h>

namespace cluster
{

class ApplicationData : public vpr::SerializableObject
{
public:
   /**
    * Construct a new ApplicationData object.
    *
    * @param guid - The GUID used to reference this object.
    * @param host_name - The hostname of the node that should be responsible for updating this object.
    */
   ApplicationData(const vpr::GUID& guid, const std::string& host_name) : mIsLocal(false), mId(guid), mHostname(host_name)
   {
      cluster::ApplicationDataManager::instance()->addApplicationData(this);
   }

   virtual ~ApplicationData()
   {
      ;
   }

   /**
    * Returns true if this object should be updated by the local node.
    */
   bool isLocal()
   {
      return mIsLocal;
   }

   /**
    * Set the boolean value that determines if the local node
    * should be responsible for updating this object.
    */
   void setIsLocal(bool local)
   { mIsLocal = local; }
   
   /**
    * Return the GUID for this object.
    */
   vpr::GUID getId()
   {
      return mId;
   }

   /**
    * Return the hostname of the node that should update this object
    */
   std::string getHostname()
   {
      return mHostname;
   }
private:
   bool        mIsLocal;   /**< True if this object is to be updated by the local node. */
   vpr::GUID   mId;        /**< GUID for this object */
   std::string mHostname;  /**< Hostname of the machine that should update this object */
};

} // end namespace gadget

#endif
