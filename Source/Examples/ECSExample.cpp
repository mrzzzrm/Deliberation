#include <iostream>

#include <glm/glm.hpp>

#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/ECS/Component.h>
#include <Deliberation/ECS/ComponentFilter.h>
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


struct GunFired
{
    GunFired(Entity gun, Entity bullet):
        gun(gun),
        bullet(bullet)
    {}

    Entity gun;
    Entity bullet;
};

struct GunComponent:
    Component<GunComponent, ComponentSubscriptions<GunComponent, GunFired>>
{
    GunComponent(float freq):
        freq(freq)
    {
    }

    void fire()
    {
        emit(GunFired(Entity(), Entity()));
    }

    void receive(const GunFired & event)
    {
        std::cout << "I heard a shot" << std::endl;
    }

    float freq;
};

struct GunSystem:
    public System<GunSystem>
{
    GunSystem(World & world):
        Base(world, ComponentFilter::requires<GunComponent>())
    {
    }

    virtual void onUpdate(Entity & entity, float seconds) override
    {
        auto & gun = entity.component<GunComponent>();

        std::cout << "Updating '" << entity.name() << "' with frequency " << gun.freq << std::endl;

        for (auto i = 0; i < 5; i++)
        {
            auto bullet = world().createEntity("Bullet");
            bullet.addComponent<PositionComponent>(glm::vec3{i, 1.0f, 2.0f});
            bullet.addComponent<ColliderComponent>();
            world().eventManager().emit<GunFired>({entity, bullet});
        }
    }

    virtual void onEntityAdded(Entity & entity) override
    {
        std::cout << "GunSystem: Gun " << entity.name() << " added" << std::endl;
    }

    virtual void onEntityRemoved(Entity & entity) override
    {
        std::cout << "GunSystem: Gun " << entity.name() << " removed" << std::endl;
    }
};

struct PhysicsSystem:
    public System<PhysicsSystem>
{
    PhysicsSystem(World & world):
        Base(world, ComponentFilter::requires<PositionComponent, ColliderComponent>())
    {
    }

    virtual void onUpdate(Entity & entity, float seconds) override
    {
        std::cout << "Moving " << entity.name() << std::endl;
    }
};

struct AnotherSystem:
    public System<AnotherSystem>
{
    AnotherSystem(World & world):
        Base(world, ComponentFilter::none())
    {
        world.eventManager().subscribe<GunFired>(*this);
    }

    void receive(const GunFired & gunFired)
    {
        std::cout << "AnotherSystem received GunFired (Bullet=" << gunFired.bullet.name()
                  << "; id=" << gunFired.bullet.id() << ")" << std::endl;
    }
};

int main(int argc, char * argv[])
{
    World world;

    world.addSystem<AnotherSystem>();
    world.addSystem<GunSystem>();
    world.addSystem<PhysicsSystem>();

    std::cout << "------ Setup ------" << std::endl;
    auto ship = world.createEntity("Ship");
    ship.addComponent<PositionComponent>(glm::vec3(1.0f, 33.0f, 7.0f));
    ship.addComponent<ColliderComponent>();

    auto bigShip = world.createEntity("BigShip");
    bigShip.addComponent<PositionComponent>(glm::vec3(10.0f, 330.0f, 70.0f));
    bigShip.addComponent<ColliderComponent>();

    auto gun0 = bigShip.createChild("BigShipGun0");
    gun0.addComponent<GunComponent>(3.0f);
    auto gun1 = bigShip.createChild("BigShipGun1");
    gun1.addComponent<GunComponent>(4.0f);

    auto checkpoint = world.createEntity("Checkpoint");
    checkpoint.addComponent<PositionComponent>(glm::vec3(4.0, 2.0f, 0.0f));


    std::cout << "------------------ Update1 ------------------" << std::endl;
    world.update(1.0f);

    std::cout << "------------------ Update2 ------------------" << std::endl;
    world.update(1.0f);

    gun0.component<GunComponent>().fire();

    std::cout << "------------------ End ------------------" << std::endl;
    return 0;
}

