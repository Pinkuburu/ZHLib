// huang 独孤求败

#include <ansi.h>

inherit F_MASTER;
inherit NPC;
inherit F_NOCLONE;

#define QUESTGOODS_D(x)    ("/quest/questdg/quest/questgoods" + x)
#define QUESTDG_D(x)      ("/quest/questdg/quest/questdg" + x)
#define DGMSG_D      ("/quest/questdg/dugu-msg")


mixed ask_opinion();
void greeting(object);
void init();
int give_quest();
int give_up();
void npc_dest(object);
void destory(object);
string time_period(int);

#define ULTRA_OPINION   "宗师"
#define OPINION_TYPE    HIY "兵器功夫" NOR 
#define MY_OPINION      "weapon"

string  *my_opinions = ({ "sword", "blade", "staff", "hammer",
                          "club", "whip",  "dagger", });

string* dg_msg = ({
"独孤求败笑呵呵的说：“",
"独孤求败慢慢踱着方步道：“",
"独孤求败低头想了想，说道：“",
"独孤求败满脸期待的说道：“",
"独孤求败微笑着道：“",
});

string* qin_msg1 = ({
"动物是人们的朋友，就算name严重影响了生态环境，关起来也就可以了，",
"我听说name过多的繁殖，影响了当地的生态，不过还是不应该随便杀动物，",
"难道name也会给人类带来麻烦？真是稀奇，我到是想看看它，",
"我这里的花园养着很多的动物，就是没有name，",
"在空闲的时候，看看动物的生活能够放松情绪，name其实挺可爱的，",
"南海神尼是一个很喜欢动物的人，尤其是name，用来送给她做礼物最好不过，",
"江湖上打打杀杀的，少不了让name也遭受无辜连累，我有心保护它，",
});

string* qin_msg2 = ({
"你去把它带到这里来吧。”",
"去找到它，把它带来。”",
"你能把它带我这里来么？”",
"你就辛苦一躺去帮我把它找来吧。”",
"把它弄来给我。”",
});

// 该头文件必须在这里嵌入
#include <ultra.h>

void check_weapon();

void create()
{
        set_name("独孤求败", ({ "dugu qiubai", "dugu" }) );
        set("title", HIC "大宗师" NOR);
        set("gender", "男性");
        set("age", 48);
        set("long",
"这是一个普普通通的老者，一袭粗布衣裳，\n"
"颏下一缕白须，神情详和。目光转动之间，\n"
"隐隐流露出一丝王者之气。\n");      
        set("attitude", "peaceful");
        set("str", 38);
        set("int", 39);
        set("con", 32);
        set("dex", 33);

        set("max_qi", 9999);
        set("max_jing", 9999);
        set("neili", 9999);
        set("max_neili", 9999);
        set("jiali", 50);

        set("combat_exp", 50000000);

        set("inquiry", ([
                "任务"     : "独孤求败叹了口气，道：“如今江湖上危机四伏，我正道人士还有很多事情要去做啊！”\n",
                "独孤九剑" : "我把天下武功的变化归结在九剑之中，算不得是剑法，若拘泥于形势就无法理解其中奥妙。",
                "评价"     : (: ask_opinion :),
                "opinion"  : (: ask_opinion :),
                "武学修养" : "武学修养(martial-cognize)乃武术大义，可谓是以一通通百通！",
        ]));

        set_skill("unarmed", 500);
        set_skill("sword", 500);
        set_skill("blade", 500);
        set_skill("staff", 500);
        set_skill("hammer", 500);
        set_skill("club", 500);
        set_skill("whip", 500);
        set_skill("dagger", 500);
	    set_skill("throwing", 500);
        set_skill("parry", 500);
        set_skill("dodge", 500);
        set_skill("force", 500);
        set_skill("never-defeated", 500);
        set_skill("martial-cognize", 500);

        map_skill("sword", "never-defeated");
        map_skill("blade", "never-defeated");
        map_skill("staff", "never-defeated");
        map_skill("hammer", "never-defeated");
        map_skill("club", "never-defeated");
        map_skill("whip", "never-defeated");
        map_skill("dagger", "never-defeated");
        map_skill("parry", "never-defeated");
        map_skill("dodge", "never-defeated");
        map_skill("force", "never-defeated");

        set("opinion/ultra",  1);
        set("opinion/master", 1);

        set("chat_chance_combat", 120);
        set("chat_msg_combat", ({
                (: check_weapon :),
        }) );

        set("rank_info/self", "在下");

        setup();
        carry_object("/clone/cloth/cloth")->wear();
        carry_object("/clone/weapon/zhujian")->wield();
	set("startroom", "/d/huashan/shandong");
        check_clone();
}

