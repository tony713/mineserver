/*
  Copyright (c) 2010, The Mineserver Project
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
  * Neither the name of the The Mineserver Project nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _PLUGIN_H
#define _PLUGIN_H

#include <string>
#include <map>
#include <vector>
#include <ctime>

//Fix Winsock2 error that occurs when Windows.h is included before it.
#define _WINSOCKAPI_

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "delegate/delegate.hpp"
#include "constants.h"
#include "tools.h"
#include "user.h"
#include "hook.h"

// A wild INCONSISTENCY appears!
//
//  --------------
// | >FIGHT  PkMn |
// |  ITEM   RUN  |
//  --------------
//
// deoxxa uses MACRO ATTACK!
//
#ifdef WIN32
#define LIBRARY_HANDLE HINSTANCE
#define LIBRARY_LOAD(x) LoadLibrary(x)
#define LIBRARY_SELF() GetModuleHandle(NULL)
#define LIBRARY_SYMBOL(x, y) GetProcAddress(x, y)
#define LIBRARY_ERROR() "Windows error handling needs work!" // <- NOTE
#define LIBRARY_CLOSE(x) FreeLibrary(x)
#define LIBRARY_EXTENSION ".dll"
#else
#define LIBRARY_HANDLE void*
#define LIBRARY_LOAD(x) dlopen(x, RTLD_LAZY)
#define LIBRARY_SELF() dlopen(NULL, RTLD_LAZY)
#define LIBRARY_SYMBOL(x, y) dlsym(x, y)
#define LIBRARY_ERROR() dlerror()
#define LIBRARY_CLOSE(x) dlclose(x)
#define LIBRARY_EXTENSION ".so"
#endif
//
// It's SUPER EFFECTIVE!
// Foe INCONSISTENCY fainted!
// You got 374¥ for winning!

typedef srutil::delegate6<void, User*, sint8, sint32, sint8, sint32, sint8> Function;

class Callback
{
public:
  void add(const std::string name, Function func)
  {
    remove(name);
    callbacks.insert(std::pair<std::string, Function>(name, func));
  }

  bool remove(const std::string name)
  {
    Events::iterator iter = callbacks.find(name);

    if (iter == callbacks.end())
    {
      return false;
    }
      
    callbacks.erase(iter);
    return true;
  }

  Function* get(const std::string name)
  {
    Events::iterator iter = callbacks.find(name);

    if (iter == callbacks.end())
    {
      return NULL;
    }

    return &iter->second;
  }

  bool run(const std::string name, const Function::invoker_type function)
  {
    Events::iterator iter = callbacks.find(name);

    if (iter == callbacks.end())
    {
      return false;
    }

    function(iter->second);
    return true;
  }

  void reset()
  {
    callbacks.clear();
  }

private:
  typedef std::map<std::string, Function> Events;
  Events callbacks;
};

class Plugin
{
public:
  // Hook registry stuff
  bool hasHook(const std::string name);
  void setHook(const std::string name, Hook* hook);
  Hook* getHook(const std::string name);
  void remHook(const std::string name);
  // Load/Unload plugins
  bool loadPlugin(const std::string name, const std::string file="");
  void unloadPlugin(const std::string name);
  // Plugin version registry
  void setPluginVersion(const std::string name, float version);
  float getPluginVersion(const std::string name);
  void remPluginVersion(const std::string name);
  // Pointer registry stuff
  bool hasPointer(const std::string name);
  void setPointer(const std::string name, void* pointer);
  void* getPointer(const std::string name);
  void remPointer(const std::string name);
  // Create default hooks
  Plugin()
  {
    setHook("Timer200", new Hook0<void>);
    setHook("Timer1000", new Hook0<void>);
    setHook("Timer10000", new Hook0<void>);
    setHook("LoginPre", new Hook2<bool,User*,std::string*>);
    setHook("LoginPost", new Hook1<void,User*>);
    setHook("ChatPre", new Hook3<bool,User*,time_t,std::string>);
    setHook("ChatPost", new Hook3<void,User*,time_t,std::string>);
    setHook("PlayerArmSwing", new Hook1<void,User*>);
    setHook("PlayerDamagePre", new Hook3<bool,User*,User*,int>);
    setHook("PlayerDamagePost", new Hook3<void,User*,User*,int>);
    setHook("PlayerDisconnect", new Hook3<void,User*,uint32,uint16>);
    setHook("PlayerDiggingStarted", new Hook4<void,User*,sint32,sint8,sint32>);
    setHook("PlayerDigging", new Hook4<void,User*,sint32,sint8,sint32>);
    setHook("PlayerDiggingStopped", new Hook4<void,User*,sint32,sint8,sint32>);
    setHook("BlockBreakPre", new Hook4<bool,User*,sint32,sint8,sint32>);
    setHook("BlockBreakPost", new Hook4<void,User*,sint32,sint8,sint32>);
    setHook("BlockNeighbourBreak", new Hook7<void,User*,sint32,sint8,sint32,sint32,sint8,sint32>);
    setHook("BlockPlacePre", new Hook5<bool,User*,sint32,sint8,sint32,sint16>);
    setHook("BlockPlacePost", new Hook5<void,User*,sint32,sint8,sint32,sint16>);
    setHook("BlockNeighbourPlace", new Hook7<void,User*,sint32,sint8,sint32,sint32,sint8,sint32>);
    setHook("BlockReplacePre", new Hook6<void,User*,sint32,sint8,sint32,sint16,sint16>);
    setHook("BlockReplacePost", new Hook6<void,User*,sint32,sint8,sint32,sint16,sint16>);
    setHook("BlockNeighbourReplace", new Hook9<void,User*,sint32,sint8,sint32,sint32,sint8,sint32,sint16,sint16>);
  }
  // Remove existing hooks
  ~Plugin()
  {
    std::map<const std::string,Hook*>::iterator it = m_hooks.begin();
    for (;it!=m_hooks.end();++it)
    {
      delete it->second;
    }
    m_hooks.clear();
  }

  // Old code
  // This needs to be phased over to the new plugin architecture

  void init();
  void free();

  void setBlockCallback(const int type, Callback call);
  Callback* getBlockCallback(const int type);
  bool runBlockCallback(const int type, const std::string name, const Function::invoker_type function);
  bool removeBlockCallback(const int type);

private:
  std::map<const std::string, Hook*> m_hooks;
  std::map<const std::string, LIBRARY_HANDLE> m_libraryHandles;
  std::map<const std::string, void*> m_pointers;
  std::map<const std::string, float> m_pluginVersions;

  // Old stuff
  typedef std::map<sint16, Callback> Callbacks;
  Callbacks blockevents;
};

#endif
