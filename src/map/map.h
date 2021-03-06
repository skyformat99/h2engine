#ifndef _FF_MAP_H_
#define _FF_MAP_H_

#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

#include "common/entity.h"
#include "base/smart_ptr.h"

namespace ff
{

struct MapPoint{//!坐标点
    std::map<userid_t, EntityRef> entities;
    void getAllSession(std::vector<userid_t>& ret);
    void getAllEntity(std::vector<EntityPtr>& ret, userid_t uidIgnore = 0);
};
struct Map9Grid{//!九宫格
    std::map<userid_t, EntityRef> entities;
    
    void getAllSession(std::vector<userid_t>& ret);
    void getAllEntity(std::vector<EntityPtr>& ret, userid_t uidIgnore = 0);
};

class MapObj{
public:
    struct EntityPos{
        EntityPos():x(0), y(0){}
        int       x;
        int       y;
        EntityRef entity;
    };
public:
    MapObj(std::string s, int w, int h);
    
    const std::string& getName(){ return name; }
    
    std::vector<EntityPtr> rangeGetEntities(int x, int y, int radius);
    MapPoint* getPoint(int x, int y);
    
    Map9Grid* getGrid(int x, int y, std::set<Map9Grid*>* retSet = NULL);
    void get9Grid(int x, int y, std::set<Map9Grid*>& retSet);
    
    bool enter(EntityPtr& entity, int x, int y);
    bool leave(EntityPtr& entity);
    bool move(EntityPtr& entity, int xTo, int yTo);
    
    bool getEntityPos(userid_t uid, int& x, int& y);
    void updateEntityPos(EntityPtr& entity, int x, int y);
    bool getAllEntity(std::vector<EntityPtr>& ret, int nType = -1);
public:
    std::string name;
    int         width;
    int         height;
    int         width9Grid;
    int         height9Grid;
    std::vector<MapPoint>           m_allPos;
    std::vector<Map9Grid>           m_all9Grid;
    std::map<userid_t, EntityPos>   m_allEntity;
};

typedef SharedPtr<MapObj> MapObjPtr;

#define VIEW_RAIDUS Singleton<MapMgr>::instance().m_nViewRadius
#define GRID_SIZE   int(::ceil(Singleton<MapMgr>::instance().m_nViewRadius / 1.5))

class MapMgr{
public:
    MapMgr():m_nViewRadius(10){}
    virtual ~MapMgr(){}
    
    bool init();
    bool cleanup();
    
    MapObjPtr gen(const std::string& s, int w, int h);
    void add(MapObjPtr& p);
    bool del(const std::string& s);
    MapObjPtr get(const std::string& s);
    size_t size() const { return m_maps.size(); }
    
    template <typename T>
    void foreach(T f){
        std::map<std::string, MapObjPtr>::iterator it = m_maps.begin();
        for (; it != m_maps.end(); ++it){
            f(it->second);
        }
    }

public:
    int                                 m_nViewRadius;//!视野半径
    std::map<std::string, MapObjPtr>    m_maps;
};

#define CMD_MAP_ENTER  100
#define CMD_MAP_LEAVE  101
#define CMD_MAP_MOVE   102

class MapCtrl:public EntityField
{
public:
    MapObjPtr       curMap;
    int             x;
    int             y;
};

}
#endif
