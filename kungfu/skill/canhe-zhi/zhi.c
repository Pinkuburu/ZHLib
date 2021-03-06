// zhi.c 金刚指

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

int perform(object me, object target)
{
        string msg;
        int skill, ap, dp, damage;

        if (! target)
        {
                me->clean_up_enemy();
                target = me->select_opponent();
        }

        if (! me->is_fighting(target))
                return notify_fail("「金刚指」只能对战斗中的对手使用。\n");

	if (me->query_temp("weapon"))
		return notify_fail("你不放下武器，如何使用参合指呢？\n");

        skill = me->query_skill("canhe-zhi", 1);

        if (skill < 150)
                return notify_fail("你的参合指修为有限, 不能使用「金刚指」！\n");

        if (me->query_skill_mapped("finger") != "canhe-zhi")
                return notify_fail("你没有激发参合指, 不能使用「金刚指」！\n");

	if( me->query_skill_prepared("finger") != "canhe-zhi" )
		return notify_fail("你所备的并非参合指，不能施展金刚指法！\n");

        if (me->query("neili") < 300)
                return notify_fail("你的真气不够，无法运用「金刚指」！\n");

        msg = HIY "$N" HIY "中指蓦然伸出，看来正是佛门秘技金刚指法，招"
              "式极快，插向$n" HIY "的胸前大穴。\n" NOR;

        ap = me->query_skill("finger");
        if(living(target))
              dp = target->query_skill("parry");
        else  dp = 0;
        if (ap / 2 + random(ap) > dp)
        {
                me->add("neili", -300);
                me->start_busy(2);
                damage = skill + random(skill) + me->query("jiali");
                msg += COMBAT_D->do_damage(me, target, UNARMED_ATTACK, damage, 50,
                                           HIR "$n" HIR "招架不住，“扑哧”一下被$P"
                                           HIR "这一指插中了胸前，鲜血直流！\n" NOR);
		target->start_busy(1 + random(2));
        } else
        {
                me->add("neili",-50);
                msg += CYN "可是$n" CYN "看破了$N" CYN "的招式，"
                       "巧妙的挡住了$N" CYN "的攻击。\n" NOR;
                me->start_busy(3);
        }
        message_combatd(msg, me, target);

        return 1;
}
