//sanyin-wugongzhao.c �������ץ
#include <ansi.h>
inherit SKILL;

mapping *action = ({
([      "action" : "$N������Ц��Ц���������$n�����ţ���$nȫ���м�֮�ʣ�һת��������ץ��$n��$l",
        "dodge": 80,
        "force": 140,
//      "poison": 80,
        "damage": 430,
        "lvl" : 0,  
        "damage_type": "ץ��"
]),
([      "action" : "$NͻȻ����װ��������$n�ɻ󲻶�֮�䣬��ȻԾ��˫��ץ��$n��$l��$n��֮�£���֪����",
        "dodge": 80,
        "parry": 100,
        "force": 160,
//      "poison": 100,
        "damage": 440,
        "lvl" : 100,
        "weapon": "�����ָ",
        "damage_type": "����"
]),
([      "action" :"$N�˵ض���������һ����򼣬�ڵ��Ͽ������˼������Ƶ�$n����һצץ��",
        "dodge": 85,
        "parry": 115,
        "force": 180,
//      "poison": 100,
        "damage": 450,
        "lvl" : 150,
        "damage_type": "ץ��"
]),

([      "action" : "������У�$N��������������ɫ��һ�Լ��ţ���צ�͵�̽��$n��$l",
        "dodge": 95,
        "parry": 125,
        "force": 200, 
//      "poison": 120,
        "damage": 460,
        "lvl" : 200,
        "damage_type": "����"
]),     
([      "action" :"$N����Ķ���$n���ϴ���һ������$n˲�䷽����ң�$N�������$n��ǰץ������",

        "dodge": 95,
        "parry": 130,
        "force": 280,
//      "poison": 70,
        "damage": 560,
        "lvl" : 250, 
        "damage_type": "ץ��"
]),
([      "action" : "$N����$n���ٵ�ת�˼�Ȧ������һ������צ���棬��������ָ�������Ϯ��$n",
        
        "dodge": 100,
        "parry": 115,
        "force": 450,
//      "poison": 200,
        "damage": 580,
        "lvl" : 245, 
        "damage_type": "ץ��"
]),

([      "action" : "$N��Ӱ��ǰ���ᣬ����Ʈ��������ͻȻһת��������$n��$l£ȥ",
        "dodge": 125,
        "parry": 150,
        "force": 520,
//      "poison": 240, 
        "damage": 580,
        "lvl" : 265,
        "damage_type": "����"
]),

([      "action" : "$Nʹ��һ�С������֡���ʮָ������������ɫ��â����ס$n���������總��֮��",
        "dodge": 120,
        "parry": 145,
        "force": 450,
//      "poison": 300,
        "damage": 585,
        "lvl" : 300, 
        "skills_name" : "������",
        "damage_type": "ץ��"
])
});

int valid_enable(string usage) { return usage=="claw" ||  usage=="parry"; }

int valid_combine(string combo) { return combo=="chousui-zhang"; }

int valid_learn(object me)
{
        int  nb,  nh;
        nb = (int)me->query_skill("poison", 1);
        nh = (int)me->query_skill("sanyin-zhua", 1);

        if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
                return notify_fail("���������צ������֡�\n");

        if ((int)me->query_skill("huagong-dafa", 1) < 100)
                return notify_fail("��Ļ����󷨻�򲻹����޷�ѧ�������צ��\n");

        if ( nb < 100 && nb <= nh )
                return notify_fail("��Ķ�����Ϊ�������޷�����������צ��\n");

        if ((int)me->query("max_neili") < 1500)
                return notify_fail("�������̫�����޷����������צ��\n");

	if ((int)me->query_skill("claw", 1) < (int)me->query_skill("sanyin-wugongzhao", 1))
	    return notify_fail("��Ļ���צ��ˮƽ���ޣ��޷�����������������צ����\n");

	return 1;
}

string query_skill_name(int level)
{
	int i;
	for(i = sizeof(action); i > 0; i--)
		if(level >= action[i-1]["lvl"])
			return action[i-1]["skill_name"];
}

mapping query_action(object me, object weapon)
{
	int i, level;
	level = (int) me->query_skill("sanyin-wugongzhao",1);
	for(i = sizeof(action); i > 0; i--)
		if(level > action[i-1]["lvl"])
                        return action[NewRandom(i, 5, level/5)];

}

