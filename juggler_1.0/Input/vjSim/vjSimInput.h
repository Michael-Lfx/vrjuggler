/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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


#ifndef _VJ_SIM_INPUT_H
#define _VJ_SIM_INPUT_H
//#pragma once


#include <vjConfig.h>
#include <Input/InputManager/vjKeyboardInterface.h>
#include <Config/vjVarValue.h>

class vjConfigChunk;

//: Base class for all simulated input devices
//
// Has the common routines for accessing and seting
// up keyboard interface
//
//!PUBLIC_API:
class vjSimInput
{
protected:
   class vjKeyModPair
   {
   public:
      vjKeyModPair() : mKey(-1), mModifier(-1)
      {;}

      vjKeyModPair (const vjKeyModPair& in) {
         copy(in);
      }

      inline const vjKeyModPair& operator= (const vjKeyModPair& in) {
         copy(in);
         return *this;
      }

      inline void copy (const vjKeyModPair& in) {
         mKey      = in.mKey;
         mModifier = in.mModifier;
      }

   public:
      int mKey;
      int mModifier;
   };

public:
   vjSimInput()
   {
      //vjDEBUG(vjDBG_ALL,4)<<"*** vjSimInput::vjSimInput()\n"<< vjDEBUG_FLUSH;
   }
   virtual ~vjSimInput() {}

   //: Configure the simulated input device
   //! POST: Keyboard proxy is configured <br>
   virtual bool config(vjConfigChunk* chunk);

protected:
   //: Check the given keypair
   //! RETURNS: Number of times the key was pressed
   //           with the modifier alone held down
   virtual int  checkKeyPair(vjKeyModPair& pair);

   //: Get number of time key pressed
   //! RETURNS: Number of time the key is pressed
   virtual int   checkKey(int keyId);

   //: Constructs a vector of key mod pairs
   // Takes as input a chunk that has a list of KeyModPair embeded chunks
   //! PRE: keyList must be full of var values containing chunks of the type "KeyModPair"
   //+      The KeyModPair chunk type must have fields name key and modKey
   //! RETURNS: vector of KeyModPairs
   std::vector<vjKeyModPair> readKeyList(std::vector<vjVarValue*>& keyList);

private:
   vjKeyboardInterface     mKeyboard;        //: The keyboard we are getting events from
};

#endif