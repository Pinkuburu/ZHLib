// yang.c 杨过

inherit NPC;
inherit F_MASTER;
inherit F_GUARDER;

#include <ansi.h>
#include "gumu.h"

string ask_yunv();
mixed ask_me();
int not_to_learn_ss();

void create()
{
        set_name("杨过", ({"yang guo", "yang"}));
        set("gender", "男性");
        set("age", 26);
        set("per", 20);
        set("long", @LONG
他看上去眉目之间有一股清秀之气，然而隐隐约约却让人感到一阵哀伤之情，
左袖空荡荡的，显然已是残疾。
LONG );
        set("attitude", "friendly");

        set("str", 35);
        set("int", 36);
        set("con", 34);
        set("dex", 32);

        set("chat_chance_combat", 80);
        set("chat_msg_combat", ({
                (: exert_function, "recover" :),
                (: perform_action, "sword.juan" :),
                (: perform_action, "sword.juan" :),
                (: perform_action, "sword.dongqu" :),
                (: perform_action, "sword.dongqu" :),
        }));

        set("qi", 5800);
        set("max_qi", 5800);
        set("jing", 2900);
        set("max_jing", 2900);
        set("neili", 8500);
        set("max_neili", 8500);
        set("jiali", 120);

        set("combat_exp", 2750000);

        set_skill("force", 290);
        set_skill("nuhai-kuangtao", 300);
        set_skill("yunv-xinfa", 290);
        set_skill("sword", 280);
        set_skill("yunv-jian", 280);
        set_skill("quanzhen-jian", 280);
	set_skill("xuantie-jian", 280);
        set_skill("dodge", 250);
        set_skill("yunv-shenfa", 250);
        set_skill("parry", 285);
        set_skill("cuff", 285);
        set_skill("meinv-quan", 270);
        set_skill("strike", 280);
        set_skill("sad-strike", 280);
        set_skill("fireice-strike", 280);
        set_skill("literate", 150);
        set_skill("taoism", 80);
	set_skill("medical", 180);
	set_skill("yunv-xinjing", 180);
        set_skill("throwing", 240);
        set_skill("yufeng-zhen", 240);

        map_skill("force", "nuhai-kuangtao");
        map_skill("sword", "xuantie-jian");
        map_skill("dodge", "yunv-shenfa");
        map_skill("parry", "meinv-quan");
        map_skill("cuff", "meinv-quan");
        map_skill("strike", "sad-strike");

        prepare_skill("strike", "sad-strike");

        create_family("古墓派", 3, "弟子");

        set("inquiry", ([
                "王重阳" : "嘿...嘿！",
	        "郭靖"   : "他可真是真正的大侠。",
		"黄蓉"   : "黄师叔么...真不好说啊。",
		"小龙女" : "你打听我妻子干什么？",
                "杨康"   : "算了，不提他也罢。",
                "玉蜂针" : "我手头可没有，很久没有用它了。",
                "万念俱灰": (: ask_me :),
        ]));

	set("guarder", ([
		"refuse_home" : "$N对$n冷冷道：滚！",
		"refuse_other": "$N对$n道：后面是古墓弟子练功的地方，你少要打扰！",
		"refuse_carry": "$N对$n喝道：把你身上得人给我撂下！",
	]));

        set("no_teach/sad-strike", (: not_to_learn_ss :));

        setup();

        carry_object("/clone/weapon/changjian")->wield();
        carry_object("/clone/cloth/qingyi")->wear();
}

int not_to_learn_ss()
{
	object ob = this_player();

        if (ob->query("gender") == "女性")
        {
                command("say 我这门武功不适合女孩子学习，还是算了吧！");
                return 1;
        }

        if (ob->query_temp("can_learn/yang/sad-strike"))
                return 0;

        if (ob->query("family/family_name") != query("family/family_name"))
	{
                command("say " + RANK_D->query_respect(ob) + "你不是我的亲传弟子，找我何干？");
		return 1;
	}

        if (ob->query("static/sadly") < 3)
        {
                command("say 你体会不到那种思念妻子的情怀，"
                        "无法领会我的黯然销魂掌！");
                return 1;
        }

        message_vision("$n看了看$N，道：好！你我也算是颇有"
                       "渊源，我就传你黯然销魂掌！\n",
                       ob, this_object());
        ob->set_temp("can_learn/yang/sad-strike", 1);
        return 0;
}

int recognize_apprentice(object ob, string skill)
{
	if (skill == "unarmed" &&
	    this_player()->query_skill("sad-strike", 1) > 0 &&
	    this_player()->query_skill(skill, 1) < 180)
		return 1;

        if (skill != "sad-strike")
        {
                command("say 你还是找你的师傅去学习吧！");
                return 0;
        }

        if (not_to_learn_ss())
                return 0;

        if (this_player()->query("family/family_name") != query("family/family_name"))
	{
                command("say " + RANK_D->query_respect(this_player()) + "你不是我的亲传弟子，找我何干？");
		return 0;
	}

        return 1;
}

void attempt_apprentice(object ob)
{
        if (! permit_recruit(ob))
                return;
	
	if (ob->query("gender") == "女性" &&
	    ob->query("per") < 25)
	{
		command("say 你的容貌太差了点吧？");
		return;
	}

        if (ob->query_int() < 32)
	{
                command("say 你的资质不够，难以领会上乘武功。");
                return;
        }

        if ((int)ob->query_skill("force") < 150)
	{
                command("say 想拜我为师么？你把内功练好了再来吧！");
                return;
        }

        if ((int)ob->query("max_neili") < 1000)
	{
                command("say 你把内力锻炼好了再来也不迟啊！");
                return;
        }

        command("say 好吧，看你也是性情中人，我就收下你这个徒弟了。");
        command("recruit " + ob->query("id"));
}

mixed ask_me()
{
        object me;

        me = this_player();
        if (me->query("can_perform/sad-strike/wannian"))
                return "自己好好多练习吧！";

        if (me->query("family/family_name") != query("family/family_name"))
                return RANK_D->query_respect(me) + "你不是我的亲传弟子，何出此言？";

        if (me->query("shen") < 200000)
                return "你行侠仗义的事情做的还很不够，我不能传你这一招！";

        if (me->query_skill("sad-strike", 1) < 180)
                return "你的黯然销魂掌还不够熟练，我还不能教你这一招！";

        message_vision(HIM "$n" HIM "慢慢运起内功，然后以疾风般的速度向$N"HIM"挥出一掌！\n"
                       HIM "$n" HIM "这一掌虽然奇快无比，但是$N"HIM"确把一招的来势看得很透澈，然后轻而易举的避开了！\n"
                       "正当$N" HIM "奇怪为什么这一招这么差的时候，$n"HIM"的掌势突然向旁边一转，正好打在了$N"HIM"的胸口上！！！\n"
                       "$N"HIM"大吃一惊，躲避已经不行，连忙动起内功硬生生的受了这一招，但是......\n" NOR,
                       me, this_object());
        command("sigh");
        command("say 你就先躺一下吧。");
        tell_object(me, HIG "你学会了万念俱灰。\n" NOR);
        if (me->can_improve_skill("unarmed"))
                me->improve_skill("unarmed", 160000);
        me->set("can_perform/sad-strike/wannian", 1);
        me->unconcious();
        return 1;
}
