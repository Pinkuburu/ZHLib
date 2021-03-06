/************************炎黄转世系统核心文件*********************************
created by Rcwiz for YHONLINE
*******************************************************************************/

#pragma optimize
#pragma save_binary

inherit F_DBASE;

#include <ansi.h>
#include <localtime.h>

#define MEMBER_D                        "/adm/daemons/memberd" 

int change_data(object me, string specials);

mapping special2 = ([
       "zhuha"      :   "朱蛤奇缘",
       "yuan"       :   "破元大法",
       "jin"        :   "神魔金身",
       "tong"       :   "通慧神眼",
       "bian"       :   "诡辩奇学",
       "guimai"     :   "六阴鬼脉",
       "xunying"    :   "遁影擒踪",
       "wushuang"   :   "国士无双",
       "zhushang"   :   "诸伤之术",
       "jinghua"    :   "仙音净化",
       "dian"       :   "点石成金",
]);

mixed get_all_specials()
{
	return special2;
}

string get_special_name(string arg)
{
	return special2[arg];
}
/**************************************************************************
 分析转世数据，是否符合条件闯十二重天
***************************************************************************/
int check_data(object me)
{
       if (wizardp(me))return 1;
       
       if (me->query("scborn/ok"))
       {
       	      write("你已经转过世了！\n");
       	      return 0;
       }
       if (! ultrap(me))
       {
              write("你还没有成为武学大宗师！\n");
              return 0;
       }
       if (! me->query("breakup"))
       {
              write("你还没有打通任督二脉！\n");
              return 0;
       }
       if (! me->query("animaout"))
       {
              write("你还没有修炼元婴出世！\n");
              return 0;
       }
       if (! me->query("death"))
       {
              write("你还没有打通生死玄关！\n");
              return 0;
       }
       if (me->query("league"))
       {
              write("既然你对今生已无所留恋，那就尽快给你同盟的义士们告别吧！\n");
              write(HIR "看来你得先脱离你的同盟！\n" NOR);
        
              return 0;
       }
       
       return 1;
}
/***************************************************************************
 分析转世数据，是否符合转世条件
     1、成为武学大宗师
     2、闯入十二重天界
     3、打通生死玄关，元婴出世，任督二脉
***************************************************************************/
int check_data2(object me)
{
       if (wizardp(me))return 1;
       
       if (me->query("scborn/ok"))
       {
       	      write("你已经转过世了！\n");
       	      return 0;
       }       
       if (! ultrap(me))
       {
              write("你还没有成为武学大宗师！\n");
              return 0;
       }
       if (! me->query("breakup"))
       {
              write("你还没有打通元婴出世！\n");
              return 0;
       }
       if (! me->query("animaout"))
       {
              write("你还没有修炼元婴出世！\n");
              return 0;
       }
       if (! me->query("death"))
       {
              write("你还没有打通生死玄关！\n");
              return 0;
       }
       if (me->query("sky12/floor") != 13)
       {
              write("你还没有通过十二重天界的考验！\n");
              return 0;
       }

       return 1;
}


/**************************************************************************
 开始转世
 初始化一些转世必须的数据，然后更新数据
**************************************************************************/
int start_scborn(object me)
{
       string *specials;
       string spe, user, login;

       specials = keys(special2);
      
       user =  me->query("id");
       login = user;       
       user = "/data/user/" + user[0..0] + "/" + user + ".o";
       login = "/data/login/" + user[0..0] + "/" + user + ".o";
       
       // 备份数据
       mkdir("/data/scborn/" + user);
       cp(user, "/data/scborn/" + me->query("id") + ".u"); // user.o
       cp(user, "/data/scborn/" + me->query("id") + ".l"); // login.o

       // 随机选取一个转世技能 
       while(1)
       {
               spe = specials[random(sizeof(specials))];
               if (spe == "wushuang")continue;
               
               break;
       }

       // 更新数据
       if (MEMBER_D->is_valib_member(me->query("id")))return change_data(me, spe);

       else return change_data(me, 0);

}