int practice_skill(object me)
{
	object* ob;
	int i,skill,damage;

	skill = me->query_skill("jiuyin-baiguzhao",1);
	if ((int)me->query("qi") < 40)
		return notify_fail("�������̫���ˡ�\n");

	if ((int)me->query("neili") < 40)
		return notify_fail("��������������������צ��\n");

	me->receive_damage("qi", 30);
        me->add("neili", -31);
	return 1;
}

mixed hit_ob(object me, object victim, int damage_bonus, int factor)
{
        int neili, neili2, cases, csz, par, mein, exp1, exp2;
        string result;
        neili=me->query("neili");
        neili2=victim->query("neili");
	exp1=me->query("combat_exp")/10000;
	exp2=victim->query("combat_exp")/10000;
	csz=me->query_skill("sanyin-wugongzhao",1);
	par=victim->query_skill("parry",1);
	mein=0;
	
	if(csz>150){
	if (present("huo yan", environment(me))){
        	mein=13;
        	if(csz<200) cases=2;
        	else if (csz>=200 && csz<300) cases=4;
        	else cases=7;
        	switch(random(cases)){
        		case 0:
        		result=HIR"\n$Nһ��������Ϻ��һ�֣��ؿڿ��ǣ�˫�����һ����磬���Ż���������$n��ȥ��\n"NOR;
        		damage_bonus+=350+random(100);
			break;
        		case 1:
        		result=HIB"\n$NͻȻ���Ʒ�һ�䣬��������һĨ�����������$n�������ƣ�$n����һ�����˺�һ����\n"NOR;
        		damage_bonus+=350+random(150);
        		break;
        		case 2:
        		result=HIY"\n$N����һ�Σ�������ȣ�˫�Ʒ���һ����ɫ���棬Ѹ���ޱȵ���$n��ȥ��\n"NOR;
        		damage_bonus+=350+random(200);
        		break;
        		case 3:
        		result=HIR"\n$N������У��͵�ҧ���Լ���ͷ��һ����Ѫ��$n��ȥ��$nɲʱ�䲻֪���룬ֻ�����ܿ���\n"NOR;
        		damage_bonus+=350+random(250);
        		break;
        		case 4:
        		result=HIW"\n$N����һ�Σ���������$nͻȻ�����һ˿�������Լ�Ϯ������������һ���ʹ��\n"NOR;
        		damage_bonus+=350+random(300);
        		break;
        		case 5:
        		result="\n$N"HIB"������"NOR"��"HIR"������"NOR"��"HIW"˫�����һ�󾢷�"NOR"����û���"HIG"��"HIY"��"HIM"��"HIR"ɫ"NOR"��Ư���ޱȡ�\n"NOR;
        		damage_bonus+=350+random(400);
			default:
        		result="\n$N"HIB"������"NOR"��"HIR"������"NOR"��"HIW"˫�����һ�󾢷�"NOR"����û���"HIG"��"HIY"��"HIM"��"HIR"ɫ"NOR"��Ư���ޱȡ�\n"NOR;
        		damage_bonus+=350+random(400);
        		break;
        	}
	        if(random(neili)>random(neili2)){
        	        message_vision(MAG"$n��Ȼ����һ����Ϯ��������һ�Σ�ͷ��΢΢��Щ���衣\n"NOR, me, victim);
                	victim->affect_by("xx_poison",
                        	([ "level" : csz + random(csz/2),
                           	   "id"    : me->query("id"),
	                           "duration" : csz/50 ]));
	        } else message_vision(YEL"$næ�������ֿ�����$Nצ�ϵľ綾�������⡣\n"NOR,me,victim);
       		return ([ "msg" : result, "damage" : damage_bonus ]);
        }}
	
        if(random(neili)>random(neili2)){
                message_vision(MAG"$n��Ȼ����һ����Ϯ��������һ�Σ�ͷ��΢΢��Щ���衣\n"NOR, me, victim);
                victim->affect_by("xx_poison",
                        ([ "level" : csz + random(csz/2),
                           "id"    : me->query("id"),
                           "duration" : csz/50 ]));
        } else message_vision(YEL"$næ�������ֿ�����$Nצ�ϵľ綾�������⡣\n"NOR,me,victim);
}

string perform_action_file(string action)
{
	return __DIR__"sanyin-wugongzhao/" + action;
}