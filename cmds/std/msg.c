/***********************中华短消息小系统***************
 可用于在线/离线发消息，十分方便
                           Create by Rcwiz for HERO
****************************************************/
// msg.c

inherit F_CLEAN_UP;
#include <ansi.h>
#include <localtime.h>

#define PLAYER_MAX_MSG          20
#define WIZ_MAX_MSG             30
#define MEMBER_D                "/adm/daemons/memberd"

int help(object me);

int send_msg(object me, string tar, string msg);
int read_msg(object me, mixed id);
int discard_msg(object me, string id);

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
        object tar;
        string para, id;
        int i, num;;
        
        if (! arg)
              return help(me);
        
        if (sscanf(arg, "%s %s", para, id) != 2)
              return help(me);

        switch(para)
        {
              case "send":

                 tar = UPDATE_D->global_find_player(id);

                 if (! objectp(tar))
                          return notify_fail("没有这个玩家。\n");

                 UPDATE_D->global_destruct_player(tar, 1);

                 if (! wizardp(me) 
                     && ! MEMBER_D->is_valib_member(me->query("id"))
                     && ! me->query("can_send_msg")
                     && id != "rcwiz")
                          return notify_fail("只有会员才能发送短消息。\n");

                 write(HIG "请输入短消息内容（长度不超过500个汉字）。\n" NOR);

                 me->edit((: call_other, __FILE__, "done", me, id :), me);

                 break;
              
              case "read":

                 read_msg(me, id);

                 break;                  

              case "discard":
                 
                 discard_msg(me, id);

                 break;
 
              case "snoop":
        
                 //snoop_msg(me, id);

                 break;

              default:

                   write("指令格式 : msg <send id | read 短消息编号> \n");
                  
                   break;
        }

        return 1;
}

void done(object me, string tar, string msg)
{

        if (! me || ! msg || msg == "")
                return;

        if (! wizardp(me) && time() - me->query_temp("last_send_msg") < 60)
        {
                tell_object(me, HIC "你在一分钟内只能发送一条短消息。\n" NOR);
                return;
        }

        me->set_temp("last_send_msg", time());

        if (! send_msg(me, tar, msg))
        {       
                tell_object(me, HIR "【中华信使】：发送短消息失败。\n" NOR);
                return;
                
        } else
                tell_object(me, HIC "【中华信使】：你的短消息已经成功发送！\n" NOR);

        return;

}

int send_msg(object me, string tar, string msg)
{
         int num_tail, msg_all, t;
         string tmp_num_tail;
         object ob;
         mapping message;
         mixed lt;

         ob = UPDATE_D->global_find_player(tar);
         message = ob->query("message");
         msg_all = sizeof(message);
         
         if (! wizardp(ob) && msg_all >= PLAYER_MAX_MSG)
         {
               write("对象玩家短消息已满！\n");
               return 0;
         }

         if (wizardp(ob) && msg_all >= WIZ_MAX_MSG)
         {
               write("该巫师短消息已满！\n");
               return 0;
         }

         //if (! ob->query("evn/receive_msg")) --->禁止接收

         // 检查长度
         if (sizeof(msg) > 1000)
         {
              write("短消息过长，最长不能大于1000个字节。\n");
              return 0;
         }
         write(sizeof(msg) + " bytes.\n");

         t = time();
         lt = localtime(t);

         msg_all += 1;
         ob->set("message/msg" + msg_all + "/is_new", 1);
         ob->set("message/msg" + msg_all + "/sender", me->name() + "(" + me->query("id") + ")");
         ob->set("message/msg" + msg_all + "/time", sprintf("%d-%02d-%02d %02d:%02d",
                                                    lt[LT_YEAR], 
                                                    lt[LT_MON] + 1,
                                                    lt[LT_MDAY],
                                                    lt[LT_HOUR],
                                                    lt[LT_MIN])
         );

         ob->set("message/msg" + msg_all + "/content", msg);
          
         ob->save();
         UPDATE_D->global_destruct_player(ob, 1);

         return 1; 
}

