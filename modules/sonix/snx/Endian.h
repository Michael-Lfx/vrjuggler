
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




#ifndef AUDIOJUGGLER_ENDIAN_FUNCS
#define AUDIOJUGGLER_ENDIAN_FUNCS


namespace snxEndian
{
   //: Swap the bytes in any data type.
   // Motorola and Intel store their bytes in reversed formats <BR>
   // ex: An SGI image is native to the SGI system,            <BR>
   // to be read on an intel machine, it's bytes need to be reversed <BR>
   // NOTE: chars aren't affected by this since it only        <BR>
   // affects the order of bytes, not bits.
   template< class Type >
   inline void  byteReverse(Type& bytes)
   {    	
      const int size = sizeof(Type);
      Type buf = 0;
      int x, y;

      //we want to index the bytes in the buffer
      unsigned char* buffer = (unsigned char*) &buf;

      for ( x = 0, y = size-1; 
         x < size;
         ++x, --y )
      {
         buffer[x] |= ((unsigned char*)&bytes)[y];
      }
      bytes = buf;
   }

   enum Endianness
   {
      BIG, LITTLE
   };   
      
   template< class Type >
   inline void  byteReverse( Endianness& e, Type& bytes )
   {
      if (e == BIG && snxEndian::isLittle())
         byteReverse( bytes );
      if (e == LITTLE && snxEndian::isBig())
         byteReverse( bytes );
   }   
   
   //: check the system for endianess
   inline bool isLittle() 
   {
      union 
      {
         short   val;
         char    ch[sizeof(short)];
      } un;

      // initialize the memory that we'll probe
      un.val = 256;       // 0x10

      // If the 1 byte is in the low-order position (un.ch[1]), this is a
      // little-endian system.  Otherwise, it is a big-endian system.
      return un.ch[1] == 1;
   }

   //: check the system for endianess
   inline bool isBig()
   {
      return !snxEndian::isLittle();
   }   

}; // end namespace.
#endif
