#include <bandit/bandit.h>

#include <iostream>

#include <Deliberation/Context.h>

using namespace bandit;

go_bandit([]()
{
    describe("Bandit", []()
    {
        deliberation::Context context;

        it("works", []()
        {
            AssertThat(5, Equals(5));
        });
    });
});