int read_msg(object me, mixed id)
{
        int num, msg_all;
        int i, count;
        string tmp, tmp_num;
        
        if (stringp(id))sscanf(id, "%d", num);
        if (intp(id)) num = id;

        msg_all = sizeof(me->query("message"));
       
        if (me->is_busy() || me->is_fighting())
        {
              write("等你忙完了再说吧！\n");
              return 1;
        }

        // 读取所有短消息
        if (id == "all")
        {
              count = 0;
              for (i = 1; i <= sizeof(me->query("message")); i ++)
                        if (me->query("message/msg" + i + "/is_new"))
                                  count ++;
              if (count)write(HIY "\n您有 " HIG + chinese_number(count) + HIY " 条新消息。"
                             "请使用 " HIG "msg read new" HIY " 来阅读。\n" NOR);
              else write(HIY "\n您没有收到新消息。\n" NOR);
              write(HIY "≡———————————————————————————————————≡\n" NOR);
              if (msg_all == 0)
              {
                      write(HIG "无短消息！\n" NOR);
                      write(HIY "≡———————————————————————————————————≡\n" NOR);
                      return 1;
              }

              for (i = 1; i <= msg_all; i ++)
              {               
                      tmp = " ";
                      tmp += "『" + i + "』\t";
                      tmp += me->query("message/msg" + i + "/time") + "\t";
                      tmp += "By： " + me->query("message/msg" + i + "/sender") + "\t";
                      if (me->query("message/msg" + i + "/is_new"))
                             write(BBLU + HIW + tmp + "\n" NOR);
                      else write(HIC + tmp + "\n" NOR);                    

              }
              write(HIY "≡———————————————————————————————————≡\n" NOR);             
              return 1;

        } else 
        // 读取新短消息
        if (id == "new")
        {

              if (msg_all == 0)
              {
                      write(HIY "≡———————————————————————————————————≡\n" NOR);
                      write("无短消息！\n");
                      write(HIY "≡———————————————————————————————————≡\n" NOR);
                      return 1;
              }

              for (i = 1; i <= msg_all; i ++)
              {               
                      if (me->query("message/msg" + i + "/is_new"))
                      {
                              read_msg(me, i);
                              break;         
                      }                      
              }
              
              if (i - 1 == msg_all)write("无新消息。\n");

              return 1;

        } else
        // 读取指定短消息  
        if (num <= msg_all && num > 0)
        {          
              write(HIY "≡———————————————————————————————————≡\n" NOR);   
              if (msg_all == 0)
              {
                      write("无短消息！\n");
                      write(HIY "≡———————————————————————————————————≡\n" NOR);
                      return 1;
              }
              
              write(NOR + WHT "\tBy：\t" + me->query("message/msg" + num + "/sender") + "\t" NOR);
              write(NOR + WHT + me->query("message/msg" + num + "/time") + "\n" NOR);
              write(NOR + WHT "↓-----------------------------------------------------------------------↓\n" NOR);
              tmp = NOR + WHT + me->query("message/msg" + num + "/content") + "\n\n" NOR;
              write("\t" + sort_msg(tmp));
              write(NOR + WHT "↑----------------------THE END------------------------------------------↑\n" NOR);
              write(HIY "≡———————————————————————————————————≡\n" NOR);
              me->set("message/msg" + num + "/is_new", 0);
              return 1;
        }
        
        else 
              return help(me);

}

int discard_msg(object me, string id)
{
        int num, msg_all;
        int i;
        string *msgs;
        mapping mmsgs;
        
        sscanf(id, "%d", num);
        
        msg_all = sizeof(me->query("message"));       

        // 删除所有短消息
        if (id == "all")
        {
              if (msg_all == 0)
              {
                      write("无短消息！\n");
                      return 1;
              }
        
              me->delete("message");
              write("OK.\n");
              return 1;
        } else         
        if (num <= msg_all && num > 0)
        {
              if (msg_all == 1)
              {
              	  me->delete("message");
              	  write("OK.\n");
              	  return 1;
              }
              
              me->delete("message/msg" + num);
              
              // 删除最后一个不需要重组序列
              if (msg_all == num){write("OK.\n");return 1;}
              
              // 重组序列              
              msg_all = sizeof(me->query("message"));
              
              mmsgs = me->query("message");  
              msgs = keys(mmsgs);

              me->delete("message");
              for (i = 1; i <= sizeof(msgs); i ++)
              {
              	     me->set("message/msg" + i + "/sender",
              	             mmsgs[msgs[i - 1]]["sender"]);
              	     me->set("message/msg" + i + "/time",
              	             mmsgs[msgs[i - 1]]["time"]);
              	     me->set("message/msg" + i + "/content",
              	             mmsgs[msgs[i - 1]]["content"]);  
              	     me->set("message/msg" + i + "/is_new",
              	             mmsgs[msgs[i - 1]]["is_new"]);              	                         	                           	             
              	          
              }

              write("OK.\n");
              return 1;
        }
        
        else 
              return help(me);

}

int help(object me)
{
        write(@HELP
指令格式 : 
           msg <send [id | -multiuser]>      发送短消息到指定/多个玩家。
           msg <read [all | new | 编号]>     读取所有/新/指定短消息。
           msg <discard [all | 编号]>        删除所有/指定短消息。

这个指令可以发送或管理你的短消息。
发送的短消息即使对象玩家离线也可以收到。

其中msg read all中高亮显示的为新消息。

HELP );
        return 1;
}
