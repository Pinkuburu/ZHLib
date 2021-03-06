// workroom.c 
// Modified by Haiyan 

#include <ansi.h>
#include <room.h> 
inherit ROOM; 

void create() 
{ 
   set("short", "燕窝"); 
   set("long", @LONG
这里是悬崖峭壁上的一个山洞，外面是汹涌的大海——太平洋，
不时见到一条小鱼儿窜出水面，瞪着一双大眼睛东张西望。这里放着
一张桌子(desk)，几把破椅子。桌上有一盏台灯，几包方便面，一本
翻得破旧不堪的笔记本。 
LONG); 
   set("exits", ([ 
           "out"      : "/u/smallfish/workroom",  
           "down"     : "/d/wizard/guest_room", 
      ])); 
   set("objects", ([  
       __DIR__"mishu" : 1,  
      ]));  
//   set("no_fight", 1); 
   set("no_steal", 1); 
   set("no_sleep_room", 1); 
   set("item_desc", ([  
       "desk": "这张桌子似乎可以移开(move)。\n",  
      ]));  
   setup(); 
} 

void init()  
{ 
     add_action("do_move", "move");  
     add_action("do_jingao","jg");
}  
      
void close_path()  
{  
     if (! query("exits/enter")) return;  
     message("vision","桌子又自动移了回来，盖住了通道。\n",  
             this_object());  
             delete("exits/enter");  
}  

int do_move(string arg)  
{  
    if (! arg || arg != "desk" )
       return notify_fail("你要推开什么？\n");  
    else  
    {  
       write("你把桌子推开，露出了一个通道。\n");  
       if (! query("exits/enter"))  
       {  
          set("exits/enter", __DIR__ "hy_houyuan");  
          call_out("close_path", 5);  
       }
       return 1;
    }
}

int valid_leave(object ob, string dir) 
{
    ob = this_player();
    switch(dir)
    {
      case "out":
        if (!wizardp(ob))
           return  notify_fail("芳芳拦住你道：“外面是大海！你想找死啊？”\n");
           return ::valid_leave(ob,dir);
           break;
      case "enter":
        if ((string)ob->query("id") != "haiyan") 
        { 
          if (! ob->query_temp("desk_open") && dir == "enter")  
             return notify_fail("芳芳拦住你道：“那里是海燕的密室，不要乱逛！\n");
          else 
          { 
             ob->delete_temp("desk_open"); 
             return ::valid_leave(ob, dir);  
          } 
        } 
        else  return ::valid_leave(ob, dir);
        break;
      default:
        return ::valid_leave(ob, dir); 
        break;
     }
}

int do_jingao(string arg)
{
     object ob, me = this_player();
     if (wizardp(me))
     {
         if (! arg)   return notify_fail("你要警告谁？\n"); 
         ob = find_player(arg); 
         if (! ob)    return notify_fail("没有这个玩家。\n"); 
         if (ob == me)   return notify_fail("疯了... 你真是疯了！\n"); 

         message("vision", HIR "【严重警告】" + ob->name() + "(" + ob->query("id") + ")" + 
                 "的言行已违反了RULES，现予以警告。如有再犯，将打入第十八层地狱。\n" NOR, 
                 all_interactive()); 
     }
     else   tell_object(me, "你无权使用这个命令。\n");
     return 1;
}

