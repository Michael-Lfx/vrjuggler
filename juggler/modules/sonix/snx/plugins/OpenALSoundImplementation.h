
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

#ifndef SNXOPENALSOUNDIMPLEMENTATION_H
#define SNXOPENALSOUNDIMPLEMENTATION_H

#include <map>
#include <string>
#include <vector>
#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/VecOps.h>
#include <gmtl/Xforms.h>

#include "snx/SoundImplementation.h"
#include "snx/SoundInfo.h"
#include "snx/SoundAPIInfo.h"

#include <AL/al.h>
#include <AL/alc.h>

namespace snx
{

class OpenALSoundImplementation : public snx::SoundImplementation
{
public:
    
   /**
    * constructor for the OpenAL implementation 
    */
   OpenALSoundImplementation();

   /**
    * destructor for the OpenAL implementation
    */
   virtual ~OpenALSoundImplementation();

   /**
     * every implementation can return a new copy of itself
     */
   virtual void clone( snx::SoundImplementation* &newCopy )
   {
      newCopy = new OpenALSoundImplementation;
      
      // copy state, so that we return a true "clone"
      newCopy->copy( *this );
   }
   
   /**
    * @input alias of the sound to trigger, and number of times to play
    * @preconditions alias does not have to be associated with a loaded sound.
    * @postconditions if it is, then the loaded sound is triggered.  if it isn't then nothing happens.
    * @semantics Triggers a sound
    */
   virtual void trigger( const std::string& alias, const unsigned int& looping = 0);

   /*
    * when sound is already playing then you call trigger,
    * does the sound restart from beginning?
    * (if a tree falls and no one is around to hear it, does it make sound?)
    */
   virtual void setRetriggerable( const std::string& alias, bool onOff )
   {
      snx::SoundImplementation::setRetriggerable( alias, onOff );
   }

   /**
    * ambient or positional sound.
    * is the sound ambient - attached to the listener, doesn't change volume
    * when listener moves...
    * or is the sound positional - changes volume as listener nears or retreats..
    */
   virtual void setAmbient( const std::string& alias, bool setting = false )
   {
   }

   /**
     * is the sound currently playing?
     */
   bool isPlaying( const std::string& alias );
  
   /**
    * @semantics stop the sound
    * @input alias of the sound to be stopped
    */
   virtual void stop( const std::string& alias );

   /**
    * set sound's 3D position 
    */
   virtual void setPosition( const std::string& alias, float x, float y, float z );

   /**
    * get sound's 3D position
    * @input alias is a name that has been associate()d with some sound data
    * @output x,y,z are returned in OpenGL coordinates.
    */
   virtual void getPosition( const std::string& alias, float& x, float& y, float& z );
   
   /**
    * set the position of the listener
    */
   virtual void setListenerPosition( const gmtl::Matrix44f& mat );

   /**
    * get the position of the listener
    */
   virtual void getListenerPosition( gmtl::Matrix44f& mat );
   
public:
   /**
    * start the sound API, creating any contexts or other configurations at startup
    * @postconditions sound API is ready to go.
    * @semantics this function should be called before using the other functions in the class.
    */
   virtual void startAPI();

   /*
    * configure the sound API global settings
    */
   
   /**
    * kill the sound API, deallocating any sounds, etc...
    * @postconditions sound API is ready to go.
    * @semantics this function could be called any time, the function could be called multiple times, so it should be smart.
    */
   virtual void shutdownAPI();

   /**
     * query whether the API has been started or not
     * @semantics return true if api has been started, false otherwise.
     */
   virtual bool isStarted() const
   {
      return mDev != NULL && mContextId != NULL;
   }

   virtual void configure( const snx::SoundAPIInfo& sai )
   {
      snx::SoundImplementation::configure( sai );
      // TODO: configure the engine based on the settings!!
   }

   /**
     * configure/reconfigure a sound
     * configure: associate a name (alias) to the description if not already done
     * reconfigure: change properties of the sound to the descriptino provided.
     * @preconditions provide an alias and a SoundInfo which describes the sound
     * @postconditions alias will point to loaded sound data
     * @semantics associate an alias to sound data.  later this alias can be used to operate on this sound data.
     */
   virtual void configure( const std::string& alias, const snx::SoundInfo& description );

   /**
     * remove a configured sound, any future reference to the alias will not
     * cause an error, but will not result in a rendered sound
     */
   virtual void remove( const std::string alias );

   /**
    * clear all associate()tions.
    * @semantics any existing aliases will be stubbed. sounds will be unbind()ed
    */
   virtual void clear();
   
   /**
    * load/allocate the sound data this alias refers to the sound API
    * @postconditions the sound API has the sound buffered.
    */
   virtual void bind( const std::string& alias );

   /**
    * unload/deallocate the sound data this alias refers from the sound API
    * @postconditions the sound API no longer has the sound buffered.
    */
   virtual void unbind( const std::string& alias );

   /**
    * take a time step of [timeElapsed] seconds.
    * @semantics call once per sound frame (doesn't have to be same as your graphics frame)
    * @input time elapsed since last frame
    */
   virtual void step( const float & timeElapsed );

private:
    /** @link dependency */
    /*#  snx::SoundInfo lnkSoundInfo; */
   
   struct AlSoundInfo
   {
      AlSoundInfo() : source( 0 ), buffer( 0 ), data()
      {
      }

      ALuint source, buffer;
      std::vector<unsigned char> data; // TODO: use the source snx::SoundInfo
                                       // instead of this separate copy of the data...
   };
   std::map< std::string, AlSoundInfo > mBindLookup;
   void*       mContextId;
   ALCdevice*  mDev;

   /** @link dependency */
   /*#  snx::SoundAPIInfo lnksnx::SoundAPIInfo; */
};

}; // end namespace

#endif //SNXOPENALSOUNDIMPLEMENTATION_H