void init()
{
        object me,ob;

        if (! interactive(me = this_player()))
                return;

        me->delete_temp("want_opinion");
        add_action("give_quest", "quest");
        add_action("give_up","giveup");
}

void check_weapon()
{
        if (objectp(query_temp("weapon")))
                return;

        return;
        message_vision("$N长叹一口气道：我$N多年来兵器还从未离过"
                       "手，罢！罢了！我去也！\n", this_object());
        destruct(this_object());
}

mixed ask_opinion()
{
        object me;
        object ob;

        me = this_player();

        if (me->query("combat_exp") < 200000)
                return "呵呵，你这点武功还谈什么评价？";

        if (me->query("opinion/" MY_OPINION) == ULTRA_OPINION)
                return "哈哈，你兵器方面的修为已经称得上是宗师了！咱们要不要过过招？";

        if (is_fighting(me))
                return "哈哈，废什么话，打赢了我，你自己给自己评价吧！";

        if (me->is_fighting())
                return "你等会儿吧！待我赶走他们。";

        if (objectp(ob = query_temp("opinion")))
        {
                if (ob == me)
                {
                        command("say 你着什么急？不要烦我。");
                        return 1;
                }

                message_vision("$N正在沉思，没有回答$n，只是摆了摆手。\n",
                               this_object(), me);
                return 1;
        }

        switch(me->query_temp("want_opinion/dugu"))
        {
        case 0:
                command("look " + me->query("id"));
                command("say 好吧，你用兵器攻我一招，我看看你的水平如何。");
                break;
        case 1:
                command("say 不是说让你来一招我看看吗？");
                break;
        case 2:
                message_vision(CYN "$N" CYN "皱眉对$n" CYN
                               "道：“我看你这人是够罗嗦的。”\n" NOR,
                               this_object(), me);
                break;
        case 3:
                command("say 给我闭上你的嘴！");
                return 1;
        }

        me->add_temp("want_opinion/dugu", 1);
        return 1;
}

int calc_level(object me, string skill)
{
        int lvl;
        string sp;
        mapping action;

        lvl = me->query_skill(skill, 1) / 2;
        sp = me->query_skill_mapped(skill);
        if (! sp) return lvl;

	lvl += me->query_skill(sp, 1);
        action = SKILL_D(sp)->query_action(me, me->query_temp("weapon")); 
        lvl += (action["force"] + action["parry"] + action["dodge"] - 250) / 10;
        return lvl;
}

void do_congration(object me)
{
        command("chat 这位" + RANK_D->query_respect(me) + "可要善自珍重，须知强中更有强中手。");
}

int accept_hit(object me)
{
        if (me->query_temp("want_hit/dugu") >= 2)
        {
                command("heng");
                command("say 事不过三，你要找死那就去死吧！");
                kill_ob(me);
                return 1;
        }

        command("shake");
        command("say 这位" + RANK_D->query_respect(me) + "，"
                "练武之人切忌贸然挑衅。");
        me->add_temp("want_hit/dugu", 1);
        return 0;
}

