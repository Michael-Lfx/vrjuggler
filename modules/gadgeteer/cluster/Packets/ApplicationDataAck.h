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

#ifndef _GADGET_USER_DATA_ACK_H
#define _GADGET_USER_DATA_ACK_H

#include <gadget/gadgetConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <cluster/Packets/Header.h>                                                       
#include <cluster/Packets/Packet.h>

namespace cluster
{

class GADGET_CLASS_API ApplicationDataAck : public Packet
{
public:
   /**
    * Create a ApplicationDataAck packet
    *   
    * @param packet_head -Header which has already been received and 
    *                     determined to be for a ApplicationDataAck.
    * @param stream -A SocketStream that we will use to receive the packet data.
    */
   ApplicationDataAck(Header* packet_head, vpr::SocketStream* stream);

   /**
    * Create a ApplicationDataAck packet to acknowledge a ApplicationDataRequest.
    *
    * @param plugin_guid -GUID of the ClusterPlugin that should handle this packet.
    * @param id -GUID of the ApplicationData object that we are acknowledging.
    * @param ack -Boolean determining if this is a positive(ACK) or a negative(NACK) responce.
    */
   ApplicationDataAck(const vpr::GUID& plugin_guid, 
                      const vpr::GUID& id, 
                      const bool ack);
   
   /**
    * Serializes member variables into a data stream.
    */
   void serialize();

   /**
    * Parses the data stream into the local member variables.
    */
   void parse();
   
   /**
    * Print the data to the screen in a readable form.
    */
   virtual void printData(int debug_level);

   /**
    * Return the type of this packet.
    */
   static vpr::Uint16 getBaseType()
   {
       return(Header::RIM_APPDATA_ACK);
   }
   
   /**
    * Return the GUID of the ApplicationData object being acknoledged
    */
   vpr::GUID getId() { return mId; }

   /**
    * Return a boolean determining if this is a positive(ACK) or a negative(NACK) responce.
    */
   bool getAck() { return mAck; }
private:
   vpr::GUID   mId;  /**< GUID for the ApplicationData object being acknoledged. */
   bool        mAck; /**< Boolean determining if this is a positive(ACK) or a negative(NACK) responce. */
};
}

#endif




