//room: didao5.c
inherit ROOM;

void create()
{
  set("short","�ص�");
  set("long", @LONG
�������棬�㲻���뵽���ƺ���ÿ�����䶼��������ɱ����
��е����˽��ˣ�Խ��Խ���ˡ�
LONG );

  set("exits",([
      "east" : __DIR__"didao3",
      "west" : __DIR__"didao3",
      "north" : __DIR__"didao2",
      "south" : __DIR__"didao1",
  ]));
  set("objects",([
      "/clone/beast/dushe" : 4,
  ]));
  setup();
  replace_program(ROOM);
}