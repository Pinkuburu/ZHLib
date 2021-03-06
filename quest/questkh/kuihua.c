#include <ansi.h>
#define QUESTKILL_D    "/quest/questkh/quest/questkill"
#define QUESTKH_D(x)   ("/quest/questkh/quest/questkh" + x)
#define KHMSG_D      ("/quest/questkh/kuihua-msg")

inherit NPC;

void greeting(object);
void init();
int give_quest();
int give_up();
void destory(object);
void npc_dest(object);
string time_period(int timep);

void create()
{
   set_name(HIR"葵花太监"NOR, ({ "dongfang bubai", "dongfang", "bubai" }));
   set("gender", "无性");
   set("age", 52);

   set("str", 20);
   set("dex", 30);
   set("per",19);
    set("con",3000);

   set("long",
"这就是天下武林人物、黑白两道，闻之色变\n"
"的魔教教主。他身着一袭红袄，脸上薄施脂\n"
"粉，竟有三分姿色。左手拿着一个绣花绷架，\n"
"右手持着一枚绣花针，上面是鸳鸯戏水，方\n"
"绣了一半。\n");
   set("combat_exp", 24000000);
   set("shen_type", -1);
   set("attitude", "heroism");
   set_skill("unarmed", 100);
   set_skill("force", 500);
   set_skill("sword", 500);
   set_skill("dodge", 500);
   set_skill("parry", 500);
   set_skill("pixie-jian", 500);
   map_skill("dodge","pixie-jian"); 
   map_skill("parry","pixie-jian");
   map_skill("sword","pixie-jian"); 

   set("inquiry", ([
          "任务" : "东方不败低了头，一针一线，全神贯注，理都不理。”\n",
    ]) );

    set("qi",9900000);
    set("max_qi",9900000);
    set("jing",9900000);
    set("max_jing",9900000);
    set("neili",900000);
    set("max_neili",900000);
    set("jingli",900000);
    set("max_jingli",900000);
   setup();
}

void init()
{
   object ob;

   ::init();

   if( interactive(ob = this_player()) && query("kill_all") )
   {
      call_out("greeting", 2, ob);
   }
   add_action("give_quest", "quest");
   add_action("give_up","giveup");
}

