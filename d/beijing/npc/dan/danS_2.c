#include <ansi.h>
inherit ITEM;
void create()
{
        set_name(HIR"朱雀"NOR+HIY"神农丹"NOR, ({"zhuque dan", "dan"}));
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "粒");
                set("no_drop", "这样东西不能离开你。\n");
                set("no_sell", "这样东西不能离开你。\n");
                set("no_put", "这样东西不能放在那儿。\n");
                set("no_get", "这样东西不能离开那儿。\n");
                set("no_steal", "这样东西不能离开那儿。\n");
                set("no_beg", "这样东西不能离开那儿。\n");
                set("only_do_effect", 1);
        }
        setup();
}

int do_effect(object me)
{
        mapping my;

        if (time() - me->query_temp("last_eat/dan(S)") < 600)
        {
                write("你刚服用过药，需药性发挥完效用以后才能继续服用。\n");
                return 1;
        }

        my = me->query_entire_dbase();

        if( me->is_busy() )
                return notify_fail("急什么，小心别噎着了。\n");

        me->set_temp("last_eat/dan(S)", time());

        message_vision(HIR
"    $N吃下一颗朱雀神农丹，只觉全身筋脉逆流而上, 内力源源不断的涌入丹田, 说\n"
"不出的舒服受用。顿时感到灵台处如湖面一般平静, 以往所学的武学知识一一涌向心\n"
"头, 在灵台处交融贯通。$N感到的经验和潜能有了一定的进展。\n"NOR,me);
        me->set("jingli",me->query("max_jingli"));
        me->set("neili",me->query("max_neili"));
        me->add("combat_exp", 250);
        me->add("potential", 250);

        if (me->query("potential") > me->query_potential_limit()) 
        me->set("potential", me->query_potential_limit()); 

        me->start_busy(2);
        destruct(this_object());
        return 1;
}
void owner_is_killed()
{
        destruct(this_object());
}
