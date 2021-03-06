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

#ifndef _PLUGIN_API_H
#define _PLUGIN_API_H

#include <string>
#include <map>
#include <vector>
#include <ctime>

#ifdef WIN32
#define PLUGIN_API_EXPORT extern "C" __declspec(dllexport)
#else
#define PLUGIN_API_EXPORT extern "C"
#endif

struct position_struct
{
  double x;
  double y;
  double z;
  double stance;
  float yaw;
  float pitch;
};

struct plugin_pointer_struct
{
  float (*getPluginVersion)(const std::string name);
  void (*setPluginVersion)(const std::string name, float version);
  void *temp[10];
};

struct user_pointer_struct
{
  bool (*teleport)(std::string user,double x, double y, double z);
  position_struct* (*getPosition)(std::string user);
  void *temp[10];
};

struct chat_pointer_struct
{
  bool (*sendmsgTo)(std::string user,std::string msg);
  bool   (*sendmsg)(std::string msg);
  void *temp[10];
};

struct screen_pointer_struct
{
  void (*log)(std::string message);
  void *temp[10];
};


struct map_pointer_struct
{
  void (*createPickupSpawn)(int x, int y, int z, int type, int count, int health, std::string user);
  bool (*setTime)(std::string timeValue);
  void (*getSpawn)(int* x, int* y, int* z);
  void *temp[10];
};

struct callback_pointer_struct
{
  bool (*add_hook)(std::string name, void *function);
  void *temp[20];
};

struct mineserver_pointer_struct
{
  map_pointer_struct map;
  screen_pointer_struct screen;
  chat_pointer_struct chat;
  plugin_pointer_struct plugin;
  user_pointer_struct user;
  callback_pointer_struct callback;

  void *temp[10];
};


#ifdef MINESERVER
bool plugin_api_chatpre_callback(std::string user, std::string msg);
void init_plugin_api(void);
extern mineserver_pointer_struct plugin_api_pointers;
#endif


#endif
