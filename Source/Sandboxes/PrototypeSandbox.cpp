#include <chrono>
#include <iostream>
#include <thread>

#include <Deliberation/ECS/PrototypeManager.h>
#include <Deliberation/ECS/ComponentPrototype.h>
#include <Deliberation/ECS/EntityPrototype.h>
#include <Deliberation/ECS/Level.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Deliberation.h>

using namespace deliberation;


struct Head:
    public Component<Head>
{
    float x = 0.0f;
    float y = 0.0f;
};

struct Body:
    public Component<Body>
{
    float a = 0.0f;
    std::string b;
};

struct Leg:
    public Component<Leg>
{
    int c;
    int length = 0;
};

struct Shoe:
    public Component<Shoe>
{
    int size = 0;
};

struct HeadPrototype:
    public ComponentPrototype<Head>
{
    void updateComponent(Head & head)
    {
        head.x = m_json.value("x", head.x);
        head.y = m_json.value("y", head.y);

        std::cout << "  Setting Head " << this << ": ";
        std::cout << head.x << ", " << head.y << std::endl;
    }
};

struct BodyPrototype:
    public ComponentPrototype<Body>
{
    void updateComponent(Body & body)
    {
        body.a = m_json.value("a", body.a);
        body.b = m_json.value("b", body.b);

        std::cout << "  Setting Body " << this << ": ";
        std::cout << body.a << ", " << body.b << std::endl;
    }
};

struct LegPrototype:
    public ComponentPrototype<Leg>
{
    void updateComponent(Leg & leg)
    {
        leg.length = m_json.value("length", leg.length);
        leg.c = m_json.value("c", leg.c);

        std::cout << "  Setting Leg " << this << ": ";
        std::cout << leg.length << " " << leg.c << std::endl;
    }
};

struct ShoePrototype:
    public ComponentPrototype<Shoe>
{
    void updateComponent(Shoe & shoe)
    {
        shoe.size = m_json.value("size", shoe.size);

        std::cout << "  Setting Shoe " << this << ": ";
        std::cout << shoe.size << std::endl;
    }
};

int main(int argc, char ** argv)
{
    World world;
    const auto listPath = DeliberationDataPath("Data/Examples/PrototypeSandbox/list.json");
    auto prototypeManager = std::make_shared<PrototypeManager>(world, listPath);

//    auto entityPrototype = prototypeManager->getOrCreateEntityPrototype("HumanBody");
//
//    auto headPrototype = std::make_shared<HeadPrototype>();
//    headPrototype->setJson(Json::parse("{\"x\": 2.0, \"y\": 3.0}"));
//    entityPrototype->addComponentPrototype(headPrototype);
//
//    auto body0 = prototypeManager->createEntity("HumanBody");
//
//    auto neckPrototype = std::make_shared<NeckPrototype>();
//    neckPrototype->setJson(Json::parse("{\"Length\": 77.0}"));
//    entityPrototype->addComponentPrototype(neckPrototype);
//
//    std::cout << "Updating Entities" << std::endl;
//    prototypeManager->updateEntities();
//    std::cout << "...done" << std::endl;
//
//
//    auto derivedEntityPrototype = prototypeManager->getOrCreateEntityPrototype("EnhancedHumanBody");
//    derivedEntityPrototype->addBaseEntityPrototype(entityPrototype);
//
//    auto derivedHeadPrototype = std::make_shared<HeadPrototype>();;
//    derivedHeadPrototype->setJson(Json::parse("{\"x\": 5.0}"));
//
//    derivedEntityPrototype->addComponentPrototype(derivedHeadPrototype);
//
//    auto enhancedBody0 = prototypeManager->createEntity("EnhancedHumanBody");
//
//    std::cout << "Updating Entities" << std::endl;
//    prototypeManager->updateEntities();
//    std::cout << "...done" << std::endl;
//
//    std::cout << std::endl;
//
//
//    auto alienPrototype = prototypeManager->getOrCreateEntityPrototype("AlienBody");
//    auto tentaclesPrototype = std::make_shared<TentaclesPrototype>();
//    tentaclesPrototype->setJson(Json::parse("{\"NumTentacles\": 11}"));
//    alienPrototype->addComponentPrototype(tentaclesPrototype);
//    derivedEntityPrototype->addBaseEntityPrototype(alienPrototype);
//    derivedHeadPrototype->setJson(Json::parse("{\"x\": 6.0}"));
//
//    std::cout << "Updating Entities" << std::endl;
//    prototypeManager->updateEntities();
//    std::cout << "...done" << std::endl;

    prototypeManager->registerComponentPrototype<HeadPrototype>("Head");
    prototypeManager->registerComponentPrototype<BodyPrototype>("Body");
    prototypeManager->registerComponentPrototype<LegPrototype>("Leg");
    prototypeManager->registerComponentPrototype<ShoePrototype>("Shoe");

    const auto mapPath = DeliberationDataPath("Data/Examples/PrototypeSandbox/map.json");

    Level level(prototypeManager, mapPath);

    while (true)
    {
        prototypeManager->reloadList();
        level.reload();

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}