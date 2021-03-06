// SKILL bagua-zhang.c
// 太极生两仪（阴、阳）
// 两仪生四象（太阴、少阳、少阴、太阳）
// 四象生八卦
// 八卦，是指坤、艮（太阴）、坎、巽（少阳）、震、离（少阴）、兑、乾（太阳）。
// Modify by Haiyan

#include <ansi.h>
inherit SKILL;

mapping *action = ({
([      "action" : "$N脚踏「乾」位，左掌一推，右掌缓缓攻向$n",
        "force" : 180,
        "attack": 15,
        "dodge" : 60,
        "parry" : 70,
        "lvl"   : 10,
        "damage_type" : "瘀伤"
]),
([      "action" : "$N右脚迈向「艮」位，面色凝重，双掌轻飘飘地拍向$n",
        "force" : 220,
        "attack": 28,
        "dodge" : 80,
        "parry" : 95,
        "lvl"   : 50,
        "damage_type" : "内伤"
]),
([      "action" : "$N向左一退，右脚已踩在「坎」位，双掌幻出一片掌影，将$n笼罩于内。",
        "force" : 260,
        "attack": 40,
        "dodge" : 102,
        "parry" : 118,
        "lvl"   : 80,
        "damage_type" : "瘀伤"
]),
([      "action" : "$N向右一转，双脚分立「巽」位，反身向$n连击三掌",
        "force" : 310,
        "attack": 48,
        "dodge" : 126,
        "damage": 10,
        "parry" : 140,
        "lvl"   : 100,
        "damage_type" : "震伤"
]),
([      "action" : "$N左脚一并，右脚迈向「震」位，双掌慢慢攻向$n",
        "force" : 370,
        "attack": 60,
        "dodge" : 160,
        "damage": 25,
        "parry" : 170,
        "lvl"   : 130,
        "damage_type" : "瘀伤"
]),
([      "action" : "$N向左跨出，已站在「离」位，双掌平挥，击向$n",
        "force" : 420,
        "attack": 82,
        "dodge" : 175,
        "damage": 40,
        "parry" : 190,
        "lvl"   : 180,
        "damage_type" : "震伤"
]),
([      "action" : "$N右脚一抬，已移向「兑」位，只见一片掌影攻向$n",
        "force" : 470,
        "attack": 103,
        "dodge" : 205,
        "damage": 50,
        "parry" : 215,
        "lvl"   : 250,
        "damage_type" : "内伤"
]),
([      "action" : "$N反身站于「坤」位，左掌一晃，右掌缓缓击向$n",
        "force" : 550,
        "attack": 118,
        "dodge" : 230,
        "damage": 70,
        "parry" : 235,
        "lvl"   : 300,
        "damage_type" : "瘀伤"
]),
});

int valid_enable(string usage) { return usage=="parry" || usage=="strike"; }

int valid_combine(string combo) 
{
        return combo == "taiji-quan"; 
}

int valid_learn(object me)
{
        if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
                return notify_fail("练「八卦掌」必须空手。\n");

        if ((int)me->query_skill("taiji-shengong",1) < 100)
                return notify_fail("你的太极神功火候不够，无法练「八卦掌」。\n");

        if ((int)me->query_skill("taoism",1) < 250)
                return notify_fail("你的道学心法修为不够，无法练「八卦掌」。\n");

        if ((int)me->query_skill("taiji-quan",1) < 150)
                return notify_fail("你的太极拳水平太低，不能练「八卦掌」。\n");

        if ((int)me->query("max_neili") < 1000)
                return notify_fail("你的内力太弱，无法练「八卦掌」。\n");

        if ((int)me->query_skill("strike", 1) < (int)me->query_skill("bagua-zhang", 1))
                return notify_fail("你的基本掌法水平有限，无法领会更高深的「八卦掌」。\n");

        return 1;
}

mapping query_action(object me, object weapon)
{
        int i, level;
        level = (int) me->query_skill("bagua-zhang",1);
        for(i = sizeof(action); i > 0; i--)
                if(level > action[i-1]["lvl"])
                        return action[NewRandom(i, 20, level/5)];
}

mixed valid_damage(object ob, object me, int damage, object weapon) 
{ 
        mapping result; 
        int ap, dp, acter; 

        if ((int)me->query_skill("bagua-zhang", 1) < 100 || 
            me->query_temp("weapon")) 
                return; 

        ap = (ob->query_skill("force", 1) + 
             ob->query_skill("dodge", 1) +
             ob->query_skill("martial-cognize", 1)) / 3 +
             ob->query_skill("count", 1); 
        if (me->query("character") == "光明磊落" || me->query("character") == "狡黠多变") 
             acter = 4; 
        else
             acter = 5; 
        dp = (me->query_skill("force", 1) / 3 +
             me->query_skill("parry", 1) +
             me->query_skill("bagua-zhang", 1) +
             me->query_skill("martial-cognize", 1) + 
             me->query_skill("taoism", 1)) / acter; 

        if ( ap / 2 + random(ap) < dp ) 
        { 
                result = ([ "damage": -damage ]); 

                switch (random(4)) 
                { 
                case 0: 
                        result += (["msg" : HIC "$n" HIC "脚踩八卦方位，在$N"
                                            HIC "四周快速游走，$N"
                                            HIC "眼前一花，攻击全然落空。\n" NOR]); 
                        break; 
                case 1: 
                        result += (["msg" : HIC "$n" HIC "双掌连续晃动，在身前"
                                            HIC "形成一股巨大的气旋，将$N"
                                            HIC "的攻击化为乌有。\n" NOR]); 
                        break; 
                case 2: 
                        result += (["msg" : HIC "$n" HIC "看清$N"
                                            HIC "的招式来路，伸手一带，$N" 
                                            HIC "看到自己的招式全部反击回来，"
                                            HIC "连忙向后跃开。\n" NOR ]); 
                        message_vision(HIY "$N" HIY "双手互圈，施展出太极「借力打力」绝技，将"
                                       HIY "$n" HIY "的攻击全部转到" 
                                       HIY "$n" HIY "自己身上!\n"NOR,me,ob); 
                        ob->receive_damage("qi", damage / 6); 
                        me->add("neili", -30); 
                        break; 
                default: 
                        result += (["msg" : HIC "$n" HIC "脚下一顿，双掌幻出漫天掌影，使$N"
                                            HIC "的招数全部滑向一边。\n" NOR]);
                        break; 
                } 
                return result; 
        } 
} 

int practice_skill(object me)
{
        int cost;
        if ((int)me->query("qi") < 100)
                return notify_fail("你的体力太低了。\n");
        cost = me->query_skill("bagua-zhang", 1) / 2 + 80; 
        if ((int)me->query("neili") < cost)
                return notify_fail("你的内力不够练「八卦掌」。\n");

        me->receive_damage("qi", 80);
        me->add("neili", -cost);
        return 1;
}

string perform_action_file(string action)
{
        return __DIR__"bagua-zhang/" + action;
}

