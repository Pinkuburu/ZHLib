// taiji-shengong.c 太极神功
// Modified by haiyan

#include <ansi.h> 

inherit FORCE;

int valid_enable(string usage) { return usage == "force"; }

int valid_force(string force)
{
        return force == "hunyuan-yiqi" ||
               force == "yijinjing" ||
               force == "wudang-xinfa" ||
               force == "shaolin-xinfa";
}

int query_neili_improve(object me)
{
    int lvl;

    lvl = (int)me->query_skill("taiji-shengong", 1);
    return lvl * lvl * 15 * 18 / 100 / 200;
}

int valid_learn(object me)
{
    if ( me->query("gender") == "无性" && (int)me->query_skill("taiji-shengon", 1) > 19)
         return notify_fail("你无根无性，阴阳不调，难以领会高深的太极神功。\n");

    if ((int)me->query_skill("force", 1) < 100)
         return notify_fail("你的基本内功火候还不够。\n");

    if ((int)me->query_skill("taoism", 1) < 100)
         return notify_fail("你对道家心法领悟的太浅，无法理解太极神功。\n");

    if ((int)me->query_skill("taoism", 1) < (int)me->query_skill("taiji-shengong", 1))
         return notify_fail("你对道家心法领悟的有限，无法理解更高深太极神功。\n");

    if ((int)me->query_skill("force", 1) < (int)me->query_skill("taiji-shengong", 1))
         return notify_fail("你对基本内功水平有限，无法理解更高深太极神功。\n");

    return ::valid_learn(me);
}

int practice_skill(object me)
{
        return notify_fail("太极神功只能用学(learn)的来增加熟练度。\n");
}

mixed hit_by(object ob, object me, int damage) 
{
        object weapon = me->query_temp("weapon");
        int ap, dp, shan;
        string result;

        if (me->query_skill("taiji-shengong",1) < 300) return damage;

        if (me->query("neili") < 1000) return damage;

        if (damage <=0) return damage;

        if (!living(me) || (me->is_busy() && random(3) == 0))  return damage;

        dp = ob->query_skill("force",1) + ob->query_skill("parry",1);

        ap = me->query_skill("taiji-shengong",1);
        if (objectp(weapon))
            ap += me->query_skill("taiji-jian", 1);
        else
            ap += me->query_skill("taiji-quan", 1);

        if (me->query("jiali"))  {
        if ( ap / 3 + random(ap) > dp )
        {
             if (me->query("character") == "光明磊落" ||
                 me->query("character") == "狡黠多变") 
                   shan = damage / 2;
             else  shan = damage / 4;

             if (objectp(weapon))
             {
                result = HIY "$n" HIY "默运太极神功，手中" + weapon->name() +
                         HIY "划圈成圆，引劲入意，向$N" HIY "带出。\n"
                         HIY "$N" HIY "看到自己的招式忽而反转，正好击中自己，"
                         HIY "忍不住闷哼一声，“哇”地吐出一口" HIR "鲜血" HIY "。\n"NOR;
                me->add("neili", -150);
                ob->receive_damage("qi", shan);
                ob->receive_wound("qi", shan/2 + random(shan/2));
                return ([ "msg" : result, "damage" : -damage ]);
             }
             else
             {
                result = HIY "$N" HIY "一招正好打在$n"
                         HIY "身上，正待加劲，却发现如同打在棉絮上一般，"
                         HIY "使不出丝毫力道，急忙抽手，已然不及，被$n"
                         HIY "借力反震，“嘭”的一声，只见$N" HIY "身子像稻草般向后飞出。\n"NOR;
                me->add("neili", -150);
                ob->receive_damage("qi", shan * 2 );
                ob->receive_wound("qi", shan + random(shan));
                return ([ "msg" : result, "damage" : -damage ]);
             }
        } }
        return damage;
}

string exert_function_file(string func)
{
        return __DIR__"taiji-shengong/" + func;
}

mapping curing_msg(object me)
{
        return ([
"apply_short"   : me->name() +"分足而立，头顶笼罩"HIW"氤氲白气"NOR"，汗气上蒸。",
"start_self"    : HIW"你脚踏太极，潜运内力，在周身穴道流转疗伤，调匀气息。\n"NOR,
"on_self"       : HIM"你真气在体内分两路运转，阴阳周济，感觉伤势大有起色。\n"NOR,
"start_other"   : HIM"只见"+ me->name() +"衣衫起伏，鼻息沉缓，头顶冒出丝丝白气。\n"NOR,
"finish_self"   : HIW"你只觉内息运转顺畅，将真气还合丹田，站起身来。\n"NOR,
"finish_other"  : me->name() +"惨白的脸色渐渐红润，缓缓睁开眼，站起身来。\n",
"unfinish_self" : "你渐感真气不纯，后劲不继，不得不将在体内搬运的内息收回。\n",
"unfinish_other": me->query("eff_qi") < me->query("max_qi")*3/4 ?
                  "猛地里"+ me->name() +"口一张，喷出几口鲜血。\n" :
                  "猛地里"+ me->name() +"口一张，吐出口紫黑瘀血。\n",
"halt_self"     : "你将两路真气压制下来，还合丹田，深吸几口气，呼吸逐渐平缓。\n",
"halt_other"    : me->name() +"身子微微一震，睁开双眼，伤势似乎看上好了不少。\n",
        ]);
}

