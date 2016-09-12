#include <iostream>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Math/Box.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Draw.h>

#include <Deliberation/Physics/Contacts/BoxContact.h>
#include <Deliberation/Physics/Contacts/Manifold.h>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/DebugGeometryRenderer.h>
#include <Deliberation/Scene/DebugGrid3DRenderer.h>
#include <Deliberation/Scene/DebugCameraNavigator3D.h>

using namespace deliberation;

void TestBoxes(const Box & boxA, const Box & boxB)
{
    std::cout << "  -------------- TestBoxes ----------------" << std::endl;

    Manifold manifold;

    auto r = CollideBoxBox(boxA, boxB, manifold);

    if (r)
    {
        std::cout << "    Collision";
    }
    else
    {
        std::cout << "    Separated";
    }

}

void AABBTest()
{
    std::cout << "---------------- AABBTest ----------------" << std::endl;

    Box originUnitBox({0, 0, 0}, {0.5f, 0, 0}, {0, 0.5f, 0}, {0, 0, 0.5f});
    Box testBox0({0.3, 0, 0}, {0.5f, 0, 0}, {0, 0.5f, 0}, {0, 0, 0.5f});

    TestBoxes(originUnitBox, testBox0);
}


class BoxBoxSandbox:
    public Application
{
public:
    BoxBoxSandbox():
        Application("BoxBoxSandbox")
    {

    }

    virtual void onStartup() override
    {
        m_grid.reset(context(), 2.5f, m_camera);

        m_camera.setPosition({0.0f, 5.0f, 15.0f});
        m_camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
        m_camera.setAspectRatio((float)context().backbuffer().width() / (float)context().backbuffer().height());

        m_clear = context().createClear();

        m_navigator.reset(m_camera, inputAdapter(), 10.0f);

        m_geometryRenderer.reset(context(), m_camera);

        m_boxA = m_geometryRenderer.get().addBox({0.5f, 1.0f, 1.5f}, {1.0f, 0.6f, 0.6f}, true);
        m_geometryRenderer.get().box(m_boxA).transform().setPosition({3.0f, 0.0f, 0.0});

        m_geometryRenderer.get().addBox({1.3f, 1.5f, 1.7f}, {0.0f, 1.0f, 0.6f}, true);

        auto box = m_geometryRenderer.get().box(m_boxA).toBox();
        auto p = box.p();
        m_halfExtent[0] = m_geometryRenderer.get().addArrow(p, box.x(), {0.8f, 0.2f, 0.2f});
        m_halfExtent[1] = m_geometryRenderer.get().addArrow(p, box.y(), {0.2f, 0.8f, 0.2f});
        m_halfExtent[2] = m_geometryRenderer.get().addArrow(p, box.z(), {0.2f, 0.2f, 0.8f});
    }

    virtual void onFrame(float seconds) override
    {
        m_navigator.get().update(seconds);

        {
            auto box = m_geometryRenderer.get().box(m_boxA).toBox();
            auto p = box.p();
             m_geometryRenderer.get().arrow(m_halfExtent[0]).reset(p, box.x() * box.halfExtent().x);
             m_geometryRenderer.get().arrow(m_halfExtent[1]).reset(p, box.y() * box.halfExtent().y);
             m_geometryRenderer.get().arrow(m_halfExtent[2]).reset(p, box.z() * box.halfExtent().z);
        }

        auto prevTransform = m_geometryRenderer.get().box(0).transform();

        {
            glm::vec3 t;
            glm::vec3 a;
            auto speed = 5.0f;
            auto r = glm::pi<float>();

            t.x += -speed * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_J) ? 1 : 0);
            t.x -= -speed * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_L) ? 1 : 0);
            t.y += -speed * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_K) ? 1 : 0);
            t.y -= -speed * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_I) ? 1 : 0);
            t.z += -speed * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_U) ? 1 : 0);
            t.z -= -speed * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_O) ? 1 : 0);

            a.x += -r * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_Y) ? 1 : 0);
            a.x -= -r * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_X) ? 1 : 0);
            a.y += -r * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_C) ? 1 : 0);
            a.y -= -r * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_V) ? 1 : 0);
            a.z += -r * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_B) ? 1 : 0);
            a.z -= -r * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_N) ? 1 : 0);

            m_geometryRenderer.get().box(0).transform().worldTranslate(t);
            m_geometryRenderer.get().box(0).transform().worldRotate(glm::quat(a));
        }

        Manifold manifold;
        auto r = CollideBoxBox(m_geometryRenderer.get().box(0).toBox(), m_geometryRenderer.get().box(1).toBox(), manifold);

        std::cout << "Collision: " << r << " " << manifold.depth() << " " << manifold.position() << std::endl;

        if (r)
        {
            m_geometryRenderer.get().box(0).transform() = prevTransform;

            auto origin = m_geometryRenderer.get().box(0).transform().position();
            auto delta = manifold.normal();

            if (!m_contactSeparationIndex.engaged())
            {
                m_contactSeparationIndex.reset(
                    m_geometryRenderer.get().addArrow(origin, delta, glm::vec3(1.0f, 1.0f, 1.0f))
                );
            }
            else
            {
                m_geometryRenderer.get().arrow(m_contactSeparationIndex.get()).reset(origin, delta);
            }


            if (!m_contactPointIndex.engaged())
            {
                m_contactPointIndex.reset(
                    m_geometryRenderer.get().addPoint(manifold.position(), glm::vec3(1.0f, 1.0f, 1.0f))
                );
            }
            else
            {
                m_geometryRenderer.get().point(m_contactPointIndex.get()).setPosition(manifold.position());
            }
        }
        else
        {
            if (m_contactSeparationIndex.engaged())
            {
                m_geometryRenderer.get().removeArrow(m_contactSeparationIndex.get());
                m_contactSeparationIndex.disengage();
            }

            if (m_contactPointIndex.engaged())
            {
                m_geometryRenderer.get().removePoint(m_contactPointIndex.get());
                m_contactPointIndex.disengage();
            }
        }

        m_clear.schedule();
        m_grid.get().draw();
        m_geometryRenderer.get().schedule();
    }

private:
    Optional<DebugGeometryRenderer>     m_geometryRenderer;
    Clear                               m_clear;
    Optional<DebugGrid3DRenderer>       m_grid;
    Camera3D                            m_camera;
    Uniform                             m_viewProjectionHandle;
    Uniform                             m_transformHandle;
    Optional<DebugCameraNavigator3D>    m_navigator;

    Optional<size_t>                    m_contactPointIndex;
    Optional<size_t>                    m_contactSeparationIndex;

    size_t                              m_boxA;
    size_t                              m_halfExtent[3];
};

int main()
{
    AABBTest();

    return BoxBoxSandbox().run();
}