# **************** <Tweek heading BEGIN do not edit this line> ****************
# Tweek
# 
# -----------------------------------------------------------------
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
# **************** <Tweek heading END do not edit this line> ******************

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2003 by Iowa State University
#
# Original Authors:
#   Allen Bierbaum, Christopher Just,
#   Patrick Hartling, Kevin Meinert,
#   Carolina Cruz-Neira, Albert Baker
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# ************** <auto-copyright.pl END do not edit this line> ****************

import sys
from omniORB import CORBA, PortableServer
import CosNaming
import tweek

class CorbaService:
   orb = None
   rootContext = None
   localContext = None
   subjectManager = None

   def __init__(self, nsHost, nsPort = 2809, iiopVer = "1.2", subcontextId = ""):
      self.nameServiceHost    = str(nsHost)
      self.nameServicePort    = str(nsPort)
      self.nameServiceIiopVer = str(iiopVer)
      self.nameSubcontext     = str(subcontextId)

      self.nameServiceURI = "corbaloc:iiop:" + str(iiopVer) + "@" + str(nsHost) + ":" + str(nsPort) + "/NameService"
      print self.nameServiceURI

   def initialize(self):
      self.orb = CORBA.ORB_init(sys.argv, CORBA.ORB_ID)

      try:
         self.rootPoa = self.orb.resolve_initial_references("RootPOA")
         self.rootPoa._get_the_POAManager().activate()
      except CosNaming.NamingContext.InvalidName:
         print "Invalid name when looking up Root Portable Object Adapter"
      except PortableServer.POAManager.AdapterInactive:
         print "Inactive adapter when looking up Root Portable Object Adapter"

      try:
         obj = self.orb.string_to_object(self.nameServiceURI)
         print "obj:", obj
         self.rootContext = obj._narrow(CosNaming.NamingContext) 

         if self.rootContext != None:
            name = [CosNaming.NameComponent("tweek", "context")]

            try:
               init_ref = self.rootContext.resolve(name)
               self.localContext = init_ref._narrow(CosNaming.NamingContext)
            except CosNaming.NamingContext.NotFound, ex:
               print "Name not found:", ex.why
         else:
            print "Failed to get root naming context!"
      except CORBA.UserException, ex:
         print "Caught a CORBA.UserException:", ex.why

   def shutdown(self, waitForCompletion):
      if self.orb != None:
         self.rootPoa.destroy(True, waitForCompletion)
         self.orb.shutdown(waitForCompletion)
         self.orb = None

   def __str__(self):
      return self.nameServiceHost + ":" + str(self.nameServicePort) + "/" + self.nameSubcontext

   def getNameServiceURI(self):
      return self.nameServiceURI

   def getLocalContext(self):
      return self.localContext

   def getSubjectManagerList(self):
      subj_mgrs = []

      if self.localContext != None:
         data_size = 100

         list, iter = self.localContext.list(data_size)

         self.addSubjectManagers(list, subj_mgrs)

         # XXX: The following is a mess...  I need to figure out exactly how
         # these NamingContext and BindingList classes work in Python...
         if iter != None:
            have_more = True
            while have_more:
               list, next_iter = iter.next_n(data_size)

               # XXX: What the hell is this?
               if list != 0:
                  self.addSubjectManagers(list, subj_mgrs)
                  iter.destroy()
                  iter = next_iter
               else:
                  have_more = False

            iter.destroy()

      return subj_mgrs

   def addSubjectManagers(self, bindingList, mgrList):
      for binding in bindingList:
         print "binding:", binding

         if CosNaming.ncontext != binding.binding_type:
            if binding.binding_name[0].id.startswith("SubjectManager"):
               name_comp = binding.binding_name

               try:
                  ref = self.localContext.resolve(name_comp)
                  mgr = ref._narrow(tweek.SubjectManager)
                  print mgr

                  try:
                     if not mgr._non_existent():
                        mgrList.append(mgr)
                  except CORBA.TRANSIENT, ex:
                     print "Caught CORBA.TRANSIENT exception when trying to verify reference validity:", ex.why
               except CosNaming.NamingContext.InvalidName, ex:
                  print "Invalid name:", ex.why
               except CosNaming.NamingContext.CannotProceed, ex:
                  print "Cannot proceed:", ex.why
               except CosNaming.NamingContext.NotFound, ex:
                  print "Not found:", ex.why

   def setSubjectManager(self, mgr):
      self.subjectManager = mgr

   def getSubjectManager(self):
      return self.subjectManager

   def registerObject(self, servant, name):
      obj_id = None

      try:
         obj_id = self.rootPoa.activate_object(servant)
      except PortableServer.ServantAlreadyActive, ex:
         print "WARNING: [CorbaService.registerObject()] Servant already active in root POA!"
         print ex.why
      except PortableServer.WrongPolicy, ex:
         print ex.why

      return obj_id

   def unregisterObject(self, servantID):
      if servantID != None:
         try:
            self.rootPoa.deactive_object(servantID)
         except PortableServer.ObjectNotActive, ex:
            print "WARNING: [CorbaService.unregisterObject()] Servant not active in root POA!"
            print ex.why
         except PortableServer.WrongPolicy, ex:
            print ex.why
      else:
         print "WARNING: Tried to unregister empty ID in CorbaService.unregisterObject()!"
