// ����lywin 2000/8/27 

#include <ansi.h>
inherit ROOM;

 void create()
{
        set("short", "��ɽ");
        set("long", @LONG
�����һ���������̵Ŀ�ɽ��ɽ��͵������ܿ����ö�ö�Ŀ�����
��ؼ�Ϊ�ḻ����˵��������ڳ��������¼��������커���õĺÿ�ʯ��
������Ӧ���������ԣ�
LONG
        );
        set("no_clean_up", 0);
        set("exits", ([
                "north" : __DIR__"kuangshan",
        ]));
        set("objects", ([
                __DIR__"kuang/kuangmai" : 10,
        ])); 

        set("no_fight",1);
        set("no_beg",1);
        set("no_steal",1);
        setup();
        replace_program(ROOM);
}
