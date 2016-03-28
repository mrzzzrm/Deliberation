#include <iostream>

#include <glm/glm.hpp>

#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/ECS/Component.h>
#include <Deliberation/ECS/Entity.h>
#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/World.h>

using namespace deliberation;

struct PositionComponent:
    public Component<PositionComponent>
{
    PositionComponent(const glm::vec3 & position):
        position(position)
    {
    }

    glm::vec3 position;
};

struct ColliderComponent:
    public Component<ColliderComponent>
{
//    virtual void onCreate() override
//    {
//        auto & position = entity().getComponent<PositionComponent>();
//        std::cout << "Collider with position at " << position << std::endl;
//    }
};

struct GunComponent:
    public Component<GunComponent>
{
    GunComponent(float freq):
        freq(freq)
    {
    }

    float freq;
};

//struct GunFired
//{
//    Entity gun;
//    Entity bullet;
//};

struct GunSystem:
    public System<GunSystem>
{
//    GunSystem():
//        Base(ComponentFilter<GunComponent>()),
//    {
//    }
//
//    virtual void update(Entity entity, float seconds)
//    {
//        auto & position = entity.getComponent<PositionComponent>();
//
//        std::cout << "Updating '" << entity.name() << "' with frequency " << gun.freq << std::endl;
//
//        auto bullet = world().createEntity("Bullet");
//        bullet.addComponent<PositionComponent>({0.0f, 1.0f, 2.0f});
//        bullet.addComponent<ColliderComponent>();
//
//        world().eventManager().emit<GunFired>({entity, bullet});
//    }
//
//    virtual void onEntityAdded(Entity entity)
//    {
//        std::cout << "GunSystem: Gun " << entity.toString() << " added" << std::endl;
//    }
//
//    virtual void onEntityRemoved(Entity entity)
//    {
//        std::cout << "GunSystem: Gun " << entity.toString() << " removed" << std::endl;
//    }
};

//struct AnotherSystem:
//    public System<AnotherSystem>
//{
//    AnotherSystem(EventManager & eventManager)
//    {
//        eventManager.subscribe<GunFired>(*this);
//    }
//
//    void receive(const GunFired & gunFired)
//    {
//        std::cout << "AnotherSystem received GunFired" << std::endl;
//    }
//}

int main(int argc, char * argv[])
{
    World world;

    std::cout << "------ Setup ------" << std::endl;

    auto ship = world.createEntity("Ship");
    ship.addComponent<PositionComponent>(glm::vec3(1.0f, 33.0f, 7.0f));
    ship.addComponent<ColliderComponent>();

    auto bigShip = world.createEntity("BigShip");
    ship.addComponent<PositionComponent>(glm::vec3(10.0f, 330.0f, 70.0f));
    ship.addComponent<ColliderComponent>();

    auto gun0 = bigShip.createChild("BigShipGun0");
    gun0.addComponent<GunComponent>(3.0f);
    auto gun1 = bigShip.createChild("BigShipGun1");
    gun1.addComponent<GunComponent>(4.0f);

    auto checkpoint = world.createEntity("Checkpoint");
    checkpoint.addComponent<PositionComponent>(glm::vec3(4.0, 2.0f, 0.0f));

//    world.addSystem<AnotherSystem>();
//    world.addSystem<GunSystem>();

    std::cout << "World: " << std::endl << world.toString() << std::endl;

    std::cout << "------ Update 0 ------" << std::endl;
    world.update();
    std::cout << "World: " << std::endl << world.toString() << std::endl;

    std::cout << "------ Update 1 ------" << std::endl;
    gun0.deactivate();
    world.update();
    std::cout << "World: " << std::endl << world.toString() << std::endl;

    std::cout << "------ Update 2 ------" << std::endl;
    bigShip.deactivate();
    world.update();
    std::cout << "World: " << std::endl << world.toString() << std::endl;

    std::cout << "------ Update 3 ------" << std::endl;
    gun0.removeComponent<GunComponent>();
    world.update();
    std::cout << "World: " << std::endl << world.toString() << std::endl;

    return 0;
}

