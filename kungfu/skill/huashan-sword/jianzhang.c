// Code of ShenZhou
//jianzhang.c 剑掌五连环 

#include <ansi.h>

inherit F_DBASE;
inherit F_SSERVER;

int perform(object me, object target)
{
	string weapon;
	object weapon1;
	int    lvl1, lvl2, amount;

	if( !target ) target = offensive_target(me);

	if( !target
	||	!target->is_character()
	||	!me->is_fighting(target) )
		return notify_fail("剑掌五连环只能对战斗中的对手使用。\n");

	weapon1 = me->query_temp("weapon");
        if( !objectp(weapon1) || weapon1->query("skill_type") != "sword" )
                return notify_fail("你手中无剑，如何使得剑掌五连环？\n");

        if(me->query_skill_mapped("sword") != "huashan-sword") 
                return notify_fail("你并未使用华山剑法，不能使出剑掌五连环！\n");

        if( me->query_temp("secondary_weapon"))
                return notify_fail("你正握着短兵,没有多余的手再能使出混元掌。\n");

	if( me->query_skill_mapped("strike") != "hunyuan-zhang" )
		return notify_fail("你所用的并非混元掌，不能与华山剑法配合施展剑掌五连环！\n");

	if( me->query_skill_prepared("strike") != "hunyuan-zhang" )
                return notify_fail("你所备的并非混元掌，不能与华山剑法配合施展剑掌五连环！\n");

	if( me->query_skill_mapped("force") != "zixia-shengong" )
		return notify_fail("你所用的并非紫霞神功，无法施展剑掌五连环！\n");

	if( me->query_skill("zixia-shengong", 1) < 60 )
		return notify_fail("你的紫霞神功火候未到，无法施展剑掌五连环！\n");

	if( (lvl1=me->query_skill("strike")) < 100 )
		return notify_fail("剑掌五连环需要精湛的混元掌配合，方能有效施展！\n");

	if( (lvl2=me->query_skill("sword")) < 100 )
		return notify_fail("你华山剑法修为不足，还不会使用剑掌五连环！\n");

	amount = (lvl1+lvl2) / 15;

	if ( amount < 30 ) amount = 30;
	if ( amount > 100 ) amount = 100;

	if( me->query("neili") < amount*3 )
		return notify_fail("你现在内力不足，无法使用剑掌五连环！\n");

	if( me->query("neili") <= amount*4 )
		return notify_fail("你的内力不够使用剑掌五连环！\n");
        if (me->queyr("jingli") < 0)
		return notify_fail("你的精力不够使用剑掌五连环！\n");

	if( me->query_skill_prepared("cuff") == "pishi-poyu" ) {
		me->set_temp("restore", 1);
		me->prepare_skill("cuff");
	}

	weapon = me->query_temp("weapon");

	me->add_temp("apply/damage", amount);
	me->add_temp("apply/attack", amount*2);
	me->add_temp("apply/dodge", amount);
	me->set_temp("kuangfeng_kuaijian",1);

	message_vision(HIR "\n突然间$N身形电闪，瞬间逼近$n，满面紫气隐现中向$n奋力击出三剑两掌！\n\n" NOR, me, target);

	COMBAT_D->do_attack(me, target, me->query_temp("weapon"));
	weapon->unequip();
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"));
	weapon->wield();
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"));
	weapon->unequip();
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"));
	weapon->wield();
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"));

	me->start_busy(1+random(3));
	me->add_temp("apply/damage", -amount);
	me->add_temp("apply/attack", -amount*2);
	me->add_temp("apply/dodge", -amount);
	me->delete_temp("kuangfeng_kuaijian");
	me->add("neili", -amount*3);
	if( me->query_temp("restore") ) me->prepare_skill("cuff", "pishi-poyu");

	return 1;
}
