// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <vpr/IO/ObjectWriter.h>
#include <vpr_ObjectWriter_Adapter.h>

extern "C"
{
   SHARPPY_API void delete_vpr_ObjectWriter(vpr_ObjectWriter_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method vpr::ObjectWriter::isBinary()
   SHARPPY_API bool vpr_ObjectWriter_isBinary__0(vpr_ObjectWriter_Adapter* self_)
   {
      bool result;
      result = self_->isBinary();
      return result;
   }


   // Wrapper for virtual method vpr::ObjectWriter::beginTag()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectWriter_beginTag__std_string1(vpr_ObjectWriter_Adapter* self_, char* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->beginTag(p0));
      return result;
   }


   // Wrapper for virtual method vpr::ObjectWriter::endTag()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectWriter_endTag__0(vpr_ObjectWriter_Adapter* self_)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->endTag());
      return result;
   }


   // Wrapper for virtual method vpr::ObjectWriter::beginAttribute()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectWriter_beginAttribute__std_string1(vpr_ObjectWriter_Adapter* self_, char* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->beginAttribute(p0));
      return result;
   }


   // Wrapper for virtual method vpr::ObjectWriter::endAttribute()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectWriter_endAttribute__0(vpr_ObjectWriter_Adapter* self_)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->endAttribute());
      return result;
   }


   // Wrapper for virtual method vpr::ObjectWriter::writeUint8()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectWriter_writeUint8__vpr_Uint81(vpr_ObjectWriter_Adapter* self_, vpr::Uint8 p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->writeUint8(p0));
      return result;
   }


   // Wrapper for virtual method vpr::ObjectWriter::writeUint16()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectWriter_writeUint16__vpr_Uint161(vpr_ObjectWriter_Adapter* self_, vpr::Uint16 p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->writeUint16(p0));
      return result;
   }


   // Wrapper for virtual method vpr::ObjectWriter::writeUint32()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectWriter_writeUint32__vpr_Uint321(vpr_ObjectWriter_Adapter* self_, vpr::Uint32 p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->writeUint32(p0));
      return result;
   }


   // Wrapper for virtual method vpr::ObjectWriter::writeUint64()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectWriter_writeUint64__vpr_Uint641(vpr_ObjectWriter_Adapter* self_, vpr::Uint64 p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->writeUint64(p0));
      return result;
   }


   // Wrapper for virtual method vpr::ObjectWriter::writeFloat()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectWriter_writeFloat__float1(vpr_ObjectWriter_Adapter* self_, float p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->writeFloat(p0));
      return result;
   }


   // Wrapper for virtual method vpr::ObjectWriter::writeDouble()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectWriter_writeDouble__double1(vpr_ObjectWriter_Adapter* self_, double p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->writeDouble(p0));
      return result;
   }


   // Wrapper for virtual method vpr::ObjectWriter::writeString()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectWriter_writeString__std_string1(vpr_ObjectWriter_Adapter* self_, char* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->writeString(p0));
      return result;
   }


   // Wrapper for virtual method vpr::ObjectWriter::writeBool()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectWriter_writeBool__bool1(vpr_ObjectWriter_Adapter* self_, bool p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->writeBool(p0));
      return result;
   }


} // extern "C" for vpr::ObjectWriter

