// ding.c 丁春秋

inherit NPC;
inherit F_MASTER;
inherit F_COAGENT;
inherit F_QUESTER;

#include <ansi.h>
#include "xingxiu.h"

#define ANTI_ROBOT "/kungfu/class/xingxiu/robot"

void no_answer(object user);
int ask_ding();
int ask_book();
int ask_poison();
string ask_suo();
int ask_staff();
int do_answer(string arg);
int ask_job();
string ask_me();
string ask_du1();
string ask_du2();
string ask_xiangliao();

void create()
{
        set_name("丁春秋", ({ "ding chunqiu", "ding" }));
        set("nickname", "星宿老怪");
        set("long", @LONG
他就是星宿派开山祖师、令正派人士深恶痛绝的星宿老怪丁春秋。
可是他看起来形貌清奇，仙风道骨。
LONG );
        set("gender", "男性");
        set("age", 60);
        set("attitude", "friendly");
        set("class", "scholar");
        set("shen_type", -1);
        set("str", 25);
        set("int", 30);
        set("con", 28);
        set("dex", 28);
        
        set("max_qi", 5400);
        set("max_jing", 2700);
        set("neili", 6300);
        set("max_neili", 6300);
        set("jiali", 100);
        set("combat_exp", 1850000);
        set("no_get", "别人在做任务，你想把人搬到哪里去？\n");

        set_skill("force", 220);
        set_skill("huagong-dafa", 220);
        set_skill("guixi-gong", 200);
        set_skill("throwing", 210);
        set_skill("feixing-shu", 210);
        set_skill("dodge", 180);
        set_skill("zhaixinggong", 180);
        set_skill("strike", 185);
        set_skill("chousui-zhang", 185);
        set_skill("claw", 180);
        set_skill("sanyin-wugongzhao", 180);
        set_skill("whip", 180);
        set_skill("chanhun-suo", 180);
        set_skill("parry", 185);
        set_skill("staff", 190);
        set_skill("poison", 255);
        set_skill("tianshan-zhang", 190);
        set_skill("literate", 120);
        set_skill("medical", 265);
        set_skill("xingxiu-qishu", 265);

        map_skill("force", "huagong-dafa");
        map_skill("dodge", "zhaixinggong");
        map_skill("strike", "chousui-zhang");
        map_skill("claw", "sanyin-wugongzhao");
        map_skill("whip", "chanhun-suo");
        map_skill("parry", "tianshan-zhang");
        map_skill("staff", "tianshan-zhang");
        prepare_skill("strike", "chousui-zhang");
        prepare_skill("claw", "sanyin-wugongzhao");

        set("book_count", 1);
        set("zhen_count", 1);
        set("san_count", 3);
        set("suo_count",1);
        set("zhen_count", 7);
        set("wxs_count",3);
        set("xiangliao_count", random(10) + 5);

        set("inquiry", ([
                "工作" : (: ask_job :),
                "job" : (: ask_job :),
                "柔丝索" : (: ask_suo :),
                "碧磷针" : (: ask_du1 :),
                "三笑逍遥散" : (: ask_du2 :),
                "逍遥三笑散" : (: ask_du2 :),
                "香料" : (: ask_xiangliao :),
                "冰蚕剧毒" : (: ask_poison :),
                "解毒" : (: ask_poison :),
                "book" : (: ask_book :),
                "易筋经残篇" : (: ask_book :),
		"披风杖" : (: ask_staff :),
                "神木王鼎" : (: ask_ding : ),
        ]));

        create_family("星宿派", 1, "开山祖师");
        set("class", "taoist");

        set_temp("apply/damage", 80);
        set_temp("apply/armor", 100);
        set_temp("apply/attack", 80);
        set_temp("apply/dodge", 80);
        set_temp("apply/parry", 80);

        setup();
        carry_object("/clone/weapon/gangzhang")->wield();
}

