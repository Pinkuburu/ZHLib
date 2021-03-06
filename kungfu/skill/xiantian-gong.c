// xiantian-gong.c 先天功
#include <combat.h>
#include <ansi.h>
inherit FORCE;

int valid_enable(string usage) { return usage == "force"; }

int valid_force(string force)
{
        return force == "duanshi-xinfa" ||
               force == "quanzhen-xinfa";
}

int query_neili_improve(object me)
{
        int lvl;

        lvl = (int)me->query_skill("xiantian-gong", 1);
        return lvl * lvl * 15 * 18 / 100 / 200;
}

int valid_learn(object me)
{
        if (me->query("gender") == "无性" &&
            (int)me->query_skill("xiantian-qigong", 1) > 49)
                return notify_fail("你无根无性，阴阳难调，无法学习更高深的先天功。\n");

        if ((int)me->query_skill("force", 1) < 110)
                return notify_fail("你的基本内功火候还不够。\n");

        return ::valid_learn(me);
}

int practice_skill(object me)
{
        return notify_fail("先天功只能用学(learn)的来增加熟练度。\n");
}

mixed hit_by(object me, object victim, int damage, int damage_bonus, int factor)
{
	object weapon1 = me->query_temp("weapon");
	object weapon2 = victim->query_temp("weapon");
	int ap, dp;
	string result;

	if ( (int)victim->query_skill("xiantian-gong", 1) < 200 ) return damage;
	if (victim->query("neili") < 1000) return damage;

	dp = COMBAT_D->skill_power(victim, "force", SKILL_USAGE_DEFENSE);
	if (objectp(weapon2) && (string)weapon2->query("skill_type") == "sword" ) {
		if ( victim->query_skill_mapped("parry") != "quanzhen-jian" )
			return damage;
		dp /= 3;
		dp += COMBAT_D->skill_power(victim, "parry", SKILL_USAGE_DEFENSE)/2;
	} else if (!objectp(weapon2) ) {
		if ( victim->query_skill_prepared("strike") != "haotian-zhang" &&
                     victim->query_skill_prepared("strike") != "chongyang-shenzhang" &&
                     victim->query_skill_prepared("cuff") != "chunyang-quan" &&
                     victim->query_skill_prepared("cuff") != "kongming-quan" )
			return damage;
		dp += COMBAT_D->skill_power(victim, "parry", SKILL_USAGE_DEFENSE);
	} else return damage;

	ap = COMBAT_D->skill_power(me, "force", SKILL_USAGE_ATTACK);
	if (objectp(weapon1))
		ap += COMBAT_D->skill_power(me, weapon1->query("skill_type"), SKILL_USAGE_ATTACK);
	else
		ap *= 2;

	ap /= 100;
	dp /= 100;

	if ( random(ap+dp) > ap ) {
		if ( weapon2 && objectp(weapon2) ) {
			if (damage + damage_bonus > 0)
			{
				me->receive_damage("qi", (damage + damage_bonus)/2, victim);
				result = HIG"只见$n手中剑巧妙地一粘一拉，$N的力道通通击在空处，$P用力过猛，失去平衡，跌了个跤。\n"NOR;
				result += "( $N"+ COMBAT_D->status_msg((int)me->query("qi")*100/(int)me->query("max_qi")) + " )\n";
				me->start_busy( 2+random(2) );
				victim->add("neili",-100);
			}
			else
		        {
				result = "只见$n手中剑巧妙地一粘一引，$N的力道被化解为无形。\n";
				victim->add("neili",-60);
			}
		}
		else if( victim->query_temp("powerup") || victim->is_busy() )
		{
			if( !weapon1 )
			{
				result = HIM"只见$n先天护体神功自然而然发动，将$N的力道尽数反震向$P自己。\n"NOR;
				if (damage + damage_bonus > 0)
					me->receive_damage("qi", (damage + damage_bonus)/2, victim);
				result += "( $N"+ COMBAT_D->status_msg((int)me->query("qi")*100/(int)me->query("max_qi")) + " )\n";
				victim->add("neili",-100);
			}
			else
			{
				victim->add("neili",-60);
				result = HIM"只见$n先天护体神功自然而然发动，将$N的力道引到一边。\n"NOR;
			}
		}
		else return damage;

		damage_bonus = -5000; // just a number negative enough
		return ([ "msg" : result, "damage" : damage_bonus ]);

	} else if( random(ap+dp+dp+dp) > ap ) {

		if (objectp(weapon2)) {
			result = "$n手中剑轻轻斜引，$N的力道被化解了大半。\n";
			victim->add("neili",-50);
			damage_bonus = 0;
		}
		else if( victim->query_temp("powerup") || victim->is_busy())
		{
			victim->add("neili",-80);
			result = MAG"只见$n先天护体神功自然而然发动，将$N的攻击化为无形。\n"NOR;
			damage_bonus = -5000;
		}
		else return damage;

		return ([ "msg" : result, "damage" : damage_bonus ]);
	} else if( random(ap+dp+dp+dp+dp) > ap ) {

		 if( victim->query_temp("powerup") || victim->is_busy() )
		{
			victim->add("neili",-50);
			result = MAG"只见$n先天护体神功自然而然发动，抵抗$N的攻击。\n"NOR;
			damage_bonus = 0;
		}
		else return damage;

		return ([ "msg" : result, "damage" : damage_bonus ]);
	} else
		return damage;
}

string exert_function_file(string func)
{
        return __DIR__"xiantian-gong/" + func;
}
 mapping curing_msg(object me)
{
        return ([
"apply_short"   : me->name() +"坐在地下，头上一缕缕热气袅袅而上，犹如蒸笼一般。",
"start_self"    : HIY"你盘膝坐下，调气护伤，运气在周身大穴运转。\n"NOR,
"on_self"       : YEL"你运用先天神功，真气流转周身，感觉伤势大有起色。\n"NOR,
"start_other"   : HIY""+me->name()+"面色祥和，头上一缕热气袅袅升起，脸色有所好转。\n"NOR,
"finish_self"   : HIY"你感到先天真气运转全身，再无半分阻碍，伤势已然痊愈，缓缓站起身来。\n"NOR,
"finish_other"  : me->name()+"微微一笑，站起身来，面色红润，精神振奋。\n",
"unfinish_self" : "你正运功疗伤间，渐觉丹田空虚，后力不继，只能暂缓疗伤，站起身来。\n",
"unfinish_other": me->name()+"缓缓站起，微微气喘，脸色有些苍白。\n",
"halt_self"     : "你将先天真气收回丹田，站起身来。\n",
"halt_other"    : me->name() +"深吸一口气，站了起来，伤势似乎看上好了不少。\n",
        ]);
}