int give_quest()
{
    mapping questtemp,questkh,questkill,skills;
    object ob,npc;
    int exp,time,level,i,lev,lev1;
    string msg,*sname,place;
        
    ob = this_player();
    exp = (int)ob->query("combat_exp");

    if( (ob->query("character") != "阴险奸诈") && (ob->query("character") != "心狠手辣"))  
    {
        message_vision("葵花太监抬头看了$N一眼，尖着嗓子道：“你这号假仁假义的货色，再不马上给我滚开，我让你血溅五步！”\n", ob);
        return 1;
    }
    if( ob->query("shen") >= 0)  
    {
        write("葵花太监头也不抬，轻轻叹了口气，道：“好好一个人，偏要弃暗投明，竟还敢到这里来，真是不想活啦？”\n");
        return 1;
    }
    if(exp < 1000)
    {
        write("葵花太监头也不抬，轻轻道：“你先去练练功夫吧，想为我做事，你还不够呢。”\n");
        return 1;
    }
    if (mapp(ob->query("quest_kh")))
    {
       write("葵花太监把玩着手中古玩，微微点头来，道：“你手头的任务可是完成了？”\n");
        return 0;
    }
    if (mapp(ob->query("quest_ws")))
    {
        ob->delete_temp("questws_times");
        ob->delete("quest_ws");
    }

    if((time()-ob->query_temp("questkh_giveup_time")) < (2+random(2)))
    {
        write("不要急，慢一点嘛。\n");
        return 1;
    }

    if(exp <= 5000)         level = 0;
    else if(exp <= 22000)   level = 1;
    else if(exp <= 72000)   level = 2;
    else if(exp <= 170000)  level = 3;
    else if(exp <= 410000)  level = 4-random(2);
    else if(exp <= 800000)  level = 5-random(3);
    else if(exp <= 1500000) level = 6-random(3);
    else
    {
        if(random(6)==0||(exp>10000000&&random(3)==0))
            level = 8;                  //8就是专门杀clone NPC的任务
        else
            level = 7-random(3);
    }

    i = ob->query_temp("questkh_times");
   
    if(i > 15 && random(20) > 15 || wizardp(ob))
    {
        questkill = QUESTKILL_D->query_questkill();
        msg = questkill["msg"];
        write("葵花太监用眼角瞟了你一眼，微露讶色，道：“你倒是好象挺能干的嘛，嗯？”\n");
        write(msg+"\n");
        time = questkill["time"]/2+random(questkill["time"]/2);
        write("葵花太监回头看了看你，又道：“我给你"+time_period(time)+"时间，你快去罢。”\n");
        time += time();
      //不要直接修改questkill
        questkh = ([
            "name" :  questkill["name"], 
            "khmsg":  msg, 
            "dir"  :  questkill["dir"], 
            "type" :  questkill["type"], 
            "time" :  time,
            "level":  level,
            "desc" :  "灭",
            ]),
        ob->set("quest_kh",questkh);
        return 1;
    }
    else
    {
        if(level==8)
        {
            npc = new(CLASS_D("generate") + "/killed.c");
            NPC_D->place_npc(npc, ob->query("combat_exp") < 500000  ? ({ "大理一带", "终南山", "关外", "西域" }) :
                                  ob->query("combat_exp") < 800000 ? ({ "大理一带", "终南山", "西域" }) : ({"西域"}));
            NPC_D->set_from_me(npc, ob, 100);
            npc->add_temp("apply/attack", npc->query_skill("force") * (level - 1) / 15);
            npc->add_temp("apply/dodge", npc->query_skill("force") * (level - 1) / 15);
            npc->add_temp("apply/parry", npc->query_skill("force") * (level - 1) / 15);
            npc->add_temp("apply/damage", 5 + level * 7);
            npc->add_temp("apply/unarmed_damage", 5 + level * 7);
            npc->add_temp("apply/armor", 10 + level * 15);
            npc->set_temp("quester", ob->query("id"));
            NPC_D->random_move(npc);
            NPC_D->random_move(npc);
            NPC_D->random_move(npc);
            place = npc->query("place");

            npc->set("shen", npc->query("combat_exp") / 2000);
            msg="葵花太监恨恨地说：“"+npc->name(1)+
                "这个所谓的大侠屡次和我作对，听说他最近在" + 
                place + "，你去宰了他，提头来见！”\n";

            questkh = ([
                "name" : npc->name(1),
                "id"   : npc->query("id"),
                "bonus": (100+random(41)),
                "type" :"杀",
                "level":8,
                "chinese_w":place,
                "where":npc->query("startroom"),
                "khmsg":msg,
            ]);
            tell_object(ob,questkh["khmsg"]+"\n");
            ob->set("quest_kh",questkh);

            return 1;       
        }
        else
            {
                questtemp = QUESTKH_D(level)->query_questkh(ob);
                if(undefinedp(msg = questtemp["msg"]) || msg == "" || random(2))
                {
                                if(questtemp["race"])
                                        msg=KHMSG_D->query_khmsg("宰",questtemp["name"]);
                                else
                                        msg=KHMSG_D->query_khmsg(questtemp["type"],questtemp["name"]);
                        }
                questkh=([
                        "id":questtemp["id"],
                        "name":questtemp["name"],
                        "bonus":questtemp["bonus"],
                        "type":questtemp["type"],
                        "khmsg":msg,
                        "level":level,
                        ]);
                write(questkh["khmsg"]+"\n");
                ob->set("quest_kh",questkh);
            }
    }  
    return 1;
}

