// Room: /d/henshan/wangyuetai.c
#include <room.h>
inherit JIETI;
inherit ROOM;
void create()
{
        set("short", "����̨");
        set("long", @LONG
����̨��ף�ڵ��һ���ʾ�ʯ��ÿ��ҹ������µ��գ��������̱�
�֣�����������ԣ�������ɫ����ӳ�Ͱ���Ⱥ�塣��ʱ "��Ȼ�����
��ʮ���壬����������У�ʼ���·��޴���Ҳ��" ǰ�������ʫ: �˼�
�����ѽԾ����˵������δ�͡�
LONG);
        set("no_fight",1);
        set("exits", ([ /* sizeof() == 4 */
           "southup"    : __DIR__"zhurongdian",
        ]));
        set("outdoors", "henshan");
        set("no_clean_up", 0);
        setup();
}

void init()
{
        add_action("do_jieti","jieti");
        add_action("do_decide","decide");
}