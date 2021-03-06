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

#ifndef _INVENTORY_H_
#define _INVENTORY_H_

#include "user.h"

struct openInventory
{
  sint8 type;
  sint32 x;
  sint32 y;
  sint32 z;
  Item workbench[10];
  std::vector<User *> users;  
};

enum { WINDOW_CURSOR = -1, WINDOW_PLAYER = 0, WINDOW_WORKBENCH, WINDOW_CHEST, WINDOW_LARGE_CHEST, WINDOW_FURNACE };

enum { INVENTORYTYPE_CHEST = 0,INVENTORYTYPE_WORKBENCH, INVENTORYTYPE_FURNACE };

class Inventory
{
public:

  struct Recipe
  {
    sint8 width;
    sint8 height;
    sint16 *slots;
    Item output;
  };
  
  std::vector<Recipe*> recipes;

  Inventory();

  ~Inventory()
  {
    for(uint32 i = 0; i < recipes.size(); i++)
    {
      delete [] recipes[i]->slots;
      delete recipes[i];
    }
  }

  //Open chest/workbench/furnace inventories
  std::vector<openInventory *> openWorkbenches;
  std::vector<openInventory *> openChests;
  std::vector<openInventory *> openFurnaces;

  bool onwindowOpen(User *user,sint8 type, sint32 x, sint32 y, sint32 z);
  bool onwindowClose(User *user,sint8 type,sint32 x, sint32 y, sint32 z);


  bool windowOpen(User *user, sint8 type, sint32 x, sint32 y, sint32 z);

  bool windowClick(User *user,sint8 windowID, sint16 slot, sint8 rightClick, sint16 actionNumber, sint16 itemID, sint8 itemCount,sint8 itemUses);

  bool windowClose(User *user,sint8 windowID);
    
  //Check inventory for space
  bool isSpace(User *user, sint16 itemID, char count);

  //Add items to inventory (pickups)
  bool addItems(User *user, sint16 itemID, char count, sint16 health);

  bool doCraft(Item *slots, sint8 width, sint8 height);

  bool setSlot(User *user, sint8 windowID, sint16 slot, sint16 itemID, sint8 count, sint16 health);

  sint16 itemHealth(sint16 itemID, sint8 block, bool &rightUse);

};


#endif