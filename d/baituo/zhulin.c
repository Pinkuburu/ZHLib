//zhulin.c
inherit ROOM;
void create()
{
        set("short","����");
        set("long",
                "�����������֣������߳�û���ϱ�����ݴԡ�������һ�����Ҷ���\n"
        );

        set("exits",([
                "northup" : __DIR__"cave",
                "south" : __DIR__"cao2",
        ]));

        set("objects",([
                "/clone/beast/qingshe" : random(3),
                "/clone/beast/dushe" : random(2),
        ]));

        setup();
        replace_program(ROOM);
}