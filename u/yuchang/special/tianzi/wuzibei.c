// 无字碑
#include <ansi.h>
inherit ITEM;
void create()
{
        set_name(HIY "无字碑" NOR, ({ "wuzi bei", "bei" }));
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", HIW "石碑上面空无一字，但是据说真命天子可以从这面碑上\n"
                            "(mianbi)到一门极为厉害的剑法。\n" NOR);
                set("unit", "面");               
                set("no_sell",1);
                set("weight", 10000);
                set("no_get",1);                
             }
        setup();
}

void init()
{
        add_action("do_mianbi", "mianbi");
        add_action("do_canwu", "canwu");
}

int do_mianbi()
{
        object me;
        int s_skill,t_skill;

        me = this_player();

        if ( ! me->query("special_skill/emperor") )
        return 0;
        write(HIW"只见无字碑上隐隐约约现出一些人舞剑的身影。\n"NOR);    

        s_skill = (int)me->query_skill("sword", 1);
        t_skill = (int)me->query_skill("tianzi-jian", 1);

        if (s_skill < t_skill)
        {
                message_vision("$N对基本剑法的了解太低，无"
                               "法领悟无字碑上的内容。\n", me);
                return 1; 
        }

        if (me->query("jing")< 90)
        {
                message_vision("$N太累了，现在无法领悟无字碑上的内容。\n",me);
                return 1; 
        }

        if (t_skill >= 180)
        {
                message_vision("$N觉得无字碑上的内容太肤浅了。\n", me);
                return 1;
        }

        message_vision("$N面对着无字碑趺坐静思，良久，对天子"
                       "剑法似有所悟。\n", me);
        me->improve_skill("tianzi-jian", 1 + random(me->query("int")));
        me->receive_damage("jing", 75);
        return 1;
}

int do_canwu(string arg)
{
    object me = this_player();
    

    if ( ! me->query("special_skill/emperor") )
        return 0;    
  
    if (me->is_busy())
          return notify_fail("你现在正忙着呢。\n");

    if (me->is_fighting())
          return notify_fail("你还是先打完架再说吧！\n");

    if ( ! arg || ( arg != "天子降世" && arg != "君临天下"
         && arg != "九洲臣服") )
          return notify_fail("你要领会什么绝招？格式为(canwu 绝招)。\n");

    if ( arg == "天子降世" 
        && me->query("can_perform/tianzi-jian/jiangshi"))
          return notify_fail("这招你不是已经会了吗？\n");

    if ( arg == "九洲臣服" 
        && me->query("can_perform/tianzi-jian/jiuzhou"))
          return notify_fail("这招你不是已经会了吗？\n");
  
    if ( arg == "君临天下" 
        && me->query("can_perform/tianzi-jian/junlin"))
          return notify_fail("这招你不是已经会了吗？\n");

    if ((int)me->query("jing") < 100)
          return notify_fail("你的精力无法集中，无法领会绝招！\n");
 
    me->receive_damage("jing", 90);

    me->start_busy(2 + random(4));

    if (random(5) == 1 && arg == "天子降世" )
    {
        me->add("tianzi-jian/jiangshi/count", 1);

        if (me->query("tianzi-jian/jiangshi/count") >= 10)
        {
            write(HIM "猛然间，你胸中豁然贯通，再无疑虑。\n" NOR);
            write(HIC "你终于通晓了绝学「天子降世」。\n" NOR);

            me->set("can_perform/tianzi-jian/jiangshi", 1);
            me->delete("tianzi-jian/jiangshi/count");

            if (me->can_improve_skill("sword"))
                    me->improve_skill("sword", 1500000);
            if (me->can_improve_skill("tianzi-jian"))
                    me->improve_skill("tianzi-jian", 1500000);
            if (me->can_improve_skill("martial-cognize"))
                    me->improve_skill("martial-cognize", 1500000);
            return 1;
        }

        write(HIC "你对「天子降世」这招有了新的认识，但是仍有许多不解之处。\n" NOR);
        me->start_busy(2 + random(4));
        return 1;   
    }


    if (random(10) == 1 && arg == "九洲臣服" )
    {
        me->add("tianzi-jian/jiuzhou/count", 1);

        if (me->query("tianzi-jian/jiuzhou/count") >= 10)
        {
            write(HIM "猛然间，你胸中豁然贯通，再无疑虑。\n" NOR);
            write(HIC "你终于通晓了绝学「天子降世」。\n" NOR);

            me->set("can_perform/tianzi-jian/jiuzhou", 1);
            me->delete("tianzi-jian/jiuzhou/count");

            if (me->can_improve_skill("sword"))
                    me->improve_skill("sword", 1500000);
            if (me->can_improve_skill("tianzi-jian"))
                    me->improve_skill("tianzi-jian", 1500000);
            if (me->can_improve_skill("martial-cognize"))
                    me->improve_skill("martial-cognize", 1500000);
            return 1;
        }

        write(HIC "你对「九洲臣服」这招有了新的认识，但是仍有许多不解之处。\n" NOR);
        me->start_busy(2 + random(4));
        return 1;   
    }

    if (random(15) == 1 && arg == "君临天下")
    {
        me->add("tianzi-jian/junlin/count", 1);

        if (me->query("tianzi-jian/junlin/count") >= 20)
        {
            write(HIM "猛然间，你胸中豁然贯通，再无疑虑。\n" NOR);
            write(HIC "你终于通晓了绝学「君临天下」。\n" NOR);

            me->set("can_perform/tianzi-jian/junlin", 1);
            me->delete("tianzi-jian/junlin/count");

            if (me->can_improve_skill("sword"))
                    me->improve_skill("sword", 1500000);
            if (me->can_improve_skill("tianzi-jian"))
                    me->improve_skill("tianzi-jian", 1500000);
            if (me->can_improve_skill("martial-cognize"))
                    me->improve_skill("martial-cognize", 1500000);

            return 1;
        }

        write(HIC "你对「君临天下」这招有了新的认识，但是仍有许多不解之处。\n" NOR);
        me->start_busy(2 + random(4));
        return 1;   
    }
        return notify_fail("你试图从无字碑上领会有关「" + arg + "」的内容，\n"
                           "但是这次没什么效果，看来得再来过！\n");

}
