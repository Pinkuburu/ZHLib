// twentyfour-poem.c
// 华夏玩家yxer的自创武功：诗经二十四品
// Modified by smallfish@huaxia Sep.1998
#include <ansi.h>

inherit SKILL;

string is_private_skill() {return "yxer";}
int can_not_hubo() {return 1;}

mapping *action = ({
([      "skill_name":    "飘逸",
        "action":  "$N迎风扶摇直上，拳风「飘逸」仿是寻着轻风缝隙，轰然击向$n",
        "dodge" : 80,
        "damage": 190,
        "force" : 340,
        "attack": 130,
        "parry" : 80,
        "lvl"   : 0,
        "damage_type":  "瘀伤"
]),
([      "skill_name":    "冲淡",
        "action":  "$N洒然一笑，双手随意舒展，看似破绽处处，却令$n无从出手。待得$n不耐，欲行出招，一招「冲淡」已后发先至",
        "dodge" : 90,
        "damage": 200,
        "force" : 350,
        "attack": 135,
        "parry" : 85,
        "lvl"   : 120,
        "damage_type":  "瘀伤"
]),
([      "skill_name":    "纤穠",
        "action":  "$N口吐真言，一式「纤穠」，双足一弹，长袖迎风，左手微张成指是纤，右手紧握成拳作穠，漫天真气如风雨骤至直往$n身上扑去",
        "dodge" : 95,
        "damage": 210,
        "force" : 360,
        "attack": 140,
        "parry" : 90,
        "lvl"   : 140,
        "damage_type":  "瘀伤"
]),
([      "skill_name":    "沉著",
        "action":  "$N身似苍松不动如山，「沉著」巍然，竟是不避不迎，双拳直击，真气一蓄而发轰向$n空当处",
        "dodge" : 105,
        "damage": 220,
        "force" : 370,
        "attack": 145,
        "parry" : 95,
        "lvl"   : 160,
        "damage_type":  "瘀伤"
]),
([      "skill_name":    "绮丽",
        "action":  "$N双足连弹，脚下不停，欲旋作舞，真气于全身凝而不放，双拳于虚空处一挥一引，真气相触而爆，$n只觉满天花雨扑面而至，「绮丽」丛生，竟不忍相拒",
        "dodge" : 105,
        "damage": 220,
        "force" : 370,
        "attack": 145,
        "parry" : 95,
        "lvl"   : 160,
        "damage_type":  "瘀伤"
]),
([      "skill_name":    "含蓄",
        "action":  "$N左手五指微曲，作拈花状「含蓄」不语，右手一抹一引，$n正惑然间，一式拳风已无声无息迎来",
        "dodge" : 110,
        "damage": 230,
        "force" : 380,
        "attack": 150,
        "parry" : 100,
        "lvl"   : 180,
        "damage_type":  "瘀伤"
]),
([      "skill_name":    "委曲",
        "action":  "$N身形忽动，直若矫龙，忽左忽右，又如流水般曲折蜿蜒偏又自在潇洒，$n尽无处回招，正是极尽婉约之至「委曲」",
        "dodge" : 115,
        "damage": 240,
        "force" : 390,
        "attack": 155,
        "parry" : 105,
        "lvl"   : 200,
        "damage_type":  "瘀伤"
]),
([      "skill_name":    "悲概",
        "action":  "$N一声「悲概」，浩然弥哀，拳风盛盛，但见大风卷水，林木为摧，非人力所能相抗",
        "dodge" : 115,
        "damage": 300,
        "force" : 450,
        "attack": 155,
        "parry" : 105,
        "lvl"   : 200,
        "damage_type":  "瘀伤"
]),
([      "skill_name":    "旷达",
        "action":  "$N静默不语，真气运转全身，气势直往外泄，三丈之内，竟无一丝空尘驻足，朗朗笑中，俨然长河落日之圆浑，又仿是大漠孤烟，拳风直来直去，「旷达」雄奇",
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
        return notify_fail("你的内力不够，没有办法练诗经二十四品。\n");

    if( (int)me->query_skill("martial-cognize",1) < 300 )
        return notify_fail("你的武学修养不够，没有办法练诗经二十四品。\n");

    if( (ob = me->query_temp("weapon")) || 
        (ob = me->query_temp("handing")) )
        return notify_fail("你只有空着双手才能修炼掌法。\n");

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
    level   = (int) me->query_skill("twentyfour-poem",1);
    for(i = sizeof(action); i > 0; i--)
        if(level > action[i-1]["lvl"])
            return action[NewRandom(i, 20, level/5)];
}

int practice_skill(object me)
{
    if ((int)me->query("qi") < 60)
        return notify_fail("你的体力不足以练习诗经二十四品。\n");

    if ((int)me->query("neili") < 60 )
        return notify_fail("你的内力不够，没有办法练习诗经二十四品。\n");

    me->receive_damage("qi", 50);
    me->add("neili", -50);

    return 1;
}

string perform_action_file(string action)
{
        return __DIR__"twentyfour-poem/" + action;
}
