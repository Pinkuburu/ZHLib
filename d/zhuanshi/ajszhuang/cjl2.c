// by yuchang
inherit ROOM; 
#include <ansi.h> 
#include <room.h>

void create()
{ 
      set("short", "藏剑庐密道");
      set("long",
"这里是傲剑山庄藏剑庐的密道内部，光线仍是十分暗淡，\n"
"地上也是十分的潮湿，实在是阴森无比。\n");
      set("exits",([
            "north" : __DIR__"cjl3",
            "south" : __DIR__"cjl",
      ]));     
      setup();
}

int valid_leave(object me, string dir)
{     
        object ob;
    
        if ( ! me->query_temp("reborn_start/ao2") 
            && dir == "north" )   
        {
           if ( objectp(present("ao xiaotian", environment(me))) )
              return -1; 
           ob = new(__DIR__"npc/aoxiaotian");   
           ob->move(environment(me));
           tell_room(environment(me),
                     CYN "\n突然只见一条黑影不知从哪里蹿了出来，"
                     "原来是傲剑山庄右圣傲啸天。\n\n"NOR);          
           return -1;              
        }
       return 1;

}


