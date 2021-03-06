// beimin-shengong.c 北冥神功
// By Hop
// Update by smallbear 03.04.29

#include <ansi.h>
inherit FORCE;

int valid_enable(string usage) { return usage=="force"; }

int valid_force(string force)
{
        return force == "xiaoyao-xinfa" ||
               force == "xiaowuxiang";
}

int query_neili_improve(object me)
{
        int lvl;

        lvl = (int)me->query_skill("beiming-shengong", 1);
        return lvl * lvl * 15 * 20 / 100 / 200;
}

int valid_learn(object me)
{
        int lvl = (int)me->query_skill("beiming-shengong", 1);

        if (me->query("gender") == "无性" && lvl > 49)
                return notify_fail("你无根无性，阴阳不调，难以领会高深的北冥神功。\n");

        if (me->query("int") < 32)
                return notify_fail("你觉得北冥神功艰深无比，难以领会。\n");

        if (me->query("con") < 24)
                return notify_fail("你试着运转了一下内力，眼前登时一黑！\n");

        return ::valid_learn(me);
}

int practice_skill(object me)
{
    return notify_fail("北冥神功只能用学的，或是从运用(exert)中增加熟练度。\n");
}

mixed hit_by(object me, object victim, int damage, int damage_bonus, int factor)
{
        int percent,force1,force2,dodge1,dodge2,lv1,lv2;

        force1 = victim->query_skill("force"); 
        force2 = me->query_skill("force"); 
        dodge1 = victim->query_skill("dodge"); 
        dodge2 = me->query_skill("dodge"); 

        lv1= force1*dodge1;
        lv2= force2*dodge2;

        lv2 = lv2*3/2;
        if ((int)victim->query_skill("beiming-shengong",1)>100
         && !me->query_temp("weapon")
         && me->query("max_neili")>500
        && lv1/2 + random(lv1) > lv2 )
        {
                victim->add("max_neili", 1*(5+((int)victim->query_skill("beiming-shengong",1)/100)));
                me->add("max_neili", -1*(5+((int)victim->query_skill("beiming-shengong",1)/100)));

                if (victim->query("max_neili") > (int)victim->query_skill("force") * (int)victim->query("con") * 2 / 3 * 2)
                        victim->set("max_neili",victim->query_skill("force") * (int)victim->query("con") * 2 / 3 * 2);

                return ([ "msg": HIR "$N运足内力一击命中，却不料只觉息关一开，一股内力被$n吸了过去！\n" NOR ]);
        }
        
        return damage;
}

string exert_function_file(string func)
{
    return __DIR__"beiming-shengong/" + func;
}

