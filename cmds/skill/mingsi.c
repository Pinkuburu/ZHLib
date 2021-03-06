// mingsi.c
#include <skill.h>
#include <ansi.h>

inherit F_CLEAN_UP;

void finish(object me, int pot_gain);
int main(object me, string arg)
{

int busy_time, pot_gain;
int mingsi_lvl;
mapping fam;
object where;
mingsi_lvl = me->query("con");

seteuid(getuid());

where = environment(me);
if(where->query("no_fight") || where->query("no_flee") )
return notify_fail("安全区内禁止练功。\n");
if (!me->query("special_skill/lonely",1))
return notify_fail("你不是天煞孤星不能使用这个命令！\n");

if (me->is_busy() || me->query_temp("pending/exercising")
     || me->query_temp("doing_mingsi"))
    return notify_fail("你现在正忙着呢。\n");

if( me->is_fighting() )
  return notify_fail("战斗中冥思，找死啊？！\n");

if( (int)me->query("jing") * 100 / (int)me->query("max_jing") < 50 )
  return notify_fail("你现在神智不清，再炼恐怕要走火入魔！\n");

if( (int)me->query("qi") * 100 / (int)me->query("max_qi") < 50 )
   return notify_fail("你现在体力不够，再练就要累死了！\n");

pot_gain = mingsi_lvl/20 + random(5);

busy_time = random(120)/pot_gain/10+20;

pot_gain = 1 + random(pot_gain*2);

busy_time *= pot_gain;

pot_gain += (int)me->query("con")*pot_gain/2/busy_time;

message_vision(HIY "$N闭上眼睛，盘膝坐下，嘴里默默念念不知在说些什么。\n" NOR, me);
me->start_busy(busy_time+15);

me->set_temp("doing_mingsi",1);

call_out("finish",busy_time, me, pot_gain);
return 1;
}

void finish(object me, int pot_gain)

{

if(!me) return;

me->delete_temp("doing_mingsi");

message_vision(HIY "$N缓缓睁开眼睛，长舒一口气站了起来。\n" NOR, me);

tell_object(me, HIC "你的潜能。经验各增加了"HIR+"一百点"+HIC",体会增加了"HIR+"五十点"+HIC"！\n" NOR);

me->add("combat_exp", 100);
me->add("potential", 100);
me->add("experience", 50);
me->set("jing",me->query("con"));
me->set("qi",me->query("con"));
me->set("neili",me->query("neili")/2);

if (me->query("food") >= pot_gain) me->add("food",-pot_gain);
else me->set("food", 0);

if (me->query("water") >= pot_gain) me->add("water",-pot_gain);
else me->set("water", 0);
me->receive_damage("jing", me->query("con"));
me->receive_damage("qi", me->query("con"));
}
/*

int help(object me)
{
       write(@HELP
指令格式 : mingsi

 天煞孤星用以提高潜能的命令。
  HELP
);
    return 1;
}
*/
