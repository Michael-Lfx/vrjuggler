// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.18 of RCSfile: class_cxx_adapter.tmpl,v


#ifndef __GADGET_TYPEDPROXY_GADGET__DIGITAL_ADAPTER__
#define __GADGET_TYPEDPROXY_GADGET__DIGITAL_ADAPTER__

#include "sharppy.h"
#include <gadget/Type/Digital.h>
#include <gadget/Type/Proxy.h>

class SHARPPY_API gadget_TypedProxy_gadget__Digital_Adapter : public gadget::TypedProxy<gadget::Digital>
{
public:
   gadget_TypedProxy_gadget__Digital_Adapter()
      : gadget::TypedProxy<gadget::Digital>()
   {;}

   virtual ~gadget_TypedProxy_gadget__Digital_Adapter()
   {;}

   typedef bool (*config_callback_jccl_ConfigElementPtr_t)(jccl::ConfigElementPtr*);
   config_callback_jccl_ConfigElementPtr_t config_callback_jccl_ConfigElementPtr;

   /// Override for virtual function gadget::Proxy::config.
   virtual bool config(jccl::ConfigElementPtr p0)
   {
      jccl::ConfigElementPtr* p_p0 = new jccl::ConfigElementPtr(p0);
      bool result = config_callback_jccl_ConfigElementPtr(p_p0);
      return result;
   }

   typedef void (*updateData_callback_t)();
   updateData_callback_t updateData_callback;

   /// Override for virtual function gadget::Proxy::updateData.
   virtual void updateData()
   {
      updateData_callback();
   }

   typedef gadget::Input* (*getProxiedInputDevice_callback_t)();
   getProxiedInputDevice_callback_t getProxiedInputDevice_callback;

   /// Override for virtual function gadget::Proxy::getProxiedInputDevice.
   virtual gadget::Input* getProxiedInputDevice()
   {
      gadget::Input* result = getProxiedInputDevice_callback();
      return result;
   }

   typedef bool (*isStupified_callback_t)();
   isStupified_callback_t isStupified_callback;

   /// Override for virtual function gadget::Proxy::isStupified.
   virtual bool isStupified() const
   {
      bool result = isStupified_callback();
      return result;
   }

   typedef vpr::Interval* (*getTimeStamp_callback_t)();
   getTimeStamp_callback_t getTimeStamp_callback;

   /// Override for virtual function gadget::Proxy::getTimeStamp.
   virtual vpr::Interval getTimeStamp() const
   {
      vpr::Interval result = *(getTimeStamp_callback());
      return result;
   }

   typedef bool (*refresh_callback_t)();
   refresh_callback_t refresh_callback;

   /// Override for virtual function gadget::TypedProxy<gadget::Digital>::refresh.
   virtual bool refresh()
   {
      bool result = refresh_callback();
      return result;
   }

   typedef char* (*getDeviceName_callback_t)();
   getDeviceName_callback_t getDeviceName_callback;

   /// Override for virtual function gadget::TypedProxy<gadget::Digital>::getDeviceName.
   virtual std::basic_string<char,std::char_traits<char>,std::allocator<char> > getDeviceName() const
   {
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > result = getDeviceName_callback();
      return result;
   }

}; // class gadget_TypedProxy_gadget__Digital_Adapter




#endif
