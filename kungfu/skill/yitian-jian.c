//yitian-jian.c 倚天剑法

inherit SKILL;

mapping *action = ({
([        "action" : "$N举剑过顶，弯腰躬身，恭敬一式「星落长空」，$w向$n的$l轻轻刺去，",
        "force" : 220,
        "dodge" : 70,
        "attack"  : 120,
        "damage" : 200,
        "lvl" : 0,
        "damage_type" : "刺伤"
]),
([        "action" : "突然间剑光一吐，长剑化作一道白虹，$N使出一式「排云推月」，剑气古朴，$w直刺$n的$l",
        "force" : 240,
        "attack"  : 260,
        "dodge" : 90,
        "damage" : 150,
        "lvl" : 10,
        "damage_type" : "刺伤"
]),
([        "action" : "$N左手向外一分，右手$w向右掠出一招「星河在天」，剑气森然地向$n的$l挥去",
        "force" : 270,
        "attack"  : 160,
        "dodge" : 150,
        "damage" : 160,
        "lvl" : 20,
        "damage_type" : "刺伤"
]),
([        "action" : "$N一剑从空中疾劈而下，一招「斗摇星汉」,$w如虹地刺向$n的$l",
        "force" : 240,
        "dodge" : 110,
        "attack"  : 180,
        "damage" : 170,
        "lvl" : 30,
        "damage_type" : "刺伤"
]),
([        "action" : "$N一柄$w自半空中横过，剑身似曲似直，如一件活物一般，一式「起凤腾蛟」奔腾矫夭，气势雄浑地斩向$n的$l",
        "force" : 250,
        "dodge" : 110,
        "attack"  : 140,
        "damage" : 200,
        "lvl" : 40,
        "damage_type" : "割伤"
]),
([        "action" : "$N威仪整肃一式「青龙取水」，$w轰然一动，犹如千军万马奔驰而来，疾疾刺向$n的$l",
        "force" : 280,
        "dodge" : 105,
        "attack"  : 200,
        "damage" : 190,
        "lvl" : 50,
        "damage_type" : "刺伤"
]),
([        "action" : "$N剑转一式「倚天不出」，俨然长枪大戟，黄沙千里，$w电闪雷鸣，将$n笼罩于重重剑气之中",
        "force" : 300,
        "dodge" : 80,
        "attack" : 150,
        "damage" : 200,
        "lvl" : 60,
        "damage_type" : "刺伤"
]),
([        "action" : "$N剑尖向下，一招「谁与争锋」，$w忽然急转直上，皇者般剑气将$n的上身要害团团围住",
        "force" : 360,
        "dodge" : 100,
        "attack" : 180,
        "damage" : 235,
        "lvl" : 70,
        "damage_type" : "刺伤"
]),
});


int valid_enable(string usage) { return (usage == "sword") || (usage == "parry"); }

int valid_learn(object me)
{

        if ((int)me->query("max_neili") < 600)
                return notify_fail("你的内力不够。\n");
        if ((int)me->query_skill("sword", 1) < 80)
                return notify_fail("你的基本剑法太浅。\n");
        if ((int)me->query_skill("force", 1) < 50)
                return notify_fail("你的内功火候太浅。\n");
        return 1;
}
mapping query_action(object me, object weapon)
{
        int i, level;
        level   = (int) me->query_skill("yitian-jian",1);
        for(i = sizeof(action); i > 0; i--)
                if(level > action[i-1]["lvl"])
                        return action[NewRandom(i, 20, level/5)];

}

int practice_skill(object me)
{
        object weapon;

        if (!objectp(weapon = me->query_temp("weapon"))
        || (string)weapon->query("skill_type") != "sword")
                return notify_fail("你使用的武器不对。\n");
        if ((int)me->query("qi") < 50)
                return notify_fail("你的体力不够练倚天剑法。\n");
        if ((int)me->query("neili") < 100)
                return notify_fail("你的内力不足以练倚天剑法。\n");

        me->receive_damage("qi", 40);
        me->add("neili", -50);

        return 1;
}

string perform_action_file(string action)
{
        return __DIR__"yitian-jian/" + action;
}