int accept_fight(object me)
{
        object weapon;

        if (me->query("opinion/" MY_OPINION) == ULTRA_OPINION)
        {
                message_vision("$N大喜，对$n道：“很好，近年我还没有"
                               "和宗师高手过过招，来吧！”\n",
                               this_object(), me);
                return 1;
        }

        if (! me->query_temp("want_opinion/dugu"))
        {
                command("say 平白无故动什么手？");
                return 0;
        }

        if (! objectp(weapon = me->query_temp("weapon")))
        {
                command("say 没兵器你动什么手？");
                return 0;
        }

        if (me->query("qi") * 100 / me->query("max_qi") < 80||
            me->query("jing") * 100 / me->query("max_jing") < 80)
        {
                command("say 你现在状态不佳，等休息好了再说吧！");
                return 0;
        }

        command("nod");
        COMBAT_D->do_attack(me, this_object(), weapon);
        COMBAT_D->do_attack(this_object(), me, query_temp("weapon"));

        if (me->query("qi") <= 0)
        {
                message_vision("\n$N摇摇头对$n道：“你现在的武功"
                               "还差得远，连我一招都接不住！”\n",
                               this_object(), me);
                return notify_fail("看来要好好的修炼功夫了。\n");
        }

        call_out("check_opinion", 2, me);
        set_temp("opinion", me);
        me->delete_temp("want_opinion");
        message_vision("\n$N点点头道：“好了，你先等会儿，待我想想。”\n",
                       this_object(), me);
        return notify_fail("看来不能心急，还得等一会儿。\n");
}

int accept_kill(object me)
{
        if (me->query("combat_exp") < 500000)
        {
                message_vision("$N大怒道：“无名小辈捣什么"
                               "乱？给我滚！”说罢一掌把$n劈倒在地。\n",
                               this_object(), me);
                me->unconcious();
                return -1;
        }
        command("sigh");
        command("say 很好！近年还真没有遇到什么敢和我较量的高手！");
        return 1;
}

int recognize_apprentice(object me, string skill)
{
        if (me->query("character") != "狡黠多变")
        {
                command("say 我看你的性格不够机智灵活，不适宜"
                        "向我学习武功，还是算了吧！");
                return -1;
        }

        if (! ultrap(me))
        {
                command("say 想跟我学武功？你还不够资格，"
                        "等你成了大宗师再说吧！");
                return -1;
        }

	if (skill == "never-defeated")
	{
                command("say 想学我的不败神功？这个岂是凡人能用的，"
                        "你从我这里学点武学修养才是正经！");
                return -1;
	}
	
/*
	if (skill == "martial-cognize")
		return 1;

        if (me->query_skill(skill, 1) >= 180)
        {
                command("say 够了！我就教你到这里吧，武功还是"
                        "要靠自己多研究才是！");
                return -1;
        }

        if (skill != "never-defeated")
                return 1;

        if (me->query_temp("can_learn/dugu/never-defeated"))
                return 1;

        me->set_temp("can_learn/dugu/never-defeated", 1);
        command("nod");
        command("say 我可以教你我多年领悟出来的不败神功，可是"
                "你能不能学，还要看你的机缘了！");
*/
        return 1;
}

void unconcious()
{
 //       message("vision", "\n"HIR + name() + "脸色一变，吐了一口鲜"
 //               "血，身形一闪，消逝不见。\n\n" NOR, environment());
 //       command("chat 罢！罢！罢！在下去也！");
 //       destruct(this_object());
return;
}

void die()
{
//        message("vision", "\n"HIR + name() + "大叫一声，狂吐了一口鲜"
//                "血，身形一闪，就此不见。\n\n" NOR, environment());
//        command("chat 在下去也！");
//        destruct(this_object());
return ;
}

