// ��������ػ����̣�girl.c

#include <ansi.h>

// ���ܹ�Ӧ��NPC��Ҫ����Щ�˱�����ָ���ĳ�������������ָ����
// �ǳ�����NPC��ID��
mapping rcv_npcs = ([
        "/d/city3/jiudian"        : "tang hua",
        "/d/city3/tidufu"         : "wu tiande",
        "/d/dali/qiandian"        : "duan zhengming",
        "/d/dali/wangfugate"      : "chu wanli",
        "/d/changan/yamen-datang" : "liang xinglu",
        "/d/changan/fangzhangshi" : "huitong",
        "/d/city/duchang"         : "pang tongtuo",
        "/d/city/houyuan"         : "cui yuanwai",
        "/d/city/ymzhengting"     : "cheng yaofa",
        "/d/beijing/kangfu_dating": "kang qinwang",
        "/d/beijing/dangpu"       : "zhang degui",
        "/d/suzhou/yamen"         : "feng zhengdong",
        "/d/suzhou/jubaozhai"     : "sun baopi",
        "/d/hangzhou/road12"      : "li kexiu",
        "/d/hangzhou/lingyinsi"   : "lao heshang",
        "/d/fuzhou/yamen"         : "ge shiha",
        "/d/lingzhou/neizai"      : "haolian tieshu",
        "/d/lingzhou/yipindayuan" : "yun zhonghe",
]);

// ��Ӧ����Ʒ�����ڽ��ܵ�ʱ��NPCֻ����˶��������֣���������
// MUD�еĴ���һ���Բ��õ�Ե�ʣ�������ÿ���ṩ����Ʒ���Ҫ��
// ���������Ҫ��MUD�е�������Ʒ������
string *supply_objs = ({
        "/clone/weapon/changjian",
        "/clone/weapon/jili",
        "/clone/weapon/tiegun",
        "/clone/weapon/zhubang",
        "/clone/weapon/zhujian",
        "/clone/weapon/changbian",
        "/clone/weapon/dagger",
        "/clone/weapon/duanjian",
        "/clone/weapon/dadao",
        "/clone/weapon/gangzhang",
        "/clone/weapon/tiejia",
        "/clone/weapon/hammer",
        "/clone/weapon/gangdao",
        "/clone/weapon/gangjian",
        "/clone/weapon/jiedao",
        "/clone/weapon/qimeigun",
        "/clone/weapon/mudao",
        "/clone/weapon/lianzi",
        "/clone/weapon/feihuangshi",
        "/clone/cloth/tiejia",
});

void startup();

// ������󴴽�
void create()
{
        seteuid(getuid());
        if (! clonep() && find_object(QUEST_D)) startup();
}

void start_quest()
{
        object env;             // ������Ʒ��NPC���ڵĻ���
        object npc;             // ������Ʒ��NPC����
        object *obs;            // Ŀǰ���е�SUPPLY����
        string *env_room;       // �������ܹ��ڵĵص�
        string room;            // NPC���ڵĻ����ļ�����
        string sob_file;        // ���յ���Ʒ��Ӧ���ļ�

        object qob;             // �������

        // ѡ��һ���µĵص�(Ŀǰû��SUPPLY�����)
        env_room = keys(rcv_npcs);
        obs = children("/clone/quest/supply");
        if (arrayp(obs) && sizeof(obs) > 0)
                env_room -= obs->query("quest_position");

        if (sizeof(env_room) < 1)
                // �޷��ҵ������ĵص�
                return;

        room = env_room[random(sizeof(env_room))];
        env = get_object(room);

        // ѡ��ĵص��е���
        if (! objectp(npc = present(rcv_npcs[room], env)) ||
            npc->query("startroom") != base_name(env))
                // �޷��ҵ��õص��к��ʵ�NPC������
                return;


        // ѡ��һ�ֱ���
        sob_file = supply_objs[random(sizeof(supply_objs))];

        // ��ʼ�������һЩ��Ϣ�������ص㣩
        qob = new("/clone/quest/supply");
        qob->set("quest_position", room);

        CHANNEL_D->do_channel(find_object(QUEST_D),
                              "task", "����(SUPPLY)��" + env->short(1) +
                              NOR WHT "�ص�����" + npc->name() +
                              NOR WHT "������һ������");

        // ���������Լ����г�ʼ��
        qob->init_quest(npc, sob_file);
}

private void heart_beat()
{
        if (! find_object(QUEST_D))
                return;

        // ������ԣ�ÿ����������һ��QUEST
        start_quest();
}

// �����ػ����̻����������
void startup()
{
        // ����
        if (! find_object(QUEST_D))
                return;

        if (! query_heart_beat())
                CHANNEL_D->do_channel(find_object(QUEST_D),
                                      "task", "����(SUPPLY)�����ˡ�");

        // ƽ��ÿ�ķ��Ӳ���һ������
        set_heart_beat(110 + random(20));
}

// ֹͣ����������
void stop()
{
        set_heart_beat(0);
}