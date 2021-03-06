// qijue-cuff.c
// 华夏玩家fastidium的自创武功：七绝拳
// Modified by smallfish@huaxia Sep.1998
#include <ansi.h>

inherit SKILL;

string is_private_skill() {return "fastidium";}
int can_not_hubo() {return 1;}

mapping *action = ({
([      "skill_name":    "笑问七剑一指仙",
        "action":  "$N凝声长吟「笑问七剑一指仙」,迅捷地连挥七拳,$n正想看清拳路,可最后一拳却笔直地击向$n的胸膛",
        "dodge" : 90,
        "damage": 200,
        "force" : 350,
        "attack": 135,
        "parry" : 85,
        "lvl"   : 0,
        "damage_type":  "瘀伤"
]),
([      "skill_name":    "叶落枫舞二穿山",
        "action":  "$N长喝一声「叶落枫舞二穿山」,潜运内力双拳交错，沉重无比地击向$n的双肩",
        "dodge" : 90,
        "damage": 200,
        "force" : 350,
        "attack": 135,
        "parry" : 85,
        "lvl"   : 120,
        "damage_type":  "瘀伤"
]),
([      "skill_name":    "袖里风云三点头",
        "action":  "$N长身卓立，双拳拢于袖中，遥向$n连摁三下，三股劲风一浪高过一浪向$n卷去，正是一招「袖里风云三点头」",
        "dodge" : 95,
        "damage": 210,
        "force" : 360,
        "attack": 140,
        "parry" : 90,
        "lvl"   : 140,
        "damage_type":  "瘀伤"
]),
([      "skill_name":    "回首太白四气刀",
        "action":  "$N一击未中，飘身而退，突然一个急停，回头化拳为掌，向$n当头连劈四下,好一招「回首太白四气刀」",
        "dodge" : 105,
        "damage": 220,
        "force" : 370,
        "attack": 145,
        "parry" : 95,
        "lvl"   : 160,
        "damage_type":  "瘀伤"
]),
([      "skill_name":    "化月孤心五看地",
        "action":  "$N一声清笑「化月孤心五看地」,一个旱地拔葱,盘旋在$n的上空，全身功力凝于双拳，狠狠地连捣五拳",
        "dodge" : 115,
        "damage": 240,
        "force" : 390,
        "attack": 155,
        "parry" : 105,
        "lvl"   : 200,
        "damage_type":  "瘀伤"
]),
([      "skill_name":    "茫茫秋水六荒天",
        "action":  "$N快速运气一周天,全身功力聚于双臂，双臂犹如铁铸，一招「茫茫秋水六荒天」向$n横扫而去,顿时飞砂走石",
        "dodge" : 115,
        "damage": 260,
        "force" : 410,
        "attack": 155,
        "parry" : 105,
        "lvl"   : 200,
        "damage_type":  "瘀伤"
]),
([      "skill_name":    "笑问叶落看风云",
        "action":  "$N一声怒喝「笑问叶落看风云」,满场游走,瞅准一个破绽，左手抓向$n的手臂,右手握拳,闪电般按向$n的背心",
        "dodge" : 115,
        "damage": 300,
        "force" : 450,
        "attack": 155,
        "parry" : 105,
        "lvl"   : 200,
        "damage_type":  "瘀伤"
]),
([      "skill_name":    "回首化月望秋水",
        "action":  "$N使出最后一招「回首化月望秋水」，一个鱼跃身子腾空，双拳交替向$n击出,身形不断升高,强大的气劲令$n一阵窒息",
        "dodge" : 115,
        "damage": 300,
        "force" : 450,
        "attack": 155,
        "parry" : 105,
        "lvl"   : 200,
        "damage_type":  "瘀伤"
]),
});

int valid_learn(object me)
{
    object ob;
    if( (int)me->query("max_neili") < 1000 )
        return notify_fail("你的内力不够，没有办法练七绝拳。\n");

    if( (int)me->query_skill("martial-cognize",1) < 300 )
        return notify_fail("你的武学修养不够，没有办法练七绝拳。\n");

    if( (ob = me->query_temp("weapon")) || 
        (ob = me->query_temp("handing")) )
        return notify_fail("你只有空着双手才能修炼拳法。\n");

    return 1;
}

int valid_enable(string usage)
{
    return usage=="cuff" || usage=="parry";
}

string query_skill_name(int level)
{
    int i;
    for(i = sizeof(action)-1; i >= 0; i--)
        if(level >= action[i]["lvl"])
            return action[i]["skill_name"];
}

mapping query_action(object me, object weapon)
{
    int i, level;
    level   = (int) me->query_skill("qijue-cuff",1);
    for(i = sizeof(action); i > 0; i--)
        if(level > action[i-1]["lvl"])
            return action[NewRandom(i, 20, level/5)];
}

int practice_skill(object me)
{
    if ((int)me->query("qi") < 60)
        return notify_fail("你的体力不足以练习七绝拳。\n");

    if ((int)me->query("neili") < 60 )
        return notify_fail("你的内力不够，没有办法练习七绝拳。\n");

    me->receive_damage("qi", 50);
    me->add("neili", -50);

    return 1;
}

string perform_action_file(string action)
{
        return __DIR__"qijue-cuff/" + action;
}
