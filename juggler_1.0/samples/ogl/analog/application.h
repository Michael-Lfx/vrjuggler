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

#ifndef _ANALOG_APP_
#define _ANALOG_APP_

// C++ and STL (standard template library - http://www.sgi.com/Technology/STL)
#include <iostream> // for std::cout
#include <vector>   // for std::vector

// C
#include <math.h>

// OpenGL
#include <GL/gl.h>  // OpenGL API
#include <GL/glu.h> // GLU functions (extended opengl stuff)

// VR juggler
#include <vjConfig.h>          // needed by other juggler classes
#include <Kernel/GL/vjGlApp.h>     // base OpenGL application API
#include <Kernel/GL/vjGlContextData.h> // for OpenGL resource IDs
#include <Utils/vjTimer.h> // so that the cube always spins the same speed

// VR Juggler Input devices:
#include <Input/InputManager/vjAnalogInterface.h>

// texture application objects
#include "renderGeometry.h"  // render interleaved vertex array data 
#include "cubeGeometry.h"    // cube geometry data

//: VR Juggler application demonstration to show you 
//  how to do texturing in an OpenGL juggler application
class AnalogDemoApplication : public vjGlApp
{
public:
   //: Constructor
   AnalogDemoApplication( vjKernel* kern );

   //: destructor
   virtual ~AnalogDemoApplication();

   //: Called immediately upon opening a new OpenGL context
   // (called for every window that is opened)
   // put your opengl resource allocation here...
   virtual void contextInit();
   
   //: Called immediately upon closing an OpenGL context 
   // (called for every window that is closed)
   // put your opengl deallocation here...
   virtual void contextClose();

   //: put vrj device inits here
   virtual void init();
   
   //: Function to "draw" the scene 
   //  put your opengl draw functions here...
   //  PRE: OpenGL state has correct transformation and buffer selected
   //  POST: The current scene has been drawn
   virtual void draw();
   
   // Function called before updating trackers but after the frame is drawn
   // do calculations here...
   virtual void postFrame();

// private stuff!
private:
   float x;

   // use the type: vjGlContextData<ResourceID>
   // to store your OpenGL texture object and display list IDs
   // i.e. vjGlContextData<ResourceID>  mTexObj;
   class ResourceID
   {
   public:
      ResourceID() : id( -1 )
      {
      }
      int id;
   };
   
   cubeGeometry                  mCubeGeometry;
   vjGlContextData<ResourceID>   mCubeDisplayList;

   // interface to analog input.
   vjAnalogInterface             mAnalog0;

   // timer
   vjTimer timer;
};   


#endif
