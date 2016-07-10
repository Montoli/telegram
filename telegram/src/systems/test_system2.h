#ifndef TEST_SYSTEM2_H
#define TEST_SYSTEM2_H
#include "corgi/system.h"

struct TestData2 {
  int num = 42;
};


class TestSystem2 : public corgi::System<TestData2> {
public:

  virtual void UpdateAllEntities(corgi::WorldTime delta_time);

  virtual void DeclareDependencies();

  virtual void InitEntity(corgi::EntityRef& entity);

};

CORGI_REGISTER_SYSTEM(TestData2, TestSystem2)


#endif // TEST_SYSTEM_H2