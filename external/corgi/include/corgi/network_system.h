// Copyright 2015 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef CORGI_NETWORK_SYSTEM_H_
#define CORGI_NETWORK_SYSTEM_H_

#include "system.h"

namespace corgi {

template <typename T>
class NetworkSystem : public System<T> {
 public:
	 virtual void SetRewindBufferProperties(WorldTime buffer_length,
		 WorldTime buffer_resolution);

	 virtual void StartRewindBuffer();

	 virtual void UpdateAllEntities(WorldTime delta_time);

	 virtual void RewindToTimestamp(Worldtime new_timestamp);


 private:
	 virtual void SaveStateToBuffer();

 protected:
	 WorldTime timestamp;

};

}  // corgi

#endif  // CORGI_NETWORK_SYSTEM_H_
