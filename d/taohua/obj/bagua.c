// Code of JHSH
// /d/taohua/obj/bagua.c
// Lara 2001/11/28
// snowyu 2002/1
#include <ansi.h>
inherit ITEM;

#include "/d/REGIONS.h";

#define ROUTER "/clone/obj/traverser"
/******************* 这里定义占卜busy***********************/
#define zhanbu_busy 10
/*********************************************************/

static object route_finder;
static object target_room;
// 0: idle, 1: searching
static int status;

int do_route(string, object);
int do_printroute(object);
void init();

void create()
{
	set_name(GRN"铁八卦"NOR, ({ "tie bagua", "bagua" }) );
	set_weight(10000);
	if( clonep() )
		set_default_object(__FILE__);
	else
	{
		set("long", "铁八卦是纯铁所铸，分量极重，上有八卦图案。\n"
			"出阵指令：chuzhen <方向>\n");
		set("unit", "枚");
		set("wield_msg", GRN "$N由口袋里拿出$n。\n" NOR );
		set("unwield_msg", GRN "$N将$n放回口袋中。\n" NOR );
		set("value", 1600);
		set("tanzhi",200);
	}
	setup();
}

void init()
{
	add_action("do_divine", "divine");
	add_action("do_divine", "zhanbu");
	add_action("do_chuzhen", "chuzhen");
	add_action("do_printroute", "printroute");
}

int do_divine(string arg)
{
	object ob, where, env, me = this_player();
	mapping myfam;
	string region;
	int i, lvl, diff; 

	if (me->is_busy() || 
            me->query_temp("pending/exercising") || 
            me->query_temp("exit_blocked")) 
		return notify_fail("你现在正忙着呢。\n");

	myfam = (mapping)me->query("family");	

	if( !myfam || myfam["family_name"] != "桃花岛" ) 
		return notify_fail("你非桃花弟子，不能将奇门五行用于占卜。\n");

	if( (lvl = (int)me->query_skill("qimen-wuxing", 1)) < 60 ) 
		return notify_fail("你奇门五行等级不够。\n");

	if( me->query("jingli") < 100 ) 
		return notify_fail("你的精力不够，不足以占卜！\n");

	if( me->query("jing") < 100 ) 
		return notify_fail("你的精神不够，不足以占卜！\n");

	if (!arg) 
		return notify_fail("你要占卜谁的位置？\n");

	if( !(ob = find_player(arg)) && !(ob = find_living(arg)) ) 
			return notify_fail("现在没这个人.\n");

	message_vision(HIG"$N拿起一只铁八卦屏心凝气，拇指依次掐向铁八卦上子、丑、寅、卯、辰、巳、\n午、未、申、酉、戌、亥各个方位，细心推算起来......\n\n"NOR, me);

	me->add("jing", -50);
	me->add("jingli", -50);

	if ( strsrch(base_name(ob), "/kungfu/class/generate/") == 0 )
		return notify_fail("这个人不知道在那里耶...\n");

	if( !(where = environment(ob))) 
		return notify_fail("这个人不知道在那里耶...\n");

	if( !me->visible(ob)) 
		return notify_fail("这个人不知道在那里耶...\n");

	if( !wizardp(me) && wizardp(ob) ) 
		return notify_fail("嘿嘿！竟然敢对巫师动什么歪念头么？\n");

	if( undefinedp(region = region_names[getuid(where)]) ) 
		return notify_fail("这个人不知道在那里耶...\n");

	env = environment(me);

        diff = (int)(me->query("combat_exp")/(ob->query("combat_exp")+1));

	if( lvl > 120 && !ob->query_temp("apply/name") && 
	    random(lvl / 10 + me->query_int() + diff) > ob->query("kar")*2 )
	{ 
		me->add("jing", -random(200-lvl));
		printf("%s(%s)现在在%s%s。\n", 
			(string)ob->name(), 
			(string)ob->query("id"), 
			region, 
			(string)where->query("short"));
		if( lvl > 140 )
			do_route(file_name(where), me);
		if( (random(lvl/3)+diff) < ob->query("kar") ) 
			tell_object(ob, HIB"你感觉心神不定，似乎有人在暗中窥视着你......\n"NOR);
	}
	else if( me->query_int() > random(ob->query("kar") * 3) )
	{
		me->add("jing", -random(200-lvl));
		printf("%s(%s)现在在%s一带。\n", 
			(string)ob->name(), 
			(string)ob->query("id"), 
			region);
	 	if( (random(lvl/3)+diff) < ob->query("kar")/2 ) 
			tell_object(ob, HIB"你感觉心神不定，似乎有人在暗中窥视着你......\n"NOR);
	}
	else
	{
		write("你殚精竭虑推算了半晌，却发现一无所得…… \n");
		me->set("jing", 10);
	}
	me->start_busy(2);
	return 1;
}