int accept_object(object who, object ob)
{
   int exp, pot,level,bonus,i;
   mapping questkh;
   object me,gold;
   int shen,mar,weiwang,score,quest_count;
   string msg;
   mixed special;
        
   me = this_object();
   if(!(questkh =  who->query("quest_kh")) ||
      (questkh["type"] != "寻" && questkh["type"] != "杀"))
   {
      return notify_fail("葵花太监满脸不耐烦的冲你说：“走开，不要来烦我。”\n");
   }
   switch(questkh["type"])
   {
      case "寻"  :
         if(ob->name(1) != questkh["name"] || userp(ob))
         {
            tell_object(who,"葵花太监大怒，斥道：“这是什么东西？你想来糊弄我么！”\n");
            return notify_fail("葵花太监吁了口气，向你甩甩手，道：“你去吧，不要烦我。”\n");
         }
         break ;
      case "杀"  :
         if(!ob->is_head() && !ob->is_corpse())
         {
            tell_object(who,"葵花太监斜着眼睛描了一下，斥道：“这是个什么东西？你想来糊弄我么！”\n");
            return notify_fail("葵花太监白了你一眼，甩甩手，道：“你还站在这里干什么？还不快去！”\n");
         }
         else if (! stringp(ob->query("owner_id")) || ob->query("owner_id") != questkh["id"])
            {
                tell_object(who,"葵花太监捂着鼻子挥手说道：“什么臭烘烘的？快给我拿走！”\n");
                return notify_fail("葵花太监白了你一眼，甩甩手，道：“我给你的任务你是否不想做了？”\n");
            }
            else if(ob->query("killed_by") != who->query("id"))
            {
                tell_object(who,"葵花太监哼了一声道：“你是从哪里捡来的？你自己做不了么？”\n");
                return notify_fail("葵花太监白了你一眼，甩甩手，道：“你还站在这里干什么？还不快出去！”\n");
             }
         break ;
      default    :   
         return 0;
   }
   message_vision("$N将$n交给葵花太监。\n",who,ob);
   tell_object(who,"葵花太监冷笑了一声，抬头看了看你，点头道：“嗯，你可以去了。”\n");
   if((i=who->query_temp("questkh_times")) < 2)   i =10-i;
   else i = 1;
   level = questkh["level"]+1;
   exp = level*questkh["bonus"]*12/(i*(10+random(10)));
   pot = level*questkh["bonus"]/(3+random(5));

   score = random(exp/20);
   weiwang = random(exp/15);

   if (who->query_temp("questkh_times") + 1 > 200)
	mar = exp/5 + random(who->query_temp("questkh_times"));
   else mar = random(20);

   if(!random(18))
   {
      i=(exp/100+1);
      if(i>20) i=20;
      gold=new("/clone/money/gold");
      gold->set_amount(i);
      tell_object(who,"突然一锭黄金飞入你的手中，背后葵花太监冷冷说道：“这些金子拿去，日后办事卖力点！”\n"
          HIW"\n你被奖励了 "+i+" 两黄金。\n"NOR);
       if(!gold->move(who))
       {
          message_vision("但是$N身上东西太重，黄金一下子从手中滑到了地上。\n",who);
          gold->move(environment());
      }
      QUEST_D->bonus(who, ([ "exp" : exp, "pot" : pot, "mar" : mar, 
                            "weiwang" : weiwang, "score" : score, "percent" : 50 ]), 1);
   }
   else QUEST_D->bonus(who, ([ "exp" : exp, "pot" : pot, "mar" : mar,
                              "weiwang" : weiwang, "score" : score ]), 1);
   
   quest_count = who->query_temp("questkh_times") + 1;
   if (quest_count == 100)
   {
       msg = "$N嘿嘿冷笑两声道：“你小子还可以，连着上百"
       "次任务都完成的干净利索，还算有点前途。”\n";
	special = 1;
	   } else
       if (quest_count == 200)
   { 
       msg = "$N抬头使劲看了你一眼道：“真是不错，两百"
       "次任务干得还算妥当。”\n";
       special = "/d/shaolin/obj/puti-zi";
   } else
       if (quest_count == 500)
   {
       msg = "$N叹道：“真是长江后浪推前浪，想不到你接连五百"
       "次任务都无一疏漏，不易，不易！”\n";
       special = "/clone/misc/tianxiang";
   } else
       if (quest_count == 1000)
   {
       msg = "$N深深叹了口气，道：“想不到你连着上千次"
       "任务无一失手，看来以后的江湖就是你的天下啦！”\n";
       special = "/clone/gift/jiuzhuan";
   } else
       if ((quest_count % 10) == 0)
   {
       msg = "$N微微点头：“干的不错，居然连着" +
       chinese_number(quest_count) +
       "次都没有失手，继续给我好好表现！”\n";
   }
   if (special) {
       message_vision(msg, me, who);
       QUEST_D->special_bonus(me, who, special);
   }
   who->add_temp("questkh_times",1);
   who->delete("quest_kh");
   return 1;
}

