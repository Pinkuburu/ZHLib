// huan.c 玄虚刀法「乱环诀」
// Modify by Haiyan

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

int perform(object me, object target)
{
        object weapon;
        string msg;
        int ap, dp;
        int count;
        int i, zhao;

        if (! target) target = offensive_target(me);

        if (userp(me) && ! me->query("can_perform/xuanxu-dao/huan")) 
                return notify_fail("你现在还不会使用「乱环诀」！\n"); 

        if (! target || ! me->is_fighting(target))
                 return notify_fail("「乱环决」只能在战斗中对对手使用。\n"); 

        if (! objectp(weapon = me->query_temp("weapon")) || 
              (string)weapon->query("skill_type") != "blade") 
                 return notify_fail("你使用的武器不对。\n"); 

        if (me->query_skill("force") < 200) 
                 return notify_fail("你的内功的修为不够，不能使用乱环诀！\n"); 

        if ((int)me->query_skill("xuanxu-dao", 1) < 135)
                  return notify_fail("你的玄虚刀法修为不够，目前不能使用乱环诀！\n"); 

        if (me->query("neili") < 250)
                return notify_fail("你的真气不够，不能使用「乱环诀」。\n");

        msg = HIW "$N" HIW "使出玄虚刀法中的绝技乱环诀，刀出成环，环环相连，只绞的$n" + HIW 
              "眼前一片影环。\n" NOR; 

        ap = me->query_skill("blade", 1) / 2 + me->query_skill("xuanxu-dao", 1); 
        dp = target->query_skill("dodge") / 2 + target->query_skill("parry", 1); 

        if ( ap / 2 + random(ap) > dp || !living(target))
        {

                msg += HIR "就听见“喀喀喀”几声脆响，$p" HIR "一声" 
                      "惨叫，全身各处骨头被刀环一一绞断，像滩软泥般" 
                      "塌了下去！\n" NOR; 
                zhao = 5; 
                count = ap / 5;
                me->add("neili", -180);
        } else
        {
                msg += CYN "可是$p" CYN "奋力格挡，$P" CYN  
                       "只觉得精力略有衰竭，手中刀光渐缓。 \n" NOR; 
                zhao = 0; 
                count = 0;
                me->add("neili", -60);
        }
                
        message_combatd(msg, me, target);
        me->add_temp("apply/attack", count);
        for (i = 0; i < zhao; i++)
        {
                if (! me->is_fighting(target))
                        break;
                COMBAT_D->do_attack(me, target, 0, 0);
        }

        me->add_temp("apply/attack", -count);
        me->start_busy(1 + random(zhao));

        return 1;
}