int give_quest()
{
    mapping questtemp,questgoods,questdg,skills;
    object ob,npc,thing;
    int exp,time,bonus,level,i,lev,lev1;
    string msg,dest,error,dgmsg,place;
        
    ob = this_player();
    exp = (int)ob->query("combat_exp");

    if( (ob->query("character") != "光明磊落") && (ob->query("character") != "狡黠多变"))  
    {
        write("独孤求败看了你一眼，道：“你本性杀气太重，跟我等相处日久必然格格不入，你还是另找他人去吧！”\n");
        return 1;
    }
    if( ob->query("shen") < 0)  
    {
        write("独孤求败对你冷冷道：“道不同，不相为谋，你还是请便罢！”\n");
        return 1;
    }
    if(exp < 500000)
    {
       write("独孤求败看了看你，轻抚白须，道：“江湖险恶，你还是先练好基本功吧。”\n");
        return 1;
    }
 
    if (mapp(ob->query("quest_dg")))
    {
        write("独孤求败拍了拍你的肩膀，呵呵笑道：“不要急，先将手头的做完再说。”\n");
        return 1;
    }
    if (mapp(ob->query("quest_hs")))
    {
        ob->delete("quest_hs");
        ob->delete_temp("quesths_times");
    }
   
    if(!wizardp(ob) && (time()-ob->query_temp("questdg_giveup_time")) < 3)
    {
        write("不要急，慢一点嘛。\n");
        return 1;
    }
    if(random(4)+1)
    {
    // 以下给任务
        if(exp <= 600000)        level = 0;
        else if(exp <= 900000)   level = 1;
        else if(exp <= 1200000)  level = 2;
        else if(exp <= 1800000)  level = 3;
        else if(exp <= 2500000)  level = 4-random(3);
        else if(exp <= 3200000)  level = 5-random(3);
        else if(exp <= 4000000)  level =6-random(3);
        else
        {
            if(random(15)==0||(exp>10000000&&random(8)==0))
                level=8;
            else level=7-random(3);
        }
        if (level != 8)
        questtemp = QUESTDG_D(level)->query_questdg(ob);
        if(level==8)
        {
            npc = new(CLASS_D("generate") + "/player_npc.c");
            NPC_D->place_npc(npc, ob->query("combat_exp") < 500000  ? ({ "大理一带", "终南山", "关外", "西域" }) :
                                  ob->query("combat_exp") < 800000 ? ({ "大理一带", "终南山", "西域" }) : ({"大理一带", "西域"}));
            npc->set_from_me(ob, 1);
            npc->add_temp("apply/attack", npc->query_skill("force") * (level - 1) / 15);
            npc->add_temp("apply/dodge", npc->query_skill("force") * (level - 1) / 15);
            npc->add_temp("apply/parry", npc->query_skill("force") * (level - 1) / 15);
            npc->add_temp("apply/damage", 5 + level * 7);
            npc->add_temp("apply/unarmed_damage", 5 + level * 7);
            npc->add_temp("apply/armor", 10 + level * 15);
            npc->set_temp("quester", ob->query("id"));
            npc->set("auto_perform",1);
            NPC_D->random_move(npc);
            NPC_D->random_move(npc);
            NPC_D->random_move(npc);
            place = npc->query("place");
            npc->set("shen", -npc->query("combat_exp") / 2000);
            
            msg="独孤求败面色凝重道：“"+npc->name(1)+
                "居然又重现江湖，你速去除了这一害，\n此人可能会在"+
                place+"附近，慢了就找不到了。”\n",

            questtemp = ([
                "name" : npc->name(1),
                "id"   : npc->query("id"),
                "bonus": (100+random(41)),
                "type" :"杀",
                "level":level,
                "chinese_w":place,
                "where":npc->query("startroom"),
                "dgmsg":msg,
		"object":base_name(npc),
            ]);
            tell_object(ob,questtemp["dgmsg"]+"\n");
            ob->set("quest_dg",questtemp);

            return 1;
        }
        else
        {
                switch(questtemp["type"])
                {
                case "杀":
                        if (questtemp["object"]->query("race") != "人类")
                        {
                                questtemp["type"] = "擒";
                                msg = dg_msg[random(sizeof(dg_msg))] + replace_string(qin_msg1[random(sizeof(qin_msg1))],"name",questtemp["name"]) + qin_msg2[random(sizeof(qin_msg2))];
                                break;
                        }
                        if(undefinedp(msg = questtemp["msg1"])||msg==""||random(2))
                        {
                                if(questtemp["race"])
                                        msg=DGMSG_D->query_dgmsg("宰",questtemp["name"]);
                                else
                                        msg=DGMSG_D->query_dgmsg("杀",questtemp["name"]);
                        }
                        break;
                case "寻":
                        if(undefinedp(msg = questtemp["msg"])||msg==""||random(2))
                                msg=DGMSG_D->query_dgmsg("寻",questtemp["name"]);
                        break;
                case "擒":
                        if (questtemp["object"]->query("race") != "人类")
                        {
                                msg = dg_msg[random(sizeof(dg_msg))] + replace_string(qin_msg1[random(sizeof(qin_msg1))],"name",questtemp["name"]) + qin_msg2[random(sizeof(qin_msg2))];
                                break;
                        }
                        if(undefinedp(msg = questtemp["msg2"])||msg==""||random(2))
                        {
                                if(questtemp["race"])
                                        msg=DGMSG_D->query_dgmsg("捉",questtemp["name"]);
                                else
                                        msg=DGMSG_D->query_dgmsg("擒",questtemp["name"]);
                        }
                        break;
                case "送":
                        npc = new(CLASS_D("generate") + "/dg_receiver.c");
                        NPC_D->place_npc(npc, ob->query("combat_exp") < 500000  ? ({ "大理一带", "终南山", "关外", "西域" }) :
                                              ob->query("combat_exp") < 800000 ? ({ "大理一带", "终南山", "西域" }) : ({"大理一带", "西域"}));
                        NPC_D->set_from_me(npc, ob, 100);
                        npc->set_temp("quester", ob->query("id"));
                        NPC_D->random_move(npc);
                        NPC_D->random_move(npc);
                        NPC_D->random_move(npc);
                        place = npc->query("place");
                        thing = new(questtemp["object"]);
                        thing->set("send_to",npc->query("id"));
                        thing->set("send_from",query("id"));
                        thing->set("send_from_name",query("name"));
                        msg = dg_msg[random(5)]+"麻烦你帮我将"+thing->name(1)+"送至"+npc->name(1)+"处，"
                                                "他现在正在"+place+"，误了时间他可能就离开了。";
                        if (!thing->move(ob))
                        {
                                thing->move(environment());
                                message_vision("独孤求败将$N轻轻放在地上。\n",thing);
                         } else message_vision("独孤求败将$N交给$n。\n",thing,ob);
                         break;
                }
                questdg=([
                         "name":questtemp["name"],
                         "type":questtemp["type"],
                         "bonus":questtemp["bonus"],
			 "object":questtemp["object"],
                         "dgmsg":msg,
                ]);
        }
    }  
    tell_object(ob,questdg["dgmsg"]+"\n");
    questdg["level"]=level;
    if (questdg["type"] == "送") questdg["obj"] = thing;
    ob->set("quest_dg",questdg);
    return 1;
}

