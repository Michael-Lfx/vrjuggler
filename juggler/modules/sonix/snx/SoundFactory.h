
/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * sonix
 *
 * Original Authors:
 *   Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <SNX heading END do not edit this line> ******************/
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




/* Generated by Together */

#ifndef SNXSOUNDFACTORY_H
#define SNXSOUNDFACTORY_H
#include <iostream>
#include <string>
#include "snx/Singleton.h"
#include "snx/SoundImplementation.h"

namespace snx
{

class SoundFactory : public snx::Singleton<SoundFactory>
{
public:

   /**
    * @input name of api to create
    * @output an implementation is returned for you to use (new memory, you delete)
    * @postconditions if apiName is not known, then a stub implementation is returned
    * @semantics factory function used to create an implementation of a sound API 
    */
   void createImplementation( const std::string& apiName,
                              snx::SoundImplementation* &mImplementation );

   // pass valid pointer to a sound implementation with a name it's refered to
   // or... pass NULL to unregister/delete apiName...
   void reg( const std::string& apiName, snx::SoundImplementation* impl )
   {
      if (impl != NULL)
      {
         std::cout<<"[snx]SoundFactory| NOTICE: Registering sound API: "<<apiName<<" ["<<(int)impl<<"]\n"<<std::flush;
         impl->setName( apiName );
         mRegisteredImplementations[apiName] = impl;
      }
      else
      {
         mRegisteredImplementations.erase( apiName );
         std::cout<<"[snx]SoundFactory| NOTICE: UnRegistered sound API: "<<apiName<<" ["<<(int)impl<<"]\n"<<std::flush;
      }      
      
   }
   
   std::map< std::string, snx::SoundImplementation* > mRegisteredImplementations;
private:  
   /** @link dependency */
   /*#  snx::SoundImplementation lnkSoundImplementation; */

   /** @link dependency */
   /*#  snx::StubSoundImplementation lnksnx::StubSoundImplementation; */

   /** @link dependency */
   /*#  snx::AudioWorksSoundImplementation lnksnx::AudioWorksSoundImplementation; */

   /** @link dependency */
   /*#  snx::OpenALSoundImplementation lnksnx::OpenALSoundImplementation; */
};

template <class _type>
class SoundFactoryReg
{
public:
   _type mSoundAPI;
   std::string mNameOfAPI;
   SoundFactoryReg( const std::string& apiName ) : mSoundAPI(), mNameOfAPI( apiName )
   {
      mSoundAPI.setName( mNameOfAPI );
      snx::SoundFactory::instance()->reg( mNameOfAPI, &mSoundAPI );
   }
   virtual ~SoundFactoryReg()
   {
      mSoundAPI.setName( "unavailable/deleted/nomore" );
      snx::SoundFactory::instance()->reg( mNameOfAPI, NULL );
   }   
};

}; // end namespace

#endif //SNXSOUNDFACTORY_H