void init()
{
        object ob,ob1;
        string poison;
               
        ::init();
        add_action("do_answer", "answer", 1);
        if( interactive(ob=this_player()) && ob->query_condition("bingcan_posion") ) 
        {
		   command("? " + ob->query("id"));
		   command("say " + ob->query("name") + "好徒儿,你似乎中了冰蚕剧毒。\n");
        }
	
        /*if (interactive(ob=this_player()) && ob->query("family/master_id") == "ding chunqiu"
        && ob->query("shen") > 100 && !wizardp(ob)) {
                command("say " + RANK_D->query_rude(ob) + "，你胆敢替白道的做事，显然是不想在我这混了吧。\n");
                command("expell " + ob->query("id"));
        }*/

        if (interactive(ob=this_player()) && ob->query("family/master_id") == "ding chunqiu"
        && (!present("yu di", this_player()))){
                ob1=new("/d/xingxiu/obj/yudi");
                ob1->move(this_player());
                command("say 这只玉笛是咱们星宿弟子的护身符，你好好拿着，不可弄丢。");
                write("丁春秋给你一只玉笛。\n");
        }
        add_action("do_flatter", "flatter");
}

int ask_book()
{
	object ob;
       object me=this_player();

       if(me->query("canbook")==1 && (!present("yijinjing", me)) ) {
		tell_object(me,"丁春秋说道：“好徒儿，还没参透这本易筋经的奥秘么？\n");
		tell_object(me,"那好吧。这本副本给你，拿回去好好参悟吧！\n");
		tell_object(me,"徒儿你一定要记住，千万配合神木王鼎一起修炼，要不然很容易走火入魔。”\n");
              ob=new("/clone/book/yjj_book2.c");

		ob->move(me);
		write("丁春秋给你一本易筋经。\n");
		return 1;
	}
	else
		return 0;
}

int ask_ding()
{	
	object ob;
	object me=this_player();
        if(me->query("ask/ding")==1 && (!present("shenmu wangding",me)) && me->query("family/master_id") == "ding chunqiu")
	{
		tell_object(me,"幸亏你把阿紫捉回来，神木王鼎才得以失而复返，你既然需要就拿去吧。\n");
        ob=new("/clone/misc/swmuding");
		ob->move(me);
              me->delete("ask/ding");
		return 1;
	}
	else return 0;
	
}

