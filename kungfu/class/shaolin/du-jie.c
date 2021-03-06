// dujie.c

inherit NPC;
inherit F_MASTER;

mixed ask_me();

void create()
{
	set_name("渡劫", ({ "du jie", "jie" }) );
	set("title", "少林长老");
	set("gender", "男性");
	set("age", 90);
	set("long",
		"这是一个面颊深陷，瘦骨零丁的老僧，他脸色惨白，象一张纸一样。\n");

	set("attitude", "peaceful");
	set("class", "bonze");
	set("combat_exp", 2300000);

	set("str", 32);
	set("int", 30);
	set("con", 33);
	set("dex", 31);

	set("qi", 5000);
	set("max_qi", 2500);
	set("neili", 6700);
	set("max_neili", 6700);
	set("jiali", 90);

	create_family("少林派", 35, "弟子");
	assign_apprentice("弟子", 0);

	set_skill("buddhism", 250);
	set_skill("force", 255);
	set_skill("whip", 255);
	set_skill("parry", 250);
	set_skill("dodge", 240);

	set_skill("hunyuan-yiqi", 255);
	set_skill("shaolin-xinfa", 255);
	set_skill("riyue-bian", 255);
	set_skill("shaolin-shenfa", 240);

	map_skill("force", "hunyuan-yiqi");
	map_skill("whip",  "riyue-bian");
	map_skill("parry", "riyue-bian");
	map_skill("dodge", "shaolin-shenfa");

	set_skill("finger", 250);
	set_skill("jingang-zhi", 250);

	map_skill("finger", "jinggang-zhi");

	prepare_skill("finger", "jingang-zhi");

	set_skill("medical", 200);
	set_skill("shaolin-yishu", 200);

	set("inquiry", ([
		"金刚罩"     : (: ask_me :),
	]));

	setup();

	carry_object("/d/shaolin/obj/changbian")->wield();
}

mixed ask_me()
{
	mapping fam; 
	object ob;
	
	if (! this_player()->query_temp("valid_in_fumoquan"))
	{
		command("say 大胆鼠辈，乳臭未干，竟敢偷入金刚伏魔圈，且让老衲来超度与你！");
		kill_ob(this_player());
                return 1;
	}

        if (this_player()->query_temp("valid_in_fumoquan") != 1)
        {
                command("heng");
                return 1;
        }

	if (present("jingang zhao", this_player()))
		return RANK_D->query_respect(this_player()) + 
		"宝罩只有一件，而且就在你身上，真是贪得无餍！";

	if (present("jingang zhao", environment()))
		return RANK_D->query_respect(this_player()) + 
		"宝罩只有一件，而且就在这里任你取走，真是贪得无餍！";

	ob = get_object("/d/shaolin/obj/jingang-zhao");
        if (! ob || environment(ob) && environment(ob) != this_object())
                return "抱歉，你来晚了，金刚罩已经给人取走了。";

	ob->move(this_player());

	message_vision("\n渡劫一声不吭地瞧了$N半饷，扭过身，从树洞里取出金刚罩递给$N。\n\n", this_player());

	return "你能挨我们三个老不死的三招日月神鞭不死，确实是命大，这件金刚罩就是你的了！";
}

#include "/kungfu/class/shaolin/du.h"
