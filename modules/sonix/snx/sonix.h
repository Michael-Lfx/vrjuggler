
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

/* Generated by Together */

#ifndef SONIX___H
#define SONIX___H

#include <snx/snxConfig.h>

#include <stdlib.h>
#include <string>
#include <gmtl/Matrix.h>
#include <vpr/Util/Singleton.h>

#include "snx/SoundInfo.h"
#include "snx/SoundImplementation.h"
#include "snx/SoundAPIInfo.h"

class SNX_CLASS_API sonix : public vpr::Singleton<sonix>
{
protected:
   friend class vpr::Singleton<sonix>;

   //: default constructor
   sonix() : mImplementation( NULL )
   {
   }

   //: virtual destructor
   virtual ~sonix();

public:

   /**
    * @input alias of the sound to trigger, and number of times to play, -1 to 
    *        repeat infinately, 1 (single shot) is default.
    * @preconditions alias does not have to be associated with a loaded sound.
    * @postconditions if it is, then the loaded sound is triggered.  if it isn't then nothing happens.
    * @semantics Triggers a sound
    */
   virtual void trigger( const std::string& alias, const int& repeat = 1 );
   
   /**
     * is the sound currently playing?
     */
   virtual bool isPlaying( const std::string& alias );

   /*
    * when sound is already playing then you call trigger,
    * does the sound restart from beginning?
    */
   virtual void setRetriggerable( const std::string& alias, bool onOff );

   /**
    * is the sound retriggerable?
    */
   virtual bool isRetriggerable( const std::string& alias );

   /**
    * @semantics stop the sound
    * @input alias of the sound to be stopped
    */
   virtual void stop( const std::string& alias );

   /**
    * pause the sound, use unpause to return playback where you left off...
    */
   virtual void pause( const std::string& alias );

   /**
    * resume playback from a paused state.  does nothing if sound was not paused.
    */
   virtual void unpause( const std::string& alias );
   
   /** if the sound is paused, then return true. */
   virtual bool isPaused( const std::string& alias );
   
   /**
    * ambient or positional sound.
    * is the sound ambient - attached to the listener, doesn't change volume
    * when listener moves...
    * or is the sound positional - changes volume as listener nears or retreats..
    */
   virtual void setAmbient( const std::string& alias, bool setting = false );

   /** is the sound ambient? */
   virtual bool isAmbient( const std::string& alias );

   /** alters the frequency of the sample.
    *  1 is no change
    *  < 1 is low
    *  > 1 is high.
    */
   virtual void setPitchBend( const std::string& alias, float amount );

   /** affect volume.  set to a value between [0..1]. */
   virtual void setVolume( const std::string& alias, float amount );
   
   /** affect cutoff.
    *  set to a value between [0..1]... 1 is no change.  0 is total cutoff.
    */
   virtual void setCutoff( const std::string& alias, float amount );
   
   /**
    * set sound's 3D position 
    * @input x,y,z are in OpenGL coordinates.  alias is a name that has been associate()d with some sound data
    */
   virtual void setPosition( const std::string& alias, const float& x, const float& y, const float& z );

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


   /**
    * change the underlying sound API to something else.
    * @input usually a name of a valid registered sound API implementation
    * @preconditions sound implementation should be registered
    * @postconditions underlying API is changed to the requested API name.   if apiName's implementation is not registered, then underlying API is changed to the stub version.
    * @semantics function is safe: always returns a valid implementation.
    * @time O(1)
    * @output a valid sound API.  if apiName is invalid, then a stub implementation is returned.
    */
   virtual void changeAPI( const std::string& apiName );

   /*
    * configure the sound API global settings
    */
   virtual void configure( const snx::SoundAPIInfo& sai );

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
     * @semantics call once per sound frame (doesn't have to be same as your graphics frame)
     * @input time elapsed since last frame
     */
   virtual void step( const float& timeElapsed );
   
protected:
   snx::ISoundImplementation& impl();

private:
   /** @link dependency */
   /*#  snx::SoundFactory lnkSoundFactory; */

   /** @link aggregation 
    * @clientCardinality 1
    * @supplierCardinality 1*/
   snx::ISoundImplementation* mImplementation;
        
   /** sonix API includes objects of this type
    * @link dependency */
   /*#  snx::SoundInfo lnkSoundInfo; */

   /** @link dependency */
   /*#  snx::SoundAPIInfo lnksnx::SoundAPIInfo; */
};

#endif //SONIX___H