int ask_poison()
{
	object ob=this_player();
	if(ob->query("family/master_id" )=="ding chunqiu" && ob->query_condition("bingcan_poison")) 
	{
	tell_object(ob,HIY "要解冰蚕剧毒有两种办法，如果有易筋经残篇，
	然后配合神木王鼎修炼化功大法，这点毒算得了什么！
	可惜啊，那本残篇至尽仍然下落不明，如果你能找到那
	本书的话，我看了以后必然能弥补化功大法的缺憾。除
	此之外，只能靠药物逃过此劫了！\n" NOR);
	return 1; }
	else
	return 0;
}	

int do_answer(string arg)
{
        int result;
        object me = this_object(), user = this_player();
        result = ANTI_ROBOT->do_answer(arg, user, me);

        if (result == 2) {
                command("whisper " + user->query("id") + " 好！好！好！老仙现在放心了。");
                command("say " + user->name(1) + "，你有什么问题只管问我吧！");
                user->set("passed_ding", 1);
                return 1;
        }

        if (result == -1) {
                command("hummer " + user->query("id"));
                command("say " + user->name(1) + "你这个畜生！居然敢欺骗老仙，给我滚一边去。");
                user->add("ding_robot", 1);
                user->set("robot_ding", user->query("mud_age") + user->query("ding_robot")*600);
                return 1;
        }

        return result;
}

void no_answer(object user)
{
        if(objectp(present(user->query("id"), environment()))) {
        command("beep " + user->query("id"));
        command("heihei " + user->query("id"));
        command("say " + user->name(1) + "，你以为你装马虎我就治不了你了吗！");
        command("say 给我滚一边去吧！");
        }
else {
        command("tell " + user->query("id") + " " + user->name(1) + "，你以为你装马虎我就治不了你了吗！");
        command("tell " + user->query("id") + "你暂时用不着给老仙做事了！");
}
        user->add("ding_robot", 1);
        user->set("robot_ding", user->query("mud_age") + user->query("ding_robot")*600);
}

void attempt_apprentice(object me)
{
        if ((int)me->query("shen") > -10000)
        {
                command("say 老仙我越看你越象白道派来卧底的。");
                return;
        }

        if ((int)me->query("combat_exp") < 100000)
        {
                command("say 你这点微末道行如何能学到老仙的神妙功夫。");
                return;
        }

        if ((int)me->query_skill("poison", 1) < 50)
        {
                command("say 你毒技都没学明白，还有脸来见我？");
                return;
        }

        if( me->query_temp("pending/flatter"))
        {
                command("say 你如此不把老仙放在眼里，老仙怎会收你？");
                return;
        } else
        {
                command("say 我星宿老仙比起古往今来的圣人怎么样啊？");
                message_vision("星宿老怪微闭双眼，手捻长须，一副等人拍马(flatter)的样子。\n",
                this_player());
                me->set_temp("pending/flatter", 1);
        }
}

int do_flatter(string arg)
{
        if (! this_player()->query_temp("pending/flatter"))
                return 0;

        if (! arg) return notify_fail("你说老仙什么？\n");
        this_player()->set_temp("pending/flatter", 0);
        message_vision("$N大声说道：" + arg + "\n", this_player());
        if( strsrch(arg, "星宿老仙") >=0 && (strsrch(arg, "德配天地") >=0
         || strsrch(arg, "威震寰宇") >=0 || strsrch(arg, "古今无比") >=0 )) {
                command("smile");
                command("say 这还差不多。\n");
                welcome(this_player());
                command("recruit " + this_player()->query("id"));
        } else
        {
                command("say 你如此不把我放在眼里，我怎能收你？");
        }
        return 1;
}

// 星宿老仙，德配天地，威震寰宇，古今无比

int ask_staff()
{
	object me=this_player();
	object waguan;
	mapping fam;
	
	if (!(fam = this_player()->query("family")) || fam["family_name"] !="星宿派"){
		command("?");
		return 1;
	}
		
	if (me->query("tianshan")==1){
		command("say 不要开这种无聊的玩笑。");
		return 1;
	}
	
	if (me->query_skill("pixie-jian",1)>=10){
		command("hmm");
		command("say 你阴阳失调，不宜修炼此功。");
		return 1;	
	}
			
	if (me->query_skill("staff", 1) < 250){
		command("say 你基本杖法的修为不够。");
	}
	
	if (me->query_str() < 50) {
		command("say 你的体能太低了，好好锻炼锻炼在来问我吧。");
		return 1;
	}
	
	if (me->query_con() < 50) {
		command("say 你的内功修为太低了，好好锻炼锻炼在来问我吧。");
		return 1;
	}
		
	if (me->query_int() < 35) {
		command("sneer");
		command("say 我派武学讲究领悟，你悟性太差，回去好好读一下书吧。");
		return 1;
	}
		
	
                message_vision(HIG"$N微微一笑，将$n拉到一旁，慢慢地解释天山披风杖这一招。\n", this_object(), this_player());
                message_vision(HIG"$n在$N的讲解下，对天山杖法又有了一层新的理解。", this_object(), this_player());
                me->set("tianshan", 1);
                tell_object(this_object(),HIC"你的天山杖法进步了！");
        	me->add_skill("tianshan-zhang", 10);
                return 1;
        
        
	return 1;
}

string ask_suo()
{
        mapping fam;
        object ob;
                
        if (!(fam = this_player()->query("family")) || fam["family_name"]!="星宿派")
                return RANK_D->query_respect(this_player()) +
        "与本派素无来往，不知此话从何谈起？";

        if (query("suo_count") < 1)
                return "你来晚了，都被你师兄们拿走了。";

        if (  present("rousi suo", this_player()) )
                return "不是刚给你了吗？怎么又来要了？";

        if ( this_player()->query_skill("feixing-shu", 1) < 120)
                return "这东西你可不能玩。";

        add("suo_count", -1);

        ob = new("/d/xingxiu/obj/rousi-suo");  
        ob->move(this_player());   

        say("丁春秋给你一条柔丝索。\n");
        return "好吧，拿去好好享用。这是为师多年心血所得，你可要珍惜！";
}

string ask_me()
{
        mapping fam; 
        object ob;
        
        if (!(fam = this_player()->query("family")) || fam["family_name"] != "星宿派")
                return RANK_D->query_respect(this_player()) + 
                "与本派素无来往，不知此话从何谈起？";
        if (query("book_count") < 1)
                return "你来晚了，本派的内功心法不在此处。";
        add("book_count", -1);
        ob = new("/clone/book/dujing_2");
        ob->move(this_player());
        return "好吧，这本「星宿毒经」你拿回去好好钻研。";
}

string ask_du1()
{
        mapping fam; 
        object ob;
        
        if (!(fam = this_player()->query("family")) || fam["family_name"] != "星宿派")
                return RANK_D->query_respect(this_player()) + 
                "与本派素无来往，不知此话从何谈起？";
        if (this_player()->query_skill("poison", 1) < 50)
                return "你的毒技火候未到，用不了这种东西。";
        if (query("zhen_count") < 1)
                return "你来晚了，碧磷针都发完了。";
        add("zhen_count", -1);
        ob = new("/d/xingxiu/obj/bilinzhen");
        ob->move(this_player());
        return "好吧，那你就先用这些针吧。";
}

string ask_du2()
{
        mapping fam; 
        object ob;
        
        if (!(fam = this_player()->query("family")) || fam["family_name"] != "星宿派")
                return RANK_D->query_respect(this_player()) + 
                "与本派素无来往，不知此话从何谈起？";
        if (this_player()->query_skill("poison", 1)<100)
                return "你的毒技火候未到，用不了这种东西。";
        if (query("san_count") < 1)
                return "你来晚了，现在没什么剩的了。";
        add("san_count", -1);
        ob = new("/d/xingxiu/obj/sanxiaosan");
        ob->move(this_player());
        return "好吧，这包毒药你拿回去善加利用吧。";
}

string ask_xiangliao()
{
        mapping fam;
        object ob;

        if (!(fam = this_player()->query("family")) || fam["family_name"] != "星宿派")
                return RANK_D->query_respect(this_player()) + 
                "与本派素无来往，不知此话从何谈起？";
        if (is_busy() || is_fighting())
                return "等会儿，没看我这正忙着呢么？";
        if (this_player()->query_skill("poison", 1) < 40)
                return "你的毒技火候太差，还是练练再说吧！";
        if (query("xiangliao_count") < 1)
                return "你来晚了，现在没什么剩下的香料了，你还是去仓库找找看吧！";
        add("xiangliao_count", -1);
        ob = new("/clone/misc/xiangliao");
        ob->set_amount(random(50) + 100);
        ob->move(this_player());
        start_busy(2);
        return "好吧，这些香料你拿去慢慢用吧。";
}

int ask_job(){
	
	object me=this_player();
	object waguan;
	mapping fam;
	
	if (!(fam = this_player()->query("family")) || fam["family_name"] !="星宿派"){
		command("?");
		return 0;
	}
		
        if (me->query("robot_ding") > me->query("mud_age")) {
                command("say 哼！上次你骗老仙的事，老仙还没有和你算帐呢！你现在还有脸问这问那！");
                command("say 你先给我回去好好反省自己一下，以后再来！");
                return 1;
        }

        if(me->query("robot_answer")) return notify_fail("快给老夫把先前的问题答了！\n");

        if (random(4) == 1 && !me->query("passed_ding")) {
                command("say 昨夜老夫受仙人告戒，说你" + me->name(1) + "做事不老实。用写机器来欺骗老夫");
                command("say 所以老夫现在要问你一问，你听好了！");
                ANTI_ROBOT->get_question(me, this_object());
                return 1;
        }

        if(me->query_temp("xx_job")==1) {
                command("say 好小子，上一个工作没有完成就敢来问我工作。");
                return 1;
        }
        command ("say 「老仙我最近练功需要一些毒丹，你到后山拿些来给我吧。」");
	waguan=new("/d/xingxiu/obj/waguan");
        me->set_temp("xx_job",1);
	if (!present("wa guan", me)) waguan->move(me);
	    message_vision("丁春秋顺手从身后架子上取出一个小瓦罐扔给你。\n", me);
		command ("say 「快去快回，路上小心。」");
	return 1;

}

int accept_object(object who, object ob){

	object me=this_player();
	mapping fam;
	int pot, exp;
        string msg;

	if (!(fam = this_player()->query("family")) || fam["family_name"] !="星宿派"){
		command("?");
		return 0;
	}		

	if(ob->query("id")=="du dan"){

              if (this_player()->query_temp("xx_job")!=1 || ob->query("player")!=me->query("id")) {
			command("hehe");
			return 0;
		} else {
                     command("pat "+this_player()->query("id"));        
			command("say 好样的，老仙我越看你越顺眼了。\n");
			// exp triple
			pot = random(100)+100;
			exp = 200 + random( 50 );

          if(me->query("combat_exp") > 6400000)
          {
            pot /= 2;
            exp = exp / 2;
          }
			me->add("combat_exp", exp);
			me->add("potential", pot);
			if(me->query("potential")>me->query_potential_limit())
				me->set("potential",me->query_potential_limit());
                        msg = sprintf("你抓虫得到 %d 点经验和 %d 点潜能。\n", exp, pot);
                        tell_object(me,msg);
			log_file("/job/xx-zhua",sprintf("[%s] %s(%s) 抓虫得到 %d 点经验和 %d 点潜能。\n", ctime(time()), me->name(), me->query("id"), exp, pot));
			me->delete_temp("xx_job");
			me->delete_temp("found");
			me->delete_temp("bug_hold");
			me->delete_temp("bug_out");
			call_out("destob",1,ob);
                     me->delete("passed_ding");
			return 1;
		}
	}
	if(ob->query("id")=="a zi"){

                {
                        command("say 太好了，终于有把这个孽徒抓回来了！\n");
                        command("say 来人哪！把这个孽徒给我先关到牢房里面去！\n");
                        tell_object(me,"门外进来两名星宿弟子把阿紫拖出去了。\n");
			pot = random(300)+80;
			exp = 500 + random( 20 );
			me->add("combat_exp", exp);
			me->add("potential", pot);
			if(me->query("potential")>me->query_potential_limit())
				me->set("potential",me->query_potential_limit());
			me->set("ask/ding", 1);
                        destruct(ob); 
			return 1;
		}
	}
	if(ob->query("id")=="noname book")
	{
        if (!ob->query("my_owner") || ob->query("my_owner") != me)
                return notify_fail("丁春秋怒道：这是你哪里拣来的破东西，还不扔了！\n");
		if(me->query("ask/ding")==1)
		{
			tell_object(me,"这……这是易筋经残篇！！好
徒儿，咱们星宿一统江湖的日子来了！此书既然由你取得，日后你当然也可
以修习，待为师将其译出并抄录副本。咱们师徒共同研究！\n");
			me->set("canbook",1);
			return 1;
		}
		else
		return notify_fail("丁春秋怒道：“我老仙是何等身份？！怎会收你这种垃圾，我不要！”\n");
	}
			
	if(ob->query("id")=="ling pai"){
		if(ob->query("player")!=me->query("id")){
			command("hehe");
			return 0;
		} else {
			command("hehehe");
			command("say 好样的，让这些自称名门正派的知道老仙的利害。\n");
			if (ob->query("menpai")=="huashan" || ob->query("menpai")=="quanzhen")
				exp = 100 + random( 50 );
			else  if(ob->query("menpai")=="mingjiao") {
				command("say 嗯。。明教竟然也跟我干上了。\n");
				//exp triple
				exp = 120 + random( 60 );
			} else	exp = 70+random(30);
          if(me->query("combat_exp") > 6400000)
          {
            exp = exp / 2;
          }
			me->add("combat_exp", exp);
                        msg = sprintf("你捡令牌得到 %d 点经验。\n", exp);
                        tell_object(me,msg);
			log_file("/job/xx-zhua",sprintf("[%s] %s(%s) 捡令牌得到 %d 点经验。\n", ctime(time()),me->name(), me->query("id"), exp));			
			call_out("destob",1,ob);
                        me->delete("passed_ding");
			return 1;					
		}
	}
	
	return ::accept_object(who, ob);
}	

void destob(object ob){
	destruct(ob);
}

void reset()
{
        set("book_count", 1);
        set("zhen_count", 1);
        set("san_count", 3);
        set("suo_count",1);
        set("zhen_count", 7);
        set("wxs_count",3);
        set("xiangliao_count", random(10) + 5);
}