int accept_object(object who, object ob)
{
   int exp, pot,level,bonus,time,times,i,quest_count,mar,score,weiwang,all_quest_dg;
   mapping questdg;
   string dest,msg;
   object gold,obj;
   mixed special = 0;

   if(!(questdg =  who->query("quest_dg")))
   {
      tell_object(who,"独孤求败看了看你，似乎不知道你是什么意思。\n");
      return notify_fail("独孤求败淡淡一笑，道：“我可不缺什么东西。”\n");
   }
   
   if(ob->query("money_id"))
   {
           if (ob->value() < 5000000)
           {
                   tell_object(who,"独孤求败对你语重心长的说：你拿这么点钱来干什么呢？我有不是缺钱花。\n");
                   return notify_fail("独孤求败摇摇头：如果给个500两黄金，或许我就消除了你的任务。\n");
           }
           who->delete("quest_dg");
           tell_object(who,"独孤求败盯者" + ob->name(1) + "注视良久，叹气道：那你这个任务不做就算了吧。\n");
           destruct(ob);
           return 1;
   }

   if(questdg["type"] != "寻" && questdg["type"] != "擒" && questdg["type"] != "送")
   {
      tell_object(who,"独孤求败看了看你，似乎不知道你是什么意思。\n");
      return notify_fail("独孤求败淡淡一笑，道：“我可不缺什么东西。”\n");
   }

   if( ob->name(1) != questdg["name"] || userp(ob))
   {
      tell_object(who,"独孤求败叹了口气，道：这并不是我想要的。\n");
      return notify_fail("独孤求败看着你，道：“你是不是再去找找看啊？”\n");
   }

   if(questdg["type"]=="送")
   {
      if (!ob->is_receipt() || ob->query("reply_to") != this_object()->query("id"))
      {
          write("独孤求败接过来一看，惊讶的说：“我不是叫你帮我送东西了么，你是不是记错了？”\n");
          return 0;
      }
   } 

    if(userp(ob) || ob->query("user"))
    {
        write("独孤求败大怒道：“好你个小子，竟然想蒙骗本宗师！来人！把他给我抓起来！”\n");
        who->move("/d/register/prison");
        log_file( "static/QUEST_DIJIAO", sprintf("%s(%s)(独孤任务)制造%s(%s)(%s)作弊\n", who->query("name"),
             who->query("id"),ob->name(),ob->query("id"),ctime(time()) ) );
        return 1;
    }

   message_vision("$N将$n交给独孤求败。\n",who,ob);
   tell_object(who,"独孤求败点头微笑，其意颇为嘉许，道：“不错！不错！”\n");
   who->delete("quest_dg");
   destruct(ob);

   level = questdg["level"]+1;
   if((i=who->query_temp("questdg_times")) < 2)   i = 10-i;
   else i = 1;

   exp = level*120 + questdg["bonus"]*3;
   exp += random(exp/2);
   exp /= i;

   pot = exp/(2+random(2));
   score = random(exp/20);
   weiwang = random(exp/15);

   if (who->query_temp("questkh_times") + 1 > 200)
	mar = exp/5 + random(who->query_temp("questkh_times"));
   else mar = random(20);
   
   if(wizardp(who)||!random(10))
   {
      i=(exp/100+1);
      if(i>20) i=20;
       gold=new("/clone/money/gold");
       gold->set_amount(i);
       tell_object(who,"接着说道：“整日行走江湖，手头有点紧吧？！喏，拿点黄金做盘缠吧！”\n"
          HIW"\n你被奖励了 "+i+" 两黄金。\n");
       if(!gold->move(who))
       {
           message_vision("但是$N身上东西太重，黄金一下子掉到了地上。\n",who);
           gold->move(environment());
       }
       QUEST_D->bonus(who, ([ "exp" : exp, "pot" : pot, "mar" : mar,
                             "weiwang" : weiwang, "score" : score, "percent" : 50 ]), 1);
   }
   else QUEST_D->bonus(who, ([ "exp" : exp, "pot" : pot, "mar" : mar,
                              "weiwang" : weiwang, "score" : score ]), 1);

   who->add_temp("questdg_times",1);
   who->add("pk_score",-5);
   if (who->query("pk_score") < 0) who->set("pk_score",0);

   quest_count = who->query_temp("questdg_times");
   if (quest_count == 50)
   {
       msg = "$N呵呵笑了两声道：“你还可以，连着五十"
       "次任务都完成的漂漂亮亮，继续努力！”\n";
	special = 1;
	   } else
       if (quest_count == 100)
   { 
       msg = "$N笑的合不拢嘴，看了你一眼道：“有志气！一百"
       "次任务干得干净利索！”\n";
       special = "/d/shaolin/obj/puti-zi";
   } else
    if (quest_count == 300)
    {
        msg = "$N叹道：“真是长江后浪推前浪，想不到你接连三百"
               "次任务都无一疏漏，不易，不易！”\n";
	 special = "/clone/misc/tianxiang";
    } else
    if (quest_count == 350)
    {
        msg = "$N吓了一跳：“看来你真的很适合学武，竟接连三百"
               "五十次任务都无一疏漏，不易，不易！”\n";
	 special = "/clone/misc/tianxiang";
    } else
    if (quest_count == 400)
    {
        msg = "$N叹道：“真是一个武学天才啊，想不到你接连四百"
               "次任务都无一疏漏，不易，不易！”\n";
	 special = "/clone/misc/tianxiang";
    } else
    if (quest_count == 450)
        {
        msg = "$N大声叫起来：“天才！真是天才，真有人连续四百"
               "五十次任务都无一疏漏，不易，不易！”\n";
        special = "/clone/gift/jiuzhuan";
   } else
       if ((quest_count % 10) == 0)
   {
       msg = "$N呵呵笑着：“你表现很不错啊，居然连着" +
       chinese_number(quest_count) +
       "次都没有失手，别忙着练，慢慢来。”\n";
   }
   if (special) {
       message_vision(msg, this_object(), who);
       QUEST_D->special_bonus(this_object(), who, special);
   }
   
   //记录完成任务的次数（不要求连续），如果满1000次则奖励玛瑙残片
   all_quest_dg = (int)who->query("all_quest_dg");
   if (!all_quest_dg) all_quest_dg = 0;
   all_quest_dg += 1;
   if (all_quest_dg == 1000)
   {
       obj = new("/clone/gift/cagate");
       tell_object(who,this_object()->name() + "又对你点了点头：你已经从我这里接了一千个任务，我这里正好\n" +
                   "有个" + obj->query("name") + "，就奖给你了！\n");
       if (!obj->move(who))
               obj->move(environment());
       who->set("all_quest_dg",0);
   } else who->set("all_quest_dg",all_quest_dg);
   return 1;
}