/**************************************************************************
 更新数据。
 保留必要转世前的数据，并清除其余数据
**************************************************************************/
int change_data(object me, string specials)
{
       int i;
       mixed sdata, klist, newdata;
       mapping special_sk, skills, old_sk;
       
       if (! me)me = this_player();

       sdata = me->query_entire_dbase();

       if (! sdata)return 0;

       klist = keys(sdata);

       // 以下数据是转世后仍然保留的
       newdata = ([]);

       newdata += ([ "sec_id"      :   me->query("sec_id")]);
       newdata += ([ "cwf"         :   me->query("cwf") ]);
       newdata += ([ "env"         :   me->query("env") ]);
       newdata += ([ "pmskills"    :   me->query("pmskills") ]);
       newdata += ([ "unit"        :   me->query("unit") ]);
       newdata += ([ "libs"        :   me->query("libs") ]);
       newdata += ([ "startroom"   :   me->query("startroom") ]);
       newdata += ([ "can_speak"   :   me->query("can_speak") ]);
       newdata += ([ "member"      :   me->query("member") ]);
       newdata += ([ "kar"         :   me->query("kar") ]);
       newdata += ([ "last_save"   :   me->query("last_save") ]);
       newdata += ([ "channels"    :   me->query("channels") ]);
       newdata += ([ "schedule"    :   me->query("schedule") ]);
       newdata += ([ "last_read_news"  :   me->query("last_read_news") ]);
       newdata += ([ "email"       :   me->query("email") ]);
       newdata += ([ "born"        :   me->query("born") ]);
       newdata += ([ "birthday"    :   time() ]);
       newdata += ([ "startroom"   :   me->query("startroom") ]);
       newdata += ([ "can_summon"  :   me->query("can_summon") ]);
       newdata += ([ "attitude"    :   me->query("attitude") ]);
       newdata += ([ "character"   :   "国士无双" ]);
       newdata += ([ "id"          :   me->query("id") ]);
       newdata += ([ "message"     :   me->query("message") ]);
       newdata += ([ "registered"  :   me->query("registered") ]);
       newdata += ([ "per"         :   me->query("per") ]);
       newdata += ([ "race"        :   me->query("race") ]);
       newdata += ([ "cwd"         :   me->query("cwd") ]);
       newdata += ([ "purename"    :   me->query("purename") ]);
       newdata += ([ "surname"     :   me->query("surname") ]);
       newdata += ([ "name"        :   me->query("name") ]);
       newdata += ([ "balance"     :   me->query("balance") ]);
       newdata += ([ "gender"      :   me->query("gender") == "无性" ? "男性" : me->query("gender") ]);
       newdata += ([ "type"        :   me->query("type") ]);
       newdata += ([ "board_last_read":me->query("board_last_read") ]);

       // 需要重新设置的数据
       newdata += ([ "age"         :   5 ]); // 转世后为5岁
       newdata += ([ "mud_age"     :   0 ]);
       newdata += ([ "born_family" :   "没有" ]);
       newdata += ([ "eff_jing"    :   100 ]);
       newdata += ([ "eff_qi"      :   100 ]);
       newdata += ([ "max_jingli"  :   0 ]);
       newdata += ([ "jingli"      :   0 ]);
       newdata += ([ "food"        :   200 ]);
       newdata += ([ "water"       :   200 ]);
       newdata += ([ "jing"        :   100 ]);
       newdata += ([ "max_jing"    :   100 ]);
       newdata += ([ "max_qi"      :   100 ]);
       newdata += ([ "max_neili"   :   0 ]);
       newdata += ([ "neili"       :   0 ]);
       newdata += ([ "potential"   :   999 ]);
       newdata += ([ "qi"          :   100 ]);
       newdata += ([ "shen"        :   0 ]);
       newdata += ([ "shen_type"   :   0 ]);
       newdata += ([ "title"       :   "普通百姓" ]);
       // 天赋
       newdata += ([ "str"         :   me->query("str") ]); 
       newdata += ([ "con"         :   me->query("con") ]);
       newdata += ([ "int"         :   me->query("int") ]); 
       newdata += ([ "dex"         :   me->query("dex") ]); 
       newdata += ([ "per"         :   30 ]);


       // 设置技能
       skills = ([]);
       skills += ([ "martial-cognize"     :    300 ]);

       // 如有自创武功还需保留三十级
       if (stringp(me->query("pmskills/skid")))
       {
              skills += ([ me->query("pmskills/types")     :    30 ]);
              skills += ([ me->query("pmskills/skid")      :    30 ]);
       }

       // 清除转世前的数据
       for (i = 0; i < sizeof(klist); i ++)
       {
               me->delete(klist[i]);
       }

       old_sk = me->query_skills();
       klist = keys(old_sk);
       for (i = 0; i < sizeof(klist); i ++)
       {
               me->delete_skill(klist[i]);
       }
        
       // 设置新数据
       klist = 0;
       klist = keys(newdata);
       for (i = 0; i < sizeof(klist); i ++)
       {
               me->set(klist[i], newdata[klist[i]]);
       }

       klist = 0;
       klist = keys(skills);
       for (i = 0; i < sizeof(klist); i ++)
       {
               me->set_skill(klist[i], skills[klist[i]]);
       }

       // 设置特殊技能
       me->set("special_skill/wushuang", 1); // 国士无双

       if (MEMBER_D->is_valib_member(me->query("id")) && stringp(specials))
              me->set("special_skill/" + specials, 1);

       if (specials == "bian") // 诡辩奇学
              me->set_skill("literate", 300);

       if (specials == "jinhua") // 仙音净化
       {
              me->add("int", 2);
              me->add("con", 2);
              me->add("dex", 2);
              me->add("str", 2);
       }
       if (specials == "xunying") // 遁影擒踪
       {
              me->set_skill("mathematics", 300);
              me->set_skill("count", 300);
       }

       // 转世标志数据
       me->set("scborn/ok", 1);

       me->save();

       log_file("static/scborn", sprintf("%s(%s) scborn at %s", 
                                 me->name(),
                                 me->query("id"),
                                 ctime(time())));
                                       
       return 1;
}