int give_up()
{
   object ob,obj,room;
   mapping questkh;

   ob = this_player();

   if(query("kill_all"))   return 0;

   if(mapp(questkh =  ob->query("quest_kh")))
   {
      if(questkh["desc"] == "灭")
      {
         message_vision("$N心惊胆战，双腿乱颤，请求葵花太监取消自己的任务。\n",ob);
         message_vision("葵花太监看了$N一眼，“嗯”了一声，不置可否。\n", ob);
      }
      else if(questkh["type"] == "杀"&&questkh["level"] == 8)    //by llm
      {
          message_vision("$N心惊胆战，双腿乱颤，请求葵花太监取消自己的任务。\n",ob);
          message_vision("葵花太监头也不抬，哼了一声道：“没用的东西！蠢材！”\n", ob);
          if(!(room = find_object(questkh["where"])) )
              room = load_object(questkh["where"]);

          if(objectp(obj = present(questkh["id"],room))
              &&obj->is_character()
              &&!userp(obj))
          {
              message_vision("$N哼了一声，转身走开了。\n",obj);
              obj->move("/d/register/prison");
              destruct(obj);
          }
          ob->set_temp("questkh_times",0);
      }
      else
      {
         message_vision("$N心惊胆战，双腿乱颤，请求葵花太监取消自己的任务。\n",ob);
         message_vision("葵花太监头也不抬，哼了一声道：“没用的东西！蠢材！”\n", ob);
         ob->set_temp("questkh_times",0);
      }
   }
   else
   {
      message_vision("$N心惊胆战，双腿乱颤，请求葵花太监取消自己的任务。\n",ob);
      message_vision("葵花太监专心把玩手中的古董，好象根本没看见$N\n", ob);
   }
   ob->delete("quest_kh");
   ob->set_temp("questkh_giveup_time",time());
   return 1;
}

string time_period(int timep)
{
   int t, d, h, m, s;
   string time;
   t = timep;
   s = t % 60;             t /= 60;
   m = t % 60;             t /= 60;
   h = t % 24;             t /= 24;
   d = t;
 
   if(d) time = chinese_number(d) + "天";
   else time = "";
 
   if(h) time += chinese_number(h) + "小时";
   if(m) time += chinese_number(m) + "分";
   time += chinese_number(s) + "秒";
   return time;
}

void npc_dest(object npc)
{
    if(!npc) return;
    if(npc->is_fighting()&&npc->query_temp("wait_times")<4)
    {
        npc->add_temp("wait_times",1);   //4次机会
        call_out("npc_dest",60,npc);     //再等一分钟
        return;
    }
    if(npc->is_character())
        message_vision("$N哈哈一笑：“葵花太监的手下尽是一些窝囊废！”转身一晃，已不见了身影。\n",npc);
    npc->move("/d/register/prison");
    destruct(npc);
    return ;
}