int do_chuzhen(string arg)
{
	object obj, env, me, owner;
	string dest;
	mapping exit;

	if(!arg) return notify_fail("你要从哪个方向出阵？\n");

	me = this_player();

	if (me->is_busy() || 
	    me->query_temp("pending/exercising") || 
            me->query_temp("exit_blocked")) 
		return notify_fail("你现在正忙着呢。\n");

	env = environment(me);	

	if( !env->query("th_buzhen") ) 
		return notify_fail("这里并无布有奇门阵法。\n");

	if( env->query("th_zhen_owner") == me->query("id"))
		return notify_fail("你是这个阵的主人，只能破阵，不能离开阵法！\n");

	if( !objectp(owner = find_player(env->query("th_zhen_owner"))) ||
	    environment(owner) != env)
	{
	        env->set("long", env->query("org_desc"));
        	env->set("exits", env->query("org_exits"));
	        env->set("cost", env->query("org_cost"));
        	env->delete("org_desc");
	        env->delete("org_cost");
        	env->delete("org_exits");
	        env->delete("th_buzhen");
        	env->delete("th_pozhen");
		env->delete("th_zhen_owner");
	
        	tell_object(env, CYN"一阵青雾升腾而起又四散而去，四面景致猛然一变。\n"NOR);
		return 1;
	}


	if( me->query_skill("qimen-wuxing", 1) < 60 ) 
		return notify_fail("你的奇门五行修为不够，无法看出阵法秘奥。\n");

	if( me->query("jing") < 50)
		return notify_fail("你的精神不足。\n");
	
	if( me->query_skill("qimen-wuxing",1) < env->query("th_pozhen")/3 )
	{
		message_vision(CYN"$N坐在地上冥思苦想，突然脸色大变，口吐鲜血！\n"NOR,me);
		me->set("jing",1);
		return 1;
	}

	exit = env->query("org_exits");
	dest = exit[arg];

	if(undefinedp(exit[arg])) 
		return notify_fail("这个方向没有出路。\n");

	if( !(obj = find_object(dest)) ) call_other(dest, "???");
	if( !(obj = find_object(dest)) ) 
		return notify_fail("这个方向没有出路。\n");

	message_vision(CYN"$N凝思片刻，身形左一转，右一旋，忽然消失得无影无踪。\n"NOR,me);	

	if( me->move(obj) )
	{
		message( "vision", me->name()+"快步走了过来。\n"NOR, 
		environment(me), ({me}) );
		return 1;
	}

	return 0;
}

int visit_room(string room_path, mapping traverse_info, mapping room_info, string target)
{
	string room_short, tmp;

	if (! room_info) return 0;
	// taking care of the colors in names
	room_short=explode(room_info["short"], ESC)[0];
	if (sscanf(room_short, "[%*dm%s", tmp) == 2) room_short = tmp;
	if (sscanf(room_short, "[%*d;%*dm%s", tmp) == 3) room_short = tmp;
	if (room_short == target)
	{
		target_room = find_object(room_path);
		if (! target_room)  target_room = load_object(room_path);
		// something wrong with the room, return 1 to prune the search branch
		if (! target_room)
		return 1;
		return -1;   // found room, done
	}
	return 0;      // otherwise, keep going
}

int do_route(string arg, object me)
{
	int ready;

	if (status == 1) return notify_fail("正在搜索路途. 用 printroute 来显示结果。\n");
	if (objectp(route_finder)) destruct(route_finder);
	route_finder = new (ROUTER);
	if (! objectp(route_finder))
	{
		target_room = 0;
		return notify_fail("奇怪,不能造出 "+ROUTER+"?\n");
	}
	status = 1;
	target_room = find_object(arg);
	if (! target_room)  target_room = load_object(arg);
	if (! objectp(target_room))
	{
	    // return notify_fail("目标房间 "+arg+" 不存在.\n");
	    // assume arg is a short name, not file path
	    ready = route_finder->traverse(base_name(environment(me)), (: visit_room :), arg);
	}
	else
	{
		ready = route_finder->find_route(base_name(environment(me)), base_name(target_room));
	}
	if (ready)  return do_printroute(me);
	return notify_fail("已经开始搜索路途，用 printroute 来显示结果.\n");
}

int do_printroute(object player)
{
	object me;
	mapping info;
	mixed   route;
	int     i, n, wizp, lvl;
	object  room;
	string  room_file, room_short;

	if (player) me = player;
	else me = this_player();
	lvl = me->query_skill("qimen-wuxing",1);
	if (status == 0) return notify_fail("你又没有占卜，查什么路途.\n");
	if (! objectp(route_finder))
	{
		status = 0;
		target_room = 0;
		route_finder = 0;
		return notify_fail("奇怪，object maproute 不见了.\n");
	}
	if (! route_finder->traverse_finish())
	{
		return notify_fail("还没找到，请等等再来查.\n");
	}
	status = 0;
	if (target_room)
	{
		route = route_finder->thread_route(base_name(target_room));
	}
	else
	{
		status = 2;
	}
	target_room = 0;
	destruct(route_finder);
	route_finder = 0;

	if (status == 2)
	{
		return notify_fail("没有这个房间.\n");
		status = 0;
	}
	if ((int)route == 1)
	{
		return notify_fail("没有去此房间的道路.\n");
	}
	if (sizeof(route) <= 1)
	{
		return notify_fail("你不就在这儿吗？\n");
	}
	printf("找到了道路，从这里开始：\n");
	n = sizeof(route);
	if(n > lvl/20) n = lvl/20;
	wizp = wizardp(me);
	for (i=1; i<n; i++)
	{
		room_file = keys(route[i])[0];
		room = find_object(room_file);
		if (! objectp(room) ) room = load_object(room_file);
		if (objectp(room))
		{
			room_short=room->query("short");
		}
		else
		{
			room_short="不知道哪儿";
	    }
	    info = route[i][room_file];
	    if (info["dir"] == "cross_river")
		{
			printf("摆渡过河");
		}
		else if (strsrch(info["dir"], "out_sea")==0)
		{
			printf("坐船渡海");
		}
		else
		{
			printf("往 %s ", info["dir"]);
		}
		printf("到 %s %s, 距离 %s步\n", room_short, wizp?"("+room_file+")":"", chinese_number(info["dist"]));
	}
	if(n < sizeof(route)) printf("......算着算着，你发现距离太远了，头脑开始胡涂起来.\n");
	return 1;
}