package Part1;

public class Item {
    public String name; //项目名称
    public String site; //项目进行地点
    public int duration; //项目持续时间
    public String equipment; //项目所需装备
    public int queues; //队伍数量
    public int players; //每支队伍的运动员数量
    public int format; //赛制

    //构造方法
    protected Item(String name,String site,int duration,String equipment,int queues,int players,int format)
    {
        this.name=name;
        this.site=site;
        this.duration=duration;
        this.equipment=equipment;
        this.queues=queues;
        this.players=players;
        this.format=format;
    }

    //项目名称
    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name=name;
    }

    //项目举行地点
    public String getSite() {
        return site;
    }

    public void setSite(String site) {
        this.site=site;
    }

    //项目持续时间
    public int getDuration() {
        return duration;
    }

    public void setDuration(int duration) {
        this.duration = duration;
    }

    //项目所需装备
    public String getEquipment() {
        return equipment;
    }

    public void setEquipment(String equipment) {
        this.equipment = equipment;
    }

    //队伍数量
    public int getQueues() {
        return queues;
    }

    public void setQueues(int queues) {
        this.queues = queues;
    }

    //每支队伍人数
    public int getPlayers() {
        return players;
    }

    public void setPlayers(int players) {
        this.players = players;
    }

    //赛制
    public int getFormat() {
        return format;
    }

    public void setFormat(int format) {
        this.format = format;
    }

    //转化为字符串，便于打印
    public String toString()
    {
        return "Item's name:"+this.getName()+"\n"+
                "Site:"+this.getSite()+"\n"+
                "Item's duration:"+this.getDuration()+" days\n"+
                "Need equipment:"+this.getEquipment()+"\n"+
                "The count of queues:"+this.getQueues()+"\n"+
                "The count of players:"+this.getPlayers()+"\n";
    }

    public static void main(String[] args) {
        //跳水
        Item Diving = new Item("Diving","Pool",15,"None",120,16,3);
        System.out.println(Diving.toString());

        //射击
        Item Shoot = new Item("Shoot","TargetRange",8,"Gun",100,8,8);
        System.out.println(Shoot.toString());

        //乒乓球
        Item Pingpong = new Item("Ping-pong","TableTennisTable",3,"bat",80,5,5);
        System.out.println(Pingpong.toString());
    }
}

