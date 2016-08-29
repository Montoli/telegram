#ifndef BASE_STATE_H
#define BASE_STATE_H

#include "corgi/entity_common.h"

class StateManager;

class BaseState {
public:

  virtual void Update(corgi::WorldTime delta_time) {}
  virtual void Render(corgi::WorldTime delta_time) {}

  virtual void Init() {}
  virtual void Cleanup() {}

  virtual void Suspend() {}
  virtual void Resume() {}

  // Starts the state ending.  Cleanup code should go
  // in Cleanup though.
  virtual void EndState() { is_ending_ = true; }

  virtual void SetStateManager(StateManager* state_manager) {
    state_manager_ = state_manager;
  }

  virtual bool IsEnding() { return is_ending_;  }

private:
  StateManager* state_manager_ = nullptr;
  bool is_ending_ = false;

};


#endif // BASE_STATE_H