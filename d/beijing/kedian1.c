inherit ROOM;

void create()
{
        set("short", "�͵�");
        set("long", @LONG
����һ�Ҽ�Ǯ�����Ŀ͵꣬��Ϊ�ط�ƫƧ�����ͺ��١���ʱ�д�
֮���Ӷ��ϱߴ������ƹ��粻֪���Ķ�ȥ�ˣ����������һ���ˡ��ϱ�
�Ǻ�Ժ��ͨ�����᷿��
LONG );
        set("no_sleep_room",1);
        set("exits", ([
                "south" : __DIR__"kedian2",
                "northwest" : __DIR__"yangliu1",
        ]));
        set("no_clean_up", 0);
        set("coor/x", -220);
        set("coor/y", 4000);
        set("coor/z", 0);
        setup();
        replace_program(ROOM);
}
