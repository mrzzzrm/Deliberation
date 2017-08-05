#include <Deliberation/Resource/PrototypeManager.h>

namespace deliberation
{

void PrototypeManager::reload()
{
    for (auto & pair : m_prototypeTypeContainersByTypeID)
    {
        auto & container = pair.second;

        for (const auto & path : container.paths)
        {
            std::cout << "PrototypeManager: Reloading Prototypes from '" << path << "'" << std::endl;

            std::ifstream prototypeFile(path);
            Assert(prototypeFile.is_open(), "Couldn't open '" + path + "'");

            Json prototypesJson;
            prototypesJson << prototypeFile;

            Assert(prototypesJson.is_object(), "");

            for (auto & pair2 : Json::iterator_wrapper(prototypesJson))
            {
                auto name = pair2.key();
                auto & prototypeJson = pair2.value();
                
                std::cout << "PrototypeManager: Reloading Prototype '" << name << "'" << std::endl;

                std::shared_ptr<AbstractPrototype> prototype;

                auto iter = container.prototypeByName.find(name);
                if (iter == container.prototypeByName.end())
                {
                    iter = container.prototypeByName.emplace(name, container.prototypeFactory()).first;
                }
                prototype = iter->second;

                try {
                    prototype->reload(prototypeJson);
                } catch (const std::exception & e) {
                    Fail("Error while loading prototype '" + name + "': " +
                             e.what() + ". JSON:\n" + prototypeJson.dump());
                }
            }
        }
    }
}

}

