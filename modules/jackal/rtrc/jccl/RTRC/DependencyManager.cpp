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


#include <jccl/jcclConfig.h>
#include <jccl/Plugins/ConfigManager/DependencyManager.h>
#include <jccl/Util/Debug.h>
#include <jccl/Config/ConfigChunk.h>


namespace jccl {

// Initialize the singleton ptr
vprSingletonImp(DependencyManager);


    DependencyManager::DependencyManager(): mDepCheckers(), mDefaultChecker() {
        mDepCheckers = std::vector<DepChecker*>(0);
        //debugDump();
    }


    void DependencyManager::registerChecker (DepChecker* checker) {
        vprASSERT(checker != NULL);
        mDepCheckers.push_back(checker);       // Add the checker to the list
        vprDEBUG(jcclDBG_RECONFIGURATION,vprDBG_CONFIG_LVL)
            << "DependencyManager: Registered: "
            << std::setiosflags(std::ios::right) << std::setw(25) 
            << std::setfill(' ') << checker->getCheckerName().c_str() 
            << std::resetiosflags(std::ios::right)
            << "  type: " << typeid(*checker).name() << std::endl
            << vprDEBUG_FLUSH;
        debugDump();
    }


    bool DependencyManager::isSatisfied (ConfigChunkPtr chunk) {
        vprASSERT(NULL != chunk.get());
        DepChecker* checker = findDepChecker(chunk);
        return checker->depSatisfied(chunk);
    }


    void DependencyManager::debugOutDependencies (ConfigChunkPtr chunk,
                                                  int dbg_lvl)
    {
        vprASSERT(NULL != chunk.get());
        DepChecker* checker = findDepChecker(chunk);
        checker->debugOutDependencies(chunk,dbg_lvl);
    }


    DepChecker* DependencyManager::findDepChecker (ConfigChunkPtr chunk)
    {
        vprASSERT(NULL != chunk.get());

        //std::string chunk_type;
        //chunk_type = (std::string)chunk->getType();

        for(unsigned int i=0;i<mDepCheckers.size();i++) {
            // Get next constructor
            DepChecker* checker = mDepCheckers[i];
            vprASSERT(checker != NULL);
            
            if(checker->canHandle(chunk))
                return checker;
        }

        return &mDefaultChecker;
    }


    void DependencyManager::debugDump()
    {
        //vprDEBUG_BEGIN(jcclDBG_RECONFIGURATION, jcclDBG_CONFIG_LVL) << "DepChecker::debugDump\n" << vprDEBUG_FLUSH;
        vprDEBUG_NEXT(jcclDBG_RECONFIGURATION,vprDBG_STATE_LVL) 
            << "----- Current dependency checkers -----\n" << vprDEBUG_FLUSH;
        vprDEBUG_NEXT(jcclDBG_RECONFIGURATION,vprDBG_STATE_LVL) 
            << "num checkers:" << mDepCheckers.size() << "\n" 
            << vprDEBUG_FLUSH;
        vprDEBUG_NEXT(jcclDBG_RECONFIGURATION,vprDBG_STATE_LVL) 
            << "-1: Checker: default   type: default checker  recog: all (this is a fallback)\n" 
            << vprDEBUG_FLUSH;

        for(unsigned int cNum=0;cNum<mDepCheckers.size();cNum++) {
            DepChecker* checker = mDepCheckers[cNum];
            vprDEBUG_NEXT(jcclDBG_RECONFIGURATION,vprDBG_STATE_LVL)
                << cNum << ": Checker:" << (void*)checker
                << "   type:" << typeid(*checker).name()
                << "   recog:" << checker->getCheckerName().c_str() 
                << "\n" << vprDEBUG_FLUSH;
        }

        vprDEBUG_END(jcclDBG_RECONFIGURATION,vprDBG_STATE_LVL) 
            << "---------------------\n" << vprDEBUG_FLUSH;
    }


}; // namespace jccl
