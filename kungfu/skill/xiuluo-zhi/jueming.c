#include <ansi.h>
#include <combat.h> 
         
inherit F_SSERVER; 
         
int perform(object me, object target) 
{ 
        object weapon; 
        string msg; 
        int ap, dp; 
        int bs;
        int damage; 
        
        if (! target) target = offensive_target(me); 
        
        if (! target || ! me->is_fighting(target)) 
                return notify_fail("「修罗绝命指」只能在战斗中对对手使用。\n"); 
         
        if (me->query_temp("weapon") || 
            me->query_temp("secondary_weapon")) 
                return notify_fail("你必须空手才能使用「修罗绝命指」！\n"); 

        if (me->query_skill_mapped("force") != "hunyuan-yiqi" &&
            me->query_skill_mapped("force") != "yijinjing" &&
            me->query_skill_mapped("force") != "shaolin-xinfa" &&
            me->query_skill_mapped("force") != "jiuyang-shengong")
                return notify_fail("你使用的非少林内功，无法施展「修罗绝命指」！\n");

        if (me->query_skill("force") < 250) 
                return notify_fail("你的内功的修为不够，不能使用「修罗绝命指」！\n"); 
        
        if (me->query_skill("xiuluo-zhi", 1) < 150) 
                return notify_fail("你的修罗指修为不够，目前不能使用「修罗绝命指」！\n"); 
         
        if (me->query("neili") < 500) 
                return notify_fail("你的真气不够，无法使用「修罗绝命指」！\n"); 
         
        if (me->query_skill_mapped("finger") != "xiuluo-zhi") 
                return notify_fail("你没有激发修罗指，不能使用「修罗绝命指」！\n"); 
         
        msg = HIB "$N忽然面露凶光，身形变的异常飘渺，在$n" HIB
                  "的四周游走个不停，$n" HIB "正迷茫时，$N" HIB
                  "突然近身，毫无\n声息的一指戳出！\n" NOR; 
         
        ap = me->query_skill("finger") + me->query_skill("force"); 
        dp = target->query_skill("dodge") + target->query_skill("parry"); 
         
        if (ap / 2 + random(ap) > dp || !living(target))
        { 
                damage = ap + random(ap / 2); 
                bs = 1 + random(3);
                me->add("neili", -350); 
                me->start_busy(bs);
         
                msg += HIR "只见$n" HIR "一声惨叫，以被击中要害部位，只觉得眼前一黑，似乎以看不清周围的一切，身体摇摇欲坠！\n" NOR;
                msg += COMBAT_D->do_damage(me, target, REMOTE_ATTACK, damage, 100); 
        } else 
        { 
                me->add("neili", -150); 
                me->start_busy(3); 
                msg += CYN "可是$n" CYN "看破了$N" CYN "的企图，轻轻" 
                       CYN "向后飘出数丈，躲过了这一致命的一击！\n"NOR; 
        } 
                message_combatd(msg, me, target); 
         
                return 1; 
}

