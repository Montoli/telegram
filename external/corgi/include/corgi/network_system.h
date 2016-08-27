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
#include <memory>

namespace corgi {

	// Basic implementation for 



template <typename T>
class NetworkSystem : public System<T> {
 public:
	NetworkSystem() {}

	virtual void SetRewindBufferProperties(int snapshot_count,
			WorldTime snapshot_frequency) override {

		//rewind_buffer = std::unique_ptr<RewindBufferEntry>(new RewindBufferEntry[snapshot_count]);
		rewind_buffer_.resize(snapshot_count);
		rewind_buffer_size_ = snapshot_count;
		most_recent_buffer_index_ = 0;

	}

	virtual void TakeRewindSnapshot(WorldTime timestamp) override {
		most_recent_buffer_index_ = (most_recent_buffer_index_ + 1) % rewind_buffer_size_;
		RewindBufferEntry& snapshot = rewind_buffer_[most_recent_buffer_index_];
		snapshot.timestamp = timestamp;
		snapshot.component_data = component_data_;
		snapshot.is_valid = true;
	}

		// x % y is negative, if x is negative.  This makes sure it is
		// always positive, so -2 % 10 = 8 instead of -2.
	inline int SafeMod(int dividend, int divisor) {
		return divisor + (dividend % divisor) % divisor;
	}

	virtual bool RewindToTimestamp(WorldTime timestamp) override {
		int start = most_recent_buffer_index_;
		// Find the most recent snapshot that is before the requested timestamp.
		bool done = false;
		while (!done) {
			/*
			RewindBufferEntry& snapshot = rewind_buffer_[most_recent_buffer_index_];
			// if we ever hit an invalid snapshot then that means we don't have
			// enough data to perform the rewind operation.  Return true and abort.
			if (!snapshot.is_valid) return true;
			if (snapshot.timestamp < timestamp) {
				done = true;
				break;
			} else {
				snapshot.is_valid = false;
					most_recent_buffer = SafeMod(most_recent_buffer_index_ - 1, rewind_buffer_size_)
			}
			*/
			//done = most_recent_buffer_index_ < 
		}
		return false;
	}

 protected:

	struct RewindBufferEntry {
		RewindBufferEntry() : is_valid(false) {}

		bool is_valid;
		WorldTime timestamp;
		std::vector<ComponentData> component_data;
		std::unordered_map<EntityIdType, ComponentIndex> component_index_lookup;
	};


	//std::unique_ptr<RewindBufferEntry> rewind_buffer_;
	std::vector<RewindBufferEntry> rewind_buffer_;

	size_t most_recent_buffer_index_;
	size_t rewind_buffer_size_;


};

}  // corgi

#endif  // CORGI_NETWORK_SYSTEM_H_