int give_up()
{
    object ob,obj,room;
    mapping questdg;

    ob = this_player();

    if(mapp(questdg =  ob->query("quest_dg")))
    {
        write("你满脸羞惭，请求独孤求败取消自己的任务。\n");
        write("独孤求败和颜悦色地拍拍你的脑袋，道：“没关系，要继续努力啊！”\n");
        if(questdg["type"] == "送")
        {
            if(objectp(obj = questdg["obj"]))
            {
                if(obj->is_character())
                    message_vision("$N一跃而起，跑得无影无踪了。\n",obj);
                obj->move(environment(ob));
                destruct(obj);
                if(ob->query_temp("questdg_times") > 0)
                    ob->add_temp("questdg_times",-1);
            }
        }
        else if(questdg["type"] == "杀"&&questdg["level"] == 8)    //by llm
        {
            if(!(room = find_object(questdg["where"])) )
                room = load_object(questdg["where"]);

            if(objectp(obj = present(questdg["id"],room))
                &&obj->is_character()
                &&!userp(obj))
            {
                message_vision("$N哼了一声，转身走开了。\n",obj);
                obj->move("/d/wizard/prison");
                destruct(obj);
            }
            ob->set_temp("questdg_times",0);
        }
        else
            ob->set_temp("questdg_times",0);
    }
    else
        write("独孤求败和颜悦色地看着你，道：“没关系，你现在根本就没有任务啊！”\n");
    ob->delete("quest_dg");
    ob->set_temp("questdg_giveup_time",time());
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

void destory(object ob)
{
   if(!ob)
      return;
   if(ob->is_character())
      message_vision("$N一跃而起，跑得无影无踪了。\n",ob);
   ob->move("/d/wizard/prison");
   destruct(ob);
   return ;
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
        message_vision("$N重重地哼了一声：“看来都是胆小鬼”，转身一晃，已不见了身影。\n",npc);
    npc->move("/d/wizard/prison");
    destruct(npc);
    return ;
}
